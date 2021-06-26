#include "libepubedit/epubedit.h"

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

#include "forms/metadataform.h"
#include "libepubedit/ebookcommon.h"
#include "libepubedit/ebookmetadata.h"
#include "util/csvsplitter.h"

const QString EPubEdit::MIMETYPE_FILE = "mimetype";
const QByteArray EPubEdit::MIMETYPE = "application/epub+zip";
const QString EPubEdit::CONTAINER_FILE = "META-INF/container.xml";
const QString EPubEdit::TOC_TITLE = "<h2>%1</h2>";
const QString EPubEdit::LIST_START = "<html><body><ul>";
const QString EPubEdit::LIST_END = "</ul></body></html>";
const QString EPubEdit::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
const QString EPubEdit::LIST_BUILD_ITEM = "<li><a href=\"%1#%2\">%3</li>";
const QString EPubEdit::LIST_FILEPOS = "position%1";
const QString EPubEdit::HTML_DOCTYPE =
  "<!DOCTYPE html PUBLIC "
  "\"-//W3C//DTD XHTML 1.1//EN\" "
  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
const QString EPubEdit::XML_HEADER = "<?xml version='1.0' encoding='utf-8'?>";
const QString EPubEdit::HTML_XMLNS = "http://www.w3.org/1999/xhtml";

EPubEdit::EPubEdit(QWidget* parent)
  : QTabWidget(parent)
{
  initGui();
}

bool
EPubEdit::loadEpubDocument(const QString& filename)
{
  // epub file are basically a zipped set of files, primarily
  // html files.
  // open the zip file
  auto archive = new QuaZip(filename);
  if (!archive->open(QuaZip::mdUnzip)) {
    qDebug() << tr("Failed to open %1").arg(filename);
    return false;
  }

  // read the internal file list
  auto files = archive->getFileNameList();
  if (files.isEmpty()) {
    qDebug() << tr("Empty EPUB file").arg(filename);
    return false;
  }

  // Get and check that the mimetype is correct.
  // According to the standard this must be unencrypted.
  if (!parseMimetype(files, archive)) {
    return false;
  }

  if (!parseContainer(files, archive)) {
    return false;
  }

  updateMetadataForm();

  return true;
}

void
EPubEdit::updateMetadataForm()
{
  m_metadataForm->setTitles(m_metadata->orderedTitles());
  m_metadataForm->setAuthors(m_metadata->creatorList());
}

void
EPubEdit::metadataHasChanged()
{
  if (m_metadataForm->titlesModified()) {
    auto titles = m_metadataForm->titles();
    m_metadata->setOrderedTitles(titles);
  }

  if (m_metadataForm->authorsModified()) {
    auto authors = m_metadataForm->authors();
    m_metadata->setCreatorList(authors);
  }
}

void
EPubEdit::initGui()
{
  m_metadataForm = new MetadataForm(this);
  m_metadataForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_metadataForm,
          &MetadataForm::dataHasChanged,
          this,
          &EPubEdit::metadataHasChanged);
  addTab(m_metadataForm, "Metadata");
}

bool
EPubEdit::parseMimetype(QStringList files, QuaZip* archive)
{
  if (files.contains(MIMETYPE_FILE)) {
    archive->setCurrentFile(MIMETYPE_FILE);
    QuaZipFile mimetypeFile(archive);

    if (!mimetypeFile.open(QIODevice::ReadOnly)) {
      int error = archive->getZipError();
      qDebug() << tr("Unable to open mimetype file : error %1").arg(error);
      return false;
    }

    auto mimetype = mimetypeFile.readAll();
    if (mimetype != MIMETYPE) {
      qDebug() << tr("Unexpected mimetype %1").arg(QString(mimetype));
    }
  } else {
    qDebug() << tr("Unable to find mimetype in file");
    return false;
  }
  return true;
}

