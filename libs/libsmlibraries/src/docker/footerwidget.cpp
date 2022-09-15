#include "docker/footerwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/abstractdockwidget.h"
#include "docker/dockfooter.h"
#include "docker/dockwidget.h"
#include "docker/private/footerwidgetprivate.h"
#include "docker/widgetitem.h"

#include <Logger.h>

FooterWidget::FooterWidget(QWidget* parent)
  : AbstractDockWidget(*new FooterWidgetPrivate(this), parent)
{
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
}

FooterWidget::FooterWidget(FooterWidgetPrivate& d)
  : AbstractDockWidget(d)
{
}

DockFooter*
FooterWidget::footer()
{
  return d_ptr->footer();
}

//QWidget*
//FooterWidget::setWidget(QWidget* widget)
//{
//  return d_ptr->setWidget(widget);
//}

void
FooterWidget::hideWidget()
{
  Q_D(FooterWidget);
  d->hideWidget();
  adjustSize();
  emit sizeChanged(this, m_hiddenSize);
}

void
FooterWidget::showWidget()
{
  Q_D(FooterWidget);
  d->showWidget();
  adjustSize();
  emit sizeChanged(this, m_visibleSize);
}

bool
FooterWidget::isWidgetVisible()
{
  return d_ptr->isWidgetVisible();
}

void
FooterWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  d_ptr->paint(painter);
}

void
FooterWidget::hoverEnterEvent(QHoverEvent* event)
{
  auto p = event->position().toPoint();
  d_ptr->hoverEnter(p);
}

void
FooterWidget::hoverLeaveEvent(QHoverEvent*)
{
  d_ptr->hoverLeave();
}

void
FooterWidget::hoverMoveEvent(QHoverEvent* event)
{
  auto p = event->position().toPoint();
  d_ptr->hoverMove(p);
}

void
FooterWidget::mousePressEvent(QMouseEvent* event)
{
  auto pos = event->pos();
  setFocus();
  d_ptr->mousePress(pos);
}

void
FooterWidget::widgetWasClicked(QPoint pos)
{
  d_ptr->widgetWasClicked(pos);
}

bool
FooterWidget::event(QEvent* event)
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
FooterWidget::resizeEvent(QResizeEvent* /*event*/)
{
  calculateGeometry(rect());
}

void
FooterWidget::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus()) {
    emit gotFocus(this);
    LOG_DEBUG() << "Got focus";
  }

  QWidget::focusInEvent(event);
}

void
FooterWidget::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus()) {
    emit lostFocus(this);
    LOG_DEBUG() << "Lost focus";
  }

  QWidget::focusOutEvent(event);
}

QWidget *FooterWidget::clone(QWidget *widget)
{
  auto anobject = qobject_cast<FooterWidget*>(widget);
  if (anobject)
    return d_ptr->clone(anobject);
  return nullptr;
}

void
FooterWidget::calculateGeometry(const QRect& rect)
{
  d_ptr->calculateGeometry(rect);
}
