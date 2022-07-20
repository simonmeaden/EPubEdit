#include "dockwidget.h"
#include "widgetitem.h"

#include <algorithm>

#include <QDebug>

#include "abstractdockitem.h"
#include "dockcorner.h"
#include "dockfooter.h"
#include "dockheader.h"
#include "docktoolbar.h"
#include "private/abstractdockwidgetprivate.h"
#include "private/dockwidgetprivate.h"
#include "widgetitem.h"

//====================================================================
//=== DockWidget
//====================================================================
DockWidget::DockWidget(QWidget* parent)
  : AbstractDockWidget(*new DockWidgetPrivate(this),
                       parent)
{
}

DockWidget::DockWidget(DockWidgetPrivate& d)
  : AbstractDockWidget(d)
{
}

QWidget*
DockWidget::widget()
{
  Q_D(DockWidget);
  return d->widget();
}

QWidget*
DockWidget::setWidget(QWidget* widget)
{
  Q_D(DockWidget);
  return d->setWidget(widget);
}

void
DockWidget::calculateGeometry(const QRect& rect)
{
  Q_D(DockWidget);
  d->calculateGeometry(rect);
}

DockToolbar*
DockWidget::addToolbar(DockPosition position)
{
  Q_D(DockWidget);
  auto w = d->addToolbar(position);
  repaint();
  return w;
}

void
DockWidget::removeToolbar(DockPosition position)
{
  Q_D(DockWidget);
  d->removeToolbar(position);
  repaint();
}

void
DockWidget::removeToolbar(DockToolbar* toolbar)
{
  Q_D(DockWidget);
  d->removeToolbar(toolbar);
  repaint();
}

void
DockWidget::hideToolbar(DockPosition position)
{
  Q_D(DockWidget);
  d->hideToolbar(position);
}

void
DockWidget::hideToolbar(DockToolbar* toolbar)
{
  Q_D(DockWidget);
  d->hideToolbar(toolbar);
}

void
DockWidget::showToolbar(DockPosition position)
{
  Q_D(DockWidget);
  d->showToolbar(position);
}

void
DockWidget::showToolbar(DockToolbar* toolbar)
{
  Q_D(DockWidget);
  d->showToolbar(toolbar);
}

bool
DockWidget::moveToolbar(DockPosition newPosition, DockPosition oldPosition)
{
  Q_D(DockWidget);
  return d->moveToolbar(newPosition, oldPosition);
}

void
DockWidget::resizeEvent(QResizeEvent* /*event*/)
{
  calculateGeometry(rect());
}

AbstractDockWidget*
DockWidget::clone(DockWidget* widget)
{
  return d_ptr->clone(widget);
}

AbstractDockItem*
DockWidget::header()
{
  Q_D(DockWidget);
  return d->header();
}

AbstractDockItem*
DockWidget::footer()
{
  Q_D(DockWidget);
  return d->footer();
}

AbstractDockItem*
DockWidget::northToolbar()
{
  Q_D(DockWidget);
  return d->northToolbar();
}

AbstractDockItem*
DockWidget::southToolbar()
{
  Q_D(DockWidget);
  return d->southToolbar();
}

AbstractDockItem*
DockWidget::eastToolbar()
{
  Q_D(DockWidget);
  return d->eastToolbar();
}

AbstractDockItem*
DockWidget::westToolbar()
{
  Q_D(DockWidget);
  return d->westToolbar();
}

DockToolbar*
DockWidget::toolbarAt(DockPosition position)
{
  Q_D(DockWidget);
  return d->toolbarAt(position);
}

DockToolbar*
DockWidget::toolbarTakeAt(DockPosition position)
{
  Q_D(DockWidget);
  return d->toolbarTakeAt(position);
}

bool
DockWidget::setToolbarAt(DockPosition position, DockToolbar* toolbar)
{
  Q_D(DockWidget);
  return d->setToolbarAt(position, toolbar);
}

DockFooter*
DockWidget::addFooter()
{
  Q_D(DockWidget);
  return d->addFooter();
}

void
DockWidget::removeFooter()
{
  Q_D(DockWidget);
  return d->removeFooter();
}

void
DockWidget::hideFooter()
{
  Q_D(DockWidget);
  d->hideFooter();
}

void
DockWidget::showFooter()
{
  Q_D(DockWidget);
  d->showFooter();
}

DockHeader*
DockWidget::addHeader()
{
  Q_D(DockWidget);
  return d->addHeader();
}

void
DockWidget::removeHeader()
{
  Q_D(DockWidget);
  d->removeHeader();
}

void
DockWidget::hideHeader()
{
  Q_D(DockWidget);
  d->hideHeader();
}

void
DockWidget::showHeader()
{
  Q_D(DockWidget);
  d->showHeader();
}

void
DockWidget::setCorners(CornerType northWest,
                       CornerType northEast,
                       CornerType southWest,
                       CornerType southEast)
{
  Q_D(DockWidget);
  d->setCorners(northWest, northEast, southWest, southEast);
}

void
DockWidget::setCornerSize(DockPosition position, int width, int height)
{
  Q_D(DockWidget);
  d->setCornerSize(position, width, height);
}

void
DockWidget::setCornerSize(DockPosition position, QSize size)
{
  Q_D(DockWidget);
  d->setCornerSize(position, size.width(), size.height());
}

void
DockWidget::setCornerWidth(DockPosition position, int width)
{
  Q_D(DockWidget);
  d->setCornerWidth(position, width);
}

void
DockWidget::setCornerHeight(DockPosition position, int height)
{
  Q_D(DockWidget);
  d->setCornerHeight(position, height);
}

void
DockWidget::paintEvent(QPaintEvent* /*event*/)
{
  Q_D(DockWidget);
  QPainter painter(this);
  d->paint(painter);
}

void
DockWidget::hoverEnterEvent(QHoverEvent* event)
{
  Q_D(DockWidget);
  auto p = event->pos();
  d->hoverEnter(p);
}

void
DockWidget::hoverLeaveEvent(QHoverEvent* /*event*/)
{
  Q_D(DockWidget);
  d->hoverLeave();
}

void
DockWidget::hoverMoveEvent(QHoverEvent* event)
{
  Q_D(DockWidget);
  auto p = /*mapToGlobal(*/ event->pos() /*)*/;
  d->hoverMove(p);
}

void
DockWidget::mousePressEvent(QMouseEvent* event)
{
  Q_D(DockWidget);
  auto p = event->pos();
  d->mousePress(p);
}

bool
DockWidget::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnterEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverLeave:
      hoverLeaveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverMove:
      hoverMoveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}

void
DockWidget::widgetWasClicked(QPoint pos)
{
  Q_D(DockWidget);
  d->widgetWasClicked(pos);
  // TODO So far only headers and footers have ListWidget's and
  // ListButtonWidgets.
}

void
DockWidget::setCorner(DockPosition position, CornerType type)
{
  Q_D(DockWidget);
  d->setCorner(position, type);
}

DockCorner*
DockWidget::corner(DockPosition position)
{
  Q_D(DockWidget);
  return d->corner(position);
}

void
DockWidget::setCorner(DockCorner* corner)
{
  Q_D(DockWidget);
  d->setCorner(corner);
}
