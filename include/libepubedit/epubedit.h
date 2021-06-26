#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QSharedPointer>
#include <QTabWidget>
#include <QTextEdit>

class QuaZip;
class MetadataForm;

#include "libepubedit/ebookbasemetadata.h"
#include "libepubedit/ebookcommon.h"
#include "libepubedit/ebookmetadata.h"
// typedef QSharedPointer<EBookManifestItem> ManifestItem;
// typedef QSharedPointer<QDomDocument> DomDocument;
// typedef QSharedPointer<EBookMetadata> Metadata;
// typedef QSharedPointer<EBookManifest> Manifest;
// typedef QSharedPointer<EBookSpine> Spine

class EPubEdit : public QTabWidget
{
public:
  EPubEdit(QWidget* parent = nullptr);

  bool loadEpubDocument(const QString& filename);

protected:
private:
  QMap<QString, QMap<QString, DomDocument>> m_rootfiles;
  QMap<QString, DomDocument> mCurrentRootfile;
  int m_version;
  QString m_packageXmlns;
  QString m_packageLanguage;
  QString m_packagePrefix;
  QString
    m_packageDirection; // text direction - overridden  by Undicode values.
  QString m_packageId;  // unique identifier.
  int m_tocChapterIndex;
  Metadata m_metadata;
  Manifest m_manifest;
  Spine m_spine;
  QString m_resourcePath;

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString CONTAINER_FILE;
  static const QString TOC_TITLE;
  static const QString LIST_START;
  static const QString LIST_END;
  static const QString LIST_ITEM;
  static const QString LIST_BUILD_ITEM;
  static const QString LIST_FILEPOS;
  static const QString HTML_DOCTYPE;
  static const QString XML_HEADER;
  static const QString HTML_XMLNS;

  MetadataForm* m_metadataForm;

  void initGui();

  bool parseMimetype(QStringList files, QuaZip* archive);
  bool parseContainer(QStringList files, QuaZip* archive);
  bool parsePackageFile(QString& full_path, QuaZip* archive);
  bool parseManifestItem(const QDomNode& manifest_node,
                         const QString current_folder,
                         QuaZip* archive);
  SpineItem parseSpineItem(const QDomNode& spine_node, SpineItem item);
  bool parseTocFile(QuaZip* archive);
  TocItem parseNavPoint(QDomElement navpoint, QString& formatted_toc_data);
  void handleNestedNavpoints(QDomElement elem, QString& formatted_toc_string);
  void extractHeadInformationFromHtmlFile(ManifestItem item, QString container);

  void updateMetadataForm();
  void metadataHasChanged();
};

#endif // EPUBEDIT_H
