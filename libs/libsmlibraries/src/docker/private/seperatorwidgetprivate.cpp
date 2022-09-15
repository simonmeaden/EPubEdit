#include "docker/private/seperatorwidgetprivate.h"
#include "docker/abstractdockwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/seperatorwidget.h"
#include "utilities/x11colors.h"

//====================================================================
//=== SeperatorWidgetPrivate
//====================================================================
SeperatorWidgetPrivate::SeperatorWidgetPrivate(AbstractDockWidget* parent,
                                               WidgetItem* qptr)
  : WidgetItemPrivate(parent, qptr)
  , m_color(QColorConstants::X11::DimGray)
{
}

QColor&
SeperatorWidgetPrivate::color()
{
  return m_color;
}

void
SeperatorWidgetPrivate::setColor(const QColor& color)
{
  m_color = color;
}

int
SeperatorWidgetPrivate::thickness()
{
  return m_thickness;
}

void
SeperatorWidgetPrivate::setThickness(int thickness)
{
  m_thickness = thickness;
}

WidgetItem*
SeperatorWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new SeperatorWidget(m_parentWidget);
  }

  auto seperator = qobject_cast<SeperatorWidget*>(widget);
  if (seperator) {
    // otherwise passing the wrong type.
    WidgetItemPrivate::clone(seperator);
    setColor(seperator->color());
    setThickness(seperator->thickness());
  }
  return seperator;
}

const QSize
SeperatorWidgetPrivate::calcMinimumSize()
{
  auto p = qobject_cast<AbstractDockItem*>(parentWidget());
  auto dockPosition = p->dockPosition();
  auto ps = p->sizeHint();

  switch (dockPosition) {
    case East:
    case West: {
      auto pw = ps.width();
      m_minContentSize.setWidth(pw);
      m_minContentSize.setHeight(topMargin() + thickness() + bottomMargin());
      break;
    }
    case North:
    case South: {
      auto ph = ps.height();
      m_minContentSize.setWidth(leftMargin() + thickness() + rightMargin());
      m_minContentSize.setHeight(ph);
      break;
    }
    default:
      break;
  }
  return m_minContentSize;
}

void
SeperatorWidgetPrivate::paint(QPainter& painter)
{
  painter.save();
  paintBackground(painter);
  auto pen = painter.pen();
  pen.setColor(m_color);
  pen.setWidth(thickness());
  painter.setPen(pen);
  auto t = m_rect.top() + m_margins.top();
  painter.drawLine(m_rect.left(), t, m_rect.right(), t);
  painter.restore();
}
