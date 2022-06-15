#ifndef EBOOKMETADATA_H
#define EBOOKMETADATA_H

#include <QCoreApplication> // needed for Q_DECLARE_TR_FUNCTIONS macro
#include <QDateTime>
#include <QDomNodeList>
#include <QTextCursor>
#include <QXmlStreamWriter>

class EPubDocument;
class Foaf;
class MarcRelator;
class NavListItem;
class NavItem;
class NavTag;

#include "uniquestring.h"
#include "document/bookpointers.h"
#include "document/bookstructs.h"
#include "document/bookenums.h"

class Metadata : public DirectionFunc
{
  Q_DECLARE_TR_FUNCTIONS(EPubMetadata)

public:
  Metadata();
  ~Metadata();

  void parse(QDomNodeList metadataNodeList);
  bool write(QXmlStreamWriter* xml_writer);

  UniqueString uniqueIdentifierName() const;
  void setUniqueIdentifierName(const UniqueString& packageUniqueIdentifierName);

  bool isFoaf() const;
  void setIsFoaf(bool value);

  QStringList creatorList() const;

  PCreator creatorFromName(QString name);

  QStringList contributorList();
  PContributor contributorFromName(QString name);

  QList<PTitle> orderedTitles() const;
  void setOrderedTitles(const QList<PTitle>& ordered_titles);

  PCalibre calibre() const;
  void setCalibre(const PCalibre& calibre);

  PRights rights() const;

  QDateTime date();

  const QMap<UniqueString, PLanguage> &languages() const;

  const QMap<EBookIdentifierScheme::IdentifierScheme, PIdentifier> &identifiers() const;

  NavListItem *navList() const;
  void setNavList(NavListItem *newNavList);

private:
//  EPubDocument* m_document;
  UniqueString m_packageUniqueIdentifier;
  UniqueString m_packageUniqueIdentifierName;
  QMap<EBookIdentifierScheme::IdentifierScheme, PIdentifier>
    m_identifiers;
  UniqueString m_primaryTitleId;
  QString m_primaryTitle;
  QMap<QString, PTitle> m_titlesById;
  QMap<QString, PTitle> m_titlesByName;
  QList<PTitle> m_orderedTitles;
  QMap<QString, PCreator> m_creatorsById;
  QMultiMap<QString, PCreator> m_creatorsByName;
  //  SharedCreatorMap creators_by_name;
  QMultiMap<UniqueString, PContributor> m_contributorsById;
  QMultiMap<QString, PContributor> m_contributorsByName;
  PDescription m_description;
  QMap<UniqueString, PLanguage> m_languages;
  QMap<UniqueString, PSubject> m_subjects;
  EPubModified m_modified;
  QDateTime m_date;
  PSource m_source;
  PAttribution m_attribution;
  //  PManifestItem m_publisher;
  PFormat m_format;
  PRelation m_relation;
  PCoverage m_coverage;
  PRights m_rights;
  PType m_type;
  QMap<UniqueString, QString> m_extraMetas;
  PCalibre m_calibre;
  bool m_isFoaf;
  NavListItem* m_navList;


  bool parseMetadataItem(const QDomNode& metadata_node);
  void parseDCTitleMetadata(QDomElement metadataElement,
                            QDomNamedNodeMap& attributeMap);
  void parseDCCreatorMetadata(QDomElement metadataElement,
                              QDomNamedNodeMap& attributeMap);
  void parseDCContributorMetadata(QDomElement metadataElement,
                                  QDomNamedNodeMap& attributeMap);
  void parseDCDescriptionMetadata(QDomElement metadataElement,
                                  QDomNamedNodeMap& attributeMap);
  void parseDCIdentifierMetadata(QDomElement metadataElement,
                                 QDomNamedNodeMap& attributeMap);
  void parseDCLanguageMetadata(QDomElement metadataElement,
                               QDomNamedNodeMap& attributeMap);
  void parseDCSubjectMetadata(QDomElement metadataElement,
                              QDomNamedNodeMap& attributeMap);
  void parseDateModified(QDomNamedNodeMap attributeMap, QString text);
  void parseDCSourceMetadata(const QDomElement& metadataElement,
                             QDomNamedNodeMap& attributeMap);
  void parseDCPublisherMetadata(const QDomElement& metadataElement,
                                QDomNamedNodeMap& attributeMap);
  void parseDCFormatMetadata(const QDomElement& metadataElement,
                             QDomNamedNodeMap& attributeMap);
  void parseDCTypeMetadata(const QDomElement& metadataElement,
                           QDomNamedNodeMap& attributeMap);
  void parseDCRelationMetadata(const QDomElement& metadataElement,
                               QDomNamedNodeMap& attributeMap);
  void parseDCCoverageMetadata(const QDomElement& metadataElement,
                               QDomNamedNodeMap& attributeMap);
  void parseDCRightsMetadata(const QDomElement& metadataElement,
                             QDomNamedNodeMap& attributeMap);
  void parseDCDateMetadata(const QDomElement& metadataElement,
                           QDomNamedNodeMap& attributeMap);

