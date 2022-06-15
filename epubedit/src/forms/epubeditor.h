#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QTextEdit>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>
#include <QMenu>

#include "config.h"
#include "headerwidget.h"

class ContentsFrame;
class EPubDocument;

#include "document/bookpointers.h"
#include "document/bookstructs.h"


class EPubEditor : public HeaderWidget
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

  void setHtml(const QString& html);

  //  QUndoView* undoView();
  //  QAction* undoAction();
  //  QAction* redoAction();

  void openCloseClicked();

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void sendLogMessage(const QString& message);
  void splitWidget(Qt::Orientation orientation);
  void splitToWindow();

private:
  PConfig m_config;
  PDocument m_document;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  QTextEdit *m_editor;

  QString m_currentBookFilename;

  //  QUndoView* m_undoView;
  bool m_loaded;
  QList<UniqueString> m_pageOrder;
  QMap<UniqueString, Page> m_pages;
  Page m_page;

  void loadFileIntoTextDocument(const QString& zipfile,
                                const QString& imageName);
  void updateDocument();
  void removeSplit();
  void splitMenuClicked(QPoint pos);
  void menuClicked(QAction*action);
  QPoint adjustWidgetPositionInsideWidget(QPoint originalPos,
                                          QSize requiredSize);
};
Q_DECLARE_METATYPE(EPubEditor)

#endif // EPUBEDITOR_H
