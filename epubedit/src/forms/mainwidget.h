#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDebug>
#include <QPlainTextEdit>
#include <QUndoStack>
#include <QUndoView>

#include "dockwidget.h"
#include "document/bookpointers.h"

class CentralWidget;
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

signals:
  void newClicked();
  void openClicked();
  void saveClicked();
  void saveAsClicked();

private:
  CentralWidget* m_centralWidget = nullptr;

  WidgetItem* m_leftSidebarWidget = nullptr;
  WidgetItem* m_rightSidebarWidget = nullptr;
  WidgetItem* m_lineNoWidget = nullptr;
  ListWidget* m_listNoWidget = nullptr;

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
