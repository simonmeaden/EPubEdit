#include "document/epubmetadata.h"

#include <QDebug>
#include <QDomNodeList>

#include "document/authors.h"
#include "document/dcterms.h"
#include "document/epubdocument.h"
#include "document/foaf.h"
#include "document/library.h"
#include "document/marcrelator.h"

//============================================================================
//=== EPubMetadata
//============================================================================
EPubMetadata::EPubMetadata(QSharedPointer<UniqueStringList> uniqueIdList)
  : m_uniqueIdList(uniqueIdList)
  , m_calibre(QSharedPointer<EPubCalibre>(new EPubCalibre()))
  , m_isFoaf(false)
{}

void
EPubMetadata::parse(QDomNodeList metadataNodeList)
{
  for (int i = 0; i < metadataNodeList.count(); i++) { // should only be one.
    auto metadataChildList = metadataNodeList.at(i).childNodes();
    for (int j = 0; j < metadataChildList.count(); j++) {
      parseMetadataItem(metadataChildList.at(j));
    }
  }
}

bool
EPubMetadata::write(QXmlStreamWriter* xml_writer)
{

  xml_writer->writeStartElement("metadata");

  if (!m_orderedTitles.isEmpty()) {
    for (auto title : m_orderedTitles) {
      writeTitle(xml_writer, title);
    }
  }

  if (!m_creatorsByName.isEmpty()) {
    auto keys = m_creatorsByName.keys();
    auto ids = m_creatorsById.keys();
    for (auto key : keys) {
      auto name = writeCreatorsMetadata(xml_writer, key);
      // remove names that have been handled.
      auto shared_creator = m_creatorsByName.value(name);
      auto id = shared_creator->id;
      ids.removeAll(id);
    }
    // These were probably added from dcterms:creator metas.
    for (UniqueString creator : ids) {
      auto shared_creator = m_creatorsById.value(creator);
      writeCreator(xml_writer, shared_creator);
    }
  }

  if (!contributorsByName.isEmpty()) {
    auto keys = contributorsByName.keys();
    for (QString key : keys) {
      writeContributorMetadata(xml_writer, key);
    }
  }

  if (!description.isNull()) {
    writeDescriptionMetadata(xml_writer);
  }

  // actually should NEVER be empty as at least one is required.
  if (!m_languages.isEmpty()) {
    auto keys = m_languages.keys();
    if (keys.size() == 1) {
      writeLanguageMetadata(xml_writer, m_languages.keys().at(0), true);
    } else {
      bool first = true;
      foreach (auto key, keys) {
        writeLanguageMetadata(xml_writer, key, first);
        first = false;
      }
    }
  }

  if (!m_subjects.isEmpty()) {
    auto keys = m_subjects.keys();
    for (auto key : keys) {
      writeSubjectMetadata(xml_writer, key);
    }
  }

  if (!m_identifiers.isEmpty()) {
    auto keys = m_identifiers.keys();
    for (EBookIdentifierScheme::IdentifierScheme key : keys) {
      writeIdentifierMetadata(xml_writer, key);
    }
  }

  if (!m_date.isNull()) {
    xml_writer->writeStartElement("date");
    xml_writer->writeCharacters(m_date.toString(Qt::ISODate));
    xml_writer->writeEndElement();
  }

  if (!m_source.isNull()) {
    writeSourceMetadata(xml_writer);
  }

  if (!m_publisher.isNull()) {
    writePublisherMetadata(xml_writer);
  }

  if (!m_format.isNull()) {
    writeFormatMetadata(xml_writer);
  }

  if (!m_relation.isNull()) {
    writeRelationMetadata(xml_writer);
  }

  if (!m_rights.isNull()) {
    writeRightsMetadata(xml_writer);
  }

  if (!m_coverage.isNull()) {
    writeCoverageMetadata(xml_writer);
  }

  xml_writer->writeEndElement();

  return true;
}

UniqueString
EPubMetadata::uniqueIdentifierName() const
{
  return m_packageUniqueIdentifierName;
}

void
EPubMetadata::setUniqueIdentifierName(
  const UniqueString& packageUniqueIdentifierName)
{
  m_packageUniqueIdentifierName = packageUniqueIdentifierName;
}

bool
EPubMetadata::isFoaf() const
{
  return m_isFoaf;
}

void
EPubMetadata::setIsFoaf(bool value)
{
  m_isFoaf = value;
}

QStringList
EPubMetadata::creatorList() const
{
  return m_creatorList;
}

void
EPubMetadata::setCreatorList(const QStringList& creatorList)
{
  m_creatorList = creatorList;
}

QList<QSharedPointer<EPubTitle>>
EPubMetadata::orderedTitles() const
{
  return m_orderedTitles;
}

void
EPubMetadata::setOrderedTitles(
  const QList<QSharedPointer<EPubTitle>>& orderedTitles)
{
  m_orderedTitles = orderedTitles;
}

QSharedPointer<EPubCalibre>
EPubMetadata::calibre() const
{
  return m_calibre;
}

void
EPubMetadata::setCalibre(const QSharedPointer<EPubCalibre>& calibre)
{
  m_calibre = calibre;
}

void
EPubMetadata::writeCreator(QXmlStreamWriter* xml_writer,
                           QSharedPointer<EPubCreator> shared_creator)
{
  writeCreatorContributor("dc:creator", xml_writer, shared_creator);
}

void
EPubMetadata::writeContributor(QXmlStreamWriter* xml_writer,
                               QSharedPointer<EPubCreator> shared_creator)
{
  writeCreatorContributor("dc:contributor", xml_writer, shared_creator);
}

void
EPubMetadata::parseSourceMetadata(const QDomElement& metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  auto sharedSource = QSharedPointer<EPubSource>(new EPubSource());
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedSource->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("scheme");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedSource->scheme = EBookIdentifierScheme::fromString(node.nodeValue());
  }
  sharedSource->source = metadataElement.text();
  m_source = sharedSource;
}

void
EPubMetadata::parsePublisherMetadata(const QDomElement& metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto sharedPublisher = QSharedPointer<EPubPublisher>(new EPubPublisher());
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) {
    sharedPublisher->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) {
    sharedPublisher->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) {
    sharedPublisher->dir = direction(node.nodeValue());
  }
  node = nodeMap.namedItem("alt-rep");
  if (!node.isNull()) { // alt-rep element is NOT used in EPUB 2.0
    sharedPublisher->altRep->name = node.nodeValue();
  }
  node = nodeMap.namedItem("alt-rep-lang");
  if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
    sharedPublisher->altRep->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    sharedPublisher->fileAs->name = node.nodeValue();
  }
  sharedPublisher->name = metadataElement.text();
  m_publisher = sharedPublisher;
}

