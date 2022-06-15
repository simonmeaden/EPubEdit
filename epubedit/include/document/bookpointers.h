#ifndef POINTERS_H
#define POINTERS_H

#include <QScopedPointer>
#include <QSharedPointer>

struct Config;
struct EBookOptions;
struct EBookSeriesDB;
struct EBookAuthorsDB;
struct EBookLibraryDB;
struct BookData;
class EPubDocument;
class Metadata;
class EPubCreator;
struct EPubTitle;
struct EPubFileAs;
class Calibre;
struct EPubManifestItem;
struct Manifest;
struct TocItem;
struct EPubRights;
class EPubContributor;
struct EBookIdentifier;
struct EPubDescription;
struct EPubLanguage;
struct EPubSubject;
struct EPubSource;
struct EPubAttribution;
struct EPubFormat;
struct EPubRelation;
struct EPubCoverage;
struct EPubType;
class EPubCreatorContributorBase;
struct EPubAltRep;
class MarcRelator;
class Foaf;
struct EBookAuthorData;
struct EBookSeriesData;
struct EPubSpineItem;
struct GuideItem;

// QSharedpointer typedefs for epubedit.
typedef QSharedPointer<Config> PConfig;
typedef QSharedPointer<EBookOptions> POptions;
typedef QSharedPointer<EBookSeriesDB> PSeriesDB;
typedef QSharedPointer<EBookAuthorsDB> PAuthorsDB;
typedef QSharedPointer<EBookLibraryDB> PLibraryDB;
typedef QSharedPointer<BookData> PBookData;
typedef QSharedPointer<EPubDocument> PDocument;
typedef QSharedPointer<Metadata> PMetadata;
typedef QSharedPointer<EPubCreator> PCreator;
typedef QSharedPointer<EPubContributor> PContributor;
typedef QSharedPointer<EPubTitle> PTitle;
typedef QSharedPointer<EPubFileAs> PFileAs;
typedef QSharedPointer<Calibre> PCalibre;
typedef QSharedPointer<EPubManifestItem> PManifestItem;
typedef QSharedPointer<Manifest> PManifest;
typedef QSharedPointer<TocItem> PTocItem;
typedef QSharedPointer<EPubRights> PRights;
typedef QSharedPointer<EBookIdentifier> PIdentifier;
typedef QSharedPointer<EPubDescription> PDescription;
typedef QSharedPointer<EPubLanguage> PLanguage;
typedef QSharedPointer<EPubSubject> PSubject;
typedef QSharedPointer<EPubSource> PSource;
typedef QSharedPointer<EPubAttribution> PAttribution;
typedef QSharedPointer<EPubFormat> PFormat;
typedef QSharedPointer<EPubRelation> PRelation;
typedef QSharedPointer<EPubCoverage> PCoverage;
typedef QSharedPointer<EPubType> PType;
typedef QSharedPointer<EPubCreatorContributorBase> PCreatorContributorBase;
typedef QSharedPointer<EPubAltRep> PAltRep;
typedef QSharedPointer<MarcRelator> PMarcRelator;
typedef QSharedPointer<Foaf> PFoaf;

typedef QSharedPointer<EBookAuthorData> PAuthorData;
typedef QList<PAuthorData> AuthorList;
typedef QMap<quint64, PAuthorData> AuthorMap;
typedef QMultiMap<QString, PAuthorData> AuthorByString;

typedef QSharedPointer<BookData> PBookData;
typedef QList<PBookData> BookList;
typedef QMap<quint64, PBookData> BookByUInt;
typedef QMultiMap<QString, PBookData> BookByString;

typedef QSharedPointer<EBookSeriesData> PSeriesData;
typedef QMap<quint64, PSeriesData> SeriesMap;
typedef QMap<QString, PSeriesData> SeriesByString;

typedef QStringList SeriesList;
typedef QSharedPointer<EPubSpineItem> PSpineItem;

#endif // POINTERS_H
