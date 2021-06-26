#include "epubpackage.h"

#include "quazip.h"
#include "quazipfile.h"

EPubPackage::EPubPackage(const QString& full_path,
                         const QString& media_type,
                         QObject* parent)
  : QObject(parent)
  , m_fullPath(full_path)
  , m_mediaType(media_type)
{}

// Metadata
// EPubPackage::parseMetadata(const QDomNode& node, int version)
//{
//  Metadata metadata(new EPubMetadata(this));
//  auto map = node.attributes();
//  QDomNode att;
//  if (!(att = map.namedItem("xmlns:dc")).isNull()) { // should be only value.
//    metadata->setValue(EPubMetadata::XmlnsDc, att.nodeValue());
//  }
//  if (!(att = map.namedItem("xmlns:opf")).isNull()) { // should be only value.
//    metadata->setValue(EPubMetadata::XmlnsOpf, att.nodeValue());
//  }

//  auto children = node.childNodes();
//  for (int i = 0; i < children.size(); i++) {
//    auto n = children.item(i);
//    if (n.nodeName() == "dc:title") {
//      DCTITLE title;
//      auto titleMap = n.attributes();
//      if (!(att = titleMap.namedItem("id")).isNull()) {
//        metadata->setBaseValue(EPubMetadata::DcTitleId, att.nodeValue(),
//        title);
//      }
//      if (!(att = titleMap.namedItem("dir")).isNull()) {
//        metadata->setBaseValue(
//          EPubMetadata::DcTitleDir, att.nodeValue(), title);
//      }
//      if (!(att = titleMap.namedItem("xml:lang")).isNull()) {
//        metadata->setBaseValue(
//          EPubMetadata::DcTitleXmlLang, att.nodeValue(), title);
//      }
//      if (version > 30) {
//        if (!(att = titleMap.namedItem("opf:alt-rep")).isNull()) {
//          metadata->setBaseValue(
//            EPubMetadata::OpfAltRep, att.nodeValue(), title);
//        }
//        if (!(att = titleMap.namedItem("opf:alt-rep-lang")).isNull()) {
//          metadata->setBaseValue(
//            EPubMetadata::OpfAltRepLang, att.nodeValue(), title);
//        }
//        if (!(att = titleMap.namedItem("opf:file-as")).isNull()) {
//          metadata->setBaseValue(
//            EPubMetadata::OpfFileAs, att.nodeValue(), title);
//        }
//      }
//      metadata->setBaseValue(EPubMetadata::DcTitleValue, n.nodeValue(),
//      title); metadata->setBase(EPubMetadata::Title, title);
//    }
//  }
//  return metadata;
//}