void
EPubMetadata::parseFormatMetadata(const QDomElement& metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  auto sharedFormat = QSharedPointer<EPubFormat>(new EPubFormat());
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedFormat->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  sharedFormat->name = metadataElement.text();
  m_format = sharedFormat;
}

void
EPubMetadata::parseTypeMetadata(const QDomElement& metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  auto sharedType = QSharedPointer<EPubType>(new EPubType());
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedType->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  sharedType->name = metadataElement.text();
  m_type = sharedType;
}

void
EPubMetadata::parseRelationMetadata(const QDomElement& metadataElement)
{
  auto sharedRelation = QSharedPointer<EPubRelation>(new EPubRelation());
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedRelation->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) {
    sharedRelation->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) {
    sharedRelation->dir = direction(node.nodeValue());
  }
  sharedRelation->name = metadataElement.text();
  m_relation = sharedRelation;
}

void
EPubMetadata::parseCoverageMetadata(const QDomElement& metadataElement)
{
  auto sharedCoverage = QSharedPointer<EPubCoverage>(new EPubCoverage());
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedCoverage->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) {
    sharedCoverage->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) {
    sharedCoverage->dir = direction(node.nodeValue());
  }
  sharedCoverage->name = metadataElement.text();
  m_coverage = sharedCoverage;
}

void
EPubMetadata::parseRightsMetadata(const QDomElement& metadataElement)
{
  auto sharedRights = QSharedPointer<EPubRights>(new EPubRights());
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedRights->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) {
    sharedRights->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) {
    sharedRights->dir = direction(node.nodeValue());
  }
  sharedRights->name = metadataElement.text();
  m_rights = sharedRights;
}

void
EPubMetadata::parseDublinCoreMeta(QString tagName,
                                  QDomElement& metadataElement,
                                  QDomNamedNodeMap& nodeMap)
{
  if (tagName == "title") {
    parseTitleMetadata(metadataElement);
  } else if (tagName == "creator") {
    parseCreatorMetadata(metadataElement);
  } else if (tagName == "contributor") {
    parseContributorMetadata(metadataElement);
  } else if (tagName == "description") {
    parseDescriptionMetadata(metadataElement);
  } else if (tagName == "identifier") {
    parseIdentifierMetadata(metadataElement);
  } else if (tagName == "language") {
    parseLanguageMetadata(metadataElement);
  } else if (tagName == "description") {
    parseDescriptionMetadata(metadataElement);
  } else if (tagName == "publisher") {
    parsePublisherMetadata(metadataElement);
  } else if (tagName == "date") {
    auto node = nodeMap.namedItem("id");
    m_date = QDateTime::fromString(metadataElement.text(), Qt::ISODate);
    if (!node.isNull()) {
      m_modified.id =
        m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
    }
  } else if (tagName == "rights") {
    parseRightsMetadata(metadataElement);
  } else if (tagName == "format") {
    parseFormatMetadata(metadataElement);
  } else if (tagName == "relation") {
    parseRelationMetadata(metadataElement);
  } else if (tagName == "coverage") {
    parseCoverageMetadata(metadataElement);
  } else if (tagName == "source") {
    parseSourceMetadata(metadataElement);
  } else if (tagName == "subject") {
    parseSubjectMetadata(metadataElement);
  } else if (tagName == "type") {
    parseTypeMetadata(metadataElement);
  } else {
    // TODO
  }
}

void
EPubMetadata::parseOpfMeta(QString tagName,
                           QDomElement& metadataElement,
                           QDomNamedNodeMap& /*nodeMap*/)
{
  qWarning() << QString(
                  "Unknown OPF <meta> detected : TagName=%1, NodeName=%2, "
                  "NodeValue=%3, TagValue=%4")
                  .arg(tagName)
                  .arg(metadataElement.nodeName())
                  .arg(metadataElement.nodeValue())
                  .arg(metadataElement.text());
}

void
EPubMetadata::parseCalibreMetas(const UniqueString& id, QDomNode& node)
{
  if (id == "calibre:series")
    m_calibre->setSeriesName(node.nodeValue());
  else if (id == "calibre:series_index")
    m_calibre->setSeriesIndex(node.nodeValue());
  else if (id == "calibre:title_sort")
    m_calibre->setTitleSort(node.nodeValue());
  else if (id == "calibre:author_link_map")
    m_calibre->setAuthorLinkMap(node.nodeValue());
  else if (id == "calibre:timestamp")
    m_calibre->setTimestamp(node.nodeValue());
  else if (id == "calibre:rating")
    m_calibre->setRating(node.nodeValue());
  else if (id == "calibre:publication_type")
    m_calibre->setPublicationType(node.nodeValue());
  else if (id == "calibre:user_metadata")
    m_calibre->setUserMetadata(node.nodeValue());
  else if (id == "calibre:user_categories")
    m_calibre->setUserCategories(node.nodeValue());
  else if (id == "calibre:custom_metadata")
    m_calibre->setCustomMetadata(node.nodeValue());
  m_calibre->setModified(false);
}

void
EPubMetadata::parseTitleDateRefines(QSharedPointer<EPubTitle> sharedTitle,
                                    QDomElement& metadataElement)
{
  sharedTitle->date =
    QDateTime::fromString(metadataElement.text(), Qt::ISODate);
}

QSharedPointer<Foaf>
EPubMetadata::parseCreatorContributorFoafAttributes(QString property,
                                                    QDomNamedNodeMap nodeMap)
{
  /* Please note that these are just values I have found in examples
   * of EPUB 3.0 and 3.1 ebooks. */
  auto foaf = Foaf::fromString(property);
  auto foafNode = nodeMap.namedItem("scheme");
  if (!foafNode.isNull()) {
    foaf->setScheme(foafNode.nodeValue().toLower());
  }
  foafNode = nodeMap.namedItem("lang");
  if (!foafNode.isNull()) {
    foaf->setLang(foafNode.nodeValue().toLower());
  }
  foafNode = nodeMap.namedItem("id");
  if (!foafNode.isNull()) {
    foaf->setId(
      m_uniqueIdList->append(foafNode.nodeValue(), foafNode.lineNumber()));
  }
  foafNode = nodeMap.namedItem("content");
  if (!foafNode.isNull()) {
    foaf->setContent(foafNode.nodeValue().toLower());
  }
  foafNode = nodeMap.namedItem("about");
  if (!foafNode.isNull()) {
    foaf->setAbout(foafNode.nodeValue().toLower());
  }
  return foaf;
}

