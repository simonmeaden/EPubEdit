#ifndef EBOOKMETADATA_H
#define EBOOKMETADATA_H

#include <QCoreApplication> // needed for Q_DECLARE_TR_FUNCTIONS macro
#include <QDateTime>
#include <QDomNodeList>
#include <QTextCursor>
#include <QXmlStreamWriter>

class EPubDocument;
class QXmlStreamWriter;
class Foaf;
class MarcRelator;

#include "uniquestring.h"

enum Direction
{
  LTR,
  RTL,
  DEFAULT,
};
Q_DECLARE_METATYPE(Direction)

struct DirectionFunc
{
  static Direction convertDirection(const QString& direction);

  static QString directionStr(Direction direction);
};

struct DirectionBase : public DirectionFunc
{
  Direction dir;

  void setDirection(const QString& direction)
  {
    dir = convertDirection(direction);
  }
};

struct EPubNavPoint
{
  EPubNavPoint(const QString& classname,
               const UniqueString& id,
               QString label,
               QString src)
  {
    this->classname = classname;
    this->id = id;
    this->label = label;
    this->src = src;
  }
  QString classname;
  UniqueString id;
  QString label;
  QString src;
};

enum DocumentType
{
  PLAINTEXT,
  HTML
};

enum EPubDocumentType
{
  UNSUPPORTED_TYPE,
  EPUB,
  MOBI,
  AZW,
  PDF,
};

struct EPubManifestItem
{
  QString href;
  QString path;
  QString documentString;
  QStringList cssLinks;
  QString bodyClass;
  UniqueString id;
  QString mediaType;
  QStringList properties;
  QString fallback;
  QString mediaOverlay;
  QMap<QString, QString> nonStandardProperties;
};

struct EPubTocItem
{
  UniqueString id;
  int playorder;
  QString tagClass;
  QString label;
  QString source;
  QMap<int, QSharedPointer<EPubTocItem>> subItems;
  QString chapterTag;
};

struct Manifest
{
  Manifest();
  ~Manifest();
  UniqueString id;
  QSharedPointer<EPubManifestItem> coverImage;                      // 0 or 1
  QSharedPointer<EPubManifestItem> nav;                             // 1
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> itemsById;   // all items
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> itemsByHref; // all items
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> htmlItems;
  QMap<UniqueString, QSharedPointer<EPubManifestItem>>
    mathml; // subset of items for math markup
  QMap<UniqueString, QSharedPointer<EPubManifestItem>>
    svgImages; // subset of items for images
  UniqueStringMap images;
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> remotes;
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> scripted;
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> switches;
  UniqueStringMap css;                                        // all items
  UniqueStringMap javascript;                                 // all items
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> fonts; // all items
  QMap<UniqueString, QSharedPointer<EPubManifestItem>>
    mediaOverlay; // all items
  QString formattedTocString;
  QMap<int, QSharedPointer<EPubTocItem>> tocTtems;
  QMap<QString, QSharedPointer<EPubTocItem>> tocPaths;
};

enum PageSpread {
  LEFT,
  RIGHT,
  NONE,
};

struct EPubSpineItem
{
  EPubSpineItem()
    : linear(true)
    , pageSpread(NONE)
  {}

  UniqueString id;
  UniqueString idref;
  bool linear;
  PageSpread pageSpread;
 };

struct EPubSpine : public DirectionBase
{
  UniqueString id;
  UniqueString tocId;
  QList<UniqueString> orderedItems;
  QMap<UniqueString, QSharedPointer<EPubSpineItem>> items;
};

struct EPubModified
{
  QDateTime date;
  UniqueString id;
};

struct EPubAltRep
{
  QString name;
  QString lang;
};

struct EPubFileAs
{
  QString name;
  QString lang;
};

struct EPubTitle : public DirectionBase
{

  EPubTitle();

  UniqueString id;
  QString title;
  QString lang;
  QList<QSharedPointer<EPubAltRep>> altRepList;
  QList<QSharedPointer<EPubFileAs>> fileAsList;
  QDate date;
};

class EPubLanguage
{
public:
  UniqueString id;
  QString language;
};

struct EPubAttribution
{
  QString url;
  QString name;
  QString license;
};

class EPubSubject : public DirectionBase
{
public:
  UniqueString id;
  QString subject;
  QString authority;
  QString term;
  QString lang;
};

class EBookIdentifierScheme
{
public:
  enum IdentifierScheme
  {
    DOI,
    ISBN,
    JDCN,
    UUID,
    AMAZON,
    CALIBRE,
    UNKNOWN_SCHEME,
  };

  EBookIdentifierScheme() { scheme = IdentifierScheme::UNKNOWN_SCHEME; }

  IdentifierScheme scheme;

