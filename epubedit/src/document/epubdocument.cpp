#include "document/epubdocument.h"

#include <stdlib.h>

#include "JlCompress.h"
#include "config.h"
#include "document/bookstructs.h"
#include "document/foaf.h"
#include "document/library.h"
#include "document/metadata.h"
#include "forms/navitem.h"
#include "utilities/paths.h"
#include "utilities/qtidydoc.h"
#include "utilities/tinyxmldocument.h"

const QString EPubDocument::METAINF_FOLDER = "META-INF";
const QString EPubDocument::OEBPS_FOLDER = "OEBPS";
const QString EPubDocument::MIMETYPE_FILE = "mimetype";
const QByteArray EPubDocument::MIMETYPE = "application/epub+zip";
const QString EPubDocument::CONTAINER_FILE = "META-INF/container.xml";
const QString EPubDocument::TOC_FILE = "toc.ncx";

const QString EPubDocument::TITLE = "title";
const QString EPubDocument::CREATOR = "creator";
const QString EPubDocument::IDENTIFIER = "identifier";
const QString EPubDocument::LANGUAGE = "language";

const QString EPubDocument::TOC_TITLE = "<h2>%1</h2>";
const QString EPubDocument::LIST_START = "<html><body><ul>";
const QString EPubDocument::LIST_END = "</ul></body></html>";
const QString EPubDocument::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
const QString EPubDocument::LIST_BUILD_ITEM = "<li><a href=\"%1#%2\">%3</li>";
const QString EPubDocument::LIST_FILEPOS = "position%1";
const QString EPubDocument::HTML_DOCTYPE =
  "<!DOCTYPE html PUBLIC "
  "\"-//W3C//DTD XHTML 1.1//EN\" "
  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
const QString EPubDocument::XML_HEADER =
  "<?xml version='1.0' encoding='utf-8'?>";

const QString EPubDocument::WORKING = "working";
const QString EPubDocument::DEPRECATED = "deprecated";

const QRegularExpression EPubDocument::reAnchorComplete(
  "<a[\\s]+([^>]+)>((?:.(?!\\<\\/a\\>))*.)</a>");
const QRegularExpression EPubDocument::reAnchorTag("<a[^>]*>");
const QRegularExpression EPubDocument::reHref("href=\\\"[^\"]*\"");
const QRegularExpression EPubDocument::reHtmlList(
  "<ol\\s*.*>\\s*.*<\\/ol>|<ul\\s*.*>\\s*.*<\\/ul>"); // detect html ol
const QRegularExpression EPubDocument::reHtmlListItem(
  "<li\\s*.*>\\s*.*<\\/li>"); // detect html ul

EPubDocument::EPubDocument(PConfig config,
                           POptions options,
                           PLibraryDB libDb,
                           PSeriesDB series,
                           PAuthorsDB authors,
                           QObject* parent)
  : QObject(parent)
  , m_config(config)
  , m_options(options)
  , m_libraryDB(libDb)
  , m_seriesDB(series)
  , m_authorsDB(authors)
{}

EPubDocument::~EPubDocument() {}

// QString
// EPubDocument::buildTocFromData()
//{
//   // NOTE may not be used
//   return buildTocfromHtml();
// }

bool
EPubDocument::isModified()
{
  return m_modified;
}

// EPubDocument::~EPubDocument() {}

bool
EPubDocument::loaded()
{
  return m_loaded;
}

bool
EPubDocument::parseMimetype(QuaZip* archive)
{
  if (m_config->zipFileList().contains(MIMETYPE_FILE)) {
    archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = archive->getZipError();
      qDebug() << tr("Unable to open mimetype file : error %1").arg(error);
      return false;
    }

    m_mimetype = mimetypeFile.readAll();
    if (m_mimetype != MIMETYPE) {
      qDebug() << tr("Unexpected mimetype %1").arg(QString(m_mimetype));
    }
  } else {
    qDebug() << tr("Unable to find mimetype in file");
    return false;
  }
  return true;
}

bool
EPubDocument::parseContainer(QuaZip* archive)
{
  if (m_config->zipFileList().contains(CONTAINER_FILE)) {
    archive->setCurrentFile(CONTAINER_FILE);
    QuaZipFile containerFile(archive);
    containerFile.setZip(archive);

    if (!containerFile.open(QIODevice::ReadOnly)) {
      int error = archive->getZipError();
      qDebug() << tr("Unable to open container file error %1").arg(error);
      return false;
    }

    QString container(containerFile.readAll());
    QDomDocument containerDocument;
    containerDocument.setContent(container);
    QDomElement rootElem = containerDocument.documentElement();
    QDomNamedNodeMap attMap = rootElem.attributes();
    m_containerVersion = rootElem.attribute("version");
    m_containerXmlns = rootElem.attribute("xmlns");

    QDomNodeList rootFiles = rootElem.elementsByTagName("rootfiles");
    if (rootFiles.size() > 0) {
      QDomElement rootfilesElem = rootFiles.at(0).toElement();
      QDomNodeList rootNodes = rootElem.elementsByTagName("rootfile");
      for (int i = 0; i < rootNodes.count(); i++) {
        QDomElement root_element = rootNodes.at(i).toElement();
        m_containerFullpath = root_element.attribute("full-path");
        m_containerMediatype = root_element.attribute("media-type");
        if (m_containerFullpath.isEmpty()) {
          qWarning() << tr("Invalid root file entry");
          continue;
        }
        if (parsePackageFile(archive, m_containerFullpath)) {
          return true;
        }
      }
    }

  } else {
    qDebug() << tr("Unable to find container information");
    return false;
  }

  // Limitations:
  //  - We only read one rootfile
  //  - We don't read the following from META-INF/
  //     - manifest.xml (unknown contents, just reserved)
  //     - metadata.xml (unused according to spec, just reserved)
  //     - rights.xml (reserved for DRM, not standardized)
  //     - signatures.xml (signatures for files, standardized)
  // Actually these are rarely included in an epub file anyway.

  qDebug() << tr("Unable to find and use any content files");
  return false;
}

void
EPubDocument::saveDeprecatedWorkingFile(const QString& filePath,
                                        const GuideItem& item,
                                        const QString data)
{
  auto path = Paths::join(filePath, WORKING, DEPRECATED);
  // build path
  QDir dir(path);
  dir.mkpath(dir.absolutePath());

  auto filename = Paths::join(path, item.href);
  QFile file(filename);
  auto info = QFileInfo(dir, item.href);
  //  auto splits = file.fileName().split("/", Qt::SkipEmptyParts);

  if (file.exists(filename)) {
    if (file.remove(filename)) {
      if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&file);
        out << data << Qt::endl;
      } else {
        // TODO Unable to open file
      }
    } else {
      // TODO error unable to replace file
      qWarning() << "Couldn't remove file to create a new one";
    }
  } else {
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
      QTextStream out(&file);
      out << data << Qt::endl;
    } else {
      // TODO Unable to open file
    }
  }
}