void
EPubMetadata::parseCreatorContributorRefines(
  QSharedPointer<EPubCreator> sharedCreator,
  QDomElement& metadataElement,
  const UniqueString& id,
  QDomNamedNodeMap nodeMap)
{
  auto node = nodeMap.namedItem("property");
  if (!node.isNull()) {
    auto property = node.nodeValue().toLower();
    if (property == "role") {
      auto node = nodeMap.namedItem("scheme");
      if (!node.isNull()) {
        auto scheme = node.nodeValue().toLower();
        if (scheme == "marc:relators") {
          //            shared_creator->scheme = id;
          sharedCreator->relator =
            MarcRelator::fromString(metadataElement.text());
          if (sharedCreator->relator->type() == MarcRelator::NO_TYPE) {
            sharedCreator->stringCreator = metadataElement.text();
            qWarning() << QString("An unexpected role has come up. %1")
                            .arg(metadataElement.text());
          }
        } else {
          // TODO treat as a string if not a recognised scheme type;
          sharedCreator->stringScheme = metadataElement.text();
        }
      }
    } else if (property == "alternate-script") {
      auto altRep = QSharedPointer<EPubAltRep>(new EPubAltRep());
      altRep->name = metadataElement.text();
      node = nodeMap.namedItem("lang");
      if (!node.isNull()) {
        altRep->lang = node.nodeValue();
      }
      sharedCreator->altRepList.append(altRep);
    } else if (property == "file-as") {
      auto file_as = QSharedPointer<EPubFileAs>(new EPubFileAs());
      file_as->name = node.nodeValue();
      node = nodeMap.namedItem("lang");
      if (!node.isNull()) {
        file_as->lang = node.nodeValue();
      }
      sharedCreator->fileAsList.append(file_as);
    } else if (property.startsWith("foaf:")) {
      if (m_creatorsById.contains(id)) {
        auto sharedCreator = m_creatorsById.value(id);
        auto foaf = parseCreatorContributorFoafAttributes(property, nodeMap);
        sharedCreator->foafList.append(foaf);
      } else if (contributorsById.contains(id)) {
        auto sharedContributor = contributorsById.value(id);
        auto foaf = parseCreatorContributorFoafAttributes(property, nodeMap);
        sharedContributor->foafList.append(foaf);
      }
    }
  }
}

void
EPubMetadata::parseTitleRefines(const UniqueString& id,
                                QDomElement metadataElement,
                                QDomNamedNodeMap nodeMap)
{
  QSharedPointer<EPubTitle> sharedTitle = m_titlesById.value(id);
  QDomNode node = nodeMap.namedItem("property");
  if (!node.isNull()) {
    QString property = node.nodeValue().toLower();
    if (property == "title-type") {
      // "title-type" is not used in 3.1 so ignore that node.
    } else if (property == "display-seq") {
      // refines/display-seq has been superceded in 3.1. Titles should
      // appear in the order required so reorder them in display-seq
      // order if these items appear in the metadata.
      //      int newSeq = metadataElement.text().trimmed().toInt();
      //      int oldSeq = m_orderedTitles.indexOf(sharedTitle);
      //      if (newSeq != oldSeq) { // dont change if key is correct.
      //        if (m_orderedTitles.contains(newSeq)) { // bugger key already
      //        exists
      //          Title existing_title = m_orderedTitles.value(newSeq);
      //          m_orderedTitles.remove(newSeq);
      //          m_orderedTitles.remove(oldSeq);
      //          m_orderedTitles.insert(newSeq, sharedTitle);
      //          m_orderedTitles.insert(getNextTitleIndex(), existing_title);
      //        } else { // otherwise it is a free key
      //          m_orderedTitles.remove(oldSeq);
      //          m_orderedTitles.insert(newSeq, sharedTitle);
      //        }
      //      }
    } else if (property == "alternate-script") {
      QSharedPointer<EPubAltRep> alt_rep =
        QSharedPointer<EPubAltRep>(new EPubAltRep());
      alt_rep->name = metadataElement.text();
      node = nodeMap.namedItem("lang");
      if (!node.isNull()) {
        alt_rep->lang = node.nodeValue();
      }
      sharedTitle->altRepList.append(alt_rep);
    } else if (property == "file-as") {
      QSharedPointer<EPubFileAs> file_as =
        QSharedPointer<EPubFileAs>(new EPubFileAs());
      file_as->name = node.nodeValue();
      node = nodeMap.namedItem("lang");
      if (!node.isNull()) {
        file_as->lang = node.nodeValue();
      }
      sharedTitle->fileAsList.append(file_as);
    } else if (property.startsWith("dcterms:")) {
      if (property == "dcterms:date") {
        node = nodeMap.namedItem("scheme");
        parseTitleDateRefines(sharedTitle, metadataElement);
      }
    }
  }
}

void
EPubMetadata::parseRefineMetas(QDomElement& metadataElement,
                               QDomNode& node,
                               QDomNamedNodeMap& nodeMap)
{
  QString id = node.nodeValue();
  auto uniqueId = m_uniqueIdList->append(id, node.lineNumber());
  if (id.startsWith("#")) {
    id = id.mid(1);
    if (m_titlesById.contains(uniqueId)) {
      // is it a title?
      parseTitleRefines(
        UniqueString(id, node.lineNumber()), metadataElement, nodeMap);
    }
  } else if (m_languages.contains(uniqueId)) {
    // is it a language?
    auto language = m_languages.value(uniqueId);
    node = nodeMap.namedItem("property");
    if (!node.isNull()) {
      id = node.nodeValue().toLower();
      if (id == "language")
        language->language = metadataElement.text();
    }
  } else if (m_creatorsById.contains(uniqueId)) {
    // is it a creator
    auto creator = m_creatorsById.value(uniqueId);
    parseCreatorContributorRefines(creator, metadataElement, uniqueId, nodeMap);
  } else if (contributorsByName.contains(id)) {
    // is it a creator
    auto contributor = contributorsByName.value(id);
    parseCreatorContributorRefines(
      contributor, metadataElement, uniqueId, nodeMap);
  }
}

