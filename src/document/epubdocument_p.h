#ifndef EPUBDOCUMENT_P_H
#define EPUBDOCUMENT_P_H

#include <QElapsedTimer>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QSvgRenderer>
#include <QTextCursor>

#include <quazip/quazip.h>

#include "document/htmlparser.h"
#include "libepubedit/document/epubcontainer.h"
#include "libepubedit/document/epubdocument.h"
#include "libepubedit/ebookcommon.h"

// const QString EPubEdit::MIMETYPE_FILE = "mimetype";
// const QByteArray EPubEdit::MIMETYPE = "application/epub+zip";
// const QString EPubEdit::CONTAINER_FILE = "META-INF/container.xml";
// const QString EPubEdit::TOC_TITLE = "<h2>%1</h2>";
// const QString EPubEdit::LIST_START = "<html><body><ul>";
// const QString EPubEdit::LIST_END = "</ul></body></html>";
// const QString EPubEdit::LIST_ITEM = "<li><a href=\"%1\">%2</li>";
// const QString EPubEdit::LIST_BUILD_ITEM = "<li><a href=\"%1#%2\">%3</li>";
// const QString EPubEdit::LIST_FILEPOS = "position%1";
// const QString EPubEdit::HTML_DOCTYPE =
//  "<!DOCTYPE html PUBLIC "
//  "\"-//W3C//DTD XHTML 1.1//EN\" "
//  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
// const QString EPubEdit::XML_HEADER = "<?xml version='1.0'
// encoding='utf-8'?>"; const QString EPubEdit::HTML_XMLNS =
// "http://www.w3.org/1999/xhtml";

class EPubDocumentPrivate : public ITextDocumentPrivate
{
public:
  EPubDocumentPrivate(EPubDocument* parent);
  virtual ~EPubDocumentPrivate();

  EPubDocument* q_ptr;

  bool loaded();
  void openDocument(const QString& path = QString());
  void closeDocument(const QString& path);
  QString filename();
  //  void clearCache();
  EPubContents* cloneData();
  void setClonedData(EPubContents* cloneData);

  QStringList creators();
  QString title();
  void setTitle(QString title);
  CSSMap cssMap();
  QString css(QString key);
  QString javascript(QString key);
  QStringList spineKeys();
  QStringList imageKeys();
  QStringList cssKeys();
  QStringList jsKeys();
  QStringList spine();
  //  QMap<QString, QString> pages();
  ManifestItemMap pages();
  ManifestItem itemByHref(QString href);

  IEBookInterface* plugin() { return m_plugin; }
  void setPlugin(IEBookInterface* plugin) { m_plugin = plugin; }

  QString language() const {}
  void setLanguage(const QString& language) {}
  QDateTime date() const {}
  void setDate(const QDateTime& date) {}
  //  void setDocumentPath(const QString& documentPath);
  Metadata metadata();

  QString buildTocFromFiles();

  bool isModified() const;

protected:
  //  QString m_documentPath;
  bool m_loaded;
  bool m_current_document_index;
  bool m_current_document_lineno;
  bool m_modified;
  QuaZip* m_archive = nullptr;
  QStringList m_files;
  QByteArray m_mimetype;
  Metadata m_metadata;
  EBookManifest m_manifest;
  EBookSpine m_spine;
  QString m_container_version;
  QString m_container_xmlns;
  QString m_container_fullpath;
  QString m_container_mediatype;
  QMap<QString, QMap<QString, DomDocument>> m_rootfiles;
  QMap<QString, DomDocument> m_current_rootfile;
  int m_version;
  QString m_package_xmlns;
  QString m_package_language;
  QString m_package_prefix;
  QString
    m_package_direction; // text direction - overridden  by Undicode values.
  QString m_package_id;  // unique identifier.
  int m_toc_chapter_index;
  //  HtmlParser* m_parser;

  EPubDocumentPrivate(EPubDocumentPrivate& d);
  bool loadDocument();
  bool saveDocument();
  QString buildTocfromHtml();
  QString extractTagText(int anchor_start, QString document_string);
  QString tocAsString();

  QString toc();
  //  virtual QVariant loadResource(int, const QUrl&);
  //  void fixImages(SharedDomDocument newDocument);
  //  const QImage& getSvgImage(const QString& id);

  bool parseMimetype();
  bool writeMimetype(QuaZip* save_zip);
  bool parseContainer();
  bool writeContainer(QuaZip* save_zip);
  bool parsePackageFile(QString& full_path);
  bool writePackageFile(QuaZip* save_zip);

  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder);
  void extractHeadInformationFromHtmlFile(ManifestItem item, QString container);

  SpineItem parseSpineItem(const QDomNode& metadata_element, SpineItem item);
  bool saveSpineItem();
  bool parseTocFile();
  bool parseGuideItem(const QDomNode& guideItem);
  bool parseLandmarksItem(const QDomNode& guideItem);
  bool saveLandmarksItem();
  bool parseBindingsItem(const QDomNode& bindingsItem);
  bool saveBindingsItem();
  TocItem parseNavPoint(QDomElement navpoint, QString& formatted_toc_data);
  void handleNestedNavpoints(QDomElement navpoint,
                             QString& formatted_toc_string);

  //  const QuaZip* getFile(const QString& path);

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

private:
  Q_DECLARE_PUBLIC(EPubDocument)
};

#endif // EPUBDOCUMENT_P_H