tinyxml2::XMLElement*
EPubDocument::checkListType(tinyxml2::XMLElement* element)
{
  auto listChild = element->FirstChildElement("ul");
  if (!listChild) {
    listChild = element->FirstChildElement("ol");
  }
  if (!listChild) {
    return nullptr;
  }
  return listChild;
}

void
EPubDocument::parseAttributes(NavTag* tag,
                              const tinyxml2::XMLElement* xmlElement)
{
  auto xmlAttribute = xmlElement->FirstAttribute();

  while (xmlAttribute) {
    auto name = xmlAttribute->Name();
    auto attribute = new NavAttribute;
    attribute->setName(name);
    attribute->setValue(xmlAttribute->Value());
    tag->addAttribute(attribute);
    //    attribute->setParent(tag);
    //    tag->setChild(attribute);
    xmlAttribute = xmlAttribute->Next();
  }
}

void
EPubDocument::parseTag(tinyxml2::XMLElement* listItemChild,
                       NavValueItem* navItem)
{
  auto xmlElement = listItemChild->FirstChild()->ToElement();
  NavTag* lastTag = nullptr;
  QString tagValue;
  while (xmlElement) {
    auto tag = new NavTag;
    tag->setType(QString(xmlElement->Name()));
    parseAttributes(tag, xmlElement);
    tag->setValue(xmlElement->GetText()); // should get the contents of the tag.
    tagValue = tag->value();              // might be empty
    if (lastTag) {
      lastTag->setChild(tag);
      tag->setParent(lastTag);
    } else {
      navItem->addTag(tag);
    }
    lastTag = tag;
    xmlElement = xmlElement->NextSiblingElement(); // should catch sub tags.
  }
  navItem->setValue(tagValue);
}

NavListItem*
EPubDocument::parseTocList(tinyxml2::XMLElement* element)
{
  auto listChild = checkListType(element);
  //  NavItem* nextItem = nullptr;
  NavItem* prevItem = nullptr;
  NavListItem* navList = new NavListItem;

  // check for <li>
  auto listItemChild = listChild->FirstChild()->ToElement();
  while (listItemChild) {
    auto name = QString(listItemChild->Name()).toLower();
    if (name == "li") {
      auto navItem = new NavValueItem;
      parseTag(listItemChild, navItem);
      navList->addItem(navItem);
    } else if (name == "ul" || name == "ol") {
      // If not check for a sub <ol>/<ul>
      //      listItemChild = checkListType(listItemChild);
      // recurse into sub list element.
      auto list = parseTocList(listChild);
      if (navList->items().isEmpty()) {
        navList->addItem(list);
        return navList;
      } else {
        prevItem->setChild(list);
        list->setParent(prevItem);
      }
    }
    auto sib = listItemChild->NextSibling();
    if (!sib)
      break;
    listItemChild = sib->ToElement();
  }
  return navList;
}

void
EPubDocument::handleDeprecatedGuide(QuaZip* archive,
                                    QDomDocument packageDocument)
{
  QList<GuideItem> guideItems;

  auto guideNodeList = packageDocument.elementsByTagName("guide");
  for (auto guideIndex = 0; guideIndex < guideNodeList.count(); guideIndex++) {
    auto node = guideNodeList.at(guideIndex);
    auto guideItemList = node.toElement().elementsByTagName("reference");
    for (auto itemIndex = 0; itemIndex < guideItemList.count(); itemIndex++) {
      GuideItem item;
      auto guideItem = guideItemList.at(itemIndex).toElement();
      auto nodeMap = guideItem.attributes();
      node = nodeMap.namedItem("href");
      if (!node.isNull()) {
        item.href = node.nodeValue();
      }
      node = nodeMap.namedItem("title");
      if (!node.isNull()) {
        item.title = node.nodeValue();
      }
      node = nodeMap.namedItem("type");
      if (!node.isNull()) {
        item.type = node.nodeValue();
      }
      guideItems.append(item);
    }
  }

  for (auto& guideItem : guideItems) {
    archive->setCurrentFile(guideItem.href);
    QuaZipFile contentFile(archive);
    contentFile.setZip(archive);
    if (!contentFile.open(QIODevice::ReadOnly)) {
      qDebug() << tr("Malformed file");
      continue;
    }

    // use html tidy to clean up file.
    auto input = contentFile.readAll();
//    auto doc = QTidyDoc(this);
//    QList<TidyDocOption*> options;
    // tinyxml doesn't seem to like the DOCTYPE tag.

//    options.prepend(new TidyDocOption(TidyDoctype, new TidyDocStrData("omit")));
//    auto cleaned = doc.toCleanXHtml(input, options);

    auto filename = Paths::join(m_config->libraryDirectory(),
                                m_metadata->creatorList().first(),
                                m_metadata->orderedTitles().first()->title);
    saveDeprecatedWorkingFile(filename, guideItem, /*cleaned*/ input);

    if (guideItem.type == "toc") {
      // now parse it
      auto xmldoc = TinyXmlDocument();
      xmldoc.parse(/*cleaned*/ input);

      auto root = xmldoc.rootElement();
      if (root) {
        auto body = root->FirstChildElement("body");
        if (body) {
          auto listItem = parseTocList(body);
          m_metadata->setNavList(listItem);
        }
      }

    } else if (guideItem.type == "cover") {
      // TODO
      auto xmldoc = TinyXmlDocument();
      xmldoc.parse(/*cleaned*/ input);

      auto root = xmldoc.rootElement();
      if (root) {
        auto body = root->FirstChildElement("body");
        if (body) {
//          auto listItem = parseTocList(body);
//          m_metadata->setNavList(listItem);
        }
      }
    } else if (guideItem.type == "title-page") {
      // TODO
    } else if (guideItem.type == "copyright-page") {
      // TODO
    } else {
      qWarning() << tr("unhandled guide type");
    }
  }
}

