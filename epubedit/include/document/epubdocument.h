#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QImage>
#include <QImageReader>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QXmlStreamWriter>
#include <QtSvg/QSvgRenderer>

#include "document/bookstructs.h"
#include "utilities//csvsplitter.h"
#include "utilities/tinyxml2.h"

class Metadata;
class QuaZip;
class QuaZipFile;
class NavListItem;
class NavTag;
class NavValueItem;

// class Config;

#include "document/bookenums.h"
#include "document/bookpointers.h"
#include "document/bookstructs.h"

class EPubDocument : public QObject
{
  Q_OBJECT

  struct GuideItem
  {
    QString title;
    QString href;
    QString type;
  };

public:
  enum Version
  {
    UNDEFINED,
    V20,
    V201,
    V30,
    V31,
    V32,
    V33,
  };
  enum Error
  {
    NO_ERROR = 0,
    DOCUMENT_WRITE_ERROR = 0x1,
    METATYPE_ERROR = 0x2,
    CONTAINER_ERROR = 0x4,

    WRITE_ERROR = 0x400000,
    SIZE_ERROR = 0x800000,
  };
  Q_DECLARE_FLAGS(Errors, Error)
  enum StoreType
  {
    COPY_INTO_LIBRARY,
    OVERWRITE_EXISTING,
    USE_STORED,
  };

  EPubDocument() {}
  explicit EPubDocument(PConfig config,
                        POptions options,
                        PLibraryDB libDb,
                        PSeriesDB series,
                        PAuthorsDB authors,
                        QObject* parent);
  EPubDocument(const EPubDocument& /*doc*/)
  {
    // TODO actually create a copy
  }
  ~EPubDocument();

  bool loaded();
  PBookData loadDocument(const QString& filename);
  void saveDocument(const QString& filename = QString());
  //  EPubContentsForm* cloneData();
  //  void setClonedData(EPubContentsForm* cloneData);

  //  QString filename();
  //  void setFilename(const QString& filename);

  QString tocAsString();

  QDate published();
  void setPublished(const QDate& published);
//  QString buildTocFromData();

  EPubDocumentType type() const { return EPUB; }
  bool isModified();
  QString title();
  void setTitle(const QString& title);
  QString subject();
  void setSubject(const QString& subject);
  QString language();
  void setLanguage(const QString& language);
  QDateTime date();
  void setDate(const QDateTime& date);
  QStringList creators();
  QString creatorNames(const QStringList& names);
  QString publisher();
  void setPublisher(const QString& publisher);

  PMetadata metadata();
  QMap<UniqueString, PManifestItem> pages();
  PManifest manifest() const;
  PSpine spine();

  QList<UniqueString> cssKeys();
  QMap<UniqueString, QString> cssMap();
  QString css(UniqueString key);
  QString javascript(UniqueString key);
  PManifestItem itemByHref(UniqueString href);

signals:
  void sendLogMessage(const QString& message);

private:
  PConfig m_config;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  bool m_loaded;
  bool m_current_document_index;
  bool m_current_document_lineno;
  bool m_modified;
  QString m_resourcePath;
  QString m_publisher;
  QDate m_published;
  QString m_subject;
//  QStringList m_files;
  QByteArray m_mimetype;
  PMetadata m_metadata;
  PManifest m_manifest;
  PSpine m_spine;
  QString m_containerVersion;
  QString m_containerXmlns;
  QString m_containerFullpath;
  QString m_containerMediatype;
  QMap<QString, QMap<QString, QDomDocument>> m_rootfiles;
  QMap<QString, QDomDocument> m_currentRootFile;
  Version m_version;
  QString m_packageXmlns;
  QString m_packageLanguage;
  QString m_packagePrefix;
  QString m_packageDirection;
  UniqueString m_packageId;
  int m_tocChapterIndex;
  //  HtmlParser* m_parser;
  bool m_readonly;

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString METAINF_FOLDER;
  static const QString OEBPS_FOLDER;
  static const QString CONTAINER_FILE;
  static const QString TOC_FILE;

  static const QString TITLE;
  static const QString CREATOR;
  static const QString IDENTIFIER;
  static const QString LANGUAGE;

  static const QString TOC_TITLE;
  static const QString LIST_START;
  static const QString LIST_END;
  static const QString LIST_ITEM;
  static const QString LIST_BUILD_ITEM;
  static const QString LIST_FILEPOS;

  static const QString HTML_DOCTYPE;
  static const QString XML_HEADER;
  static const QString HTML_XMLNS;

  static const QString WORKING;
  static const QString DEPRECATED;

  static const QRegularExpression reAnchorComplete;
  static const QRegularExpression reAnchorTag;
  static const QRegularExpression reHref;
  static const QRegularExpression reHtmlList;
  static const QRegularExpression reHtmlListItem;

//  QString buildTocfromHtml();
  bool loadDocumentFromFile(const QString& filename);
  bool copyBookToLibrary(const QString& oldFilename,
                         const QString& newFilename);
  bool writeVersion30();
  bool writeVersion31();
  bool writeVersion32();
  bool parseMimetype(QuaZip* archive);
  bool parseContainer(QuaZip* archive);
  bool parsePackageFile(QuaZip* archive, QString& fullPath);
  bool parseManifestItem(QuaZip* archive,
                         const QDomNode& manifest_node,
                         const QString current_folder);
  bool parseSpineItem(const QDomNode& spine_node);
  //  bool parseTocFile(QuaZip* archive);
  void handleNestedNavpoints(QDomElement elem, QString& formatted_toc_string);
  PTocItem parseNavPoint(QDomElement navpoint, QString& formatted_toc_data);
  void extractHeadInformationFromHtmlFile(PManifestItem item,
                                          QString container);
  Errors writeMimetype(const QString& filename);
  Errors writeContainer(QuaZip* save_zip);
  Errors writePackageFile(QuaZip* save_zip);
  QString extractTagText(int anchor_start, QString document_string);
  StoreType confirmOverwriteOrUseLibrary(const QString& path,
                                         QString& libraryFilename) const;
  QString getFirstAuthorNameOrUnknown(const QStringList& authors);
  QString getFirstTitleOrTemp(QList<PTitle> titles);
  PBookData insertNewBook(const QString& filename,
                          const QString& title,
                          QStringList creators);
  PBookData getBookForTitleAndCreator(const QString& title,
                                      const QString& libraryFilename);
  void handleDeprecatedGuide(QuaZip* archive, QDomDocument packageDocument);
  void saveDeprecatedWorkingFile(const QString& filename,
                                 const GuideItem& item,
                                 const QString data);
  QString cleanToXHtml(QuaZipFile* contentFile);
  tinyxml2::XMLElement* checkListType(tinyxml2::XMLElement* element);
  NavListItem* parseTocList(tinyxml2::XMLElement* element);
  void parseAttributes(NavTag* anchor, const tinyxml2::XMLElement* xmlElement);
  void parseTag(tinyxml2::XMLElement *listItemChild, NavValueItem *navItem);

};
Q_DECLARE_OPERATORS_FOR_FLAGS(EPubDocument::Errors)
Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
