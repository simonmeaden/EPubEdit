#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QFocusEvent>
#include <QTextEdit>
#include <QWidget>

#include "iepubeditor.h"
#include "document/bookpointers.h"

class EPubEdit : public QTextEdit, public IEPubEditor
{
  Q_OBJECT
public:
  EPubEdit(PConfig config, QWidget* parent);

  // IEPubEditor interface
  QTextDocument *document() override;
  QTextCursor currentCursor() override;
  const QString &href() const override;
  void loadHref(const QString &href) override;
  void setText(const QString &text) override;

signals:
  void lostFocus(QWidget*);
  void gotFocus(QWidget*);
  void mouseClicked(QPoint pos);

protected:
  PConfig m_config;
  QString m_href;

  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  // IEPubEditor interface
public:
};

#endif // EPUBEDIT_H
