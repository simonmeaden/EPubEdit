#ifndef EBOOKMETADATA_H
#define EBOOKMETADATA_H

#include "libepubedit/authors.h"
#include "libepubedit/ebookbasemetadata.h"
#include "libepubedit/library.h"
#include "metadata/foaf.h"

class QXmlStreamWriter;

class EBookMetadata : public EBookBaseMetadata
{
public:
  EBookMetadata();

  void parse(QDomNodeList metadata_node_list);
  bool write(QXmlStreamWriter* xml_writer);

  QString uniqueIdentifierName() const;
  void setUniqueIdentifierName(const QString& package_unique_identifier_name);

  bool isFoaf() const;
  void setIsFoaf(bool value);

  QStringList creatorList() const;
  void setCreatorList(const QStringList& creator_list);

  OrderedTitleMap orderedTitles() const;
  void setOrderedTitles(const OrderedTitleMap& ordered_titles);

  Calibre calibre() const;
  void setCalibre(const Calibre& calibre);

protected:
  QString m_packageUniqueIdentifier;
  QString m_packageUniqueIdentifierName;
  IdentifierMap m_identifiers;
  TitleMap m_titlesById;
  TitleMap m_titlesByName;
  OrderedTitleMap m_orderedTitles;
  CreatorMap m_creatorsById;
  CreatorMap m_creatorsByName;
  //  SharedCreatorMap creators_by_name;
  ContributorMap m_contributorsById;
  ContributorMap m_contributorsByName;
  Description m_description;
  QStringList m_creatorList;
  LanguageMap m_languages;
  SubjectMap m_subjects;
  EBookModified m_modified;
  QDateTime m_date;
  Source m_source;
  Publisher m_publisher;
  Format m_format;
  Relation m_relation;
  Coverage m_coverage;
  Rights m_;
  BookType m_type;
  QMap<QString, QString> m_extraMetas;
  Calibre m_calibre;
  bool m_isFoaf;

  bool parseMetadataItem(const QDomNode& metadataNode);
  void parseTitleMetadata(QDomElement metadata_element);
  void parseCreatorMetadata(QDomElement metadata_element);
  void parseContributorMetadata(QDomElement metadata_element);
  void parseDescriptionMetadata(QDomElement metadata_element);
  void parseIdentifierMetadata(QDomElement metadata_element);
  void parseLanguageMetadata(QDomElement metadata_element);
  void parseSubjectMetadata(QDomElement metadata_element);
  void parseDateModified(QDomNamedNodeMap node_map, QString text);
  void parseSourceMetadata(const QDomElement& metadata_element);
  void parsePublisherMetadata(const QDomElement& metadata_element);
  void parseFormatMetadata(const QDomElement& metadata_element);
  void parseTypeMetadata(const QDomElement& metadata_element);
  void parseRelationMetadata(const QDomElement& metadata_element);
  void parseCoverageMetadata(const QDomElement& metadata_element);
  void parseRightsMetadata(const QDomElement& metadata_element);

  void parseDublinCoreMeta(QString tag_name,
                           QDomElement& metadata_element,
                           QDomNamedNodeMap& node_map);
  void parseOpfMeta(QString tag_name,
                    QDomElement& metadata_element,
                    QDomNamedNodeMap&);
  void parseCalibreMetas(QString id, QDomNode& node);
  void parseRefineMetas(QDomElement& metadata_element,
                        QDomNode& node,
                        QDomNamedNodeMap& node_map);
  void parseTitleDateRefines(Title shared_title, QDomElement& metadata_element);
  void parseCreatorContributorRefines(Creator shared_creator,
                                      QDomElement& metadata_element,
                                      QString id,
                                      QDomNamedNodeMap node_map);
  void parseTitleRefines(QString id,
                         QDomElement metadata_element,
                         QDomNamedNodeMap node_map);
  Foaf parseCreatorContributorFoafAttributes(QString property,
                                             QDomNamedNodeMap node_map);

  void writeCreator(QXmlStreamWriter* xml_writer, Creator shared_creator);
  void writeContributor(QXmlStreamWriter* xml_writer, Creator shared_creator);
  QString writeTitleMetadata(QXmlStreamWriter* xml_writer, int key);
  void writeDescriptionMetadata(QXmlStreamWriter* xml_writer);
  QString writeCreatorsMetadata(QXmlStreamWriter* xml_writer, QString key);
  QString writeContributorMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeLanguageMetadata(QXmlStreamWriter* xml_writer,
                             QString key,
                             bool first = false);
  void writeSubjectMetadata(QXmlStreamWriter* xml_writer, QString key);
  void writeIdentifierMetadata(QXmlStreamWriter* xml_writer,
                               EBookIdentifierScheme::IdentifierScheme key);
  void writeSourceMetadata(QXmlStreamWriter* xml_writer);
  void writePublisherMetadata(QXmlStreamWriter* xml_writer);
  void writeFormatMetadata(QXmlStreamWriter* xml_writer);
  void writeRelationMetadata(QXmlStreamWriter* xml_writer);
  void writeRightsMetadata(QXmlStreamWriter* xml_writer);
  void writeCoverageMetadata(QXmlStreamWriter* xml_writer);

  void writeTitle(QXmlStreamWriter* xml_writer, Title shared_title);
  QString writeCreatorContibutor(QString tag_name,
                                 QXmlStreamWriter* xml_writer,
                                 QString key);
  QString writeCreatorContributor(QString tag_name,
                                  QXmlStreamWriter* xml_writer,
                                  Creator shared_creator);
  void writeIdAttribute(QXmlStreamWriter* xml_writer, QString id);
  void writeDirAttribute(QXmlStreamWriter* xml_writer, QString dir);
  void writeLangAttribute(QXmlStreamWriter* xml_writer, QString dir);
  void writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
                               QString authority,
                               QString term);
  void writeAltRepAttribute(QXmlStreamWriter* xml_writer, AltRep alt_rep);
  void writeFileAsAttribute(QXmlStreamWriter* xml_writer, FileAs file_as);
  void writeRoleAttribute(QXmlStreamWriter* xml_writer, MarcRelator relator);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                            EBookIdentifierScheme scheme);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer, QString scheme);

  int getNextTitleIndex();
};
typedef QSharedPointer<EBookMetadata> Metadata;

#endif // EBOOKMETADATA_H
