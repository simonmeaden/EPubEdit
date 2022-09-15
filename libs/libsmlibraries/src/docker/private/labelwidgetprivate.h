#ifndef LABELWIDGETPRIVATE_H
#define LABELWIDGETPRIVATE_H

#include "docker/labelwidget.h"
#include "docker/private/widgetitemprivate.h"

class LabelWidgetPrivate : public WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(LabelWidget)
public:
  LabelWidgetPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  const QString& text();
  void setText(const QString& text);

  const QColor& textColor();
  void setTextColor(const QColor& textColor);

  WidgetItem* clone(WidgetItem* widget = nullptr);

protected:
  const QSize calcMinimumSize();
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
  QSize sizeHint();

private:
  QRect m_textRect;
  QString m_text;
  QColor m_textColor;
};

#endif // LABELWIDGETPRIVATE_H
