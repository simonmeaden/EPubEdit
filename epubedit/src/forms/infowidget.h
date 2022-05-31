#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QPlainTextEdit>
#include <QSplitter>
#include <QUndoStack>
#include <QUndoView>


#include "dockwidget.h"

class InfoWidget : public DockWidget
{
  Q_OBJECT
public:
  InfoWidget(PConfig config, QUndoStack *undoStack, QWidget* parent);

  QUndoView* undoView();
  QPlainTextEdit* logPage() const;

protected:
private:

  QSplitter* m_splitter;
  QPlainTextEdit* m_logPage=nullptr;
  QUndoView* m_undoView;
  QUndoStack* m_undoStack;
  PConfig m_config;

  void splitterHasMoved();
};

#endif // INFOWIDGET_H