bool
EPubDocument::parsePackageFile(QuaZip* archive, QString& fullPath)
{
  archive->setCurrentFile(fullPath);
  QuaZipFile contentFile(archive);
  contentFile.setZip(archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    qDebug() << tr("Malformed content file, unable to get content metadata");
    return false;
  }

  QMap<QString, QDomDocument> map;
  m_currentRootFile = map;
  m_rootfiles.insert(fullPath, m_currentRootFile);

  QString content(contentFile.readAll());

  QDomDocument packageDocument;
  // handles more than one package.
  m_currentRootFile.insert(fullPath, packageDocument);

  packageDocument.setContent(content, true); // turn on namespace processing
  // parse root element attributes.
  auto rootElement = packageDocument.documentElement();
  auto nodeMap = rootElement.attributes();
  QDomNode node;
  for (int i = 0; i < nodeMap.size(); i++) {
    auto node = nodeMap.item(i);
    auto name = node.nodeName();
    auto value = node.nodeValue().toLower();
    //    auto lineNumber = node.lineNumber();
    // parse package attributes.
    if (name == "version") {
      if (value == "2.0") {
        m_version = V20;
      } else if (value == "2.0.1") {
        m_version = V201;
      } else if (value == "3.0") {
        m_version = V30;
      } else if (value == "3.0") {
        m_version = V31;
      } else if (value == "3.0") {
        m_version = V32;
      } else if (value == "3.3") {
        m_version = V33;
      } else {
        m_version = UNDEFINED;
      }
    } else if (name == "xmlns") {
      m_packageXmlns = value;
    } else if (name == "unique-identifier") {
      m_metadata->setUniqueIdentifierName(UniqueString(value));
    } else if (name == "xml:lang") {
      m_packageLanguage = value;
    } else if (name == "prefix") { // Only 3.0
      // TODO - handle prefix mapping - may not need this, just store value.
      m_packagePrefix = value;
    } else if (name == "dir") { // Only 3.0
      m_packageDirection = value;
    } else if (name == "id") { // Only 3.0
      m_packageId = UniqueString(value);
    } else if (name == "prefix") {
      m_metadata->setIsFoaf(true);
    }
  }

  // parse metadata.
  QDomNodeList metadataNodeList = packageDocument.elementsByTagName("metadata");
  m_metadata->parse(metadataNodeList);

  // Extract current path, for resolving relative paths
  QString contentFileFolder;
  int separatorIndex = fullPath.lastIndexOf('/');
  if (separatorIndex > 0) {
    contentFileFolder = fullPath.left(separatorIndex + 1);
  }

  // Parse out all the components/items in the epub
  // should only have one manifest.
  auto manifestNodeList = packageDocument.elementsByTagName("manifest");
  for (auto manifestIndex = 0; manifestIndex < manifestNodeList.count();
       manifestIndex++) {
    auto manifestElement = manifestNodeList.at(manifestIndex).toElement();
    nodeMap = manifestElement.attributes();
    node = nodeMap.namedItem("id");
    if (!node.isNull()) {
      //      auto lineNumber = node.lineNumber();
      m_manifest->id = UniqueString(node.nodeValue());
    }
    QDomNodeList manifestItemList = manifestElement.elementsByTagName("item");

    for (auto itemIndex = 0; itemIndex < manifestItemList.count();
         itemIndex++) {
      parseManifestItem(
        archive, manifestItemList.at(itemIndex), contentFileFolder);
    }
  }

  // should only have one guide which is deprecated in 3.0
  handleDeprecatedGuide(archive, packageDocument);

  // Parse out the document guide
  // please note that this has been superceded by landmarks in EPUB 3.0
  auto spineNodeList = packageDocument.elementsByTagName("spine");
  for (int i = 0; i < spineNodeList.count(); i++) {
    auto spineElement = spineNodeList.at(i).toElement();
    nodeMap = spineElement.attributes();
    node = nodeMap.namedItem("id");
    if (!node.isNull()) { // optional
                          //      auto lineNumber = node.lineNumber();
      m_spine->id = UniqueString(node.nodeValue());
    }

    node = nodeMap.namedItem("tocid");
    if (!node.isNull()) { // optional
      m_spine->tocId = node.nodeValue();
    }

    node = nodeMap.namedItem("page-progression-dir");
    if (!node.isNull()) { // optional
      m_spine->setDirection(node.nodeValue());
    }

    // could be being rebuilt. This happens when a book is loaded but appears
    // to have library copy already.
    m_spine->orderedItems.clear();
    m_spine->items.clear();

    QDomNodeList spineItemList = spineElement.elementsByTagName("itemref");
    for (int j = 0; j < spineItemList.count(); j++) {
      parseSpineItem(spineItemList.at(j));
    }
  }

  // TODO doctor the image paths to point to local files.
  auto htmlKeys = m_manifest->htmlItems.keys();
  for (int i1 = 0; i1 < htmlKeys.size(); i1++) {
    UniqueString key = htmlKeys.at(i1);
    PManifestItem item = m_manifest->htmlItems.value(key);
    QString documentString = item->documentString;
    bool changed = false;
    for (int i2 = 0; i2 < m_manifest->images.size(); i2++) {
      auto imageKeys = m_manifest->images.keys();
      UniqueString imageId = imageKeys.at(i2);
      QString imagePath = m_manifest->images.value(imageId);
      if (documentString.contains(imageId.toString())) {
        documentString.replace(
          imageId.toString(), imagePath, Qt::CaseSensitive);
        changed = true;
      }
    }
    if (changed) {
      item->documentString = documentString;
    }
  }
  return true;
}

// bool
// EPubDocument::parseTocFile(QuaZip* archive)
//{
//   // NOTE May not be used.
//   UniqueString tocId = m_spine.tocId;
//   PManifestItem tocItem = m_manifest->itemsById.value(tocId);
//   QString tocPath = tocItem->path;

//  archive->setCurrentFile(tocPath);
//  QuaZipFile tocFile(archive);
//  tocFile.setZip(archive);

//  if (!tocFile.open(QIODevice::ReadOnly)) {
//    archive->getZipError();
//    qDebug() << tr("Unable to open toc file %1").arg(tocPath);
//  }

//  QByteArray data = tocFile.readAll();
//  QDomDocument document;
//  document.setContent(data);
//  QString formattedTocString;
//  QDomElement root = document.documentElement();
//  QDomNodeList nodeList = document.elementsByTagName("docTitle");

//  if (!nodeList.isEmpty()) {
//    QDomNode titleNode = nodeList.at(0);
//    QDomElement titleText = titleNode.firstChild().toElement();
//    if (titleText.tagName() == "text") {
//      formattedTocString += TOC_TITLE.arg(titleText.text());
//    }
//  }
//  formattedTocString += LIST_START;

//  m_tocChapterIndex = -1;
//  nodeList = document.elementsByTagName("navMap");
//  if (!nodeList.isEmpty()) {
//    QDomNode node = nodeList.at(0);
//    QDomElement elem = node.toElement();
//    QDomElement navpoint = elem.firstChildElement("navPoint");
//    while (!navpoint.isNull()) {
//      PTocItem tocItem = parseNavPoint(navpoint, formattedTocString);
//      m_manifest->tocTtems.insert(tocItem->playorder, tocItem);
//      m_manifest->tocPaths.insert(tocItem->source, tocItem);

//      handleNestedNavpoints(navpoint, formattedTocString);

//      navpoint = navpoint.nextSiblingElement("navPoint");
//    }
//  }

//  formattedTocString += LIST_END;

//  m_manifest->formattedTocString = formattedTocString;
//  return true;
//}

