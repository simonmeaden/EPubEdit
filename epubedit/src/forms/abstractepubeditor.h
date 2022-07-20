#ifndef ABSTRACTEPUBEDITOR_H
#define ABSTRACTEPUBEDITOR_H

#include <QTextCursor>
#include <QWidget>

#include "document/bookpointers.h"
#include "headerwidget.h"

class AbstractEPubEditorPrivate;
class AbstractEPubEditor : public HeaderWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractEPubEditor)
public:
  AbstractEPubEditor(QWidget* parent = nullptr);
  explicit AbstractEPubEditor(PConfig config, QWidget* parent = nullptr);
  virtual ~AbstractEPubEditor() {}

signals:
  void sendLogMessage(const QString& message);

protected:
  AbstractEPubEditor(AbstractEPubEditorPrivate& d);
  AbstractEPubEditor(AbstractEPubEditorPrivate& d, QWidget* parent);
  void mousePressEvent(QMouseEvent* event);
};

#endif // ABSTRACTEPUBEDITOR_H
