#include "docker/widgetitem.h"
#include "docker/abstractdockitem.h"
#include "docker/dockwidget.h"
#include "docker/private/widgetitemprivate.h"

#include <algorithm>

#include <QDebug>

//====================================================================
//=== WidgetItem
//====================================================================
WidgetItem::WidgetItem(AbstractDockWidget* parent)
  : QObject(parent)
  , d_ptr(new WidgetItemPrivate(parent, this))
{
}

WidgetItem::WidgetItem(WidgetItemPrivate& d)
  : d_ptr(&d)
{
}

bool
WidgetItem::isEnabled() const
{
  return d_ptr->isEnabled();
}

void
WidgetItem::setEnabled(bool enabled)
{
  d_ptr->setEnabled(enabled);
}

const QRect&
WidgetItem::rect() const
{
  return d_ptr->rect();
}

WidgetType
WidgetItem::type() const
{
  return d_ptr->type();
}

void
WidgetItem::setType(WidgetType type)
{
  d_ptr->setType(type);
}

WidgetPosition
WidgetItem::widgetPosition() const
{
  return d_ptr->widgetPosition();
}

void
WidgetItem::setWidgetPosition(WidgetPosition position)
{
  d_ptr->setWidgetPosition(position);
}

const QString&
WidgetItem::tooltip() const
{
  return d_ptr->tooltip();
}

void
WidgetItem::setTooltip(const QString& tooltip)
{
  d_ptr->setTooltip(tooltip);
}

bool
WidgetItem::isHoverOver() const
{
  return d_ptr->isHoverOver();
}

void
WidgetItem::setHoverOver(bool hoverOver)
{
  d_ptr->setHoverOver(hoverOver);
}

void
WidgetItem::setGeometry(const QRect& rect, const QRect& /*widgetRect*/)
{
  d_ptr->setGeometry(rect);
}

QSize
WidgetItem::sizeHint() const
{
  return d_ptr->sizeHint();
}

void
WidgetItem::paintBackground(QPainter& painter)
{
  d_ptr->paintBackground(painter);
}

const QSize&
WidgetItem::minContentSize() const
{
  return d_ptr->minContentSize();
}

const QVariant&
WidgetItem::data() const
{
  return d_ptr->data();
}

void
WidgetItem::setData(const QVariant& data)
{
  d_ptr->setData(data);
}

void
WidgetItem::clone(QObject* item)
{
  auto anobject = qobject_cast<WidgetItem*>(item);
  if (anobject)
    d_ptr->clone(anobject);
}

WidgetItem*
WidgetItem::createFromName(WidgetItem* item)
{
  auto metaobject = item->metaObject();
  auto name = item->objectName();
//  if (name == "ButtonWidget") {
    QObject* instance = metaobject->newInstance();
    return qobject_cast<WidgetItem*>(instance);
//  }
}

bool
WidgetItem::isSelected() const
{
  return d_ptr->isSelected();
}

void
WidgetItem::setSelected(bool selected)
{
  d_ptr->setSelected(selected);
}

void
WidgetItem::hideWidget()
{
  // TODO ??? maybe remove
  d_ptr->hideWidget();
}

bool
WidgetItem::isVisible() const
{
  return d_ptr->isVisible();
}

const QMargins&
WidgetItem::margins() const
{
  return d_ptr->margins();
}

void
WidgetItem::setMargins(const QMargins& margins)
{
  d_ptr->setMargins(margins);
  emit widgetChanged();
}

int
WidgetItem::leftMargin()
{
  return d_ptr->leftMargin();
}

int
WidgetItem::rightMargin()
{
  return d_ptr->rightMargin();
}

int
WidgetItem::topMargin()
{
  return d_ptr->topMargin();
}

int
WidgetItem::bottomMargin()
{
  return d_ptr->bottomMargin();
}

void
WidgetItem::setLeftMargin(int margin)
{
  d_ptr->setLeftMargin(margin);
  emit widgetChanged();
}

void
WidgetItem::setRightMargin(int margin)
{
  d_ptr->setRightMargin(margin);
  emit widgetChanged();
}

void
WidgetItem::setTopMargin(int margin)
{
  d_ptr->setTopMargin(margin);
  emit widgetChanged();
}

void
WidgetItem::setBottomMargin(int margin)
{
  d_ptr->setBottomMargin(margin);
  emit widgetChanged();
}
