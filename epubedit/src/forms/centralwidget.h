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

signals:
  void geometryChanged(int north, int south, int east, int west);

private:
  PConfig m_config;
  QUndoStack* m_undoStack;
  EPubEditor* m_editor = nullptr;
  QSplitter* m_splitter;
  MultiSplitter* m_editSplitter;
  InfoWidget* m_infoWidget = nullptr;
  QWidget* m_rightSidebar;
  QWidget* m_leftSidebar;

  void initGui();

  void splitterHasMoved();
};

#endif // CENTRALWIDGET_H
