#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QPlainTextEdit>
#include <QSplitter>
#include <QUndoStack>
#include <QUndoView>

#include "headerwidget.h"
#include "document/bookpointers.h"

class ButtonWidget;

class InfoWidget : public HeaderWidget
{
  Q_OBJECT
public:
  InfoWidget(PConfig config, QUndoStack* undoStack, QWidget* parent);

  QUndoView* undoView();
  QPlainTextEdit* logPage() const;

  void appendLog(const QString& text);

protected:
  void setVisible(bool visible) override;

private:
  QSplitter* m_splitter;
  QPlainTextEdit* m_logPage = nullptr;
  QUndoView* m_undoView;
  QUndoStack* m_undoStack;
  PConfig m_config;
  QIcon m_removeSplitUp, m_removeSplitDown;
  ButtonWidget*m_removeSplitWidget;

  void toggleVisible();
};

#endif // INFOWIDGET_H
