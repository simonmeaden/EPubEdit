#ifndef BOOKSTRUCTS_H
#define BOOKSTRUCTS_H

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QString>

#include "document/bookenums.h"
#include "document/bookpointers.h"
#include "document/uniquestring.h"

struct Page
{
  UniqueString idref;
  QString mediaType;
  QString page; // possibly entire page??
  QString path;
};
Q_DECLARE_METATYPE(Page)

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

struct EPubSpine : public DirectionBase
{
  UniqueString id;
  UniqueString tocId;
  QList<UniqueString> orderedItems;
  QMap<UniqueString, PSpineItem> items;
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

struct EPubTitle : public DirectionBase
{

  EPubTitle();

  UniqueString id;
  QString title;
  QString lang;
  QList<PAltRep> altRepList;
  QList<PFileAs> fileAsList;
  QDate date;
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

struct TocItem
{
  UniqueString id;
  int playorder;
  QString tagClass;
  QString label;
  QString source;
  QMap<int, PTocItem> subItems;
  QString chapterTag;
};

struct Manifest
{
  Manifest();
  ~Manifest();
  UniqueString id;
  PManifestItem coverImage;                      // 0 or 1
  PManifestItem nav;                             // 1
  QMap<UniqueString, PManifestItem> itemsById;   // all items
  QMap<UniqueString, PManifestItem> itemsByHref; // all items
  QMap<UniqueString, PManifestItem> htmlItems;
  QMap<UniqueString, PManifestItem> mathml; // subset of items for math markup
  QMap<UniqueString, PManifestItem> svgImages; // subset of items for images
  UniqueStringMap images;
  QMap<UniqueString, PManifestItem> remotes;
  QMap<UniqueString, PManifestItem> scripted;
  QMap<UniqueString, PManifestItem> switches;
  UniqueStringMap css;                            // all items
  UniqueStringMap javascript;                     // all items
  QMap<UniqueString, PManifestItem> fonts;        // all items
  QMap<UniqueString, PManifestItem> mediaOverlay; // all items
  QString formattedTocString;
  QMap<int, PTocItem> tocTtems;
  QMap<QString, PTocItem> tocPaths;
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

struct EPubLanguage
{
  UniqueString id;
  QString language;
};

struct EPubAttribution
{
  QString url;
  QString name;
  QString license;
};

struct EPubSubject : public DirectionBase
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
  PFileAs fileAs;
  UniqueString id;
  PAltRep altRep;
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
  //  QList<PFileAs> fileAsList;
  PFileAs fileAs;
  UniqueString id;
  PAltRep altRep;
  QDateTime date;

  PMarcRelator relator;
  QString stringCreator;
  QString stringScheme;
  QList<PFoaf> foafList;

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


#endif // BOOKSTRUCTS_H
