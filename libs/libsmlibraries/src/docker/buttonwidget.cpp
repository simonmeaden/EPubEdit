#include "docker/buttonwidget.h"
#include "docker/private/buttonwidgetprivate.h"

//====================================================================
//=== ButtonWrapper
//====================================================================
ButtonWidget::ButtonWidget(AbstractDockWidget* parent)
  : WidgetItem(*new ButtonWidgetPrivate(parent, this))
{
  qRegisterMetaType<ButtonWidget*>();
}

ButtonWidget::ButtonWidget(ButtonWidgetPrivate& d)
  : WidgetItem(d)
{
}

Arrangement
ButtonWidget::arrangement()
{
  Q_D(ButtonWidget);
  return d->arrangement();
}

void
ButtonWidget::setArrangement(Arrangement arrangement)
{
  Q_D(ButtonWidget);
  d->setArrangement(arrangement);
}

const QRect&
ButtonWidget::iconRect()
{
  Q_D(ButtonWidget);
  return d->iconRect();
}

void
ButtonWidget::setIconRect(const QRect& iconRect)
{
  Q_D(ButtonWidget);
  d->setIconRect(iconRect);
}

const QPixmap&
ButtonWidget::pixmap()
{
  Q_D(ButtonWidget);
  return d->m_pixmap;
}

void
ButtonWidget::setPixmap(const QPixmap& pixmap)
{
  Q_D(ButtonWidget);
  d->m_pixmap = pixmap;
  d->m_iconSize = pixmap.size();
}

const QSize&
ButtonWidget::iconSize()
{
  Q_D(ButtonWidget);
  return d->m_iconSize;
}

//void
//ButtonWidget::setIconSize(const QSize& iconSize)
//{
//  Q_D(ButtonWidget);
//  d->m_iconSize = iconSize;
//}

int
ButtonWidget::spacer()
{
  Q_D(ButtonWidget);
  return d->spacer();
}

void
ButtonWidget::setSpacer(int spacer)
{
  Q_D(ButtonWidget);
  d->setSpacer(spacer);
}

const QSize
ButtonWidget::calcMinimumSize()
{
  Q_D(ButtonWidget);
  return d->calcMinimumSize();
}

void
ButtonWidget::paint(QPainter& painter)
{
  Q_D(ButtonWidget);
  d->paint(painter);
}

void
ButtonWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  Q_D(ButtonWidget);
  d->setGeometry(widgetRect, contentsRect);
}

void
ButtonWidget::clone(QObject* widget)
{
  Q_D(ButtonWidget);
  auto anobject = qobject_cast<ButtonWidget*>(widget);
  if (anobject)
    d->clone(anobject);
}

const QColor&
ButtonWidget::textColor()
{
  Q_D(ButtonWidget);
  return d->textColor();
}

void
ButtonWidget::setTextColor(const QColor& textColor)
{
  Q_D(ButtonWidget);
  d->setTextColor(textColor);
}

const QString&
ButtonWidget::text()
{
  Q_D(ButtonWidget);
  return d->text();
}

void
ButtonWidget::setText(const QString& text)
{
  Q_D(ButtonWidget);
  d->setText(text);
}

const QRect&
ButtonWidget::textRect()
{
  Q_D(ButtonWidget);
  return d->textRect();
}

void
ButtonWidget::setTextRect(const QRect& textRect)
{
  Q_D(ButtonWidget);
  d->setTextRect(textRect);
}
