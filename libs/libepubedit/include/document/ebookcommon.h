#ifndef EBOOKCOMMON_H
#define EBOOKCOMMON_H

#include <QDateTime>
#include <QFont>
#include <QHash>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPoint>
#include <QSharedPointer>
#include <QTextCursor>
#include <QTextDocument>
#include <QtPlugin>

struct EBookNavPoint
{
  EBookNavPoint(QString classname, QString id, QString label, QString src)
  {
    this->classname = classname;
    this->id = id;
    this->label = label;
    this->src = src;
  }
  QString classname;
  QString id;
  QString label;
  QString src;
};
typedef QSharedPointer<EBookNavPoint> NavPoint;

enum DocumentType
{
  PLAINTEXT,
  HTML
};
enum EBookDocumentType
{
  UNSUPPORTED_TYPE,
  EPUB,
  MOBI,
  AZW,
  PDF,
};

struct EBookManifestItem
{
  QString href;
  QString path;
  QString document_string;
  QStringList css_links;
  QString body_class;
  QString id;
  QByteArray media_type;
  QStringList properties;
  QString fallback;
  QString media_overlay;
  QMap<QString, QString> non_standard_properties;
};
typedef QSharedPointer<EBookManifestItem> ManifestItem;
typedef QMap<QString, ManifestItem> ManifestItemMap;
typedef QList<ManifestItem> ManifestItemList;

struct EBookTocItem;
typedef QSharedPointer<EBookTocItem> TocItem;
typedef QMap<int, TocItem> TocItemMap;
typedef QMap<QString, TocItem> TocItemPathMap;
struct EBookTocItem
{
  QString id;
  int playorder;
  QString tagClass;
  QString label;
  QString source;
  TocItemMap subItems;
  QString chapterTag;
};

typedef QSharedPointer<QMap<QString, QString>> CSSMap;
struct EBookManifest
{
  EBookManifest();
  ~EBookManifest();
  QString id;
  ManifestItem coverImage;     // 0 or 1
  ManifestItem nav;            // 1
  ManifestItemMap itemsById;   // all items
  ManifestItemMap itemsByHref; // all items
  ManifestItemMap htmlItems;
  ManifestItemMap mathml;    // subset of items for math markup
  ManifestItemMap svgImages; // subset of items for images
  //  QMap<QString, QImage> rendered_svg_images; // rendered svg images
  QMap<QString, QString> images;
  ManifestItemMap remotes;
  ManifestItemMap scripted;
  ManifestItemMap switches;
  //  QMap<QString, QString> id_by_file;          // all items
  CSSMap css;                        // all items
  QMap<QString, QString> javascript; // all items
  ManifestItemMap fonts;             // all items
  ManifestItemMap mediaOverlay;      // all items
  QString formattedTocString;
  TocItemMap tocTtems;
  TocItemPathMap tocPaths;
};

struct EBookSpineItem
{
  EBookSpineItem()
  {
    linear = false;
    pageSpreadLeft = false;
    pageSpreadRight = false;
  }
  QString id;
  QString idref;
  bool linear;
  bool pageSpreadLeft;
  bool pageSpreadRight;
};
typedef QSharedPointer<EBookSpineItem> SpineItem;
typedef QMap<QString, SpineItem> SpineItemMap;
typedef QStringList SpineItemList;

struct EBookSpine
{
  QString id;
  QString toc;
  QString pageProgressionDir;
  SpineItemMap items;
  SpineItemList orderedItems;
};

#endif // EBOOKCOMMON_H
