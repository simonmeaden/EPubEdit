#include "abstractdockwidget.h"
#include "abstractdockitem.h"
#include "listbuttonwidget.h"
#include "listwidget.h"
#include "private/abstractdockwidgetprivate.h"
#include "widgetitem.h"
#include "x11colors.h"

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

const QColor&
AbstractDockWidget::textColor() const
{
  return d_ptr->textColor();
}

void AbstractDockWidget::initGui()
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

void
AbstractDockWidget::clone(AbstractDockWidget* widget)
{
  d_ptr->clone(widget);
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

