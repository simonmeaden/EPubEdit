#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QTextEdit>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>

//#include "borderlayout.h"
#include "config.h"
#include "dockwidget.h"
#include "document/bookpointers.h"
#include "forms/metadataform.h"

class ContentsFrame;
class EPubDocument;

#include "document/bookpointers.h"

struct Page
{
  UniqueString idref;
  QString mediaType;
  QString page; // possibly entire page??
  QString path;
};

class EPubEditor : public QTextEdit
{
  Q_OBJECT
public:
  explicit EPubEditor(QWidget* parent = nullptr);
  explicit EPubEditor(PConfig config, QWidget* parent = nullptr);
  EPubEditor(const EPubEditor& other);
  ~EPubEditor();

  void setEpubDocument(PDocument document);
  PDocument epubDocument();

  void saveConfig();
  void loadConfig();

  bool loadDocument(const QString& filename);
  bool saveDocument(const QString& filename = QString());
  bool newDocument();

  bool isLoaded() const;

  //  QUndoView* undoView();
  //  QAction* undoAction();
  //  QAction* redoAction();

  void openCloseClicked();

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void sendLogMessage(const QString& message);

private:
  PConfig m_config;
  PDocument m_document;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;

  QString m_currentBookFilename;

  //  QUndoView* m_undoView;
  //  QTabWidget* m_tabs;
  bool m_loaded;
  QList<UniqueString> m_pageOrder;
  QMap<UniqueString, Page> m_pages;
  Page m_page;
  void updateMetadataForm();
  void metadataHasChanged(MetadataForm::Modifications modifications);
  void loadFileIntoTextDocument(const QString& zipfile,
                                const QString& imageName);
  void splitterHasMoved(int, int);
  //  void toggleOpenClicked();
  void updateDocument();
};
Q_DECLARE_METATYPE(EPubEditor)

#endif // EPUBEDITOR_H