  void parseDublinCoreMeta(QString tagName,
                           QDomElement& metadataElement,
                           QDomNamedNodeMap& attributeMap);
  void parseOpfMeta(QString tagName,
                    QDomElement& metadataElement,
                    QDomNamedNodeMap& attributeMap);
  void parseCalibreMetas(const UniqueString& id, QDomNode& node);
  void parseRefineMetas(QDomElement& metadataElement,
                        QDomNode& node,
                        QDomNamedNodeMap& attributeMap);
  void parseTitleDateRefines(PTitle shared_title,
                             QDomElement& metadataElement);
  void parseCreatorContributorRefines(
    PCreatorContributorBase creator,
    QDomElement& metadataElement,
    const UniqueString& id,
    QDomNamedNodeMap attributeMap);
  void parseTitleRefines(const QString& id,
                         QDomElement metadataElement,
                         QDomNamedNodeMap attributeMap);
  void parseCreatorRefines(const QString& id,
                           QDomElement metadataElement,
                           QDomNamedNodeMap attributeMap);
  void parseContributorRefines(const UniqueString& id,
                               QDomElement metadataElement,
                               QDomNamedNodeMap attributeMap);
  PFoaf parseCreatorContributorFoafAttributes(
    QString property,
    QDomNamedNodeMap attributeMap);

  void writeCreator(QXmlStreamWriter* xml_writer,
                    PCreatorContributorBase shared_creator);
  void writeContributor(
    QXmlStreamWriter* xml_writer,
    PCreatorContributorBase shared_creator);
  QString writeTitleMetadata(QXmlStreamWriter* xml_writer, int key);
  void writeDescriptionMetadata(QXmlStreamWriter* xml_writer);
  QString writeCreatorsMetadata(QXmlStreamWriter* xml_writer, QString key);
  QString writeContributorMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeLanguageMetadata(QXmlStreamWriter* xml_writer,
                             UniqueString key,
                             bool first = false);
  void writeSubjectMetadata(QXmlStreamWriter* xml_writer, UniqueString key);
  void writeIdentifierMetadata(QXmlStreamWriter* xml_writer,
                               EBookIdentifierScheme::IdentifierScheme key);
  void writeSourceMetadata(QXmlStreamWriter* xml_writer);
  void writePublisherMetadata(QXmlStreamWriter* xml_writer);
  void writeFormatMetadata(QXmlStreamWriter* xml_writer);
  void writeRelationMetadata(QXmlStreamWriter* xml_writer);
  void writeRightsMetadata(QXmlStreamWriter* xml_writer);
  void writeCoverageMetadata(QXmlStreamWriter* xml_writer);

  void writeTitle(QXmlStreamWriter* xml_writer,
                  PTitle shared_title);
  QString writeCreatorContibutor(QString tag_name,
                                 QXmlStreamWriter* xml_writer,
                                 QString key);
  QString writeCreatorContributor(
    QString tag_name,
    QXmlStreamWriter* xml_writer,
    PCreatorContributorBase shared_creator);
  void writeIdAttribute(QXmlStreamWriter* xml_writer, const UniqueString& id);
  void writeDirAttribute(QXmlStreamWriter* xml_writer, Direction dir);
  void writeLangAttribute(QXmlStreamWriter* xml_writer, QString dir);
  void writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
                               QString authority,
                               QString term);
  void writeAltRepAttribute(QXmlStreamWriter* xml_writer,
                            PAltRep alt_rep);
  void writeFileAsAttribute(QXmlStreamWriter* xml_writer,
                            PFileAs file_as);
  void writeRoleAttribute(QXmlStreamWriter* xml_writer,
                          PMarcRelator relator);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                            EBookIdentifierScheme scheme);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer, QString scheme);

  //  int getNextTitleIndex();

  void deleteItem(NavItem* list);
  void deleteTag(NavTag *tagList);

};


#endif // EBOOKMETADATA_H
