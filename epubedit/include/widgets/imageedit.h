#ifndef IMAGEEDIT_H
#define IMAGEEDIT_H

#include <QWidget>
#include <QFocusEvent>

#include "document/bookpointers.h"
#include "widgets/iepubeditor.h"

class ImageEdit : public QWidget, public IEPubEditor
{
  Q_OBJECT
public:
  explicit ImageEdit(PConfig config, QWidget *parent = nullptr);

  // IEPubEditor interface
  const QString href() const override;
  void loadHref(const QString &href) override;

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

};

#endif // IMAGEEDIT_H
