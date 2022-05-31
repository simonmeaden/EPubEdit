#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QUndoStack>
#include <QUndoView>
#include <QPlainTextEdit>
#include <QDebug>

#include "dockwidget.h"

class CentralWidget;
class EPubEditor;

class MainWidget : public DockWidget
{
  Q_OBJECT
public:
  MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent);

  EPubEditor* editor();
  QPlainTextEdit* logPage() const;
  QUndoView* undoView();

signals:
  void newClicked();
  void openClicked();
  void saveClicked();
  void saveAsClicked();

private:
  CentralWidget* m_centralWidget = nullptr;

  void internalGeometryUpdate(int north, int south, int east, int west);
  void splitScreen();
  void hideLeftSidebar();
  void showLeftSidebar();
  void hideRightSidebar();
  void showRightSidebar();
  void toggleLeftSidebar();
  void toggleRightSidebar();
};

#endif // MAINWIDGET_H