bool
EPubMetadata::parseMetadataItem(const QDomNode& metadata_node)
{
  auto metadataElement = metadata_node.toElement();
  auto tagName = metadataElement.tagName();
  auto nodeMap = metadataElement.attributes();
  QDomNode node;
  UniqueString id;

  if (tagName == "meta") {
    // refines are now deprecated however store their data and we will
    // try to convert them to 3.1
    node = nodeMap.namedItem("refines");
    if (!node.isNull()) { // # refines earler values title etc.
      parseRefineMetas(metadataElement, node, nodeMap);
      return true;
    }

    // metadata elements that do not refine other elements.
    node = nodeMap.namedItem("property");
    if (!node.isNull()) {
      id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
      auto dcterms = DCTerms::fromString(id.toString());
      if (dcterms.isDCTerm()) {
        if (dcterms.term() == DCTerms::DATE) {
          parseDateModified(nodeMap, metadataElement.text());
        } else if (dcterms.term() == DCTerms::SOURCE) {
          if (!m_source.isNull()) {
            m_source->source = metadataElement.text();
          } else {
            auto shared_source = QSharedPointer<EPubSource>(new EPubSource());
            shared_source->source = metadataElement.text();
            m_source = shared_source;
          }
        } else if (dcterms.term() == DCTerms::MODIFIED) {
          m_modified.date =
            QDateTime::fromString(metadataElement.text(), Qt::ISODate);
        } else if (dcterms.term() == DCTerms::IDENTIFIER) {
          // TODO
        } else if (dcterms.term() == DCTerms::RIGHTS) {
          if (!m_rights.isNull()) {
            m_rights->name = metadataElement.text();
          } else {
            auto shared_rights = QSharedPointer<EPubRights>(new EPubRights());
            shared_rights->name = metadataElement.text();
            m_rights = shared_rights;
          }
        } else if (dcterms.term() == DCTerms::SUBJECT) {
          // TODO
        } else if (dcterms.term() == DCTerms::CREATOR) {
          auto creator = metadataElement.text();
          auto keys = contributorsByName.keys();
          if (!keys.contains(creator, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            auto shared_creator =
              QSharedPointer<EPubCreator>(new EPubCreator());
            shared_creator->name = creator;
            m_creatorsByName.insert(creator, shared_creator);
          }
        } else if (dcterms.term() == DCTerms::CREATED) {
          // TODO
        } else if (dcterms.term() == DCTerms::LANGUAGE) {
          // TODO
        } else if (dcterms.term() == DCTerms::DESCRIPTION) {
          // TODO
        } else if (dcterms.term() == DCTerms::TITLE) {
          auto title = metadataElement.text();
          auto keys = m_titlesByName.keys();
          if (keys.contains(title, Qt::CaseInsensitive)) {
            auto shared_title = QSharedPointer<EPubTitle>(new EPubTitle());
            shared_title->title = title;
            m_titlesByName.insert(title, shared_title);
          }
        } else if (dcterms.term() == DCTerms::PUBLISHER) {
          if (m_publisher.isNull()) {
            // TODO - maybe more than one publisher?
            m_publisher = QSharedPointer<EPubPublisher>(new EPubPublisher());
            m_publisher->name = metadataElement.text();
          }
        } else if (dcterms.term() == DCTerms::CONTRIBUTOR) {
          auto contributor = metadataElement.text();
          auto keys = contributorsByName.keys();
          if (!keys.contains(contributor, Qt::CaseInsensitive)) {
            // This might cause a contributor to be duplicated if the spelling
            // is different. I think that that will have to be edited manually
            // by the user.
            auto shared_contributor =
              QSharedPointer<EPubContributor>(new EPubContributor());
            shared_contributor->name = contributor;
            contributorsByName.insert(contributor, shared_contributor);
          }
        } else if (dcterms.term() == DCTerms::DATE) {
        } else if (dcterms.term() == DCTerms::DATE_COPYRIGHTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE_ACCEPTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE_SUBMITTED) {
          // TODO
        } else if (dcterms.term() == DCTerms::IS_PART_OF) {
          // TODO
        } else if (dcterms.term() == DCTerms::ISSUED) {
          // TODO
        } else if (dcterms.term() == DCTerms::DATE) {
          m_date = QDateTime::fromString(metadataElement.text(), Qt::ISODate);
        } else if (dcterms.term() == DCTerms::LICENSE) {
          // TODO
        } else {
          // TODO other dcterms.
          qWarning()
            << QString("Unknown DCTerms object : %1").arg(dcterms.code());
        }
      }
    }

    // handle <meta> tags
    node = nodeMap.namedItem("name");
    if (!node.isNull()) {
      id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
      node = nodeMap.namedItem("content");
      if (!node.isNull()) {
        // Mostly calibre tags - maintain these
        if (id.toString().startsWith("calibre:")) {
          parseCalibreMetas(id, node);
        } else // a store for unknown stuff.
          m_extraMetas.insert(id, node.nodeValue());
      }
    }
  } else if (tagName == "link") {
    // TODO link tag not yet done
  } else if (metadataElement.prefix() == "dc") {
    parseDublinCoreMeta(tagName, metadataElement, nodeMap);
  } else if (metadataElement.prefix() == "opf") {
    // might not actually be any of these.
    // just throws a log warning at the moment.
    parseOpfMeta(tagName, metadataElement, nodeMap);
  } else {
    // TODO not a dc  or OPF element, maybe calibre?
    qWarning() << QString("Unknown <meta> detected : TagName=%1, NodeName=%2, "
                          "NodeValue=%3, TagValue=%4")
                    .arg(tagName)
                    .arg(metadataElement.nodeName())
                    .arg(metadataElement.nodeValue())
                    .arg(metadataElement.text());
  }

  return true;
}

void
EPubMetadata::parseTitleMetadata(QDomElement metadataElement)
{
  QSharedPointer<EPubTitle> sharedTitle =
    QSharedPointer<EPubTitle>(new EPubTitle());
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  QDomNode node = nodeMap.namedItem("id");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedTitle->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) { // dir element is NOT used in EPUB 2.0
    sharedTitle->dir = direction(node.nodeValue());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) { // lang element is NOT used in EPUB 2.0
    sharedTitle->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("alt-rep");
  if (!node.isNull()) { // alt-rep element is NOT used in EPUB 2.0
    QSharedPointer<EPubAltRep> sharedAltRep =
      QSharedPointer<EPubAltRep>(new EPubAltRep());
    sharedAltRep->name = node.nodeValue();
    node = nodeMap.namedItem("alt-rep-lang");
    if (!node.isNull()) { // alt-rep-lang element is NOT used in EPUB 2.0
      sharedAltRep->lang = node.nodeValue();
    }
    sharedTitle->altRepList.append(sharedAltRep);
  }
  node = nodeMap.namedItem("file-as");
  if (!node.isNull()) { // file-as element is NOT used in EPUB 2.0
    QSharedPointer<EPubFileAs> fileAs =
      QSharedPointer<EPubFileAs>(new EPubFileAs());
    fileAs->name = node.nodeValue();
    sharedTitle->fileAsList.append(fileAs);
  }

  sharedTitle->title = metadataElement.text();
  if (m_titlesById.isEmpty()) {
    m_primaryTitle = sharedTitle->title;
    m_primaryTitleId = sharedTitle->id;
  }
  m_orderedTitles.append(sharedTitle);
  m_titlesById.insert(sharedTitle->id, sharedTitle);
  m_titlesByName.insert(sharedTitle->title, sharedTitle);
}

void
EPubMetadata::parseCreatorMetadata(QDomElement metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto creator = QSharedPointer<EPubCreator>(new EPubCreator());
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    creator->id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  /* Not certain if anybody is using the role information. The EPUB people
   * seem a bit ambivalent about it as well so for the time being I am
   * going to leave it as text and not use it for anything. Also if you
   * look at marc:role the list is enormous, most of which are irrelevant
   * to an ebook.
   */
  node = nodeMap.namedItem("role"); // 2.0 & 3.0
  if (!node.isNull()) {
    creator->relator = MarcRelator::fromString(node.nodeValue());
    if (creator->relator->type() == MarcRelator::NO_TYPE) {
      creator->stringCreator = node.nodeValue();
      qWarning()
        << QString("An unexpected role has come up. %1").arg(node.nodeValue());
    }
  }
  node = nodeMap.namedItem("file-as"); // 2.0 & 3.0
  if (!node.isNull()) {
    auto file_as = QSharedPointer<EPubFileAs>(new EPubFileAs());
    file_as->name = node.nodeValue();
    creator->fileAsList.append(file_as);
  }
  node = nodeMap.namedItem("scheme"); // 3.0
  if (!node.isNull()) {
    auto elem = node.toElement();
    if (!elem.isNull()) {
      auto scheme = elem.attribute("scheme");
      if (!scheme.isEmpty()) {
        if (scheme == "marc:relators") {
          if (creator->relator->type() != MarcRelator::NO_TYPE) {
            auto relator = MarcRelator::fromString(metadataElement.text());
            if (relator->type() != MarcRelator::NO_TYPE) {
              creator->relator = relator;
            }
          }
        }
      }
    }
  }
  node = nodeMap.namedItem("alt-rep"); // 3.0
  if (!node.isNull()) {
    auto altRep = QSharedPointer<EPubAltRep>(new EPubAltRep());
    altRep->name = node.nodeValue();
    node = nodeMap.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      altRep->lang = node.nodeValue();
    }
    creator->altRepList.append(altRep);
  }

  creator->name = metadataElement.text();
  m_creatorsById.insert(creator->id, creator);
  m_creatorsByName.insert(creator->name, creator);
  m_creatorList.append(creator->name);
}

