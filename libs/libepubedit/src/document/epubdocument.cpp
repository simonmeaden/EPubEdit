#include "document/epubdocument.h"

#include <stdlib.h>

#include "JlCompress.h"

#include "config.h"
#include "document/epubmetadata.h"
#include "document/foaf.h"
#include "paths.h"

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

EPubDocument::~EPubDocument() {
  qDebug();
}

QString
EPubDocument::buildTocFromData()
{
  return buildTocfromHtml();
}

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
  if (m_files.contains(MIMETYPE_FILE)) {
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
  if (m_files.contains(CONTAINER_FILE)) {
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

bool
EPubDocument::parsePackageFile(QuaZip *archive, QString& fullPath)
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
      parseManifestItem(archive, manifestItemList.at(itemIndex), contentFileFolder);
    }
  }

  // Parse out the document guide
  // please note that this has been superceded by landmarks in EPUB 3.0
  auto spineNodeList = packageDocument.elementsByTagName("spine");
  for (int i = 0; i < spineNodeList.count(); i++) {
    auto spineElement = spineNodeList.at(i).toElement();
    nodeMap = spineElement.attributes();
    node = nodeMap.namedItem("id");
    if (!node.isNull()) { // optional
                          //      auto lineNumber = node.lineNumber();
      m_spine.id = UniqueString(node.nodeValue());
    }

    node = nodeMap.namedItem("tocid");
    if (!node.isNull()) { // optional
      m_spine.tocId = node.nodeValue();
    }

    node = nodeMap.namedItem("page-progression-dir");
    if (!node.isNull()) { // optional
      m_spine.setDirection(node.nodeValue());
    }

    // could be being rebuilt. This happens when a book is loaded but appears
    // to have library copy already.
    m_spine.orderedItems.clear();
    m_spine.items.clear();

    QDomNodeList spineItemList = spineElement.elementsByTagName("itemref");
    for (int j = 0; j < spineItemList.count(); j++) {
      parseSpineItem(spineItemList.at(j));
    }
  }

  //  if (!m_spine.items.isEmpty() && !m_manifest->itemsById.isEmpty()) {
  //    for (auto& spineItem : m_spine.items) {
  //    }
  //  }

  // TODO doctor the image paths to point to local files.
  auto htmlKeys = m_manifest->htmlItems.keys();
  for (int i1 = 0; i1 < htmlKeys.size(); i1++) {
    UniqueString key = htmlKeys.at(i1);
    QSharedPointer<EPubManifestItem> item = m_manifest->htmlItems.value(key);
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

bool
EPubDocument::parseTocFile(QuaZip *archive)
{
  UniqueString tocId = m_spine.tocId;
  QSharedPointer<EPubManifestItem> tocItem = m_manifest->itemsById.value(tocId);
  QString toc_path = tocItem->path;

  archive->setCurrentFile(toc_path);
  QuaZipFile toc_file(archive);
  toc_file.setZip(archive);

  if (!toc_file.open(QIODevice::ReadOnly)) {
    archive->getZipError();
    qDebug() << tr("Unable to open toc file %1").arg(toc_path);
  }

  QByteArray data = toc_file.readAll();
  QDomDocument document;
  document.setContent(data);
  QString formatted_toc_string;
  QDomElement root = document.documentElement();
  QDomNodeList node_list = document.elementsByTagName("docTitle");

  if (!node_list.isEmpty()) {
    QDomNode title_node = node_list.at(0);
    QDomElement title_text = title_node.firstChild().toElement();
    if (title_text.tagName() == "text") {
      formatted_toc_string += TOC_TITLE.arg(title_text.text());
    }
  }
  formatted_toc_string += LIST_START;

  m_tocChapterIndex = -1;
  node_list = document.elementsByTagName("navMap");
  if (!node_list.isEmpty()) {
    QDomNode node = node_list.at(0);
    QDomElement elem = node.toElement();
    QDomElement navpoint = elem.firstChildElement("navPoint");
    while (!navpoint.isNull()) {
      QSharedPointer<EPubTocItem> toc_item =
        parseNavPoint(navpoint, formatted_toc_string);
      m_manifest->tocTtems.insert(toc_item->playorder, toc_item);
      m_manifest->tocPaths.insert(toc_item->source, toc_item);

      handleNestedNavpoints(navpoint, formatted_toc_string);

      navpoint = navpoint.nextSiblingElement("navPoint");
    }
  }

  formatted_toc_string += LIST_END;

  m_manifest->formattedTocString = formatted_toc_string;
  return true;
}

QSharedPointer<EPubTocItem>
EPubDocument::parseNavPoint(QDomElement navpoint, QString& formatted_toc_data)
{
  m_tocChapterIndex++;
  QSharedPointer<EPubTocItem> toc_item =
    QSharedPointer<EPubTocItem>(new EPubTocItem());
  //  QDomNamedNodeMap attributes = navpoint.attributes();
  QString value = navpoint.attribute("class");
  if (!value.isEmpty()) {
    toc_item->tagClass = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("id");
  if (!value.isEmpty()) {
    toc_item->id = UniqueString(value);
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("playOrder");
  if (!value.isEmpty()) {
    toc_item->playorder = value.toInt();
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  QDomElement navlabel = navpoint.firstChildElement("navLabel");
  if (!navlabel.isNull()) {
    QDomElement text = navlabel.firstChild().toElement();
    if (text.tagName() == "text") {
      toc_item->label = text.text();
    }
  }

  QDomElement content = navpoint.firstChildElement("content");
  value = content.attribute("src");
  if (!value.isEmpty()) {
    // if no chapter fragment then add one in.
    int index = value.indexOf("#");
    if (index <= 0) {
      toc_item->source = value;
      toc_item->chapterTag.clear();
    } else {
      QString tag;
      tag = QString("#part%1").arg(m_tocChapterIndex);
      value += tag;
      toc_item->chapterTag = tag;
    }
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  formatted_toc_data += LIST_ITEM.arg(toc_item->source, toc_item->label);

  // parse nested navPoints.
  QDomElement sub_navpoint = navpoint.firstChildElement("navPoint");
  if (!sub_navpoint.isNull()) {
    formatted_toc_data += LIST_START;

    while (!sub_navpoint.isNull()) {
      QSharedPointer<EPubTocItem> sub_item =
        parseNavPoint(sub_navpoint, formatted_toc_data);

      toc_item->subItems.insert(sub_item->playorder, sub_item);
      sub_navpoint = sub_navpoint.nextSiblingElement("navPoint");
    }

    formatted_toc_data += LIST_END;
  }

  return toc_item;
}

void
EPubDocument::handleNestedNavpoints(QDomElement elem,
                                    QString& formatted_toc_string)
{
  QDomElement subpoint = elem.firstChildElement("navPoint");
  while (!subpoint.isNull()) {
    QSharedPointer<EPubTocItem> toc_item =
      parseNavPoint(subpoint, formatted_toc_string);
    m_manifest->tocTtems.insert(toc_item->playorder, toc_item);
    m_manifest->tocPaths.insert(toc_item->source, toc_item);

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
    auto item = QSharedPointer<EPubSpineItem>(new EPubSpineItem());

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
    m_spine.items.insert(item->idref, item);
    m_spine.orderedItems.append(item->idref);
  }
  return true;
}

bool
EPubDocument::parseManifestItem(QuaZip *archive,
                                const QDomNode& manifest_node,
                                const QString current_folder)
{
  auto metadataElement = manifest_node.toElement();
  auto tagName = metadataElement.tagName();
  auto nodeMap = metadataElement.attributes();
  QDomNode node;
  QString name, value;

  if (tagName == "item") {
    auto item = PManifestItem(new EPubManifestItem());
    node = nodeMap.namedItem("href");
    if (!node.isNull()) {
      value = node.nodeValue();
      QString path = QDir::cleanPath(current_folder + value);
      item->href = value;
      item->path = path;
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
      item->mediaType = value.toLatin1();
      if (item->mediaType == "image/gif" || item->mediaType == "image/jpeg" ||
          item->mediaType == "image/png") {

        if (!QImageReader::supportedMimeTypes().contains(
              item->mediaType.toLatin1())) {
          qDebug() << tr("Requested image type %1 is an unsupported type")
                        .arg(QString(item->mediaType));
        }

        archive->setCurrentFile(item->path);
        QuaZipFile image_file(archive);
        image_file.setZip(archive);

        if (!image_file.open(QIODevice::ReadOnly)) {
          //          archive->getZipError();
          qDebug() << tr("Unable to open image file %1").arg(item->path);
        }

        QByteArray data = image_file.readAll();
        QImage image = QImage::fromData(data);
        QFileInfo info(item->href);
        QString path = info.path();
        if (!path.isEmpty()) {
          QDir dir(m_resourcePath);
          dir.mkpath(path);
        }

        QString res_path = m_resourcePath + QDir::separator() + item->href;
        image.save(res_path);
        res_path.prepend(QStringLiteral("file://"));
        m_manifest->images.insert(item->href, res_path);

      } else if (item->mediaType == "image/svg+xml") {
        qDebug();
      } else if (item->mediaType == "application/vnd.ms-opentype" ||
                 item->mediaType == "application/font-woff") {
        m_manifest->fonts.insert(item->href, item);

      } else if (item->mediaType == "application/xhtml+xml") {
        archive->setCurrentFile(item->path);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = archive->getZipError();
          qDebug() << tr("Unable to open container file error %1").arg(error);
          return false;
        }

        QString container(itemFile.readAll());
        // remove xml header string. This will be reinserted by
        // QXmlStreamWriter
        int length = container.trimmed().length();
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
        archive->setCurrentFile(item->path);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = archive->getZipError();
          qDebug() << tr("Unable to open css file error %1").arg(error);
          return false;
        }

        QString css_string(itemFile.readAll());
        css_string.replace("@charset \"", "@charset\"");
        m_manifest->css.insert(item->href, css_string);

      } else if (item->mediaType == "text/javascript") {
        archive->setCurrentFile(item->path);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = archive->getZipError();
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
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);
      item->properties = properties;

      for (QString& prop : properties) {
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
EPubDocument::extractHeadInformationFromHtmlFile(
  QSharedPointer<EPubManifestItem> item,
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
EPubDocument::getFirstTitleOrTemp(QList<QSharedPointer<EPubTitle>> titles)
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
  if (!archive->open(QuaZip::mdUnzip)) {
    qDebug() << tr("Failed to open %1").arg(filename);
    return false;
  }

  // get list of filenames from zip file
  m_files = archive->getFileNameList();
  if (m_files.isEmpty()) {
    qDebug() << tr("Failed to read %1").arg(filename);
    return false;
  }

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

  //  for (QSharedPointer<EPubManifestItem> item :
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
  //    QStringLiteral("<title>"); QSharedPointer<EPubTitle> shared_title =
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
    QSharedPointer<EPubTitle> firstTitle = m_metadata->orderedTitles().first();
    if (!firstTitle.isNull())
      return firstTitle->title;
  }
  return QString();
}

void
EPubDocument::setTitle(const QString& title)
{
  QSharedPointer<EPubTitle> first = m_metadata->orderedTitles().first();
  if (!first.isNull()) {
    first->title = title;
    m_modified = true;
  } else {
    first = QSharedPointer<EPubTitle>(new EPubTitle());
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
QMap<UniqueString, QSharedPointer<EPubManifestItem>>
EPubDocument::pages()
{
  return m_manifest->htmlItems;
}

EPubSpine
EPubDocument::spine()
{
  return m_spine;
}

QList<UniqueString>
EPubDocument::cssKeys()
{
  return m_manifest->css.keys();
}

UniqueStringMap
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

QSharedPointer<EPubManifestItem>
EPubDocument::itemByHref(UniqueString href)
{
  return m_manifest->itemsByHref.value(href);
}

QSharedPointer<Manifest>
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

QString
EPubDocument::buildTocfromHtml()
{
  QString formatted_toc_string = LIST_START;

  QRegularExpression re_anchor_complete(
    "<a[\\s]+([^>]+)>((?:.(?!\\<\\/a\\>))*.)</a>");
  QRegularExpression re_anchor_tag("<a[^>]*>");
  QRegularExpression re_href("href=\\\"[^\"]*\"");
  int anchor_start, pos = 0;

  auto files = m_manifest->htmlItems.values();
  for (QSharedPointer<EPubManifestItem> item : files) {
    auto document_string = item->documentString;
    if (!document_string.isEmpty()) {
      QRegularExpressionMatchIterator i =
        re_anchor_complete.globalMatch(document_string);
      QRegularExpressionMatch anchor_complete_match, anchor_tag_match,
        href_match;
      QString anchor_complete, anchor_tag, href_attr;

      while (i.hasNext()) {
        anchor_complete_match = i.next();
        anchor_complete = anchor_complete_match.captured(0);
        anchor_start =
          anchor_complete_match.capturedStart(0); // start of open anchor tag.

        anchor_tag_match = re_anchor_tag.match(anchor_complete);
        if (anchor_tag_match.hasMatch()) {
          anchor_tag = anchor_tag_match.captured(0);
          anchor_start +=
            anchor_tag_match.capturedLength(0); // end of open anchor tag

          QString text = extractTagText(anchor_start, document_string);

          href_match = re_href.match(anchor_tag);
          if (href_match.hasMatch()) {
            href_attr = href_match.captured(0);
            href_attr = href_attr.mid(6, href_attr.length() - 7);
            QStringList splits = href_attr.split("#", Qt::KeepEmptyParts);
            if (splits.length() == 1) {
              QString filename = splits.at(0);
              //              QList<QSharedPointer<EPubManifestItem>> files =
              //                m_manifest->htmlItems.values();
              for (auto item : files) {
                auto href = item->href;
                if (href == filename) {
                  // TODO add anchor & make anchor tag.
                }
              }
            } else if (!splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing file + anchor points exist.
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0), splits.at(1), text);
            } else if (!splits.at(0).isEmpty() && splits.at(1).isEmpty()) {
              // existing file but no anchor point.
              QString pos_tag = LIST_FILEPOS.arg(pos++);
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0), pos_tag, text);
              // TODO introduce anchor tag
            } else if (splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing anchor tag but no file.
              // TODO find file and add to anchor.
            } else {
              // TODO no existing anchor point ??? not certain we ever get
              // here. may need to move this outside last regex match.
            }
          }
        }
      }
    }
  }

  formatted_toc_string += LIST_END;
  return formatted_toc_string;
}

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
