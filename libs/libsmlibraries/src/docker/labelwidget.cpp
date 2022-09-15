#include "docker/labelwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/private/labelwidgetprivate.h"

//====================================================================
//=== LabelWidget
//====================================================================
LabelWidget::LabelWidget(AbstractDockWidget* parent)
  : WidgetItem(*new LabelWidgetPrivate(parent, this))
{
  qRegisterMetaType<LabelWidget*>();
}

LabelWidget::LabelWidget(LabelWidgetPrivate& d)
  : WidgetItem(d)
{
  qRegisterMetaType<LabelWidget*>();
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

void LabelWidget::clone(QObject *widget)
{
  Q_D(LabelWidget);
  auto anobject = qobject_cast<LabelWidget*>(widget);
  if (anobject)
    d->clone(anobject);
}

