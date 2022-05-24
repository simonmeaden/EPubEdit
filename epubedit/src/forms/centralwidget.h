#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QUndoView>

#include "dockwidget.h"

class EPubEdit;
class InfoWidget;
class CentralWidget : public DockWidget
{
  Q_OBJECT

public:
  CentralWidget(PConfig config,
                QUndoStack* undoStack,
                QWidget* parent = nullptr);

  EPubEdit* editor();
  QPlainTextEdit* logPage() const;
  QUndoView* undoView();

private:
  PConfig m_config;
  QUndoStack* m_undoStack;
  EPubEdit* m_editor;
  QSplitter* m_splitter;
  InfoWidget* m_infoWidget;

  void splitterHasMoved();
  void initGui(PConfig config, QUndoStack* undoStack);
};

#endif // CENTRALWIDGET_H
