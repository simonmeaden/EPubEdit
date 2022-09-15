#include "docker/private/widgetitemprivate.h"
#include "docker/abstractdockwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/widgetitem.h"
#include "utilities/x11colors.h"

//====================================================================
//=== WidgetItemPrivate
//====================================================================
WidgetItemPrivate::WidgetItemPrivate(AbstractDockWidget* parent,
                                     WidgetItem* qptr)
  : q_ptr(qptr)
  , m_parentWidget(parent)
  , m_margins(QMargins(LEFTMARGIN, TOPMARGIN, RIGHTMARGIN, BOTTOMMARGIN))
{
}

WidgetType
WidgetItemPrivate::type() const
{
  return m_type;
}

void
WidgetItemPrivate::setType(WidgetType type)
{
  m_type = type;
}

WidgetPosition
WidgetItemPrivate::widgetPosition() const
{
  return m_widgetPosition;
}

void
WidgetItemPrivate::setWidgetPosition(WidgetPosition newPosition)
{
  m_widgetPosition = newPosition;
  emit q_ptr->widgetChanged();
}

const QString&
WidgetItemPrivate::tooltip() const
{
  return m_tooltip;
}

void
WidgetItemPrivate::setTooltip(const QString& newTooltip)
{
  m_tooltip = newTooltip;
}

const QMargins&
WidgetItemPrivate::margins() const
{
  return m_margins;
}

void
WidgetItemPrivate::setMargins(const QMargins& newMargins)
{
  m_margins = newMargins;
}

int
WidgetItemPrivate::leftMargin()
{
  return m_margins.left();
}

void
WidgetItemPrivate::setLeftMargin(int margin)
{
  m_margins.setLeft(margin);
}

int
WidgetItemPrivate::rightMargin()
{
  return m_margins.right();
}

void
WidgetItemPrivate::setRightMargin(int margin)
{
  m_margins.setRight(margin);
}

int
WidgetItemPrivate::topMargin()
{
  return m_margins.top();
}

void
WidgetItemPrivate::setTopMargin(int margin)
{
  m_margins.setTop(margin);
}

int
WidgetItemPrivate::bottomMargin()
{
  return m_margins.bottom();
}

void
WidgetItemPrivate::setBottomMargin(int margin)
{
  m_margins.setBottom(margin);
}

bool
WidgetItemPrivate::isEnabled() const
{
  return m_enabled;
}

void
WidgetItemPrivate::setEnabled(bool enabled)
{
  m_enabled = enabled;
}

bool
WidgetItemPrivate::isSelected() const
{
  return m_selected;
}

void
WidgetItemPrivate::setSelected(bool selected)
{
  m_selected = selected;
}

void
WidgetItemPrivate::hideWidget()
{
  // TODO ??? maybe remove
}

const QVariant&
WidgetItemPrivate::data() const
{
  return m_data;
}

void
WidgetItemPrivate::setData(const QVariant& data)
{
  m_data = data;
}

WidgetItem*
WidgetItemPrivate::clone(WidgetItem* item)
{
  if (item) {
    item->d_ptr->setParentWidget(m_parentWidget);
    item->setType(m_type);
    item->setWidgetPosition(m_widgetPosition);
    item->setTooltip(m_tooltip);
    item->setMargins(m_margins);
    item->setData(m_data);
  }
  return item;
}

void
WidgetItemPrivate::setGeometry(const QRect& rect, const QRect&)
{
  m_rect = rect;
}

void
WidgetItemPrivate::paintBackground(QPainter& painter)
{
  painter.save();
  if (isSelected()) {
    painter.fillRect(m_rect, m_parentWidget->selectedColor());
  } else if (isHoverOver()) {
    painter.fillRect(m_rect, m_parentWidget->hoverBackColor());
  } else {
    painter.fillRect(m_rect, m_parentWidget->backColor());
  }
  painter.restore();
}

const QSize&
WidgetItemPrivate::minContentSize() const
{
  return m_minContentSize;
}

bool
WidgetItemPrivate::isHoverOver() const
{
  return m_hoverOver;
}

void
WidgetItemPrivate::setHoverOver(bool newHoverOver)
{
  m_hoverOver = newHoverOver;
}

bool
WidgetItemPrivate::isVisible() const
{
  if (m_parentWidget)
    return m_parentWidget->isVisible();
  return false;
}

QSize
WidgetItemPrivate::sizeHint() const
{
  return m_minContentSize.grownBy(m_margins);
}

const QRect&
WidgetItemPrivate::rect() const
{
  return m_rect;
}

AbstractDockWidget*
WidgetItemPrivate::parentWidget() const
{
  return m_parentWidget;
}

void
WidgetItemPrivate::setParentWidget(AbstractDockWidget* newParentWidget)
{
  m_parentWidget = newParentWidget;
}
