#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSplitter>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTextEdit>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>

//#include "borderlayout.h"
#include "config.h"
#include "document/bookpointers.h"
#include "forms/metadataform.h"
#include "dockwidget.h"

class EPubEditor;
class ContentsFrame;

struct Page
{
  UniqueString idref;
  QString mediaType;
  QString page; // possibly entire page??
  QString path;
};

class EPubEdit : public DockWidget
{
  Q_OBJECT

public:
  explicit EPubEdit(PConfig config, QWidget* parent = nullptr);
  ~EPubEdit();

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
  QSplitter* m_splitter;
  MetadataForm* m_metadataForm;
  ContentsFrame* m_contentsFrame;
  EPubEditor* m_editor;

  PConfig m_config;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  PDocument m_document;

  QString m_currentBookFilename;

  //  QUndoView* m_undoView;
  //  QTabWidget* m_tabs;
  bool m_loaded;
  QList<UniqueString> m_pageOrder;
  QMap<UniqueString, Page> m_pages;
  Page m_page;
  void initGui();
  void updateMetadataForm();
  void metadataHasChanged(MetadataForm::Modifications modifications);
  //  PBookData insertNewBook(const QString &filename, const QString& title,
  //  QStringList creators);
  void loadFileIntoTextDocument(const QString& zipfile,
                                const QString& imageName);
  void splitterHasMoved(int, int);
  void toggleOpenClicked();
};

#endif // EPUBEDIT_H