bool
EPubEdit::parseContainer(QStringList files, QuaZip* archive)
{
  if (files.contains(CONTAINER_FILE)) {
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
    QDomNamedNodeMap att_map = rootElem.attributes();
    auto containerVersion = rootElem.attribute("version");
    auto containerXmlns = rootElem.attribute("xmlns");

    QDomNodeList rootFiles = rootElem.elementsByTagName("rootfiles");
    if (rootFiles.size() > 0) {
      QDomElement rootfilesElem = rootFiles.at(0).toElement();
      QDomNodeList rootNodes = rootElem.elementsByTagName("rootfile");
      for (int i = 0; i < rootNodes.count(); i++) {
        QDomElement rootElement = rootNodes.at(i).toElement();
        auto containerFullpath = rootElement.attribute("full-path");
        auto containerMediatype = rootElement.attribute("media-type");
        if (containerFullpath.isEmpty()) {
          qWarning() << tr("Invalid root file entry");
          continue;
        }
        if (parsePackageFile(containerFullpath, archive)) {
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
EPubEdit::parsePackageFile(QString& fullpath, QuaZip* archive)
{
  archive->setCurrentFile(fullpath);
  QuaZipFile contentFile(archive);
  contentFile.setZip(archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    qDebug() << tr("Malformed content file, unable to get content metadata");
    return false;
  }

  QMap<QString, DomDocument> map;
  auto currentRootfile = map;
  m_rootfiles.insert(fullpath, mCurrentRootfile);

  QString content(contentFile.readAll());

  DomDocument package_document(new QDomDocument());
  // handles more than one package.
  mCurrentRootfile.insert(fullpath, package_document);

  package_document->setContent(content, true); // turn on namespace processing
  // parse root element attributes.
  QDomElement rootElement = package_document->documentElement();
  QDomNamedNodeMap node_map = rootElement.attributes();
  QDomNode node;
  for (int i = 0; i < node_map.size(); i++) {
    QDomNode node = node_map.item(i);
    QString name = node.nodeName();
    QString value = node.nodeValue().toLower();
    if (!m_metadata)
      m_metadata = Metadata(new EBookMetadata());
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
      m_metadata->setUniqueIdentifierName(value);
    } else if (name == "xml:lang") {
      m_packageLanguage = value;
    } else if (name == "prefix") { // Only 3.0
      // TODO - handle prefix mapping - may not need this, just store value.
      m_packagePrefix = value;
    } else if (name == "dir") { // Only 3.0
      m_packageDirection = value;
    } else if (name == "id") { // Only 3.0
      m_packageId = value;
    } else if (name == "prefix") {
      m_metadata->setIsFoaf(true);
    }
  }

  // parse metadata.
  QDomNodeList metadataNodeList =
    package_document->elementsByTagName("metadata");
  m_metadata->parse(metadataNodeList);

  // Extract current path, for resolving relative paths
  QString contentFileFolder;
  int separatorIndex = fullpath.lastIndexOf('/');
  if (separatorIndex > 0) {
    contentFileFolder = fullpath.left(separatorIndex + 1);
  }

  // Parse out all the components/items in the epub
  // should only have one manifest.
  QDomNodeList manifestNodeList =
    package_document->elementsByTagName("manifest");
  for (int i = 0; i < manifestNodeList.count(); i++) {
    QDomElement manifest_element = manifestNodeList.at(i).toElement();
    node_map = manifest_element.attributes();
    node = node_map.namedItem("id");
    if (!node.isNull()) {
      m_manifest->id = node.nodeValue();
    }
    QDomNodeList manifest_item_list =
      manifest_element.elementsByTagName("item");

    for (int j = 0; j < manifest_item_list.count(); j++) {
      parseManifestItem(manifest_item_list.at(j), contentFileFolder, archive);
    }
  }

  // Parse out the document guide
  // please note that this has been superceded by landmarks in EPUB 3.0
  QDomNodeList spine_node_list = package_document->elementsByTagName("spine");
  for (int i = 0; i < spine_node_list.count(); i++) {
    QDomElement spine_element = spine_node_list.at(i).toElement();
    SpineItem spine_item = SpineItem(new EBookSpineItem());
    node_map = spine_element.attributes();
    node = node_map.namedItem("id");
    if (!node.isNull()) { // optional
      m_spine->id = node.nodeValue();
    }
    node = node_map.namedItem("toc");
    if (!node.isNull()) { // optional
      m_spine->toc = node.nodeValue();
    }
    node = node_map.namedItem("page-progression-dir");
    if (!node.isNull()) { // optional
      m_spine->page_progression_dir = node.nodeValue();
    }

    QDomNodeList spineItemList = spine_element.elementsByTagName("itemref");
    for (int j = 0; j < spineItemList.count(); j++) {
      spine_item = parseSpineItem(spineItemList.at(j), spine_item);
    }

    if (!m_spine->toc.isEmpty()) { // EPUB2.0 toc
      parseTocFile(archive);
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

  //  QDomNodeList guide_node_list =
  //  package_document->elementsByTagName("guide"); for (int i = 0; i <
  //  guide_node_list.count(); i++) {
  //    QDomElement spine_element = guide_node_list.at(i).toElement();
  //    SharedGuideItem item = SharedGuideItem(new EPubGuideItem());
  //    node_map = spine_element.attributes();
  //    node = node_map.namedItem("id");
  //    if (!node.isNull()) { // optional
  //      m_spine->id = node.nodeValue();
  //    }
  //    node = node_map.namedItem("toc");
  //    if (!node.isNull()) { // optional
  //      m_spine->toc = node.nodeValue();
  //    }
  //    node = node_map.namedItem("page-progression-dir");
  //    if (!node.isNull()) { // optional
  //      m_spine->page_progression_dir = node.nodeValue();
  //    }

  //    QDomNodeList spineItemList = spine_element.elementsByTagName("itemref");
  //    for (int j = 0; j < spineItemList.count(); j++) {
  //      parseSpineItem(spineItemList.at(j));
  //    }
  //  }

  //    parseGuideItem(); // this has been superceded by landmarks.
  //  parseLandmarkItem();
  //  parseBindingsItem(); // Tis is used for non-epb standard media types.

  // TODO doctor the image paths to point to local files.
  for (int i1 = 0; i1 < m_manifest->htmlItems.keys().size(); i1++) {
    QString key = m_manifest->htmlItems.keys().at(i1);
    ManifestItem item = m_manifest->htmlItems.value(key);
    QString document_string = item->document_string;
    bool changed = false;
    for (int i2 = 0; i2 < m_manifest->images.size(); i2++) {
      QString image_id = m_manifest->images.keys().at(i2);
      QString image_path = m_manifest->images.value(image_id);
      if (document_string.contains(image_id)) {
        document_string.replace(image_id, image_path, Qt::CaseSensitive);
        changed = true;
      }
    }
    if (changed) {
      item->document_string = document_string;
    }
  }
  return true;
}

SpineItem
EPubEdit::parseSpineItem(const QDomNode& spine_node, SpineItem item)
{
  QDomElement metadata_element = spine_node.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "itemref") {

    // TODO EPUB2 toc element - convert to EPUB3

    node = node_map.namedItem("idref");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->idref = value;
    } else {
      qDebug() << tr("Warning invalid manifest itemref : no idref value");
    }

    node = node_map.namedItem("id");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->id = value;
    }

    node = node_map.namedItem("linear");
    if (!node.isNull()) {
      value = node.nodeValue();
      if (value == "yes" || value == "no") {
        if (value == "yes")
          item->linear = true; // false by default.
      } else {
        qDebug() << tr("Warning invalid manifest itemref : linear MUST be "
                       "either yes or no not %1")
                      .arg(value);
      }
    }

    node = node_map.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);

      foreach (QString prop, properties) {
        if (prop == "page-spread-left") {
          item->page_spread_left = true;
        } else if (prop == "page-spread-right") {
          item->page_spread_right = true;
        }
      }
    }

    m_spine->items.insert(item->idref, item);
    m_spine->ordered_items.append(item->idref);
  }
  return item;
}

bool
EPubEdit::parseTocFile(QuaZip* archive)
{
  QString toc_id = m_spine->toc;
  ManifestItem toc_item = m_manifest->itemsById.value(toc_id);
  QString toc_path = toc_item->path;

  archive->setCurrentFile(toc_path);
  QuaZipFile toc_file(archive);
  toc_file.setZip(archive);

  if (!toc_file.open(QIODevice::ReadOnly)) {
    archive->getZipError();
    qDebug() << tr("Unable to open toc file %1").arg(toc_path);
  }

  QByteArray data = toc_file.readAll();
  DomDocument document = DomDocument(new QDomDocument());
  document->setContent(data);
  QString formatted_toc_string;
  QDomElement root = document->documentElement();
  QDomNodeList node_list = document->elementsByTagName("docTitle");

  if (!node_list.isEmpty()) {
    QDomNode title_node = node_list.at(0);
    QDomElement title_text = title_node.firstChild().toElement();
    if (title_text.tagName() == "text") {
      formatted_toc_string += TOC_TITLE.arg(title_text.text());
    }
  }
  formatted_toc_string += LIST_START;

  m_tocChapterIndex = -1;
  node_list = document->elementsByTagName("navMap");
  if (!node_list.isEmpty()) {
    QString name, value;
    QDomNode node = node_list.at(0);
    QDomElement elem = node.toElement();
    QDomElement navpoint = elem.firstChildElement("navPoint");
    while (!navpoint.isNull()) {
      TocItem toc_item = parseNavPoint(navpoint, formatted_toc_string);
      m_manifest->tocItems.insert(toc_item->playorder, toc_item);
      m_manifest->tocPaths.insert(toc_item->source, toc_item);

      handleNestedNavpoints(navpoint, formatted_toc_string);

      navpoint = navpoint.nextSiblingElement("navPoint");
    }
  }

  formatted_toc_string += LIST_END;

  m_manifest->formattedTocString = formatted_toc_string;
  return true;
}

void
EPubEdit::handleNestedNavpoints(QDomElement elem, QString& formatted_toc_string)
{
  QDomElement subpoint = elem.firstChildElement("navPoint");
  while (!subpoint.isNull()) {
    TocItem toc_item = parseNavPoint(subpoint, formatted_toc_string);
    m_manifest->tocItems.insert(toc_item->playorder, toc_item);
    m_manifest->tocPaths.insert(toc_item->source, toc_item);

    handleNestedNavpoints(subpoint, formatted_toc_string);

    subpoint = elem.nextSiblingElement("navPoint");
  }
}

TocItem
EPubEdit::parseNavPoint(QDomElement navpoint, QString& formatted_toc_data)
{
  m_tocChapterIndex++;
  TocItem toc_item = TocItem(new EBookTocItem());
  //  QDomNamedNodeMap attributes = navpoint.attributes();
  QString value = navpoint.attribute("class");
  if (!value.isEmpty()) {
    toc_item->tag_class = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  value = navpoint.attribute("id");
  if (!value.isEmpty()) {
    toc_item->id = value;
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  value = navpoint.attribute("playOrder");
  if (!value.isEmpty()) {
    toc_item->playorder = value.toInt();
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
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
      toc_item->chapter_tag.clear();
    } else {
      QString tag;
      tag = QString("#part%1").arg(m_tocChapterIndex);
      value += tag;
      toc_item->chapter_tag = tag;
    }
  } else {
    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
    //            idref value"))
  }

  formatted_toc_data += LIST_ITEM.arg(toc_item->source).arg(toc_item->label);

  // parse nested navPoints.
  QDomElement sub_navpoint = navpoint.firstChildElement("navPoint");
  if (!sub_navpoint.isNull()) {
    formatted_toc_data += LIST_START;

    while (!sub_navpoint.isNull()) {
      TocItem sub_item = parseNavPoint(sub_navpoint, formatted_toc_data);

      toc_item->sub_items.insert(sub_item->playorder, sub_item);
      sub_navpoint = sub_navpoint.nextSiblingElement("navPoint");
    }

    formatted_toc_data += LIST_END;
  }

  return toc_item;
}

bool
EPubEdit::parseManifestItem(const QDomNode& manifest_node,
                            const QString current_folder,
                            QuaZip* archive)
{
  QDomElement metadata_element = manifest_node.toElement();
  QString tag_name = metadata_element.tagName();
  QDomNamedNodeMap node_map = metadata_element.attributes();
  QDomNode node;
  QString name, value;

  if (tag_name == "item") {
    ManifestItem item = ManifestItem(new EBookManifestItem());
    node = node_map.namedItem("href");
    if (!node.isNull()) {
      value = node.nodeValue();
      QString path = QDir::cleanPath(current_folder + value);
      item->href = value;
      item->path = path;
    } else {
      qDebug() << tr("Warning invalid manifest item : no href value");
    }

    node = node_map.namedItem("id");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->id = value;
    } else {
      qDebug() << tr("Warning invalid manifest item : no id value");
    }

    node = node_map.namedItem("media-type");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->media_type = value.toLatin1();
      if (item->media_type == "image/gif" || item->media_type == "image/jpeg" ||
          item->media_type == "image/png") {

        if (!QImageReader::supportedMimeTypes().contains(item->media_type)) {
          qDebug() << QString("Requested image type %1 is an unsupported type")
                        .arg(QString(item->media_type));
        }

        archive->setCurrentFile(item->path);
        QuaZipFile image_file(archive);
        image_file.setZip(archive);

        if (!image_file.open(QIODevice::ReadOnly)) {
          //          m_archive->getZipError();
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

      } else if (item->media_type == "image/svg+xml") {
        qDebug();
      } else if (item->media_type == "application/vnd.ms-opentype" ||
                 item->media_type == "application/font-woff") {
        m_manifest->fonts.insert(item->href, item);

      } else if (item->media_type == "application/xhtml+xml") {
        archive->setCurrentFile(item->path);
        QuaZipFile itemFile(archive);
        itemFile.setZip(archive);

        if (!itemFile.open(QIODevice::ReadOnly)) {
          int error = archive->getZipError();
          qDebug() << tr("Unable to open container file error %1").arg(error);
          return false;
        }

        QString container(itemFile.readAll());
        // remove xml header string. This will be reinserted by QXmlStreamWriter
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
        item->document_string = container;
        m_manifest->htmlItems.insert(item->id, item);
      } else if (item->media_type == "text/css") {
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
        m_manifest->css->insert(item->href, css_string);

      } else if (item->media_type == "text/javascript") {
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

    node = node_map.namedItem("properties");
    if (!node.isNull()) {
      name = node.nodeName();
      value = node.nodeValue();
      // space separated list
      QStringList properties = value.split(' ', Qt::SkipEmptyParts);
      item->properties = properties;

      foreach (QString prop, properties) {
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
          item->non_standard_properties.insert(name, value);
        }
      }
    }

    node = node_map.namedItem("fallback");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->fallback = value;
    }

    node = node_map.namedItem("media-overlay");
    if (!node.isNull()) {
      value = node.nodeValue();
      item->media_overlay = value;
      m_manifest->mediaOverlay.insert(item->id, item);
    }

    m_manifest->itemsById.insert(item->id, item);
    m_manifest->itemsByHref.insert(item->href, item);
  }
  return true;
}

void
EPubEdit::extractHeadInformationFromHtmlFile(ManifestItem item,
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
                item->css_links.append(href);
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
      item->body_class = att;
    }
  }
}
