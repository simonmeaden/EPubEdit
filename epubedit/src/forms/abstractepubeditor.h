#ifndef ABSTRACTEPUBEDITOR_H
#define ABSTRACTEPUBEDITOR_H

#include <QTextCursor>
#include <QWidget>

#include "document/bookpointers.h"
#include "docker/headerwidget.h"

class AbstractEPubEditor : public HeaderWidget
{
  Q_OBJECT
public:
  AbstractEPubEditor(QWidget* parent = nullptr);
  explicit AbstractEPubEditor(PConfig config, QWidget* parent = nullptr);
  virtual ~AbstractEPubEditor() {}

signals:
  void sendLogMessage(const QString& message);

protected:
  PConfig m_config;
  QString m_href;
  void mousePressEvent(QMouseEvent* event);

private:
 };

#endif // ABSTRACTEPUBEDITOR_H