void
EPubMetadata::parseContributorMetadata(QDomElement metadataElement)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  auto nodeMap = metadataElement.attributes();
  auto sharedContributor =
    QSharedPointer<EPubContributor>(new EPubContributor());
  auto node = nodeMap.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    sharedContributor->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }

  node = nodeMap.namedItem("role"); // 2.0 & 3.0
  if (!node.isNull()) {
    sharedContributor->relator = MarcRelator::fromString(node.nodeValue());
    if (sharedContributor->relator->type() == MarcRelator::NO_TYPE) {
      sharedContributor->stringCreator = node.nodeValue();
      qWarning()
        << QString("An unexpected role has come up. %1").arg(node.nodeValue());
    }
  }
  if (!node.isNull()) {
    auto fileAs = QSharedPointer<EPubFileAs>(new EPubFileAs());
    fileAs->name = node.nodeValue();
    sharedContributor->fileAsList.append(fileAs);
  }
  node = nodeMap.namedItem("scheme"); // 3.0
  if (!node.isNull()) {
    auto elem = node.toElement();
    if (!elem.isNull()) {
      auto scheme = elem.attribute("scheme");
      if (!scheme.isEmpty()) {
        if (scheme == "marc:relators") {
          if (sharedContributor->relator->type() != MarcRelator::NO_TYPE) {
            auto relator = MarcRelator::fromString(metadataElement.text());
            if (relator->type() != MarcRelator::NO_TYPE) {
              sharedContributor->relator = relator;
            }
          }
        }
      }
    }
  }
  node = nodeMap.namedItem("alt-rep"); // 3.0
  if (!node.isNull()) {
    auto altRep = QSharedPointer<EPubAltRep>(new EPubAltRep());
    altRep->name = node.nodeValue();
    node = nodeMap.namedItem("alt-rep-lang"); // 3.0
    if (!node.isNull()) {
      altRep->lang = node.nodeValue();
    }
    sharedContributor->altRepList.append(altRep);
  }

  sharedContributor->name = metadataElement.text();
  if (!sharedContributor->id.isEmpty()) {
    contributorsById.insert(sharedContributor->id, sharedContributor);
  }
  contributorsByName.insert(sharedContributor->name, sharedContributor);
  //  creator_list.append(shared_creator->name);
}

void
EPubMetadata::parseDescriptionMetadata(QDomElement metadataElement)
{
  /* Please note that according to the 3.0 spec Contributors data are
   * identical to Creators data. */
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  description = QSharedPointer<EPubDescription>(new EPubDescription());
  QDomNode node = nodeMap.namedItem("id");
  if (!node.isNull()) { //  EPUB 3.0 only
    description->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) { // dir element is NOT used in EPUB 2.0
    description->dir = direction(node.nodeValue());
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) { // lang element is NOT used in EPUB 2.0
    description->language = node.nodeValue();
  }

  description->text = metadataElement.text();
}

void
EPubMetadata::parseIdentifierMetadata(QDomElement metadataElement)
{
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  QSharedPointer<EBookIdentifier> sharedIdentifier =
    QSharedPointer<EBookIdentifier>(new EBookIdentifier());
  QDomNode node = nodeMap.namedItem("id");
  if (!node.isNull()) { // EPUB 3.0
    sharedIdentifier->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
    if (sharedIdentifier->id ==
        m_packageUniqueIdentifierName.toString().toLower()) {
      m_packageUniqueIdentifier = sharedIdentifier->id;
    }
  }
  node = nodeMap.namedItem("scheme");
  if (!node.isNull()) { // id element is NOT used in EPUB 2.0
    sharedIdentifier->identifier =
      EBookIdentifierScheme::fromString(node.nodeValue());
  }

  sharedIdentifier->name = metadataElement.text();

  m_identifiers.insert(sharedIdentifier->identifier.scheme, sharedIdentifier);
}

