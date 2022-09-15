#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDebug>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QUndoStack>
#include <QUndoView>

#include "config.h"
#include "docker/dockwidget.h"
#include "document/bookpointers.h"

class EditorWidget;
class EPubEditor;
class ListWidget;

class MainWidget : public DockWidget
{
  Q_OBJECT
public:
  MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent);

  EPubEditor* editor();
  QPlainTextEdit* logPage() const;
  QUndoView* undoView();

  void load(const QString& filename);
  void newEpub();
  void openFile();
  void saveFile();
  void saveAsFile();

signals:
  //  void newClicked();
  //  void openClicked();
  //  void saveClicked();
  //  void saveAsClicked();

protected:
private:
  QTabWidget* m_editorTabs;
  EditorWidget* m_currentWidget = nullptr;
  QList<EditorWidget*> m_editorList;
  PConfig m_config;

  WidgetItem* m_leftSidebarBtn = nullptr;
  WidgetItem* m_rightSidebarBtn = nullptr;
  WidgetItem* m_lineNoLbl = nullptr;

  void internalGeometryUpdate(int north, int south, int east, int west);
  void splitScreen();
  void hideLeftSidebar();
  void showLeftSidebar();
  void hideRightSidebar();
  void showRightSidebar();
  void toggleLeftSidebar();
  void toggleRightSidebar();
  void listClicked(int index, const QString& text);

  void setLineNoText(int lineNo, int column);
};

#endif // MAINWIDGET_H
