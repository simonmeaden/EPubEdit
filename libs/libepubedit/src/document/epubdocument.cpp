#include "document/epubdocument.h"

#include "quazip.h"
#include "quazipfile.h"

#include "document/epubmetadata.h"
#include "document/foaf.h"

const QString EPubDocument::METADATA_FOLDER = "META-INF";
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

EPubDocument::EPubDocument()
{
  m_uniqueIdList = QSharedPointer<UniqueStringList>(new UniqueStringList());
  m_metadata = QSharedPointer<EPubMetadata>(new EPubMetadata(m_uniqueIdList));
  m_manifest = QSharedPointer<EPubManifest>(new EPubManifest());
}

EPubDocument::~EPubDocument() {}

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

void
EPubDocument::openDocument(const QString& path)
{
  setFilename(path);
  loadDocument();
}

bool
EPubDocument::loadDocument()
{
  // open the epub as a zip file
  m_archive = new QuaZip(m_filename);
  if (!m_archive->open(QuaZip::mdUnzip)) {
    qDebug() << QObject::QObject::tr("Failed to open %1").arg(m_filename);
    return false;
  }

  // get list of filenames from zip file
  m_files = m_archive->getFileNameList();
  if (m_files.isEmpty()) {
    qDebug() << QObject::QObject::tr("Failed to read %1").arg(m_filename);
    return false;
  }

  // Get and check that the mimetype is correct.
  // According to the standard this must be unencrypted.
  if (!parseMimetype()) {
    return false;
  }

  if (!parseContainer()) {
    return false;
  }

  //  foreach (ManifestItem item, m_manifest->html_items.values()) {
  //    CSSMap css_strings = cssMap();
  //    QString doc_string = item->document_string;
  //    //    if (m_parser->parse(item->id, doc_string, css_strings)) {
  //    //      // TODO use data?
  //    //    }
  //  }

  //  emit q->loadCompleted();
  return true;
}

bool
EPubDocument::parseMimetype()
{
  if (m_files.contains(MIMETYPE_FILE)) {
    m_archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(m_archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      qDebug() << QObject::QObject::tr(
                    "Unable to open mimetype file : error %1")
                    .arg(error);
      return false;
    }

    m_mimetype = mimetypeFile.readAll();
    if (m_mimetype != MIMETYPE) {
      qDebug()
        << QObject::tr("Unexpected mimetype %1").arg(QString(m_mimetype));
    }
  } else {
    qDebug() << QObject::tr("Unable to find mimetype in file");
    return false;
  }
  return true;
}

bool
EPubDocument::parseContainer()
{
  if (m_files.contains(CONTAINER_FILE)) {
    m_archive->setCurrentFile(CONTAINER_FILE);
    QuaZipFile containerFile(m_archive);
    containerFile.setZip(m_archive);

    if (!containerFile.open(QIODevice::ReadOnly)) {
      int error = m_archive->getZipError();
      qDebug() << QObject::QObject::tr("Unable to open container file error %1")
                    .arg(error);
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
          qWarning() << QObject::QObject::tr("Invalid root file entry");
          continue;
        }
        if (parsePackageFile(m_containerFullpath)) {
          return true;
        }
      }
    }

  } else {
    qDebug() << QObject::QObject::tr("Unable to find container information");
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

  qDebug() << QObject::QObject::tr("Unable to find and use any content files");
  return false;
}