bool
EPubPackage::parse(QuaZip* archive)
{
  archive->setCurrentFile(m_fullPath);
  QuaZipFile contentFile(archive);
  contentFile.setZip(archive);

  if (!contentFile.open(QIODevice::ReadOnly)) {
    qDebug() << tr("Malformed content file, unable to get content metadata");
    return false;
  }

  QString content(contentFile.readAll());
  QDomDocument doc;
  doc.setContent(content);
  QDomElement rootElement = doc.documentElement();
  QDomNamedNodeMap nodeMap = rootElement.attributes();
  int i;
  for (i = 0; i < nodeMap.size(); i++) {
    QDomNode node = nodeMap.item(i);
    QString name = node.nodeName();
    QString value = node.nodeValue().toLower();
    if (name == "xmlns") {
      m_xmlns = value;
    } else if (name == "version") {
      if (value == "3.0")
        m_version = 30;
      else if (value == "3.1")
        m_version = 31;
      else if (value == "3.2") {
        m_version = 32;
      } else if (name == "xml:lang") {
        m_xmlLang = value;
      } else if (name == "unique-identifier") {
        m_uniqueIdentifier = value;
      } else if (name == "prefix") {
        m_prefix = value;
      }
    }

    auto children = rootElement.childNodes();
    for (i = 0; i < children.size(); i++) {
      auto node = children.at(i);
      auto name = node.nodeName();
      if (name == "metadata") {
        Metadata metadata(new EPubMetadata(this));
        metadata->parse(node, m_version);
      }
    }
  }

  //    // parse metadata.
  //    QDomNodeList metadataNodeList =
  //      package_document->elementsByTagName("metadata");
  //    m_metadata->parse(metadataNodeList);

  //    // Extract current path, for resolving relative paths
  //    QString contentFileFolder;
  //    int separatorIndex = fullpath.lastIndexOf('/');
  //    if (separatorIndex > 0) {
  //      contentFileFolder = fullpath.left(separatorIndex + 1);
  //    }

  //    // Parse out all the components/items in the epub
  //    // should only have one manifest.
  //    QDomNodeList manifestNodeList =
  //      package_document->elementsByTagName("manifest");
  //    for (int i = 0; i < manifestNodeList.count(); i++) {
  //      QDomElement manifest_element = manifestNodeList.at(i).toElement();
  //      node_map = manifest_element.attributes();
  //      node = node_map.namedItem("id");
  //      if (!node.isNull()) {
  //        m_manifest->id = node.nodeValue();
  //      }
  //      QDomNodeList manifest_item_list =
  //        manifest_element.elementsByTagName("item");

  //      for (int j = 0; j < manifest_item_list.count(); j++) {
  //        parseManifestItem(manifest_item_list.at(j), contentFileFolder,
  //        archive);
  //      }
  //    }

  //    // Parse out the document guide
  //    // please note that this has been superceded by landmarks in EPUB 3.0
  //    QDomNodeList spine_node_list =
  //    package_document->elementsByTagName("spine"); for (int i = 0; i <
  //    spine_node_list.count(); i++) {
  //      QDomElement spine_element = spine_node_list.at(i).toElement();
  //      SpineItem spine_item = SpineItem(new EBookSpineItem());
  //      node_map = spine_element.attributes();
  //      node = node_map.namedItem("id");
  //      if (!node.isNull()) { // optional
  //        m_spine->id = node.nodeValue();
  //      }
  //      node = node_map.namedItem("toc");
  //      if (!node.isNull()) { // optional
  //        m_spine->toc = node.nodeValue();
  //      }
  //      node = node_map.namedItem("page-progression-dir");
  //      if (!node.isNull()) { // optional
  //        m_spine->page_progression_dir = node.nodeValue();
  //      }

  //      QDomNodeList spineItemList =
  //      spine_element.elementsByTagName("itemref"); for (int j = 0; j <
  //      spineItemList.count(); j++) {
  //        spine_item = parseSpineItem(spineItemList.at(j), spine_item);
  //      }

  //      if (!m_spine->toc.isEmpty()) { // EPUB2.0 toc
  //        parseTocFile(archive);
  //      }

  //    //    /*
  //    //     * At this point not all books have anchors added for chapter
  //    links.
  //    //     Calibre
  //    //     * adds in anchors at the end of the previous chapter. For
  //    //     simplicities sake I
  //    //     * am adding them at the start of the new chapter.
  //    //     * Also remember that some
  //    //     */
  //    //    createChapterAnchorPoints(spine_item);
  //  }

  //  //  QDomNodeList guide_node_list =
  //  //  package_document->elementsByTagName("guide"); for (int i = 0; i <
  //  //  guide_node_list.count(); i++) {
  //  //    QDomElement spine_element = guide_node_list.at(i).toElement();
  //  //    SharedGuideItem item = SharedGuideItem(new EPubGuideItem());
  //  //    node_map = spine_element.attributes();
  //  //    node = node_map.namedItem("id");
  //  //    if (!node.isNull()) { // optional
  //  //      m_spine->id = node.nodeValue();
  //  //    }
  //  //    node = node_map.namedItem("toc");
  //  //    if (!node.isNull()) { // optional
  //  //      m_spine->toc = node.nodeValue();
  //  //    }
  //  //    node = node_map.namedItem("page-progression-dir");
  //  //    if (!node.isNull()) { // optional
  //  //      m_spine->page_progression_dir = node.nodeValue();
  //  //    }

  //  //    QDomNodeList spineItemList =
  //  spine_element.elementsByTagName("itemref");
  //  //    for (int j = 0; j < spineItemList.count(); j++) {
  //  //      parseSpineItem(spineItemList.at(j));
  //  //    }
  //  //  }

  //  //    parseGuideItem(); // this has been superceded by landmarks.
  //  //  parseLandmarkItem();
  //  //  parseBindingsItem(); // Tis is used for non-epb standard media types.

  //  // TODO doctor the image paths to point to local files.
  //  for (int i1 = 0; i1 < m_manifest->htmlItems.keys().size(); i1++) {
  //    QString key = m_manifest->htmlItems.keys().at(i1);
  //    ManifestItem item = m_manifest->htmlItems.value(key);
  //    QString document_string = item->document_string;
  //    bool changed = false;
  //    for (int i2 = 0; i2 < m_manifest->images.size(); i2++) {
  //      QString image_id = m_manifest->images.keys().at(i2);
  //      QString image_path = m_manifest->images.value(image_id);
  //      if (document_string.contains(image_id)) {
  //        document_string.replace(image_id, image_path, Qt::CaseSensitive);
  //        changed = true;
  //      }
  //    }
  //    if (changed) {
  //      item->document_string = document_string;
  //    }
  //    }
  return true;
}
