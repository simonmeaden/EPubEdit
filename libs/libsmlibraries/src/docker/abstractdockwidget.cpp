#include "docker/abstractdockwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/listbuttonwidget.h"
#include "docker/listwidget.h"
#include "docker/private/abstractdockwidgetprivate.h"
#include "docker/widgetitem.h"
#include "utilities/x11colors.h"

AbstractDockWidget::AbstractDockWidget(QWidget* parent)
  : QWidget(parent)
  , d_ptr(new AbstractDockWidgetPrivate(this))
{
  initGui();
}

AbstractDockWidget::AbstractDockWidget(AbstractDockWidgetPrivate& d)
  : d_ptr(&d)
{
  initGui();
}

AbstractDockWidget::AbstractDockWidget(AbstractDockWidgetPrivate& d,
                                       QWidget* parent)
  : QWidget(parent)
  , d_ptr(&d)
{
  initGui();
}

QWidget*
AbstractDockWidget::widget()
{
  Q_D(AbstractDockWidget);
  return d->m_widget;
}

QWidget*
AbstractDockWidget::setWidget(QWidget* widget)
{
  Q_D(AbstractDockWidget);
  return d->setWidget(widget);
}

const QColor&
AbstractDockWidget::textColor() const
{
  return d_ptr->textColor();
}

void
AbstractDockWidget::initGui()
{
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setLayout(d_ptr->m_layout);
}

void
AbstractDockWidget::setTextColor(const QColor& textColor)
{
  d_ptr->setTextColor(textColor);
}

const QColor&
AbstractDockWidget::spacerColor() const
{
  return d_ptr->spacerColor();
}

void
AbstractDockWidget::setSpacerColor(const QColor& spacerColor)
{
  d_ptr->setSpacerColor(spacerColor);
}

const QRect&
AbstractDockWidget::availableRect() const
{
  return d_ptr->availableRect();
}

QWidget *AbstractDockWidget::clone(QWidget* widget)
{
  auto awidget = qobject_cast<AbstractDockWidget*>(widget);
  if (awidget)
    d_ptr->clone(awidget);
  return widget;
}

void
AbstractDockWidget::checkForOpenListWidgets(AbstractDockItem* item)
{
  d_ptr->checkForOpenListWidgets(item);
}

const QBrush&
AbstractDockWidget::backColor() const
{
  return d_ptr->backColor();
}

void
AbstractDockWidget::setBackColor(const QBrush& backColor)
{
  d_ptr->setBackColor(backColor);
}

const QBrush&
AbstractDockWidget::hoverBackColor() const
{
  return d_ptr->hoverBackColor();
}

void
AbstractDockWidget::setHoverBackColor(const QBrush& hoverBackColor)
{
  d_ptr->setHoverBackColor(hoverBackColor);
}

const QBrush&
AbstractDockWidget::selectedColor() const
{
  return d_ptr->selectedColor();
}

void
AbstractDockWidget::setSelectedColor(const QBrush& selectedColor)
{
  d_ptr->setSelectedColor(selectedColor);
}

bool
AbstractDockWidget::dockItemHoverCheck(AbstractDockItem* item, QPoint pos)
{
  return d_ptr->dockItemHoverCheck(item, pos);
}

void
AbstractDockWidget::mouseClickCheck(AbstractDockItem* item, QPoint pos)
{
  d_ptr->mouseClickCheck(item, pos);
}
