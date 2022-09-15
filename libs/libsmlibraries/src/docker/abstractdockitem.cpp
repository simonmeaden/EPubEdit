#include "docker/abstractdockitem.h"
#include "docker/abstractdockwidget.h"
#include "docker/buttonwidget.h"
#include "docker/labelwidget.h"
#include "docker/listbuttonwidget.h"
#include "docker/listwidget.h"
#include "docker/private/abstractdockitemprivate.h"
#include "docker/seperatorwidget.h"
#include "docker/widgetitem.h"

//====================================================================
//=== DockItem
//====================================================================
AbstractDockItem::AbstractDockItem(DockPosition position,
                                   AbstractDockWidget* parent)
  : QObject{ parent }
  , d_ptr(new AbstractDockItemPrivate(position, parent, this))
{
}

AbstractDockItem::AbstractDockItem(AbstractDockItemPrivate& d)
  : d_ptr(&d)
{
}

ButtonWidget*
AbstractDockItem::addIconButton(WidgetPosition pos,
                                QIcon icon,
                                QSize iconSize,
                                const QString& tooltip)
{
  Q_D(AbstractDockItem);
  return d->addIconButton(pos, icon, iconSize, tooltip);
}

ButtonWidget*
AbstractDockItem::addIconListButton(WidgetPosition pos,
                                    QIcon icon,
                                    QSize iconSize,
                                    const QString& tooltip)
{
  return d_ptr->addIconListButton(pos, icon, iconSize, tooltip);
}

ButtonWidget*
AbstractDockItem::addIconTextButton(WidgetPosition pos,
                                    QIcon icon,
                                    QSize iconSize,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  return d_ptr->addIconTextButton(pos, icon, iconSize, text, textPos, tooltip);
}

ButtonWidget*
AbstractDockItem::addIconTextListButton(WidgetPosition pos,
                                        QIcon icon,
                                        QSize iconSize,
                                        const QString& text,
                                        Arrangement textPos,
                                        const QString& tooltip)
{
  return d_ptr->addIconTextListButton(
    pos, icon, iconSize, text, textPos, tooltip);
}

ButtonWidget*
AbstractDockItem::addTextButton(WidgetPosition pos,
                                const QString& text,
                                const QString& tooltip)
{
  return d_ptr->addTextButton(pos, text, tooltip);
}

ButtonWidget*
AbstractDockItem::addTextListButton(WidgetPosition pos,
                                    const QString& text,
                                    const QString& tooltip)
{
  return d_ptr->addTextListButton(pos, text, tooltip);
}

WidgetItem*
AbstractDockItem::addItem(WidgetItem* item)
{
  return d_ptr->addItem(item);
}

WidgetItem*
AbstractDockItem::addTextLabel(WidgetPosition pos,
                               const QString& text,
                               const QString& tooltip)
{
  return d_ptr->addTextLabel(pos, text, tooltip);
}

WidgetItem*
AbstractDockItem::addSeperator(WidgetPosition pos)
{
  return d_ptr->addSeperator(pos);
}

WidgetItem*
AbstractDockItem::addCustomWidget(CustomWidget* item)
{
  return d_ptr->addCustomWidget(item);
}

void
AbstractDockItem::setText(int index, const QString& text)
{
  d_ptr->setText(index, text);
}

void
AbstractDockItem::setToolTip(int index, const QString& tooltip)
{
  d_ptr->setToolTip(index, tooltip);
}

WidgetItem*
AbstractDockItem::takeAt(int index)
{
  return d_ptr->takeAt(index);
}

WidgetItem*
AbstractDockItem::at(int index)
{
  return d_ptr->at(index);
}

int
AbstractDockItem::count()
{
  return d_ptr->count();
}

bool
AbstractDockItem::replace(int index, WidgetItem* w)
{
  return d_ptr->replace(index, w);
}

bool
AbstractDockItem::widgetEnabled(int index)
{
  return d_ptr->widgetEnabled(index);
}

bool
AbstractDockItem::enableWidget(int index, bool value)
{
  return d_ptr->enableWidget(index, value);
}

bool
AbstractDockItem::selected(int index)
{
  return d_ptr->selected(index);
}

bool
AbstractDockItem::setSelected(int index, bool value)
{
  return d_ptr->setSelected(index, value);
}

void
AbstractDockItem::calculateGeometry(const QRect& rect)
{
  d_ptr->calculateGeometry(rect);
}

QMargins
AbstractDockItem::widgetMargins(int index)
{
  return d_ptr->widgetMargins(index);
}

void
AbstractDockItem::setWidgetMargins(int index,
                                   int left,
                                   int top,
                                   int right,
                                   int bottom)
{
  d_ptr->setWidgetMargins(index, left, top, right, bottom);
}

///*
// * Scans the sub widgets to find the maximum widget size. This means that all
// * the sub-widgets are the same size, at least if they are buttons. Other
// * sub-widgets may or may not affect this value.
// */
// void
// DockItem::calcMaxWidgetSizes()
//{
//  auto maxMargins = calcMaxMargins();
//  auto maxContentsSize = calcMaxContentsSize();
//  auto w = maxMargins.left() + maxContentsSize.width() + maxMargins.right();
//  auto h = maxMargins.top() + maxContentsSize.height() + maxMargins.bottom();

//  m_maxWidgetWidth = std::max(w, m_maxWidgetWidth);
//  m_maxWidgetHeight = std::max(h, m_maxWidgetHeight);
//}

const QRect&
AbstractDockItem::rect() const
{
  return d_ptr->rect();
}

void AbstractDockItem::clone(QObject *object)
{
  auto anobject = qobject_cast<AbstractDockItem*>(object);
  if (anobject)
    d_ptr->clone(anobject);
}

void
AbstractDockItem::calcMaxWidgetSizes()
{
  d_ptr->calcMaxWidgetSizes();
}

void
AbstractDockItem::setPreferredSize(int width, int height)
{
  d_ptr->setPreferredSize(width, height);
}

void
AbstractDockItem::setPreferredSize(QSize size)
{
  d_ptr->setPreferredSize(size);
}

void
AbstractDockItem::setPreferredWidth(int width)
{
  d_ptr->setPreferredWidth(width);
}

void
AbstractDockItem::setPreferredHeight(int height)
{
  d_ptr->setPreferredHeight(height);
}

DockPosition
AbstractDockItem::dockPosition() const
{
  return d_ptr->dockPosition();
}

int
AbstractDockItem::height()
{
  return d_ptr->height();
}

void
AbstractDockItem::setWidth(int width)
{
  d_ptr->setWidth(width);
}

void
AbstractDockItem::setHeight(int height)
{
  d_ptr->setHeight(height);
}

int
AbstractDockItem::width()
{
  return d_ptr->width();
}

bool
AbstractDockItem::isVisible() const
{
  return d_ptr->isVisible();
}

void
AbstractDockItem::setVisible(bool value)
{
  d_ptr->setVisible(value);
}

void
AbstractDockItem::show()
{
  d_ptr->setVisible(true);
}

void
AbstractDockItem::hide()
{
  d_ptr->setVisible(false);
}

void
AbstractDockItem::paint(QPainter& painter)
{
  d_ptr->paint(painter);
}

const QList<WidgetItem*>&
AbstractDockItem::widgets() const
{
  return d_ptr->widgets();
}