bool
EPubDocument::parsePackageFile(QString& fullPath)
{
  m_archive->setCurrentFile(fullPath);
  QuaZipFile contentFile(m_archive);
  contentFile.setZip(m_archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    qDebug() << QObject::QObject::tr(
      "Malformed content file, unable to get content metadata");
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
    auto lineNumber = node.lineNumber();
    // parse package attributes.
    if (name == "version") {
      if (value == "2.0") {
        m_version = 2;
      } else if (value == "3.0") {
        m_version = 3;
      }
    } else if (name == "xmlns") {
      m_packageXmlns = value;
    } else if (name == "unique-identifier") {
      m_metadata->setUniqueIdentifierName(
        m_uniqueIdList->append(node.nodeValue(), node.lineNumber()));
    } else if (name == "xml:lang") {
      m_packageLanguage = value;
    } else if (name == "prefix") { // Only 3.0
      // TODO - handle prefix mapping - may not need this, just store value.
      m_packagePrefix = value;
    } else if (name == "dir") { // Only 3.0
      m_packageDirection = value;
    } else if (name == "id") { // Only 3.0
      m_packageId = m_uniqueIdList->append(value, lineNumber);
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
  QDomNodeList manifestNodeList = packageDocument.elementsByTagName("manifest");
  for (int i = 0; i < manifestNodeList.count(); i++) {
    auto manifestElement = manifestNodeList.at(i).toElement();
    nodeMap = manifestElement.attributes();
    node = nodeMap.namedItem("id");
    if (!node.isNull()) {
      auto lineNumber = node.lineNumber();
      m_manifest->id = m_uniqueIdList->append(node.nodeValue(), lineNumber);
    }
    QDomNodeList manifest_item_list = manifestElement.elementsByTagName("item");

    for (int j = 0; j < manifest_item_list.count(); j++) {
      parseManifestItem(manifest_item_list.at(j), contentFileFolder);
    }
  }

  // Parse out the document guide
  // please note that this has been superceded by landmarks in EPUB 3.0
  auto spineNodeList = packageDocument.elementsByTagName("spine");
  for (int i = 0; i < spineNodeList.count(); i++) {
    auto spineElement = spineNodeList.at(i).toElement();
    auto spineItem = QSharedPointer<EPubSpineItem>(new EPubSpineItem());
    nodeMap = spineElement.attributes();
    node = nodeMap.namedItem("id");
    if (!node.isNull()) { // optional
      auto lineNumber = node.lineNumber();
      m_spine.id = m_uniqueIdList->append(node.nodeValue(), lineNumber);
      node = nodeMap.namedItem("toc");
      if (!node.isNull()) { // optional
        m_spine.tocId = node.nodeValue();
      }
      node = nodeMap.namedItem("page-progression-dir");
      if (!node.isNull()) { // optional
        m_spine.pageProgressionDir = direction(node.nodeValue());
      }

      QDomNodeList spineItemList = spineElement.elementsByTagName("itemref");
      for (int j = 0; j < spineItemList.count(); j++) {
        spineItem = parseSpineItem(spineItemList.at(j), spineItem);
      }

      if (!m_spine.tocId.isEmpty()) { // EPUB2.0 toc
        parseTocFile();
      }

      //    /*
      //     * At this point not all books have anchors added for chapter links.
      //     Calibre
      //     * adds in anchors at the end of the previous chapter. For
      //     simplicities sake I
      //     * am adding them at the start of the new chapter.
      //     * Also remember that some
      //     */
      //    createChapterAnchorPoints(spine_item);
    }
  }

  // TODO doctor the image paths to point to local files.
  for (int i1 = 0; i1 < m_manifest->htmlItems.keys().size(); i1++) {
    UniqueString key = m_manifest->htmlItems.keys().at(i1);
    QSharedPointer<EPubManifestItem> item = m_manifest->htmlItems.value(key);
    QString documentString = item->documentString;
    bool changed = false;
    for (int i2 = 0; i2 < m_manifest->images.size(); i2++) {
      UniqueString imageId = m_manifest->images.keys().at(i2);
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
EPubDocument::parseTocFile()
{
  UniqueString tocId = m_spine.tocId;
  QSharedPointer<EPubManifestItem> tocItem = m_manifest->itemsById.value(tocId);
  QString toc_path = tocItem->path;

  m_archive->setCurrentFile(toc_path);
  QuaZipFile toc_file(m_archive);
  toc_file.setZip(m_archive);

  if (!toc_file.open(QIODevice::ReadOnly)) {
    m_archive->getZipError();
    qDebug()
      << QObject::QObject::tr("Unable to open toc file %1").arg(toc_path);
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
    QString name, value;
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
    //            QLOG_DEBUG(QObject::QObject::tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("id");
  if (!value.isEmpty()) {
    toc_item->id = m_uniqueIdList->append(value, navpoint.lineNumber());
  } else {
    //            QLOG_DEBUG(QObject::QObject::tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  value = navpoint.attribute("playOrder");
  if (!value.isEmpty()) {
    toc_item->playorder = value.toInt();
  } else {
    //            QLOG_DEBUG(QObject::QObject::tr("Warning invalid manifest
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
    //            QLOG_DEBUG(QObject::QObject::tr("Warning invalid manifest
    //            itemref : no idref value"))
  }

  formatted_toc_data += LIST_ITEM.arg(toc_item->source).arg(toc_item->label);

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

QSharedPointer<EPubSpineItem>
EPubDocument::parseSpineItem(const QDomNode& spine_node,
                             QSharedPointer<EPubSpineItem> item)
{
  QDomElement metadataElement = spine_node.toElement();
  QString tag_name = metadataElement.tagName();
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "itemref") {

    // TODO EPUB2 toc element - convert to EPUB3

    node = nodeMap.namedItem("idref");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->idref = value;
    } else {
      qDebug() << QObject::QObject::tr(
        "Warning invalid manifest itemref : no idref value");
    }

    node = nodeMap.namedItem("id");
    if (!node.isNull()) {
      item->id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
    }

    node = nodeMap.namedItem("linear");
    if (!node.isNull()) {
      value = node.nodeValue();
      if (value == "yes" || value == "no") {
        if (value == "yes")
          item->linear = true; // false by default.
      } else {
        qDebug() << QObject::QObject::tr(
                      "Warning invalid manifest itemref : linear MUST be "
                      "either yes or no not %1")
                      .arg(value);
      }
    }

    node = nodeMap.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);

      foreach (QString prop, properties) {
        if (prop == "page-spread-left") {
          item->pageSpreadLeft = true;
        } else if (prop == "page-spread-right") {
          item->pageSpreadRight = true;
        }
      }
    }

    m_spine.items.insert(item->idref, item);
    m_spine.orderedItems.append(item->idref);
  }
  return item;
}

bool
EPubDocument::parseManifestItem(const QDomNode& manifest_node,
                                const QString current_folder)
{
  QDomElement metadataElement = manifest_node.toElement();
  QString tag_name = metadataElement.tagName();
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "item") {
    QSharedPointer<EPubManifestItem> item =
      QSharedPointer<EPubManifestItem>(new EPubManifestItem());
    node = nodeMap.namedItem("href");
    if (!node.isNull()) {
      value = node.nodeValue();
      QString path = QDir::cleanPath(current_folder + value);
      item->href = value;
      item->path = path;
    } else {
      qDebug() << QObject::QObject::tr(
        "Warning invalid manifest item : no href value");
    }

    node = nodeMap.namedItem("id");
    if (!node.isNull()) {
      item->id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
    } else {
      qDebug() << QObject::QObject::tr(
        "Warning invalid manifest item : no id value");
    }

    node = nodeMap.namedItem("media-type");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->mediaType = value.toLatin1();
      if (item->mediaType == "image/gif" || item->mediaType == "image/jpeg" ||
          item->mediaType == "image/png") {

        if (!QImageReader::supportedMimeTypes().contains(item->mediaType)) {
          qDebug() << QString("Requested image type %1 is an unsupported type")
                        .arg(QString(item->mediaType));
        }

        m_archive->setCurrentFile(item->path);
        QuaZipFile image_file(m_archive);
        image_file.setZip(m_archive);

        if (!image_file.open(QIODevice::ReadOnly)) {
          //          m_archive->getZipError();
          qDebug() << QObject::QObject::tr("Unable to open image file %1")
                        .arg(item->path);
        }

        QByteArray data = image_file.readAll();
        QImage image = QImage::fromData(data);
        QFileInfo info(item->href.toString());
        QString path = info.path();
        if (!path.isEmpty()) {
          QDir dir(m_resourcePath);
          dir.mkpath(path);
        }

        QString res_path =
          m_resourcePath + QDir::separator() + item->href.toString();
        image.save(res_path);
        res_path.prepend(QStringLiteral("file://"));
        m_manifest->images.insert(item->href, res_path);

      } else if (item->mediaType == "image/svg+xml") {
        qDebug();
      } else if (item->mediaType == "application/vnd.ms-opentype" ||
                 item->mediaType == "application/font-woff") {
        m_manifest->fonts.insert(item->href, item);

      } else if (item->mediaType == "application/xhtml+xml") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          qDebug() << QObject::QObject::tr(
                        "Unable to open container file error %1")
                        .arg(error);
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
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          qDebug() << QObject::QObject::tr("Unable to open css file error %1")
                        .arg(error);
          return false;
        }

        QString css_string(itemFile.readAll());
        css_string.replace("@charset \"", "@charset\"");
        m_manifest->css.insert(item->href, css_string);

      } else if (item->mediaType == "text/javascript") {
        m_archive->setCurrentFile(item->path);
        QuaZipFile itemFile(m_archive);
        itemFile.setZip(m_archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = m_archive->getZipError();
          qDebug() << QObject::tr("Unable to open javascript file error %1")
                        .arg(error);
          return false;
        }

        QString js_string(itemFile.readAll());
        m_manifest->javascript.insert(item->id, js_string);
      }
    } else {
      qDebug() << QObject::tr(
        "Warning invalid manifest item : no media-type value");
    }

    node = nodeMap.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);
      item->properties = properties;

      for (QString prop : properties) {
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
EPubDocument::saveDocument(const QString& path)
{
  if (m_modified) {
    m_filename = path;
    saveDocumentToFile();
  }
}

bool
EPubDocument::saveDocumentToFile()
{
  QFileInfo info;
  if (m_filename.isEmpty())
    info = QFileInfo(m_filename);
  else
    info = QFileInfo(m_filename);
  QString path = info.path();
  QString name = info.fileName();
  //  path = path + QDir::separator() + "temp";
  QDir dir;
  dir.mkpath(path);

  QuaZip temp_file(path + QDir::separator() + name);
  if (temp_file.open(QuaZip::mdAdd)) {

    if (!writeMimetype(&temp_file)) {
      return false;
    }
    if (!writeContainer(&temp_file)) {
      return false;
    }
    // TODO the rest
  }
  temp_file.close();

  // TODO backup/rename etc.
  // TODO remove temp file/dir - maybe on application close/cleanup

  return true;
}

bool
EPubDocument::writeMimetype(QuaZip* save_zip)
{
  QuaZipFile mimetype_file(save_zip);

  if (!mimetype_file.open(QIODevice::WriteOnly,
                          QuaZipNewInfo(MIMETYPE_FILE, MIMETYPE_FILE))) {
    int error = save_zip->getZipError();
    qDebug()
      << QObject::tr("Unable to write mimetype file : error %1").arg(error);
    return false;
  }

  qint64 size = mimetype_file.write(MIMETYPE);
  if (size != m_mimetype.size()) {
    qDebug() << QObject::tr("Unexpected mimetype size %1 should be %2")
                  .arg(size)
                  .arg(m_mimetype.size());
    return false;
  }
  return true;
}

bool
EPubDocument::writeContainer(QuaZip* save_zip)
{
  QuaZipFile container_file(save_zip);

  if (!container_file.open(QIODevice::WriteOnly,
                           QuaZipNewInfo(CONTAINER_FILE, CONTAINER_FILE))) {
    int error = save_zip->getZipError();
    qDebug()
      << QObject::tr("Unable to write container file : error %1").arg(error);
    return false;
  }

  QXmlStreamWriter xml_writer(&container_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument("1.0");

  xml_writer.writeStartElement("container");
  xml_writer.writeAttribute("version", m_containerVersion);
  xml_writer.writeAttribute("xmlns", m_containerXmlns);

  xml_writer.writeStartElement("rootfiles");
  xml_writer.writeStartElement("rootfile");
  xml_writer.writeAttribute("full-path", m_containerFullpath);
  xml_writer.writeAttribute("media-type", m_containerMediatype);
  xml_writer.writeEndElement();

  xml_writer.writeEndElement();
  xml_writer.writeEndElement();

  xml_writer.writeEndDocument();
  container_file.close();

  writePackageFile(save_zip);

  for (QSharedPointer<EPubManifestItem> item : m_manifest->htmlItems.values()) {
    QuaZipFile item_file(save_zip);
    item_file.setFileName(item->path);

    if (!item_file.open(QIODevice::WriteOnly,
                        QuaZipNewInfo(item->path, item->path))) {
      int error = save_zip->getZipError();
      qDebug()
        << QObject::tr("Unable to write html/xhtml file : error %1").arg(error);
      return false;
    }

    /* We have to use QTextStream rather than QXmlStreamWriter because the xml
     * stream escapes '<', '>' and several other characters. */
    QTextStream out(&item_file);
    out << QStringLiteral("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
    out << HTML_DOCTYPE << "\n";
    out << QStringLiteral("<head>\n");
    out << QStringLiteral("<title>");
    QSharedPointer<EPubTitle> shared_title =
      m_metadata->orderedTitles().first();
    if (shared_title) {
      out << shared_title->title;
    }
    out << QStringLiteral("</title>\n");
    out << QStringLiteral("<meta http-equiv=\"Content-Type\" "
                          "content=\"text/html; charset=utf-8\"/>\n");
    foreach (QString href, item->cssLinks) {
      out << QString(
               "<link href=\"%1\" rel=\"stylesheet\" type=\"text/css\"/>\n")
               .arg(href);
    }
    out << QStringLiteral("</head>\n");
    out << QStringLiteral("<body");
    if (!item->bodyClass.isEmpty()) {
      out << QString(" class=\"%1\">\n").arg(item->bodyClass);
    } else {
      out << QStringLiteral(">\n");
    }
    out << item->documentString;
    out << QStringLiteral("</body>\n");
    out << QStringLiteral("</html>\n");
    out.flush();

    item_file.close();
  }
  return true;
}

bool
EPubDocument::writePackageFile(QuaZip* save_zip)
{
  QuaZipFile package_file(save_zip);

  if (!package_file.open(
        QIODevice::WriteOnly,
        QuaZipNewInfo(m_containerFullpath, m_containerFullpath))) {
    int error = save_zip->getZipError();
    qDebug()
      << QObject::tr("Unable to write container file : error %1").arg(error);
    return false;
  }

  QXmlStreamWriter xml_writer(&package_file);
  xml_writer.setAutoFormatting(true);
  xml_writer.writeStartDocument("1.0");

  xml_writer.writeStartElement("package");
  xml_writer.writeAttribute("version", "3.0");
  xml_writer.writeAttribute("unique-identifier",
                            m_metadata->uniqueIdentifierName().toString());
  xml_writer.writeAttribute("xmlns", m_packageXmlns);
  if (m_metadata->isFoaf()) {
    xml_writer.writeAttribute("prefix", Foaf::prefix());
  }
  if (!m_packageLanguage.isEmpty()) { // Optional in EPUB 2.0
    xml_writer.writeAttribute("xml:lang", m_packageLanguage);
  }
  if (!m_packagePrefix.isEmpty()) {
    xml_writer.writeAttribute("prefix", m_packagePrefix);
  }
  if (!m_packageDirection.isEmpty()) {
    xml_writer.writeAttribute("dir", m_packageDirection);
  }
  if (!m_packageId.isEmpty()) {
    xml_writer.writeAttribute("id", m_packageId.toString());
  }

  m_metadata->write(&xml_writer);

  xml_writer.writeEndElement();
  xml_writer.writeEndDocument();

  package_file.close();

  // TODO - the rest of the saves.
  return true;
}

EPubContents*
EPubDocument::cloneData()
{
  //  EPubContents* contents = new EPubContents();
  //  contents->m_loaded = m_loaded;

  //  return contents;
  return nullptr;
}

void
EPubDocument::setClonedData(EPubContents* cloneData)
{
  //  Q_Q(EPubDocument);

  //  QElapsedTimer timer;
  //  timer.start();

  //  m_container = clone->m_container;
  //  m_svgs = clone->m_svgs;
  //  m_renderedSvgs = clone->m_renderedSvgs;
  //  m_currentItem = clone->m_currentItem;
  //  m_loaded = clone->m_loaded;

  //  QTextCursor cursor(q_ptr);
  //  cursor.movePosition(QTextCursor::End);

  //  QStringList items = m_container->items();

  //  QString cover = m_container->standardPage(EPubPageReference::CoverPage);
  //  if (cover.isEmpty()) {
  //    items.prepend(cover);
  //    QLOG_DEBUG(QString("Cover is empty %1").arg(cover))
  //  }

  //  QTextBlockFormat pageBreak;
  //  pageBreak.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
  //  for (const QString& chapter : items) {
  //    m_currentItem = m_container->epubItem(chapter);
  //    if (m_currentItem.path.isEmpty()) {
  //      continue;
  //    }

  //    QSharedPointer<QuaZipFile> zipFile =
  //        m_container->zipFile(m_currentItem.path);
  //    if (!zipFile) {
  //      QLOG_WARN(QString("Unable to get zipped file for chapter %1")
  //                    .arg(m_currentItem.path))
  //      continue;
  //    }

  //    QByteArray itemdata = zipFile->readAll();
  //    if (itemdata.isEmpty()) {
  //      QLOG_WARN(QString("Got an empty document"))
  //      continue;
  //    }
  //    q->setBaseUrl(QUrl(m_currentItem.path));
  //    QDomDocument newDocument;
  //    newDocument.setContent(itemdata);
  //    // TOD extract text for processing
  //    fixImages(newDocument);

  //    cursor.insertText(newDocument.toString());
  //    cursor.insertBlock(pageBreak);
  //  }

  //  q->setBaseUrl(QUrl());
  //  m_loaded = true;

  //  emit q->loadCompleted();
  //  //  QLOG_DEBUG(QString("Done in %1 mS").arg(timer.elapsed()))}
}

QString
EPubDocument::filename()
{
  return m_filename;
}

void
EPubDocument::setFilename(const QString& filename)
{
  m_filename = filename;
}

QString
EPubDocument::tocAsString()
{
  return m_manifest->formattedTocString;
}

QString
EPubDocument::title()
{
  QSharedPointer<EPubTitle> first = m_metadata->orderedTitles().first();
  if (!first.isNull())
    return first->title;
  else {
    return QString();
  }
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

QSharedPointer<EPubMetadata>
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

QStringList
EPubDocument::spine()
{
  return m_spine.orderedItems;
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

QSharedPointer<UniqueStringList>
EPubDocument::uniqueIdList() const
{
  return m_uniqueIdList;
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

  for (QSharedPointer<EPubManifestItem> item : m_manifest->htmlItems.values()) {
    QString document_string = item->documentString;
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
              QList<QSharedPointer<EPubManifestItem>> files =
                m_manifest->htmlItems.values();
              for (QSharedPointer<EPubManifestItem> item : files) {
                UniqueString href = item->href;
                if (href == filename) {
                  // TODO add anchor & make anchor tag.
                }
              }
            } else if (!splits.at(0).isEmpty() && !splits.at(1).isEmpty()) {
              // existing file + anchor points exist.
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0)).arg(splits.at(1)).arg(text);
            } else if (!splits.at(0).isEmpty() && splits.at(1).isEmpty()) {
              // existing file but no anchor point.
              QString pos_tag = LIST_FILEPOS.arg(pos++);
              formatted_toc_string +=
                LIST_BUILD_ITEM.arg(splits.at(0)).arg(pos_tag).arg(text);
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
