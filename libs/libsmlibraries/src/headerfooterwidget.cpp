#include "headerfooterwidget.h"
#include "abstractdockitem.h"
#include "dockfooter.h"
#include "dockheader.h"
#include "dockwidget.h"
#include "private/abstractdockwidgetprivate.h"
#include "private/headerfooterwidgetprivate.h"
#include "widgetitem.h"

#include <Logger.h>

HeaderFooterWidget::HeaderFooterWidget(QWidget* parent)
  : AbstractDockWidget(*new HeaderFooterWidgetPrivate(this), parent)
{
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
}

HeaderFooterWidget::HeaderFooterWidget(HeaderFooterWidgetPrivate& d)
  : AbstractDockWidget(d)
{
}

DockHeader*
HeaderFooterWidget::header()
{
  Q_D(HeaderFooterWidget);
  return d->header();
}

DockFooter*
HeaderFooterWidget::footer()
{
  Q_D(HeaderFooterWidget);
  return d->footer();
}

QWidget*
HeaderFooterWidget::setWidget(QWidget* widget)
{
  Q_D(HeaderFooterWidget);
  return d->setWidget(widget);
}

void
HeaderFooterWidget::hideWidget()
{
  Q_D(HeaderFooterWidget);
  d->hideWidget();
  adjustSize();
  emit sizeChanged(this, d->m_hiddenSize);
}

HeaderFooterWidget*
HeaderFooterWidget::clone(HeaderFooterWidget* widget)
{
  Q_D(HeaderFooterWidget);
  return d->clone(widget);
}

void
HeaderFooterWidget::paintEvent(QPaintEvent*)
{
  Q_D(HeaderFooterWidget);
  QPainter painter(this);
  d->paint(painter);
}

void
HeaderFooterWidget::hoverEnterEvent(QHoverEvent* event)
{
  Q_D(HeaderFooterWidget);
  auto p = event->pos();
  d->hoverEnter(p);
}

void
HeaderFooterWidget::hoverLeaveEvent(QHoverEvent* event)
{
  Q_D(HeaderFooterWidget);
  auto p = event->pos();
  d->hoverLeave(p);
}

void
HeaderFooterWidget::hoverMoveEvent(QHoverEvent* event)
{
  Q_D(HeaderFooterWidget);
  auto p = event->pos();
  d->hoverMove(p);
}

void
HeaderFooterWidget::mousePressEvent(QMouseEvent* event)
{
  Q_D(HeaderFooterWidget);
  auto pos = event->pos();
  d->mousePress(pos);
}

bool
HeaderFooterWidget::event(QEvent* event)
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
HeaderFooterWidget::resizeEvent(QResizeEvent* /*event*/)
{
  Q_D(HeaderFooterWidget);
  d->calculateGeometry(rect());
}

void
HeaderFooterWidget::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus()) {
    emit gotFocus(this);
    LOG_DEBUG() << "Got focus";
  }
  QWidget::focusInEvent(event);
}

void
HeaderFooterWidget::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus()) {
    emit lostFocus(this);
    LOG_DEBUG() << "Lost focus";
  }
  QWidget::focusOutEvent(event);
}

// void
// HeaderFooterWidget::calculateGeometry(const QRect& rect)
//{
//   auto northHeight = 0;
//   auto southHeight = 0;
//   auto northWidth = rect.size().width();
//   m_width = northWidth;

//  m_footerHeight = m_footer->sizeHint().height();
//  m_headerHeight = m_header->sizeHint().height();
//  m_height = rect.height();
//  southHeight += m_footerHeight;
//  northHeight += m_headerHeight;
//  m_footer->calculateGeometry(
//    QRect(0,
//          rect.height() - m_headerHeight - m_footerHeight,
//          northWidth,
//          m_headerHeight + m_footerHeight));

//  m_availableRect.moveTop(m_headerHeight);
//  m_availableRect.moveBottom(m_footerHeight);
//  m_availableRect.setHeight(rect.height() - (m_headerHeight +
//  m_footerHeight)); m_availableRect.setWidth(rect.width());

//  m_layout->setContentsMargins(0, northHeight + 1, 0, 0);
//}