  static EBookIdentifierScheme fromString(QString scheme_type)
  {
    QString type = scheme_type.toLower();
    EBookIdentifierScheme scheme;
    if (type == "doi") {
      scheme.scheme = DOI;
    } else if (type == "isbn") {
      scheme.scheme = ISBN;
    } else if (type == "jdcn") {
      scheme.scheme = JDCN;
    } else if (type == "uuid") {
      scheme.scheme = UUID;
    } else if (type == "mobi-asin") {
      scheme.scheme = AMAZON;
    } else if (type == "calibre") {
      scheme.scheme = CALIBRE;
    } else {
      scheme.scheme = UNKNOWN_SCHEME;
    }
    return scheme;
  }
  static QString toString(EBookIdentifierScheme::IdentifierScheme scheme)
  {
    switch (scheme) {
      case DOI:
        return "doi";
      case ISBN:
        return "isbn";
      case JDCN:
        return "jdcn";
      case UUID:
        return "uuid";
      case AMAZON:
        return "mobi-asin";
      case CALIBRE:
        return "calibre";
      default:
        return QString();
    }
  }
};

struct EBookIdentifier
{
  QString name;
  UniqueString id;
  EBookIdentifierScheme identifier;
};

struct EPubSource
{
  QString source;
  UniqueString id;
  EBookIdentifierScheme scheme;
};

struct EPubPublisher : public DirectionBase
{
  QString name;
  QSharedPointer<EPubFileAs> fileAs;
  UniqueString id;
  QSharedPointer<EPubAltRep> altRep;
  QString lang;
};

struct EPubRelation : public DirectionBase
{
  QString name;
  UniqueString id;
  QString lang;
};

struct EPubRights : public DirectionBase
{
  QString name;
  UniqueString id;
  QString lang;
};

struct EPubCoverage : public DirectionBase
{
  QString name;
  UniqueString id;
  QString lang;
};

struct EPubFormat
{
  QString name;
  UniqueString id;
};

struct EPubType
{
  QString name;
  UniqueString id;
};

class EPubCreatorContributorBase
{
public:
  enum SchemeType
  {
    dcterms,
    marc,
    media,
    onyx,
    xsd,
    string_scheme_type,
  };
  EPubCreatorContributorBase();
  ~EPubCreatorContributorBase();

  QString name;
  //  QList<QSharedPointer<EPubFileAs>> fileAsList;
  QSharedPointer<EPubFileAs> fileAs;
  UniqueString id;
  QSharedPointer<EPubAltRep> altRep;
  QDateTime date;

  QSharedPointer<MarcRelator> relator;
  QString stringCreator;
  QString stringScheme;
  QList<QSharedPointer<Foaf>> foafList;

  static SchemeType fromSchemeString(QString type)
  {
    if (type == "dcterms") {
      return SchemeType::dcterms;
    } else if (type == "marc") { // TODO - The only one supported at the present
      return SchemeType::marc;
    } else if (type == "media") {
      return SchemeType::media;
    } else if (type == "onyx") {
      return SchemeType::onyx;
    } else if (type == "xsd") {
      return SchemeType::xsd;
    } else {
      return SchemeType::string_scheme_type;
    }
  }
};

class EPubCreator : public EPubCreatorContributorBase
{
  // Actually identical - convenience class
};

class EPubContributor : public EPubCreatorContributorBase
{
  // Actually identical - convenience class
};

struct EPubDescription : public DirectionBase
{
  UniqueString id;
  QString text;
  QString language;
};

class Calibre
{
public:
  Calibre();

  QString seriesName() const;
  void setSeriesName(const QString& seriesName);

  quint16 seriesIndex() const;
  QString seriesIndexString() const;
  void setSeriesIndex(quint16 seriesIndex);

  QString titleSort() const;
  void setTitleSort(const QString& titleSort);

  QString authorLinkMap() const;
  void setAuthorLinkMap(const QString& authorLinkMap);

  QString timestamp() const;
  void setTimestamp(const QString& timestamp);

  QString rating() const;
  void setRating(const QString& rating);

  QString publicationType() const;
  void setPublicationType(const QString& publicationType);

  QString userMetadata() const;
  void setUserMetadata(const QString& userMetadata);

  QString userCategories() const;
  void setUserCategories(const QString& userCategories);

  QString customMetadata() const;
  void setCustomMetadata(const QString& customMetadata);

  bool isModified() const;
  void setModified(bool isModified);

protected:
  QString m_seriesName;
  quint16 m_seriesIndex;
  QString m_titleSort;
  QString m_authorLinkMap;
  QString m_timestamp;
  QString m_rating;
  QString m_publicationType;
  QString m_userMetadata;
  QString m_userCategories;
  QString m_customMetadata;
  bool m_modified;
};

class Metadata : public DirectionFunc
{
  Q_DECLARE_TR_FUNCTIONS(EPubMetadata)

public:
  Metadata();