PTocItem
EPubDocument::parseNavPoint(QDomElement navpoint, QString& formatted_toc_data)
{
  m_tocChapterIndex++;
  PTocItem tocItem = PTocItem(new TocItem());
  //  QDomNamedNodeMap attributes = navpoint.attributes();
  QString value = navpoint.attribute("class");
  if (!value.isEmpty()) {
    tocItem->tagClass = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("id");
  if (!value.isEmpty()) {
    tocItem->id = UniqueString(value);
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("playOrder");
  if (!value.isEmpty()) {
    tocItem->playorder = value.toInt();
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  QDomElement navlabel = navpoint.firstChildElement("navLabel");
  if (!navlabel.isNull()) {
    QDomElement text = navlabel.firstChild().toElement();
    if (text.tagName() == "text") {
      tocItem->label = text.text();
    }
  }

  QDomElement content = navpoint.firstChildElement("content");
  value = content.attribute("src");
  if (!value.isEmpty()) {
    // if no chapter fragment then add one in.
    int index = value.indexOf("#");
    if (index <= 0) {
      tocItem->source = value;
      tocItem->chapterTag.clear();
    } else {
      QString tag;
      tag = QString("#part%1").arg(m_tocChapterIndex);
      value += tag;
      tocItem->chapterTag = tag;
    }
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  formatted_toc_data += LIST_ITEM.arg(tocItem->source, tocItem->label);

  // parse nested navPoints.
  QDomElement sub_navpoint = navpoint.firstChildElement("navPoint");
  if (!sub_navpoint.isNull()) {
    formatted_toc_data += LIST_START;

    while (!sub_navpoint.isNull()) {
      PTocItem sub_item = parseNavPoint(sub_navpoint, formatted_toc_data);

      tocItem->subItems.insert(sub_item->playorder, sub_item);
      sub_navpoint = sub_navpoint.nextSiblingElement("navPoint");
    }

    formatted_toc_data += LIST_END;
  }

  return tocItem;
}

void
EPubDocument::handleNestedNavpoints(QDomElement elem,
                                    QString& formatted_toc_string)
{
  QDomElement subpoint = elem.firstChildElement("navPoint");
  while (!subpoint.isNull()) {
    PTocItem tocItem = parseNavPoint(subpoint, formatted_toc_string);
    m_manifest->tocTtems.insert(tocItem->playorder, tocItem);
    m_manifest->tocPaths.insert(tocItem->source, tocItem);

    handleNestedNavpoints(subpoint, formatted_toc_string);

    subpoint = elem.nextSiblingElement("navPoint");
  }
}

bool
EPubDocument::parseSpineItem(const QDomNode& spineNode)
{
  auto metadataElement = spineNode.toElement();
  auto tag_name = metadataElement.tagName();
  auto nodeMap = metadataElement.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "itemref") {
    auto item = PSpineItem(new SpineItem());

    // TODO EPUB2 toc element - convert to EPUB3

    node = nodeMap.namedItem("id"); // optional
    if (!node.isNull()) {
      item->id = UniqueString(node.nodeValue());
    }

    node = nodeMap.namedItem("idref"); // required
    if (!node.isNull()) {
      value = node.nodeValue();
      item->idref = value;
    } else {
      qDebug() << tr("Warning invalid manifest itemref : no idref value");
    }

    node = nodeMap.namedItem("linear"); // optional
    if (!node.isNull()) {
      value = node.nodeValue();
      if (!value.isEmpty()) {
        auto lValue = value.toLower();
        if (lValue == "no")
          item->linear = false;
        else
          item->linear = true; // true by default.
      } else {
        qDebug() << tr("Warning invalid manifest itemref : linear MUST be "
                       "either yes or no not %1")
                      .arg(value);
      }
    }

    node = nodeMap.namedItem("properties"); // optional
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);

      for (auto& prop : properties) {
        if (prop == "page-spread-left") {
          item->pageSpread = LEFT;
        } else if (prop == "page-spread-right") {
          item->pageSpread = RIGHT;
        } else {
          item->pageSpread = NONE;
        }
      }
    }
    m_spine->items.insert(item->idref, item);
    m_spine->orderedItems.append(item->idref);
  }
  return true;
}

bool
EPubDocument::parseManifestItem(QuaZip* archive,
                                const QDomNode& manifest_node,
                                const QString current_folder)
{
  auto metadataElement = manifest_node.toElement();
  auto tagName = metadataElement.tagName();
  auto nodeMap = metadataElement.attributes();
  QDomNode node;
  QString name, value;

  if (tagName == "item") {
    auto item = PManifestItem(new ManifestItem());
    node = nodeMap.namedItem("href");
    if (!node.isNull()) {
      value = node.nodeValue();
      //      auto path = QDir::cleanPath(current_folder + value);
      item->href = value;
      //      item->path = path;
    } else {
      qDebug() << tr("Warning invalid manifest item : no href value");
    }

    node = nodeMap.namedItem("id");
    if (!node.isNull()) {
      item->id = UniqueString(node.nodeValue());
    } else {
      qDebug() << tr("Warning invalid manifest item : no id value");
    }

    node = nodeMap.namedItem("media-type");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->mediaType = value.toLower().toLatin1();
      if (item->mediaType == "image/gif" || item->mediaType == "image/jpeg" ||
          item->mediaType == "image/png" ||
          item->mediaType == "image/svg+xhtml" ||
          item->mediaType == "image/webp") {
        // NOTE image/webp is supported by EPU3.3 but not by QImageReader

        if (!QImageReader::supportedMimeTypes().contains(
              item->mediaType.toLatin1())) {
          qDebug() << tr("Requested image type %1 is an unsupported type")
                        .arg(QString(item->mediaType));
        }

        //        archive->setCurrentFile(item->path);
        //        QuaZipFile image_file(archive);
        //        image_file.setZip(archive);

        //        if (!image_file.open(QIODevice::ReadOnly)) {
        //          //          archive->getZipError();
        //          qDebug() << tr("Unable to open image file
        //          %1").arg(item->path);
        //        }

        //        auto data = image_file.readAll();
        //        auto image = QImage::fromData(data);
        //        QFileInfo info(item->href);
        //        auto path = info.path();
        //        if (!path.isEmpty()) {
        //          QDir dir(m_resourcePath);
        //          dir.mkpath(path);
        //        }

        //        auto res_path = m_resourcePath + QDir::separator() +
        //        item->href; image.save(res_path);
        //        res_path.prepend(QStringLiteral("file://"));
        m_manifest->images.insert(item->id, item->href);

      } else if (item->mediaType == "image/svg+xml") {
        qDebug();
      } else if (item->mediaType == "application/vnd.ms-opentype" ||
                 item->mediaType == "application/font-woff") {
        m_manifest->fonts.insert(item->href, item);

      } else if (item->mediaType == "application/xhtml+xml") {
        archive->setCurrentFile(item->href);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          auto error = archive->getZipError();
          qDebug() << tr("Unable to open container file error %1").arg(error);
          return false;
        }

        QString container(itemFile.readAll());
        // remove xml header string. This will be reinserted by
        // QXmlStreamWriter
        auto length = container.trimmed().length();
        if (container.trimmed().startsWith(XML_HEADER)) {
          length -= XML_HEADER.length();
          container = container.right(length).trimmed();
        }
        // Remove DOCTYPE if exists. Again this will be reinserted later.
        if (container.trimmed().startsWith(HTML_DOCTYPE)) {
          length = container.length();
          length -= HTML_DOCTYPE.length();
          container = container.right(length).trimmed();
        }

        extractHeadInformationFromHtmlFile(item, container);
        item->documentString = container;
        m_manifest->htmlItems.insert(item->id, item);
      } else if (item->mediaType == "text/css") {
        archive->setCurrentFile(item->href);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = archive->getZipError();
          qDebug() << tr("Unable to open css file error %1").arg(error);
          return false;
        }

        QString cssString(itemFile.readAll());
        cssString.replace("@charset \"", "@charset\"");
        m_manifest->css.insert(item->href, cssString);

      } else if (item->mediaType == "text/javascript") {
        archive->setCurrentFile(item->href);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          auto error = archive->getZipError();
          qDebug() << tr("Unable to open javascript file error %1").arg(error);
          return false;
        }

        QString js_string(itemFile.readAll());
        m_manifest->javascript.insert(item->id, js_string);
      }
    } else {
      qDebug() << tr("Warning invalid manifest item : no media-type value");
    }

    node = nodeMap.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      auto properties = value.split(' ', Qt::SkipEmptyParts);
      item->properties = properties;

      for (auto& prop : properties) {
        if (prop == "cover-image") {
          // only one cover-image allowed.
          m_manifest->coverImage = item;
          //          m_manifest->images.insert(item->id, item);
        } else if (prop == "nav") {
          // only one nav allowed.
          m_manifest->nav = item;
        } else if (prop == "svg") {
          m_manifest->svgImages.insert(item->id, item);
          //          m_manifest->images.insert(item->id, item);
        } else if (prop == "switch") {
          m_manifest->switches.insert(item->id, item);
        } else if (prop == "mathml") {
          m_manifest->mathml.insert(item->id, item);
        } else if (prop == "remote-resources") {
          m_manifest->remotes.insert(item->id, item);
        } else if (prop == "scripted") {
          m_manifest->scripted.insert(item->id, item);
        } else {
          // one of the exmples had a data-nav element which is NOT standard.
          // not certain what to do with these.
          item->nonStandardProperties.insert(name, value);
        }
      }
    }

    node = nodeMap.namedItem("fallback");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->fallback = value;
    }

    node = nodeMap.namedItem("media-overlay");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->mediaOverlay = value;
      m_manifest->mediaOverlay.insert(item->id, item);
    }

    m_manifest->itemsById.insert(item->id, item);
    m_manifest->itemsByHref.insert(item->href, item);
  }
  return true;
}

