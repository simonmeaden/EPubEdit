#include "headerwidget.h"
#include "abstractdockitem.h"
#include "dockheader.h"
#include "dockwidget.h"
#include "private/headerwidgetprivate.h"
#include "widgetitem.h"

#include <Logger.h>

HeaderWidget::HeaderWidget(QWidget* parent)
  : AbstractDockWidget(*new HeaderWidgetPrivate(this), parent)
{
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
}

HeaderWidget::HeaderWidget(HeaderWidgetPrivate& d)
  : AbstractDockWidget(d)
{
}

HeaderWidget::HeaderWidget(HeaderWidgetPrivate& d, QWidget* parent)
  : AbstractDockWidget(d, parent)
{
}

DockHeader*
HeaderWidget::header()
{
  Q_D(HeaderWidget);
  return d->header();
}

QWidget*
HeaderWidget::setWidget(QWidget* widget)
{
  Q_D(HeaderWidget);
  return d->setWidget(widget);
}

void
HeaderWidget::hideWidget()
{
  Q_D(HeaderWidget);
  d->hideWidget();
  adjustSize();
  emit sizeChanged(this, d->m_hiddenSize);
}

void
HeaderWidget::showWidget()
{
  Q_D(HeaderWidget);
  d->showWidget();
  adjustSize();
  emit sizeChanged(this, d->m_hiddenSize);
}

bool
HeaderWidget::isWidgetVisible()
{
  Q_D(HeaderWidget);
  return d->isWidgetVisible();
}

void
HeaderWidget::paintEvent(QPaintEvent* /*event*/)
{
  Q_D(HeaderWidget);
  QPainter painter(this);
  d->paint(painter);
}

//void
//HeaderWidget::hoverEnterEvent(QHoverEvent* event)
//{
//  Q_D(HeaderWidget);
//  auto p = event->pos();
//  d->hoverEnter(p);
//}

//void
//HeaderWidget::hoverLeaveEvent(QHoverEvent*)
//{
//  Q_D(HeaderWidget);
//  d->hoverLeave();
//}

//void
//HeaderWidget::hoverMoveEvent(QHoverEvent* event)
//{
//  Q_D(HeaderWidget);
//  auto p = event->pos();
//  d->hoverMove(p);
//}

void
HeaderWidget::mousePressEvent(QMouseEvent* event)
{
  Q_D(HeaderWidget);
  auto pos = event->pos();
  setFocus();
  d->mousePress(pos);
}

bool
HeaderWidget::event(QEvent* event)
{
  Q_D(HeaderWidget);
  switch (event->type()) {
    case QEvent::HoverEnter:
      d->hoverEnter(static_cast<QHoverEvent*>(event)->pos());
      return true;
      break;
    case QEvent::HoverLeave:
      d->hoverLeave();
      return true;
      break;
    case QEvent::HoverMove:
      d->hoverMove(static_cast<QHoverEvent*>(event)->pos());
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}

void
HeaderWidget::resizeEvent(QResizeEvent* /*event*/)
{
  calculateGeometry(rect());
}

void
HeaderWidget::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus()) {
    emit gotFocus(this);
    LOG_DEBUG() << "Got focus";
  }

  QWidget::focusInEvent(event);
}

void
HeaderWidget::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus()) {
    emit lostFocus(this);
    LOG_DEBUG() << "Lost focus";
  }

  QWidget::focusOutEvent(event);
}

void
HeaderWidget::widgetWasClicked(QPoint pos)
{
  Q_D(HeaderWidget);
  d->widgetWasClicked(pos);
}

AbstractDockWidget*
HeaderWidget::clone(AbstractDockWidget* widget)
{
  Q_D(HeaderWidget);
  return d->clone(widget);
}

void
HeaderWidget::calculateGeometry(const QRect& rect)
{
  Q_D(HeaderWidget);
  d->calculateGeometry(rect);
}
