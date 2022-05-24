#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QUndoStack>
#include <QUndoView>
#include <QPlainTextEdit>

#include "dockwidget.h"

class CentralWidget;
class EPubEdit;

class MainWidget : public DockWidget
{
  Q_OBJECT
public:
  MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent);

  EPubEdit* editor();
  QPlainTextEdit* logPage() const;
  QUndoView* undoView();

signals:
  void newClicked();
  void openClicked();
  void saveClicked();
  void saveAsClicked();

private:
  CentralWidget* m_centralWidget = nullptr;
};

#endif // MAINWIDGET_H