void
EPubDocument::extractHeadInformationFromHtmlFile(PManifestItem item,
                                                 QString container)
{
  QDomDocument doc;
  doc.setContent(container);

  QDomNodeList node_list = doc.elementsByTagName(QLatin1String("head"));

  if (!node_list.isEmpty()) {
    QDomNode head_node = node_list.at(0); // should only be one head element
    QDomElement head_elem = head_node.toElement();
    QDomNodeList link_list = head_elem.elementsByTagName(QLatin1String("link"));
    for (int i = 0; i < link_list.length(); i++) {
      QDomNode link = link_list.at(i);
      QDomElement link_elem = link.toElement();
      QString rel = link_elem.attribute(QLatin1String("rel"));
      if (!rel.isEmpty()) {
        if (rel == QLatin1String("stylesheet")) {
          QString type = link_elem.attribute(QLatin1String("type"));
          if (!type.isEmpty()) {
            if (type == QLatin1String("text/css")) {
              QString href = link_elem.attribute(QLatin1String("href"));
              if (!href.isEmpty()) {
                item->cssLinks.append(href);
              }
            }
          }
        }
      }
    }
  }

  node_list = doc.elementsByTagName(QLatin1String("body"));

  if (!node_list.isEmpty()) {
    QDomNode body_node = node_list.item(0); // should only be one
    QDomElement body_elem = body_node.toElement();
    QString att = body_elem.attribute(QLatin1String("class"));
    if (!att.isEmpty()) {
      item->bodyClass = att;
    }
  }
}

void
EPubDocument::saveDocument(const QString& filename)
{
  //  saveBookToLibrary(filename);
  // TODO save document
}

/*
 * Copies the new file into the library returning tre if successful, otherwise
 * returning false. This will overwrite without requesting confirmation.
 */
bool
EPubDocument::copyBookToLibrary(const QString& oldFilename,
                                const QString& newFilename)
{
  QFileInfo info(newFilename);
  QDir dir = info.absoluteDir();
  dir.mkpath(dir.absolutePath());

  if (QFile::exists(newFilename)) {
    if (QFile::exists(newFilename)) {
      if (QFile::remove(newFilename)) {
        // TODO clean up other incidental files
        // e.g. intended undo edit file.
        return QFile::copy(oldFilename, newFilename);
      } else {
        // TODO error removing old file.
        return false;
      }
    } else {
      return QFile::copy(oldFilename, newFilename);
    }
  } else {
    return QFile::copy(oldFilename, newFilename);
  }
  return false;
}

/*
 * Checks whether the book is stored in your library location (returns false).
 * If not asks you whether you wish to overwrite the library copy or
 * to continue to edit the library version (returns true).
 */
EPubDocument::StoreType
EPubDocument::confirmOverwriteOrUseLibrary(const QString& path,
                                           QString& libraryFilename) const
{
  if (!path.startsWith(m_config->libraryPath())) { // is in library.
    auto titles = m_metadata->orderedTitles();
    auto creators = m_metadata->creatorList();
    auto info = QFileInfo(path);
    if (!titles.isEmpty() && !creators.isEmpty()) {
      auto title = titles.first()->title;
      auto creator = creators.first();
      // TODO check if book is in library.
      // may have to check against all authors.
      libraryFilename = m_config->libraryDirectory();
      libraryFilename =
        Paths::join(libraryFilename, creator, title, info.fileName());
      if (QFile::exists(libraryFilename)) {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowTitle(tr("Existing Library Book"));
        box.setText(
          tr("This file already in your library!\n"
             "Press \"Use library\" to use the library version,\n"
             "or \"Overwrite library\" to overwrite library version."));
        box.setToolTip(path);
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::Save);
        auto yesBtn = box.button(QMessageBox::Yes);
        yesBtn->setText(tr("Use library"));
        yesBtn->setToolTip("Use to version already stored in your library.");
        auto saveBtn = box.button(QMessageBox::Save);
        saveBtn->setText(tr("Overwrite library"));
        saveBtn->setToolTip(
          "Save this version to the library.\n"
          "This will overwrite the existing library versionand\n"
          "lose any changes you might have made to it.");

        box.setDefaultButton(QMessageBox::Yes);
        if (box.exec() == QMessageBox::Yes) {
          return USE_STORED;
        } else {
          return OVERWRITE_EXISTING;
        }
      }
    }
  }
  return COPY_INTO_LIBRARY;
}

QString
EPubDocument::getFirstAuthorNameOrUnknown(const QStringList& authors)
{
  if (authors.isEmpty()) {
    return tr("Unknown");
  } else {
    return authors.first();
  }
}