void
EPubMetadata::parseLanguageMetadata(QDomElement metadataElement)
{
  QDomNamedNodeMap nodeMap = metadataElement.attributes();
  QSharedPointer<EPubLanguage> sharedLanguage =
    QSharedPointer<EPubLanguage>(new EPubLanguage());
  QDomNode node = nodeMap.namedItem("id");
  if (!node.isNull()) {
    sharedLanguage->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  sharedLanguage->language = metadataElement.text();
  m_languages.insert(sharedLanguage->id, sharedLanguage);
}

void
EPubMetadata::parseSubjectMetadata(QDomElement metadataElement)
{
  auto nodeMap = metadataElement.attributes();
  auto node = nodeMap.namedItem("id");
  auto sharedSubject = QSharedPointer<EPubSubject>(new EPubSubject());
  node = nodeMap.namedItem("id");
  if (!node.isNull()) {
    sharedSubject->id =
      m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  node = nodeMap.namedItem("authority");
  if (!node.isNull()) {
    sharedSubject->authority = node.nodeValue();
  }
  node = nodeMap.namedItem("term");
  if (!node.isNull()) {
    sharedSubject->term = node.nodeValue();
  }
  node = nodeMap.namedItem("lang");
  if (!node.isNull()) {
    sharedSubject->lang = node.nodeValue();
  }
  node = nodeMap.namedItem("dir");
  if (!node.isNull()) {
    sharedSubject->dir = direction(node.nodeValue());
  }
  sharedSubject->subject = metadataElement.text();
  m_subjects.insert(sharedSubject->id, sharedSubject);
}

void
EPubMetadata::parseDateModified(QDomNamedNodeMap nodeMap, QString text)
{
  QDateTime dateModified = QDateTime::fromString(text, Qt::ISODate);
  m_modified.date = dateModified;
  QDomNode node = nodeMap.namedItem("id");
  if (!node.isNull()) {
    m_modified.id = m_uniqueIdList->append(node.nodeValue(), node.lineNumber());
  }
  if (!m_date.isValid()) {
    // if no document release date is set then set it to the modified value.
    // this will be overridden if a dc:date meta node is found later.
    m_date = m_modified.date;
  }
}

/*!
 * \brief Writes a creator or contributors metadata.
 *
 * Firsts decides whether we are writing a creator or a contributor, then
 * passes the object to the actual write method.
 *
 * \param tag_name
 * \param xml_writer
 * \param key
 * \return
 */
QString
EPubMetadata::writeCreatorContibutor(QString tag_name,
                                     QXmlStreamWriter* xml_writer,
                                     QString key)
{
  // Basically contributors and creators are identical in form if not in
  // definition, contributors are lesser ctreators effectively.
  QSharedPointer<EPubCreator> shared_object;
  QString name;

  // As EPubContributor is basically an unchanged EPubCreator we only need to
  // do this bit to make certain we get the correct data store.
  QSharedPointer<EPubCreator> shared_creator =
    m_creatorsByName.value(key).dynamicCast<EPubCreator>();
  if (shared_creator) {
    shared_object = shared_creator;
  } else {
    QSharedPointer<EPubContributor> shared_contributor =
      contributorsByName.value(key).dynamicCast<EPubContributor>();
    shared_object = shared_contributor;
  }

  name = writeCreatorContributor(tag_name, xml_writer, shared_object);

  return name;
}

void
EPubMetadata::writeLanguageMetadata(QXmlStreamWriter* xml_writer,
                                    UniqueString key,
                                    bool first)
{
  /*
   * There must be at least one language element in 2.0 & 3.0.
   * refines metas will be ignored by a 2.0 reader so for backwards
   * compatability set the first language in both 2.0 & 3.0 form.
   * 2.0 and 3.0 with no id set.
   *  <dc:language>en-US</language>
   * 3.1 but readable by both as id will be ignored by 2.0.
   *  <dc:language id="lang1">en-US</language>
   */
  QSharedPointer<EPubLanguage> sharedLanguage = m_languages.value(key);
  if (!sharedLanguage.isNull()) {
    UniqueString id = sharedLanguage->id;

    if (!sharedLanguage->language.isEmpty()) {
      xml_writer->writeStartElement("dc:language");

      if (!id.isEmpty()) { // 2.0 and 3.0 with id set.
        xml_writer->writeAttribute("id", id.toString());
        if (first) {
          xml_writer->writeCharacters(sharedLanguage->language.trimmed());
        }
      } else { // 2.0 and 3.0 without id set
        xml_writer->writeCharacters(sharedLanguage->language.trimmed());
      }
      xml_writer->writeEndElement();

      // refines is superceded in 3.1
      //      if (!id.isEmpty()) { // 3.0 with id set
      //        xml_writer->writeStartElement("meta");
      //        xml_writer->writeAttribute("refines", "#" + id);
      //        xml_writer->writeAttribute("property", "language");
      //        xml_writer->writeCharacters(shared_language->language.trimmed());
      //        xml_writer->writeEndElement();
      //      }
    }
  }
}

void
EPubMetadata::writeSubjectMetadata(QXmlStreamWriter* xml_writer,
                                   UniqueString key)
{
  QSharedPointer<EPubSubject> shared_subject = m_subjects.value(key);

  if (!shared_subject.isNull()) {
    if (!shared_subject->subject.isEmpty()) {
      xml_writer->writeStartElement("dc:subject");

      writeIdAttribute(xml_writer, shared_subject->id);
      writeDirAttribute(xml_writer, shared_subject->dir);
      writeLangAttribute(xml_writer, shared_subject->lang);
      writeAuthorityAttribute(
        xml_writer, shared_subject->authority, shared_subject->term);

      xml_writer->writeCharacters(shared_subject->subject);
      xml_writer->writeEndElement();
    }
  }
}

void
EPubMetadata::writeIdentifierMetadata(
  QXmlStreamWriter* xml_writer,
  EBookIdentifierScheme::IdentifierScheme key)
{
  QSharedPointer<EBookIdentifier> shared_identifier = m_identifiers.value(key);
  if (!shared_identifier->name.isEmpty()) {
    xml_writer->writeStartElement("dc:identifier");
    writeIdAttribute(xml_writer, shared_identifier->id);
    writeSchemeAttribute(xml_writer, shared_identifier->identifier);

    xml_writer->writeCharacters(shared_identifier->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeTitle(QXmlStreamWriter* xml_writer,
                         QSharedPointer<EPubTitle> sharedTitle)
{
  UniqueString id = sharedTitle->id;

  if (!sharedTitle->title.isEmpty()) {
    xml_writer->writeStartElement("dc:title");

    if (!id.isEmpty()) { // 3.0
      xml_writer->writeAttribute("id", id.toString());
    }

    // these are optional attributes.
    writeDirAttribute(xml_writer, sharedTitle->dir);
    writeLangAttribute(xml_writer, sharedTitle->lang);
    QList<QSharedPointer<EPubAltRep>> alt_reps = sharedTitle->altRepList;
    for (QSharedPointer<EPubAltRep> alt_rep : alt_reps) {
      writeAltRepAttribute(xml_writer, alt_rep);
    }
    QList<QSharedPointer<EPubFileAs>> file_as_list = sharedTitle->fileAsList;
    for (QSharedPointer<EPubFileAs> file_as : file_as_list) {
      writeFileAsAttribute(xml_writer, file_as);
    }

    xml_writer->writeCharacters(sharedTitle->title);
    xml_writer->writeEndElement();
  }
}

QString
EPubMetadata::writeTitleMetadata(QXmlStreamWriter* xml_writer, int key)
{
  QSharedPointer<EPubTitle> shared_title = m_orderedTitles.value(key);
  QString title;
  if (!shared_title.isNull()) {
    title = shared_title->title;
    writeTitle(xml_writer, shared_title);
  }
  return title;
}

void
EPubMetadata::writeDescriptionMetadata(QXmlStreamWriter* xml_writer)
{
  if (!description->text.isEmpty()) {
    xml_writer->writeStartElement("dc:description");

    writeIdAttribute(xml_writer, description->id);
    writeDirAttribute(xml_writer, description->dir);
    writeLangAttribute(xml_writer, description->language);

    xml_writer->writeCharacters(description->text);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeSourceMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubSource> shared_source = m_source;
  if (!shared_source->source.isEmpty()) {
    xml_writer->writeStartElement("dc:source");
    writeIdAttribute(xml_writer, shared_source->id);
    writeSchemeAttribute(xml_writer, shared_source->scheme);
    xml_writer->writeCharacters(shared_source->source);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writePublisherMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubPublisher> shared_publisher = m_publisher;
  if (!shared_publisher->name.isEmpty()) {

    xml_writer->writeStartElement("dc:publisher");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeFileAsAttribute(xml_writer, shared_publisher->fileAs);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);
    writeAltRepAttribute(xml_writer, shared_publisher->altRep);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeFormatMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubFormat> shared_format = m_format;
  if (!shared_format->name.isEmpty()) {
    xml_writer->writeStartElement("dc:format");

    writeIdAttribute(xml_writer, shared_format->id);

    xml_writer->writeCharacters(shared_format->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeRelationMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubRelation> shared_publisher = m_relation;
  if (!shared_publisher->name.isEmpty()) {
    xml_writer->writeStartElement("dc:relation");

    writeIdAttribute(xml_writer, shared_publisher->id);
    writeDirAttribute(xml_writer, shared_publisher->dir);
    writeLangAttribute(xml_writer, shared_publisher->lang);

    xml_writer->writeCharacters(shared_publisher->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeRightsMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubRights> shared_rights = m_rights;
  if (!shared_rights->name.isEmpty()) {
    xml_writer->writeStartElement("dc:rights");

    writeIdAttribute(xml_writer, shared_rights->id);
    writeDirAttribute(xml_writer, shared_rights->dir);
    writeLangAttribute(xml_writer, shared_rights->lang);

    xml_writer->writeCharacters(shared_rights->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeCoverageMetadata(QXmlStreamWriter* xml_writer)
{
  QSharedPointer<EPubCoverage> shared_coverage = m_coverage;
  if (!shared_coverage->name.isEmpty()) {
    xml_writer->writeStartElement("dc:coverage");

    writeIdAttribute(xml_writer, shared_coverage->id);
    writeDirAttribute(xml_writer, shared_coverage->dir);
    writeLangAttribute(xml_writer, shared_coverage->lang);

    xml_writer->writeCharacters(shared_coverage->name);
    xml_writer->writeEndElement();
  }
}

void
EPubMetadata::writeIdAttribute(QXmlStreamWriter* xml_writer,
                               const UniqueString& id)
{
  if (!id.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("id", id.toString());
  }
}

void
EPubMetadata::writeDirAttribute(QXmlStreamWriter* xml_writer, Direction dir)
{
  xml_writer->writeAttribute("dir", directionStr(dir));
}

void
EPubMetadata::writeLangAttribute(QXmlStreamWriter* xml_writer, QString lang)
{
  if (!lang.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("xml:lang", lang);
  }
}

void
EPubMetadata::writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
                                      QString authority,
                                      QString term)
{
  if (!authority.isEmpty()) { // 3.0.
    xml_writer->writeAttribute("opf:authority", authority);
  }

  if (!authority.isEmpty() && !term.isEmpty()) { // 2.0 and 3.0 with id set.
    xml_writer->writeAttribute("opf:term", term);
  }
}

void
EPubMetadata::writeAltRepAttribute(QXmlStreamWriter* xml_writer,
                                   QSharedPointer<EPubAltRep> alt_rep)
{
  if (alt_rep) {
    if (!alt_rep->name.isEmpty()) { // 3.0
      xml_writer->writeAttribute("opf:alt-rep", alt_rep->name);
    }
    if (!alt_rep->lang.isEmpty()) { // 3.0
      xml_writer->writeAttribute("opf:alt-rep-lang", alt_rep->lang);
    }
  }
}

void
EPubMetadata::writeFileAsAttribute(QXmlStreamWriter* xml_writer,
                                   QSharedPointer<EPubFileAs> file_as)
{
  if (file_as) { // 3.0
    if (file_as->name.isEmpty())
      xml_writer->writeAttribute("opf:file-as", file_as->name);
    writeLangAttribute(xml_writer, file_as->lang);
  }
}

QString
EPubMetadata::writeCreatorsMetadata(QXmlStreamWriter* xml_writer, QString key)
{
  return writeCreatorContibutor("dc:creator", xml_writer, key);
}

QString
EPubMetadata::writeContributorMetadata(QXmlStreamWriter* xml_writer,
                                       QString key)
{
  return writeCreatorContibutor("dc:contributor", xml_writer, key);
}

void
EPubMetadata::writeRoleAttribute(QXmlStreamWriter* xml_writer,
                                 QSharedPointer<MarcRelator> relator)
{
  if (relator->type() != MarcRelator::NO_TYPE) {
    xml_writer->writeAttribute("opf:role", relator->code());
  }
}

void
EPubMetadata::writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                                   EBookIdentifierScheme scheme)
{
  writeSchemeAttribute(xml_writer,
                       EBookIdentifierScheme::toString(scheme.scheme));
}

void
EPubMetadata::writeSchemeAttribute(QXmlStreamWriter* xml_writer, QString scheme)
{
  if (!scheme.isEmpty()) {
    xml_writer->writeAttribute("opf:scheme", scheme);
  }
}

/*!
 * \brief Writes the creator or contributors metadata.
 *
 * \param tag_name
 * \param xml_writer
 * \param shared_object
 */
QString
EPubMetadata::writeCreatorContributor(QString tag_name,
                                      QXmlStreamWriter* xml_writer,
                                      QSharedPointer<EPubCreator> sharedObject)
{
  QString name;
  if (!sharedObject.isNull()) {
    UniqueString id = sharedObject->id;
    name = sharedObject->name;

    if (!sharedObject->name.isEmpty()) {
      xml_writer->writeStartElement(tag_name);

      writeIdAttribute(xml_writer, sharedObject->id);
      for (QSharedPointer<EPubFileAs> file_as : sharedObject->fileAsList) {
        writeFileAsAttribute(xml_writer, file_as);
      }
      for (QSharedPointer<EPubAltRep> alt_rep : sharedObject->altRepList) {
        writeAltRepAttribute(xml_writer, alt_rep);
      }
      writeRoleAttribute(xml_writer, sharedObject->relator);

      xml_writer->writeCharacters(sharedObject->name);
      xml_writer->writeEndElement();

      for (auto foaf : sharedObject->foafList) {
        xml_writer->writeStartElement("meta");
        xml_writer->writeAttribute("property", foaf->asString());
        writeIdAttribute(xml_writer, foaf->id());
        writeLangAttribute(xml_writer, foaf->lang());
        writeSchemeAttribute(xml_writer, foaf->scheme());
        xml_writer->writeCharacters(foaf->content());
        xml_writer->writeEndElement();
      }
    }
  }
  return name;
}

//============================================================================
//==== EPubCalibre
//==============================================================================
EPubCalibre::EPubCalibre()
  : m_modified(false)
{}

QString
EPubCalibre::seriesName() const
{
  return m_seriesName;
}

void
EPubCalibre::setSeriesName(const QString& series_name)
{
  m_seriesName = series_name;
}

QString
EPubCalibre::seriesIndex() const
{
  return m_seriesIndex;
}

void
EPubCalibre::setSeriesIndex(const QString& series_index)
{
  m_seriesIndex = series_index;
}

QString
EPubCalibre::titleSort() const
{
  return m_titleSort;
}

void
EPubCalibre::setTitleSort(const QString& title_sort)
{
  m_titleSort = title_sort;
}

QString
EPubCalibre::authorLinkMap() const
{
  return m_authorLinkMap;
}

void
EPubCalibre::setAuthorLinkMap(const QString& author_link_map)
{
  m_authorLinkMap = author_link_map;
}

QString
EPubCalibre::timestamp() const
{
  return m_timestamp;
}

void
EPubCalibre::setTimestamp(const QString& timestamp)
{
  m_timestamp = timestamp;
}

QString
EPubCalibre::rating() const
{
  return m_rating;
}

void
EPubCalibre::setRating(const QString& rating)
{
  m_rating = rating;
}

QString
EPubCalibre::publicationType() const
{
  return m_publicationType;
}

void
EPubCalibre::setPublicationType(const QString& publication_type)
{
  m_publicationType = publication_type;
}

QString
EPubCalibre::userMetadata() const
{
  return m_userMetadata;
}

void
EPubCalibre::setUserMetadata(const QString& user_metadata)
{
  m_userMetadata = user_metadata;
}

QString
EPubCalibre::userCategories() const
{
  return m_userCategories;
}

void
EPubCalibre::setUserCategories(const QString& user_categories)
{
  m_userCategories = user_categories;
}

QString
EPubCalibre::customMetadata() const
{
  return m_customMetadata;
}

void
EPubCalibre::setCustomMetadata(const QString& custom_metadata)
{
  m_customMetadata = custom_metadata;
}

bool
EPubCalibre::isModified() const
{
  return m_modified;
}

void
EPubCalibre::setModified(bool modified)
{
  m_modified = modified;
}

//============================================================================
//==== EBookManifest
//============================================================================
EPubManifest::EPubManifest() {}

EPubManifest::~EPubManifest() {}

//============================================================================
//==== EBookManifest
//============================================================================
EPubTitle::EPubTitle() {}

EPubCreator::EPubCreator() {}

EPubCreator::~EPubCreator()
{
  foafList.clear();
  fileAsList.clear();
  altRepList.clear();
}

//============================================================================
//==== Freestanding functions
//============================================================================
Direction
direction(const QString& dir)
{
  if (dir.toLower() == "rtl") {
    return RTL;
  }
  return LTR;
}

QString
directionStr(Direction dir)
{
  switch (dir) {
    case RTL:
      return "rtl";
    default:
      return "ltr";
  }
}

//============================================================================
//==== UniqueString
//============================================================================
UniqueString::UniqueString()
  : m_unique(true)
{}

UniqueString::UniqueString(const QString& other, bool unique)
  : m_data(other)
  , m_unique(unique)
{}

UniqueString::~UniqueString() {}

bool
UniqueString::isUnique() const
{
  return m_unique;
}

void
UniqueString::setUnique(bool unique)
{
  m_unique = unique;
}

bool
operator==(const UniqueString& lhs, const UniqueString& rhs)
{
  return (lhs.toString() < rhs.toString());
}

bool
operator==(const QString& lhs, const UniqueString& rhs)
{
  return (lhs < rhs.toString());
}

bool
operator==(const UniqueString& lhs, const QString& rhs)
{
  return (lhs.toString() < rhs);
}

bool
operator<(const UniqueString& lhs, const UniqueString& rhs)
{
  return (lhs.toString() < rhs.toString());
}

//============================================================================
//==== UniqueStringList
//============================================================================
UniqueStringList::UniqueStringList() {}

UniqueStringList::~UniqueStringList() {}

UniqueString
UniqueStringList::append(const QString& value, int lineNumber)
{
  UniqueString uniqueValue(value, true);
  if (m_allValues.contains(value)) {
    m_unique.removeAll(value);
    uniqueValue.setUnique(false);
    if (!m_nonUnique.contains(uniqueValue)) {
      m_nonUnique.append(uniqueValue.toString());
    }
  } else {
    m_unique.append(uniqueValue.toString());
  }
  m_allValues.append(uniqueValue.toString());
  m_lineNumbers.append(lineNumber);
  return uniqueValue;
}

//! Returns a list of all non-unique values.
//!
//! Only one copy of each is returned in this list.
QStringList
UniqueStringList::nonUniqueStringList()
{
  return m_nonUnique;
}

//! Returns a list of all unique values.
//!
//! Only one copy of each is returned in this list.
QStringList
UniqueStringList::uniqueList() const
{
  return m_unique;
}

//! Returns a list of all values.
//!
//! Multiple values of non-unique strings will be
//! stored in this list.
QStringList
UniqueStringList::allValues() const
{
  return m_allValues;
}

QList<int>
UniqueStringList::lineNumbers(QString value)
{
  QList<int> lineNumbers;
  int last = 0;
  for (auto i = 0; i < m_allValues.size(); i++) {
    if (m_allValues.indexOf(value, last) > -1) {
      last = i;
      lineNumbers.append(m_lineNumbers.at(i));
    }
  }
  return lineNumbers;
}

bool
UniqueStringList::contains(const QString& value)
{
  return m_allValues.contains(value);
}

bool
UniqueStringList::contains(const UniqueString& value)
{
  return m_allValues.contains(value.toString());
}

//============================================================================
//==== UniqueStringList
//============================================================================
// UniqueStringMap::UniqueStringMap()
//{

//}
