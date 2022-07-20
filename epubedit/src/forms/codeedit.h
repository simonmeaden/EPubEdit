#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>

#include "document/bookstructs.h"
#include "iepubeditor.h"

//class CodeEditorPrivate;
class CodeEdit : public QPlainTextEdit, public IEPubEditor
{
  Q_OBJECT
//  Q_DECLARE_PRIVATE(CodeEdit)
public:
  CodeEdit(PConfig config, QWidget *parent);

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
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
protected:
//  CodeEditorPrivate *d_ptr;
  PConfig m_config;
  QString m_href;

  // IEPubEditor interface
public:
};

#endif // CODEEDIT_H