QString
EPubDocument::getFirstTitleOrTemp(QList<PTitle> titles)
{
  if (!titles.isEmpty()) {
    return titles.first()->title;
  } else {
    quint32 value = 0;
    while (value < 0xFFFFFF) {
      value = QRandomGenerator::global()->generate();
    }
    return QString("TMPEPUB_%1").arg(QString::number(value, 16).right(8));
  }
}

bool
EPubDocument::writeVersion30()
{
  //  QFileInfo info;
  //  QDir dir;
  //  QFile file;
  //  QString filename;

  return false;
}

bool
EPubDocument::writeVersion31()
{
  return false;
}

bool
EPubDocument::writeVersion32()
{
  return false;
}

PBookData
EPubDocument::insertNewBook(const QString& filename,
                            const QString& title,
                            QStringList creators)
{
  auto uid = BookData::nextUid();
  auto bookData = PBookData(new BookData(uid, filename, title, creators));
  m_libraryDB->insertOrUpdateBook(bookData);
  return bookData;
}

PBookData
EPubDocument::getBookForTitleAndCreator(const QString& title,
                                        const QString& libraryFilename)
{
  PBookData data;
  auto books = m_libraryDB->bookByTitle(title);
  auto creators = m_metadata->creatorList();
  if (books.isEmpty()) {
    data = insertNewBook(libraryFilename, title, creators);
  } else {
    // possibly more than one for same title with a different creator
    // to eliminate or may be stored under a secondary creator.
    for (auto& book : books) {
      for (auto& creator : creators) {
        if (book->creators().contains(creator)) {
          data = book;
          break;
        }
      }
    }
    if (data.isNull()) {
      data = insertNewBook(libraryFilename, title, creators);
    }
  }
  return data;
}

PBookData
EPubDocument::loadDocument(const QString& existingFilename)
{
  PBookData data; // empty book
  if (!QFile::exists(existingFilename))
    return data;

  m_metadata = PMetadata(new Metadata());
  m_manifest = PManifest(new Manifest());
  m_spine = PSpine(new Spine());

  /* Loads the document metadata. This is needed to
   determine where in the library to save the file.
   Generally this will be '{library directory}/{Author name}/{title}'
   The author name used will be the primary (first) author. */
  if (loadDocumentFromFile(existingFilename)) {
    QString libraryFilename, title;
    auto titles = m_metadata->orderedTitles();
    title = titles.first()->title;
    auto creators = m_metadata->creatorList();

    auto loadTask =
      confirmOverwriteOrUseLibrary(existingFilename, libraryFilename);
    //    auto info = QFileInfo(existingFilename);
    if (loadTask == COPY_INTO_LIBRARY) { // No existing book
      auto calibre = m_metadata->calibre();
      auto uid = BookData::nextUid();
      if (calibre.isNull()) { // this shouldn't happen as Calibre is created
                              // with metadata
        if (calibre->seriesName().isEmpty()) {
          data = PBookData(new BookData(uid, libraryFilename, title, creators));
        } else {
          auto uid = BookData::nextUid();
          auto series = calibre->seriesName();
          auto index = calibre->seriesIndex();
          data = PBookData(
            new BookData(uid, libraryFilename, title, creators, series, index));
        }
      } else {
        data = PBookData(new BookData(uid, libraryFilename, title, creators));
        if (!data->isValid()) {
          // TODO error no BookData.
        }
      }
      copyBookToLibrary(existingFilename, libraryFilename);
    } else if (loadTask == USE_STORED) {
      m_metadata = PMetadata(new Metadata());
      m_manifest = PManifest(new Manifest());
      loadDocumentFromFile(libraryFilename);
      data = getBookForTitleAndCreator(title, libraryFilename);
      if (!data->isValid()) {
        // TODO error no BookData.
      }
    } else if (loadTask == OVERWRITE_EXISTING) {
      data = getBookForTitleAndCreator(title, libraryFilename);
      copyBookToLibrary(existingFilename, libraryFilename);
      if (!data->isValid()) {
        // TODO error no BookData.
      }
      // TODO update BookData.
    }
  }
  return data;
}

bool
EPubDocument::loadDocumentFromFile(const QString& filename)
{
  // open the epub as a zip file
  auto archive = new QuaZip(filename);
  m_config->setCurrentFilename(filename);
  if (!archive->open(QuaZip::mdUnzip)) {
    qDebug() << tr("Failed to open %1").arg(filename);
    return false;
  }

  // get list of filenames from zip file
  auto filelist = archive->getFileNameList();
  if (filelist.isEmpty()) {
    qDebug() << tr("Failed to read %1").arg(filename);
    return false;
  }
  m_config->setZipFileList(filelist);

  // Get and check that the mimetype is correct.
  // According to the standard this must be unencrypted.
  if (!parseMimetype(archive)) {
    return false;
  }

  if (!parseContainer(archive)) {
    return false;
  }

  //  for (auto& item: m_manifest->html_items.values()) {
  //    CSSMap css_strings = cssMap();
  //    QString doc_string = item->document_string;
  //    //    if (m_parser->parse(item->id, doc_string, css_strings)) {
  //    //      // TODO use data?
  //    //    }
  //  }

  //  emit q->loadCompleted();
  delete archive;
  return true;
}

EPubDocument::Errors
EPubDocument::writeMimetype(const QString& filename)
{
  QFile mimetypeFile(filename);

  if (!mimetypeFile.open(QIODevice::WriteOnly)) {
    emit sendLogMessage(tr("Unable to write mimetype file"));
    return METATYPE_ERROR | WRITE_ERROR;
  }

  qint64 size = mimetypeFile.write(MIMETYPE);
  if (size != m_mimetype.size()) {
    return METATYPE_ERROR | SIZE_ERROR;
  }
  return NO_ERROR;
}