  void parse(QDomNodeList metadataNodeList);
  bool write(QXmlStreamWriter* xml_writer);

  UniqueString uniqueIdentifierName() const;
  void setUniqueIdentifierName(const UniqueString& packageUniqueIdentifierName);

  bool isFoaf() const;
  void setIsFoaf(bool value);

  QStringList creatorList() const;

  QSharedPointer<EPubCreator> creatorFromName(QString name);

  QStringList contributorList();
  QSharedPointer<EPubContributor> contributorFromName(QString name);

  QList<QSharedPointer<EPubTitle>> orderedTitles() const;
  void setOrderedTitles(const QList<QSharedPointer<EPubTitle>>& ordered_titles);

  QSharedPointer<Calibre> calibre() const;
  void setCalibre(const QSharedPointer<Calibre>& calibre);

private:
//  EPubDocument* m_document;
  UniqueString m_packageUniqueIdentifier;
  UniqueString m_packageUniqueIdentifierName;
  QMap<EBookIdentifierScheme::IdentifierScheme, QSharedPointer<EBookIdentifier>>
    m_identifiers;
  UniqueString m_primaryTitleId;
  QString m_primaryTitle;
  QMap<QString, QSharedPointer<EPubTitle>> m_titlesById;
  QMap<QString, QSharedPointer<EPubTitle>> m_titlesByName;
  QList<QSharedPointer<EPubTitle>> m_orderedTitles;
  QMap<QString, QSharedPointer<EPubCreator>> m_creatorsById;
  QMultiMap<QString, QSharedPointer<EPubCreator>> m_creatorsByName;
  //  SharedCreatorMap creators_by_name;
  QMultiMap<UniqueString, QSharedPointer<EPubContributor>> m_contributorsById;
  QMultiMap<QString, QSharedPointer<EPubContributor>> m_contributorsByName;
  QSharedPointer<EPubDescription> m_description;
  QMap<UniqueString, QSharedPointer<EPubLanguage>> m_languages;
  QMap<UniqueString, QSharedPointer<EPubSubject>> m_subjects;
  EPubModified m_modified;
  QDateTime m_date;
  QSharedPointer<EPubSource> m_source;
  QSharedPointer<EPubAttribution> m_attribution;
  //  QSharedPointer<EPubPublisher> m_publisher;
  QSharedPointer<EPubFormat> m_format;
  QSharedPointer<EPubRelation> m_relation;
  QSharedPointer<EPubCoverage> m_coverage;
  QSharedPointer<EPubRights> m_rights;
  QSharedPointer<EPubType> m_type;
  QMap<UniqueString, QString> m_extraMetas;
  QSharedPointer<Calibre> m_calibre;
  bool m_isFoaf;

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
  void parseTitleDateRefines(QSharedPointer<EPubTitle> shared_title,
                             QDomElement& metadataElement);
  void parseCreatorContributorRefines(
    QSharedPointer<EPubCreatorContributorBase> creator,
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
  QSharedPointer<Foaf> parseCreatorContributorFoafAttributes(
    QString property,
    QDomNamedNodeMap attributeMap);

  void writeCreator(QXmlStreamWriter* xml_writer,
                    QSharedPointer<EPubCreatorContributorBase> shared_creator);
  void writeContributor(
    QXmlStreamWriter* xml_writer,
    QSharedPointer<EPubCreatorContributorBase> shared_creator);
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
                  QSharedPointer<EPubTitle> shared_title);
  QString writeCreatorContibutor(QString tag_name,
                                 QXmlStreamWriter* xml_writer,
                                 QString key);
  QString writeCreatorContributor(
    QString tag_name,
    QXmlStreamWriter* xml_writer,
    QSharedPointer<EPubCreatorContributorBase> shared_creator);
  void writeIdAttribute(QXmlStreamWriter* xml_writer, const UniqueString& id);
  void writeDirAttribute(QXmlStreamWriter* xml_writer, Direction dir);
  void writeLangAttribute(QXmlStreamWriter* xml_writer, QString dir);
  void writeAuthorityAttribute(QXmlStreamWriter* xml_writer,
                               QString authority,
                               QString term);
  void writeAltRepAttribute(QXmlStreamWriter* xml_writer,
                            QSharedPointer<EPubAltRep> alt_rep);
  void writeFileAsAttribute(QXmlStreamWriter* xml_writer,
                            QSharedPointer<EPubFileAs> file_as);
  void writeRoleAttribute(QXmlStreamWriter* xml_writer,
                          QSharedPointer<MarcRelator> relator);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer,
                            EBookIdentifierScheme scheme);
  void writeSchemeAttribute(QXmlStreamWriter* xml_writer, QString scheme);

  //  int getNextTitleIndex();
};


#endif // EBOOKMETADATA_H
