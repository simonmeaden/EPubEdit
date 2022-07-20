#include "labelwidget.h"
#include "abstractdockitem.h"
#include "private/labelwidgetprivate.h"

//====================================================================
//=== LabelWidget
//====================================================================
LabelWidget::LabelWidget(AbstractDockWidget* parent)
  : WidgetItem(*new LabelWidgetPrivate(parent, this))
{
}

LabelWidget::LabelWidget(LabelWidgetPrivate& d)
  : WidgetItem(d)
{
}

const QString&
LabelWidget::text()
{
  Q_D(LabelWidget);
  return d->text();
}

void
LabelWidget::setText(const QString& text)
{
  Q_D(LabelWidget);
  d->setText(text);
}

const QColor&
LabelWidget::textColor()
{
  Q_D(LabelWidget);
  return d->textColor();
}

void
LabelWidget::setTextColor(const QColor& textColor)
{
  Q_D(LabelWidget);
  d->setTextColor(textColor);
}

/// \cond DO_NOT_DOCUMENT
const QSize
LabelWidget::calcMinimumSize()
{
  Q_D(LabelWidget);
  return d->calcMinimumSize();
}

void
LabelWidget::paint(QPainter& painter)
{
  Q_D(LabelWidget);
  d->paint(painter);
}

void
LabelWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  Q_D(LabelWidget);
  d->setGeometry(widgetRect, contentsRect);
}

QSize
LabelWidget::sizeHint()
{
  Q_D(LabelWidget);
  return d->sizeHint();
}
/// \endcond DO_NOT_DOCUMENT

WidgetItem*
LabelWidget::clone(WidgetItem* widget)
{
  Q_D(LabelWidget);
  return d->clone(widget);
}