EPubDocument::Errors
EPubDocument::writeContainer(QuaZip* save_zip)
{
  //  QuaZipFile container_file(save_zip);

  //  if (!container_file.open(QIODevice::WriteOnly,
  //                           QuaZipNewInfo(CONTAINER_FILE, CONTAINER_FILE)))
  //                           {
  //    int error = save_zip->getZipError();
  //    return CONTAINER_ERROR | WRITE_ERROR;
  //  }

  //  QXmlStreamWriter xml_writer(&container_file);
  //  xml_writer.setAutoFormatting(true);
  //  xml_writer.writeStartDocument("1.0");

  //  xml_writer.writeStartElement("container");
  //  xml_writer.writeAttribute("version", m_containerVersion);
  //  xml_writer.writeAttribute("xmlns", m_containerXmlns);

  //  xml_writer.writeStartElement("rootfiles");
  //  xml_writer.writeStartElement("rootfile");
  //  xml_writer.writeAttribute("full-path", m_containerFullpath);
  //  xml_writer.writeAttribute("media-type", m_containerMediatype);
  //  xml_writer.writeEndElement();

  //  xml_writer.writeEndElement();
  //  xml_writer.writeEndElement();

  //  xml_writer.writeEndDocument();
  //  container_file.close();

  //  writePackageFile(save_zip);

  //  for (PAuthorData item :
  //  m_manifest->htmlItems.values()) {
  //    QuaZipFile item_file(save_zip);
  //    item_file.setFileName(item->path);

  //    if (!item_file.open(QIODevice::WriteOnly,
  //                        QuaZipNewInfo(item->path, item->path))) {
  //      int error = save_zip->getZipError();
  //      qDebug() << tr("Unable to write html/xhtml file : error
  //      %1").arg(error); return false;
  //    }

  //    /* We have to use QTextStream rather than QXmlStreamWriter because the
  //    xml
  //     * stream escapes '<', '>' and several other characters. */
  //    QTextStream out(&item_file);
  //    out << QStringLiteral("<html
  //    xmlns=\"http://www.w3.org/1999/xhtml\">\n"); out << HTML_DOCTYPE <<
  //    "\n"; out << QStringLiteral("<head>\n"); out <<
  //    QStringLiteral("<title>"); PTitle shared_title =
  //      m_metadata->orderedTitles().first();
  //    if (shared_title) {
  //      out << shared_title->title;
  //    }
  //    out << QStringLiteral("</title>\n");
  //    out << QStringLiteral("<meta http-equiv=\"Content-Type\" "
  //                          "content=\"text/html; charset=utf-8\"/>\n");
  //    for (auto& href : item->cssLinks) {
  //      out << QString(
  //               "<link href=\"%1\" rel=\"stylesheet\"
  //               type=\"text/css\"/>\n") .arg(href);
  //    }
  //    out << QStringLiteral("</head>\n");
  //    out << QStringLiteral("<body");
  //    if (!item->bodyClass.isEmpty()) {
  //      out << QString(" class=\"%1\">\n").arg(item->bodyClass);
  //    } else {
  //      out << QStringLiteral(">\n");
  //    }
  //    out << item->documentString;
  //    out << QStringLiteral("</body>\n");
  //    out << QStringLiteral("</html>\n");
  //    out.flush();

  //    item_file.close();
  //  }
  //  return true;
  return NO_ERROR;
}

EPubDocument::Errors
EPubDocument::writePackageFile(QuaZip* save_zip)
{
  //  QuaZipFile package_file(save_zip);

  //  if (!package_file.open(
  //        QIODevice::WriteOnly,
  //        QuaZipNewInfo(m_containerFullpath, m_containerFullpath))) {
  //    int error = save_zip->getZipError();
  //    qDebug() << tr("Unable to write container file : error
  //    %1").arg(error); return false;
  //  }

  //  QXmlStreamWriter xml_writer(&package_file);
  //  xml_writer.setAutoFormatting(true);
  //  xml_writer.writeStartDocument("1.0");

  //  xml_writer.writeStartElement("package");
  //  xml_writer.writeAttribute("version", "3.0");
  //  xml_writer.writeAttribute("unique-identifier",
  //                            m_metadata->uniqueIdentifierName().toString());
  //  xml_writer.writeAttribute("xmlns", m_packageXmlns);
  //  if (m_metadata->isFoaf()) {
  //    xml_writer.writeAttribute("prefix", Foaf::prefix());
  //  }
  //  if (!m_packageLanguage.isEmpty()) { // Optional in EPUB 2.0
  //    xml_writer.writeAttribute("xml:lang", m_packageLanguage);
  //  }
  //  if (!m_packagePrefix.isEmpty()) {
  //    xml_writer.writeAttribute("prefix", m_packagePrefix);
  //  }
  //  if (!m_packageDirection.isEmpty()) {
  //    xml_writer.writeAttribute("dir", m_packageDirection);
  //  }
  //  if (!m_packageId.isEmpty()) {
  //    xml_writer.writeAttribute("id", m_packageId.toString());
  //  }

  //  m_metadata->write(&xml_writer);

  //  xml_writer.writeEndElement();
  //  xml_writer.writeEndDocument();

  //  package_file.close();

  //  // TODO - the rest of the saves.
  //  return true;
  return NO_ERROR;
}

// EPubContentsForm*
// EPubDocument::cloneData()
//{
//   //  EPubContents* contents = new EPubContents();
//   //  contents->m_loaded = m_loaded;

//  //  return contents;
//  return nullptr;
//}

// void
// EPubDocument::setClonedData(EPubContentsForm* cloneData)
//{
//   //  Q_Q(EPubDocument);

//  //  QElapsedTimer timer;
//  //  timer.start();

//  //  m_container = clone->m_container;
//  //  m_svgs = clone->m_svgs;
//  //  m_renderedSvgs = clone->m_renderedSvgs;
//  //  m_currentItem = clone->m_currentItem;
//  //  m_loaded = clone->m_loaded;

//  //  QTextCursor cursor(q_ptr);
//  //  cursor.movePosition(QTextCursor::End);

//  //  QStringList items = m_container->items();

//  //  QString cover =
//  m_container->standardPage(EPubPageReference::CoverPage);
//  //  if (cover.isEmpty()) {
//  //    items.prepend(cover);
//  //    QLOG_DEBUG(QString("Cover is empty %1").arg(cover))
//  //  }

//  //  QTextBlockFormat pageBreak;
//  //  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
//  //  for (const QString& chapter : items) {
//  //    m_currentItem = m_container->epubItem(chapter);
//  //    if (m_currentItem.path.isEmpty()) {
//  //      continue;
//  //    }

//  //    QSharedPointer<QuaZipFile> zipFile =
//  //        m_container->zipFile(m_currentItem.path);
//  //    if (!zipFile) {
//  //      QLOG_WARN(QString("Unable to get zipped file for chapter %1")
//  //                    .arg(m_currentItem.path))
//  //      continue;
//  //    }

//  //    QByteArray itemdata = zipFile->readAll();
//  //    if (itemdata.isEmpty()) {
//  //      QLOG_WARN(QString("Got an empty document"))
//  //      continue;
//  //    }
//  //    q->setBaseUrl(QUrl(m_currentItem.path));
//  //    QDomDocument newDocument;
//  //    newDocument.setContent(itemdata);
//  //    // TOD extract text for processing
//  //    fixImages(newDocument);

//  //    cursor.insertText(newDocument.toString());
//  //    cursor.insertBlock(pageBreak);
//  //  }

//  //  q->setBaseUrl(QUrl());
//  //  m_loaded = true;

//  //  emit q->loadCompleted();
//  //  //  QLOG_DEBUG(QString("Done in %1 mS").arg(timer.elapsed()))}
//}

// QString
// EPubDocument::filename()
//{
//   return m_filename;
// }

// void
// EPubDocument::setFilename(const QString& filename)
//{
//   if (!filename.isEmpty())
//     m_filename = filename;
// }

QString
EPubDocument::tocAsString()
{
  return m_manifest->formattedTocString;
}

