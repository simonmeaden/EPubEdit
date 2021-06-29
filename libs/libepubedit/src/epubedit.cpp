#include "epubedit.h"

#include "quazip.h"
#include "quazipfile.h"

#include "forms/metadataform.h"
//#include "util/csvsplitter.h"

EPubEdit::EPubEdit(QWidget* parent)
  : QTabWidget(parent)
{
  initGui();
}

bool
EPubEdit::loadDocument(const QString& filename)
{
  m_document = Document(new EPubDocument(this));
  m_document->openDocument(filename);
  m_metadataForm->setMetadata(m_document->metadata());

  return true;
}

bool
EPubEdit::newDocument()
{
  m_document = Document(new EPubDocument(this));
  m_metadataForm->setMetadata(m_document->metadata());

  return true;
}

void
EPubEdit::updateMetadataForm()
{
  //  m_metadataForm->setTitles(m_metadata->orderedTitles());
  //  m_metadataForm->setAuthors(m_metadata->creatorList());
}

void
EPubEdit::metadataHasChanged()
{
  //  if (m_metadataForm->titlesModified()) {
  //    auto titles = m_metadataForm->titles();
  //    m_metadata->setOrderedTitles(titles);
  //  }

  //  if (m_metadataForm->authorsModified()) {
  //    auto authors = m_metadataForm->authors();
  //    m_metadata->setCreatorList(authors);
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

// bool
// EPubEdit::parsePackageFile(QString& fullpath, QuaZip* archive)
//{

//}

// SpineItem
// EPubEdit::parseSpineItem(const QDomNode& spine_node, SpineItem item)
//{
//  QDomElement metadata_element = spine_node.toElement();
//  QString tag_name = metadata_element.tagName();
//  QDomNamedNodeMap node_map = metadata_element.attributes();
//  QDomNode node;
//  QString name, value;

//  if (tag_name == "itemref") {

//    // TODO EPUB2 toc element - convert to EPUB3

//    node = node_map.namedItem("idref");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->idref = value;
//    } else {
//      qDebug() << tr("Warning invalid manifest itemref : no idref value");
//    }

//    node = node_map.namedItem("id");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->id = value;
//    }

//    node = node_map.namedItem("linear");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      if (value == "yes" || value == "no") {
//        if (value == "yes")
//          item->linear = true; // false by default.
//      } else {
//        qDebug() << tr("Warning invalid manifest itemref : linear MUST be "
//                       "either yes or no not %1")
//                      .arg(value);
//      }
//    }

//    node = node_map.namedItem("properties");
//    if (!node.isNull()) {
//      name = node.nodeName();
//      value = node.nodeValue();
//      // space separated list
//      QStringList properties = value.split(' ', Qt::SkipEmptyParts);

//      foreach (QString prop, properties) {
//        if (prop == "page-spread-left") {
//          item->page_spread_left = true;
//        } else if (prop == "page-spread-right") {
//          item->page_spread_right = true;
//        }
//      }
//    }

//    m_spine->items.insert(item->idref, item);
//    m_spine->ordered_items.append(item->idref);
//  }
//  return item;
//}

// bool
// EPubEdit::parseTocFile(QuaZip* archive)
//{
//  QString toc_id = m_spine->toc;
//  ManifestItem toc_item = m_manifest->itemsById.value(toc_id);
//  QString toc_path = toc_item->path;

//  archive->setCurrentFile(toc_path);
//  QuaZipFile toc_file(archive);
//  toc_file.setZip(archive);

//  if (!toc_file.open(QIODevice::ReadOnly)) {
//    archive->getZipError();
//    qDebug() << tr("Unable to open toc file %1").arg(toc_path);
//  }

//  QByteArray data = toc_file.readAll();
//  DomDocument document = DomDocument(new QDomDocument());
//  document->setContent(data);
//  QString formatted_toc_string;
//  QDomElement root = document->documentElement();
//  QDomNodeList node_list = document->elementsByTagName("docTitle");

//  if (!node_list.isEmpty()) {
//    QDomNode title_node = node_list.at(0);
//    QDomElement title_text = title_node.firstChild().toElement();
//    if (title_text.tagName() == "text") {
//      formatted_toc_string += TOC_TITLE.arg(title_text.text());
//    }
//  }
//  formatted_toc_string += LIST_START;

//  m_tocChapterIndex = -1;
//  node_list = document->elementsByTagName("navMap");
//  if (!node_list.isEmpty()) {
//    QString name, value;
//    QDomNode node = node_list.at(0);
//    QDomElement elem = node.toElement();
//    QDomElement navpoint = elem.firstChildElement("navPoint");
//    while (!navpoint.isNull()) {
//      TocItem toc_item = parseNavPoint(navpoint, formatted_toc_string);
//      m_manifest->tocItems.insert(toc_item->playorder, toc_item);
//      m_manifest->tocPaths.insert(toc_item->source, toc_item);

//      handleNestedNavpoints(navpoint, formatted_toc_string);

//      navpoint = navpoint.nextSiblingElement("navPoint");
//    }
//  }

//  formatted_toc_string += LIST_END;

//  m_manifest->formattedTocString = formatted_toc_string;
//  return true;
//}

// void
// EPubEdit::handleNestedNavpoints(QDomElement elem, QString&
// formatted_toc_string)
//{
//  QDomElement subpoint = elem.firstChildElement("navPoint");
//  while (!subpoint.isNull()) {
//    TocItem toc_item = parseNavPoint(subpoint, formatted_toc_string);
//    m_manifest->tocItems.insert(toc_item->playorder, toc_item);
//    m_manifest->tocPaths.insert(toc_item->source, toc_item);

//    handleNestedNavpoints(subpoint, formatted_toc_string);

//    subpoint = elem.nextSiblingElement("navPoint");
//  }
//}

// TocItem
// EPubEdit::parseNavPoint(QDomElement navpoint, QString& formatted_toc_data)
//{
//  m_tocChapterIndex++;
//  TocItem toc_item = TocItem(new EBookTocItem());
//  //  QDomNamedNodeMap attributes = navpoint.attributes();
//  QString value = navpoint.attribute("class");
//  if (!value.isEmpty()) {
//    toc_item->tag_class = value;
//  } else {
//    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
//    //            idref value"))
//  }

//  value = navpoint.attribute("id");
//  if (!value.isEmpty()) {
//    toc_item->id = value;
//  } else {
//    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
//    //            idref value"))
//  }

//  value = navpoint.attribute("playOrder");
//  if (!value.isEmpty()) {
//    toc_item->playorder = value.toInt();
//  } else {
//    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
//    //            idref value"))
//  }

//  QDomElement navlabel = navpoint.firstChildElement("navLabel");
//  if (!navlabel.isNull()) {
//    QDomElement text = navlabel.firstChild().toElement();
//    if (text.tagName() == "text") {
//      toc_item->label = text.text();
//    }
//  }

//  QDomElement content = navpoint.firstChildElement("content");
//  value = content.attribute("src");
//  if (!value.isEmpty()) {
//    // if no chapter fragment then add one in.
//    int index = value.indexOf("#");
//    if (index <= 0) {
//      toc_item->source = value;
//      toc_item->chapter_tag.clear();
//    } else {
//      QString tag;
//      tag = QString("#part%1").arg(m_tocChapterIndex);
//      value += tag;
//      toc_item->chapter_tag = tag;
//    }
//  } else {
//    //            QLOG_DEBUG(tr("Warning invalid manifest itemref : no
//    //            idref value"))
//  }

//  formatted_toc_data += LIST_ITEM.arg(toc_item->source).arg(toc_item->label);

//  // parse nested navPoints.
//  QDomElement sub_navpoint = navpoint.firstChildElement("navPoint");
//  if (!sub_navpoint.isNull()) {
//    formatted_toc_data += LIST_START;

//    while (!sub_navpoint.isNull()) {
//      TocItem sub_item = parseNavPoint(sub_navpoint, formatted_toc_data);

//      toc_item->sub_items.insert(sub_item->playorder, sub_item);
//      sub_navpoint = sub_navpoint.nextSiblingElement("navPoint");
//    }

//    formatted_toc_data += LIST_END;
//  }

//  return toc_item;
//}

// bool
// EPubEdit::parseManifestItem(const QDomNode& manifest_node,
//                            const QString current_folder,
//                            QuaZip* archive)
//{
//  QDomElement metadata_element = manifest_node.toElement();
//  QString tag_name = metadata_element.tagName();
//  QDomNamedNodeMap node_map = metadata_element.attributes();
//  QDomNode node;
//  QString name, value;

//  if (tag_name == "item") {
//    ManifestItem item = ManifestItem(new EBookManifestItem());
//    node = node_map.namedItem("href");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      QString path = QDir::cleanPath(current_folder + value);
//      item->href = value;
//      item->path = path;
//    } else {
//      qDebug() << tr("Warning invalid manifest item : no href value");
//    }

//    node = node_map.namedItem("id");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->id = value;
//    } else {
//      qDebug() << tr("Warning invalid manifest item : no id value");
//    }

//    node = node_map.namedItem("media-type");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->media_type = value.toLatin1();
//      if (item->media_type == "image/gif" || item->media_type == "image/jpeg"
//      ||
//          item->media_type == "image/png") {

//        if (!QImageReader::supportedMimeTypes().contains(item->media_type)) {
//          qDebug() << QString("Requested image type %1 is an unsupported
//          type")
//                        .arg(QString(item->media_type));
//        }

//        archive->setCurrentFile(item->path);
//        QuaZipFile image_file(archive);
//        image_file.setZip(archive);

//        if (!image_file.open(QIODevice::ReadOnly)) {
//          //          m_archive->getZipError();
//          qDebug() << tr("Unable to open image file %1").arg(item->path);
//        }

//        QByteArray data = image_file.readAll();
//        QImage image = QImage::fromData(data);
//        QFileInfo info(item->href);
//        QString path = info.path();
//        if (!path.isEmpty()) {
//          QDir dir(m_resourcePath);
//          dir.mkpath(path);
//        }

//        QString res_path = m_resourcePath + QDir::separator() + item->href;
//        image.save(res_path);
//        res_path.prepend(QStringLiteral("file://"));
//        m_manifest->images.insert(item->href, res_path);

//      } else if (item->media_type == "image/svg+xml") {
//        qDebug();
//      } else if (item->media_type == "application/vnd.ms-opentype" ||
//                 item->media_type == "application/font-woff") {
//        m_manifest->fonts.insert(item->href, item);

//      } else if (item->media_type == "application/xhtml+xml") {
//        archive->setCurrentFile(item->path);
//        QuaZipFile itemFile(archive);
//        itemFile.setZip(archive);

//        if (!itemFile.open(QIODevice::ReadOnly)) {
//          int error = archive->getZipError();
//          qDebug() << tr("Unable to open container file error %1").arg(error);
//          return false;
//        }

//        QString container(itemFile.readAll());
//        // remove xml header string. This will be reinserted by
//        QXmlStreamWriter int length = container.trimmed().length(); if
//        (container.trimmed().startsWith(XML_HEADER)) {
//          length -= XML_HEADER.length();
//          container = container.right(length).trimmed();
//        }
//        // Remove DOCTYPE if exists. Again this will be reinserted later.
//        if (container.trimmed().startsWith(HTML_DOCTYPE)) {
//          length = container.length();
//          length -= HTML_DOCTYPE.length();
//          container = container.right(length).trimmed();
//        }

//        extractHeadInformationFromHtmlFile(item, container);
//        item->document_string = container;
//        m_manifest->htmlItems.insert(item->id, item);
//      } else if (item->media_type == "text/css") {
//        archive->setCurrentFile(item->path);
//        QuaZipFile itemFile(archive);
//        itemFile.setZip(archive);

//        if (!itemFile.open(QIODevice::ReadOnly)) {
//          int error = archive->getZipError();
//          qDebug() << tr("Unable to open css file error %1").arg(error);
//          return false;
//        }

//        QString css_string(itemFile.readAll());
//        css_string.replace("@charset \"", "@charset\"");
//        m_manifest->css->insert(item->href, css_string);

//      } else if (item->media_type == "text/javascript") {
//        archive->setCurrentFile(item->path);
//        QuaZipFile itemFile(archive);
//        itemFile.setZip(archive);

//        if (!itemFile.open(QIODevice::ReadOnly)) {
//          int error = archive->getZipError();
//          qDebug() << tr("Unable to open javascript file error
//          %1").arg(error); return false;
//        }

//        QString js_string(itemFile.readAll());
//        m_manifest->javascript.insert(item->id, js_string);
//      }
//    } else {
//      qDebug() << tr("Warning invalid manifest item : no media-type value");
//    }

//    node = node_map.namedItem("properties");
//    if (!node.isNull()) {
//      name = node.nodeName();
//      value = node.nodeValue();
//      // space separated list
//      QStringList properties = value.split(' ', Qt::SkipEmptyParts);
//      item->properties = properties;

//      foreach (QString prop, properties) {
//        if (prop == "cover-image") {
//          // only one cover-image allowed.
//          m_manifest->coverImage = item;
//          //          m_manifest->images.insert(item->id, item);
//        } else if (prop == "nav") {
//          // only one nav allowed.
//          m_manifest->nav = item;
//        } else if (prop == "svg") {
//          m_manifest->svgImages.insert(item->id, item);
//          //          m_manifest->images.insert(item->id, item);
//        } else if (prop == "switch") {
//          m_manifest->switches.insert(item->id, item);
//        } else if (prop == "mathml") {
//          m_manifest->mathml.insert(item->id, item);
//        } else if (prop == "remote-resources") {
//          m_manifest->remotes.insert(item->id, item);
//        } else if (prop == "scripted") {
//          m_manifest->scripted.insert(item->id, item);
//        } else {
//          // one of the exmples had a data-nav element which is NOT standard.
//          // not certain what to do with these.
//          item->non_standard_properties.insert(name, value);
//        }
//      }
//    }

//    node = node_map.namedItem("fallback");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->fallback = value;
//    }

//    node = node_map.namedItem("media-overlay");
//    if (!node.isNull()) {
//      value = node.nodeValue();
//      item->media_overlay = value;
//      m_manifest->mediaOverlay.insert(item->id, item);
//    }

//    m_manifest->itemsById.insert(item->id, item);
//    m_manifest->itemsByHref.insert(item->href, item);
//  }
//  return true;
//}

// void
// EPubEdit::extractHeadInformationFromHtmlFile(ManifestItem item,
//                                             QString container)
//{
//  QDomDocument doc;
//  doc.setContent(container);

//  QDomNodeList node_list = doc.elementsByTagName(QLatin1String("head"));

//  if (!node_list.isEmpty()) {
//    QDomNode head_node = node_list.at(0); // should only be one head element
//    QDomElement head_elem = head_node.toElement();
//    QDomNodeList link_list =
//    head_elem.elementsByTagName(QLatin1String("link")); for (int i = 0; i <
//    link_list.length(); i++) {
//      QDomNode link = link_list.at(i);
//      QDomElement link_elem = link.toElement();
//      QString rel = link_elem.attribute(QLatin1String("rel"));
//      if (!rel.isEmpty()) {
//        if (rel == QLatin1String("stylesheet")) {
//          QString type = link_elem.attribute(QLatin1String("type"));
//          if (!type.isEmpty()) {
//            if (type == QLatin1String("text/css")) {
//              QString href = link_elem.attribute(QLatin1String("href"));
//              if (!href.isEmpty()) {
//                item->css_links.append(href);
//              }
//            }
//          }
//        }
//      }
//    }
//  }

//  node_list = doc.elementsByTagName(QLatin1String("body"));

//  if (!node_list.isEmpty()) {
//    QDomNode body_node = node_list.item(0); // should only be one
//    QDomElement body_elem = body_node.toElement();
//    QString att = body_elem.attribute(QLatin1String("class"));
//    if (!att.isEmpty()) {
//      item->body_class = att;
//    }
//  }
//}
