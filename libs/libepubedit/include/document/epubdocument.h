#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

//#include "epubcontainer.h"

#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QElapsedTimer>
#include <QIODevice>
#include <QImage>
#include <QImageReader>
#include <QObject>
#include <QPainter>
#include <QSharedPointer>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QtSvg/QSvgRenderer>

#include "document/epubmetadata.h"
#include "util/csvsplitter.h"

//#include "document/authors.h"
////#include "document/htmlparser.h"
//#include "document/library.h"

class EPubMetadata;
class QuaZip;

struct EPubContents
{
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;
  bool m_loaded;
};

class EPubDocument
{
public:
  explicit EPubDocument();
  virtual ~EPubDocument();

  bool loaded();
  void openDocument(const QString& path);
  void saveDocument(const QString& path = QString());
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  QString filename();
  void setFilename(const QString& filename);

  QString tocAsString();

  QDate published();
  void setPublished(const QDate& published);
  QString buildTocFromData();

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

  QSharedPointer<EPubMetadata> metadata();
  QMap<UniqueString, QSharedPointer<EPubManifestItem>> pages();
  QStringList spine();
  QList<UniqueString> cssKeys();
  UniqueStringMap cssMap();
  QString css(UniqueString key);
  QString javascript(UniqueString key);
  QSharedPointer<EPubManifestItem> itemByHref(UniqueString href);

  // signals:
  //  void orderChanged();
  //  //  void listRemoved(ItemList list);
  //  void wordRemoved(int index, QString word);
  //  void wordChanged(int index, QString old_word, QString new_word);
  //  void loadCompleted();

  QSharedPointer<UniqueStringList> uniqueIdList() const;

private:
  bool m_loaded;
  bool m_current_document_index;
  bool m_current_document_lineno;
  bool m_modified;
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  QString m_filename;
  QString m_resourcePath;
  QString m_publisher;
  QDate m_published;
  QString m_subject;
  QString m_title;
  QuaZip* m_archive = nullptr;
  QStringList m_files;
  QByteArray m_mimetype;
  QSharedPointer<EPubMetadata> m_metadata;
  QSharedPointer<EPubManifest> m_manifest;
  EPubSpine m_spine;
  QString m_containerVersion;
  QString m_containerXmlns;
  QString m_containerFullpath;
  QString m_containerMediatype;
  QMap<QString, QMap<QString, QDomDocument>> m_rootfiles;
  QMap<QString, QDomDocument> m_currentRootFile;
  int m_version;
  QString m_packageXmlns;
  QString m_packageLanguage;
  QString m_packagePrefix;
  QString m_packageDirection;
  UniqueString m_packageId;
  int m_tocChapterIndex;
  //  HtmlParser* m_parser;

  bool m_readonly;

  QString buildTocfromHtml();
  bool loadDocument();
  bool parseMimetype();
  bool parseContainer();
  bool parsePackageFile(QString& fullPath);
  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder);
  QSharedPointer<EPubSpineItem> parseSpineItem(
    const QDomNode& spine_node,
    QSharedPointer<EPubSpineItem> item);
  bool parseTocFile();
  void handleNestedNavpoints(QDomElement elem, QString& formatted_toc_string);
  QSharedPointer<EPubTocItem> parseNavPoint(QDomElement navpoint,
                                            QString& formatted_toc_data);
  void extractHeadInformationFromHtmlFile(QSharedPointer<EPubManifestItem> item,
                                          QString container);
  bool saveDocumentToFile();
  bool writeMimetype(QuaZip* save_zip);
  bool writeContainer(QuaZip* save_zip);
  bool writePackageFile(QuaZip* save_zip);
  QString extractTagText(int anchor_start, QString document_string);

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString METADATA_FOLDER;
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
};
typedef QSharedPointer<EPubDocument> Document;

Q_DECLARE_METATYPE(EPubDocument);

#endif // EPUBDOCUMENT_H