QString
EPubDocument::title()
{
  if (!m_metadata->orderedTitles().isEmpty()) {
    PTitle firstTitle = m_metadata->orderedTitles().first();
    if (!firstTitle.isNull())
      return firstTitle->title;
  }
  return QString();
}

void
EPubDocument::setTitle(const QString& title)
{
  PTitle first = m_metadata->orderedTitles().first();
  if (!first.isNull()) {
    first->title = title;
    m_modified = true;
  } else {
    first = PTitle(new EPubTitle());
    first->title = title;
    m_metadata->orderedTitles().insert(1, first);
    m_modified = true;
  }
}

QString
EPubDocument::subject()
{
  return m_subject;
}

void
EPubDocument::setSubject(const QString& subject)
{
  m_subject = subject;
}

QString
EPubDocument::language()
{
  // TODO
  return QString();
}

void
EPubDocument::setLanguage(const QString& language)
{
  // TODO
}

QDateTime
EPubDocument::date()
{
  // TODO
  return QDateTime();
}

void
EPubDocument::setDate(const QDateTime& date)
{
  // TODO
}

QStringList
EPubDocument::creators()
{
  return m_metadata->creatorList();
}

QString
EPubDocument::creatorNames(const QStringList& names)
{
  QString result;
  for (int i = 0; i < names.size(); i++) {
    QString name = names.at(i);
    if (result.isEmpty()) {
      result = name;
    } else {
      result += ", ";
      result += name;
    }
  }
  return result;
}

QString
EPubDocument::publisher()
{
  return m_publisher;
}

void
EPubDocument::setPublisher(const QString& publisher)
{
  m_publisher = publisher;
}

PMetadata
EPubDocument::metadata()
{
  return m_metadata;
}

// QMap<QString, QString>
QMap<UniqueString, PManifestItem>
EPubDocument::pages()
{
  return m_manifest->htmlItems;
}

PSpine
EPubDocument::spine()
{
  return m_spine;
}

QList<UniqueString>
EPubDocument::cssKeys()
{
  return m_manifest->css.keys();
}

QMap<UniqueString, QString>
EPubDocument::cssMap()
{
  return m_manifest->css;
}

QString
EPubDocument::css(UniqueString key)
{
  return cssMap().value(key);
}

QString
EPubDocument::javascript(UniqueString key)
{
  return m_manifest->javascript.value(key);
}

PManifestItem
EPubDocument::itemByHref(UniqueString href)
{
  return m_manifest->itemsByHref.value(href);
}

PManifest
EPubDocument::manifest() const
{
  return m_manifest;
}

// IEBookInterface*
// EPubDocument::plugin()
//{
//  Q_D(EPubDocument);
//  return d->plugin();
//}

// void
// EPubDocument::setPlugin(IEBookInterface* plugin)
//{
//  Q_D(EPubDocument);
//  d->setPlugin(plugin);
//}

QDate
EPubDocument::published()
{
  return m_published;
}

void
EPubDocument::setPublished(const QDate& published)
{
  m_published = published;
}

// QString
// EPubDocument::buildTocfromHtml()
//{
//   // NOTE may not be used
//   auto formattedTocString = LIST_START;
//   auto anchorStart = 0, pos = 0;
//   auto files = m_manifest->htmlItems.values();
//   for (auto& item : files) {
//     auto documentString = item->documentString;
//     if (!documentString.isEmpty()) {
//       auto i = reAnchorComplete.globalMatch(documentString);
//       QRegularExpressionMatch anchorCompleteMatch, anchorTagMatch, hrefMatch;
//       QString anchorComplete, anchorTag, hrefAttr;

//      while (i.hasNext()) {
//        anchorCompleteMatch = i.next();
//        anchorComplete = anchorCompleteMatch.captured(0);
//        anchorStart =
//          anchorCompleteMatch.capturedStart(0); // start of open anchor tag.

//        anchorTagMatch = reAnchorTag.match(anchorComplete);
//        if (anchorTagMatch.hasMatch()) {
//          anchorTag = anchorTagMatch.captured(0);
//          anchorStart +=
//            anchorTagMatch.capturedLength(0); // end of open anchor tag

//          auto text = extractTagText(anchorStart, documentString);

//          hrefMatch = reHref.match(anchorTag);
//          if (hrefMatch.hasMatch()) {
//            hrefAttr = hrefMatch.captured(0);
//            hrefAttr = hrefAttr.mid(6, hrefAttr.length() - 7);
//            auto splits = hrefAttr.split("#", Qt::KeepEmptyParts);
//            if (splits.length() == 1) {
//              auto filename = splits.at(0);
//              //              QList<PAuthorData> files =
//              //                m_manifest->htmlItems.values();
//              for (auto& item : files) {
//                auto href = item->href;
//                if (href == filename) {
//                  // TODO add anchor & make anchor tag.
//                }
//              }
//            } else if (!splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
//              // existing file + anchor points exist.
//              formattedTocString +=
//                LIST_BUILD_ITEM.arg(splits.at(0), splits.at(1), text);
//            } else if (!splits.at(0).isEmpty() && splits.at(1).isEmpty()) {
//              // existing file but no anchor point.
//              auto pos_tag = LIST_FILEPOS.arg(pos++);
//              formattedTocString +=
//                LIST_BUILD_ITEM.arg(splits.at(0), pos_tag, text);
//              // TODO introduce anchor tag
//            } else if (splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
//              // existing anchor tag but no file.
//              // TODO find file and add to anchor.
//            } else {
//              // TODO no existing anchor point ??? not certain we ever get
//              // here. may need to move this outside last regex match.
//            }
//          }
//        }
//      }
//    }
//  }

//  formattedTocString += LIST_END;
//  return formattedTocString;
//}

QString
EPubDocument::extractTagText(int anchor_start, QString document_string)
{
  int i = anchor_start;
  bool in_dquotes = false, in_squotes = false, in_tag = false,
       in_close_tag = false;
  QChar first;
  QString text;
  while (i < document_string.length()) {
    first = document_string.at(i);
    if (first == '<') {
      if (!in_dquotes && !in_squotes) {
        in_tag = true;
      }
    } else if (first == '>') {
      if (!in_dquotes && !in_squotes) {
        if (in_tag) {
          in_tag = false;
        } else if (in_close_tag) {
          in_close_tag = false;
        }
      }
    } else if (first == '\"') {
      if (in_dquotes)
        in_dquotes = false;
      else {
        in_dquotes = true;
      }
    } else if (first == '\'') {
      if (in_squotes)
        in_squotes = false;
      else {
        in_squotes = true;
      }
    } else if (first == '/') {
      if (in_tag) {
        in_close_tag = true;
        in_tag = false;
      }
    } else {
      if (!in_tag && !in_close_tag) {
        // store everything if not in_quotes and not in_tag
        text += first;
      }
      if (in_close_tag) {
        if (first == 'a') { // end of the anchor tag.
          break;
        }
      }
    }
    i++;
  }
  return text;
}
