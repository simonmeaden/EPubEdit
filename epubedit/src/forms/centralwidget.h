#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QPlainTextEdit>
#include <QSplitter>
#include <QUndoView>
#include <QWidget>

#include "dockwidget.h"
#include "multisplitter.h"

class EPubEditor;
class InfoWidget;
class DockSplitter;
class RightSidebar;
class LeftSidebar;

#include "document/bookpointers.h"
#include "document/bookstructs.h"

class CentralWidget : public QWidget
{
  Q_OBJECT

public:
  CentralWidget(PConfig config,
                QUndoStack* undoStack,
                QWidget* parent = nullptr);

  EPubEditor* editor();
  QPlainTextEdit* logPage() const;
  QUndoView* undoView();

  bool toggleLeftSidebar();
  bool toggleRightSidebar();

  //  void load(const QString& filename);
  bool loadDocument(const QString& filename);
  bool saveDocument(const QString& filename);
  bool newDocument();

signals:
  void geometryChanged(int north, int south, int east, int west);
  // temp test signal
  void widgetClicked();
  void metadataChanged(PMetadata metadata);
  void splitWidget(Qt::Orientation orientation);
  void splitToWindow();

private:
  PConfig m_config;
  QUndoStack* m_undoStack = nullptr;
  EPubEditor* m_editor = nullptr;
  QSplitter* m_subSplitter = nullptr;
  DockSplitter* m_mainSplitter = nullptr;
  MultiSplitter* m_editSplitter = nullptr;
  InfoWidget* m_infoWidget = nullptr;
  RightSidebar* m_rightSidebar = nullptr;
  LeftSidebar* m_leftSidebar = nullptr;

  bool m_loaded = false;
  PDocument m_document;
  POptions m_options;
  PLibraryDB m_libraryDB;

  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  QList<UniqueString> m_pageOrder;
  QMap<UniqueString, Page> m_pages;
  Page m_page;
  QString m_currentBookFilename;

  void initGui();

  void splitterHasMoved();
  void tocClicked(const QString& href);
//  void loadFileIntoTextDocument(const QString& zipfile,
//                                const QString& imageName);
};

#endif // CENTRALWIDGET_H
