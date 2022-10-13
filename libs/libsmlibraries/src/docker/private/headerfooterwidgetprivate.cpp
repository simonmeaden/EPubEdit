#include "docker/private/headerfooterwidgetprivate.h"
#include "docker/dockfooter.h"
#include "docker/dockheader.h"
#include "docker/widgetitem.h"

HeaderFooterWidgetPrivate::HeaderFooterWidgetPrivate(HeaderFooterWidget* parent)
  : AbstractDockWidgetPrivate(parent)
{
  m_header = new DockHeader( q_ptr);
  m_footer = new DockFooter( q_ptr);
}

DockHeader*
HeaderFooterWidgetPrivate::header()
{
  if (m_header)
    return m_header;
  return nullptr;
}

DockFooter*
HeaderFooterWidgetPrivate::footer()
{
  if (m_footer)
    return m_footer;
  return nullptr;
}

void
HeaderFooterWidgetPrivate::hideWidget()
{
  m_layout->removeWidget(m_centralWidget);
  m_hiddenSize = QSize(m_width, 0);
  m_visibleSize = QSize(m_width, m_height);
}

HeaderFooterWidget*
HeaderFooterWidgetPrivate::clone(HeaderFooterWidget* widget)
{
  HeaderFooterWidget* w = widget;
  if (!widget)
    w = new HeaderFooterWidget(m_parent);
  AbstractDockWidgetPrivate::clone(w);
  m_header->clone(qobject_cast<DockHeader*>(w->header()));
  m_footer->clone(qobject_cast<DockFooter*>(w->footer()));
  return w;
}

void
HeaderFooterWidgetPrivate::paint(QPainter& painter)
{
  if (m_header)
    m_header->paint(painter);

  if (m_footer)
    m_footer->paint(painter);
}

void
HeaderFooterWidgetPrivate::hoverEnter(QPoint pos)
{
  if (m_footer) {
    if (dockItemHoverCheck(m_footer, pos)) {
      return;
    }
  }
  if (m_header) {
    if (dockItemHoverCheck(m_header, pos)) {
      return;
    }
  }
}

void
HeaderFooterWidgetPrivate::hoverLeave(QPoint)
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
HeaderFooterWidgetPrivate::hoverMove(QPoint pos)
{
  if (m_footer) {
    if (dockItemHoverCheck(m_footer, pos)) {
      return;
    }
  }
  if (m_header) {
    if (dockItemHoverCheck(m_header, pos)) {
      return;
    }
  }
}

void
HeaderFooterWidgetPrivate::mousePress(QMouseEvent *event)
{
  if (m_header) {
    mousePressCheck(m_header, event);
  }
  if (m_footer) {
    mousePressCheck(m_footer, event);
  }
}

void HeaderFooterWidgetPrivate::mouseMove(QMouseEvent *event)
{
  if (m_header) {
    mouseMoveCheck(m_header, event);
  }
  if (m_footer) {
    mouseMoveCheck(m_footer, event);
  }
}

void HeaderFooterWidgetPrivate::mouseRelease(QMouseEvent *event)
{
  if (m_header) {
    mouseReleaseCheck(m_header, event);
  }
  if (m_footer) {
    mouseReleaseCheck(m_footer, event);
  }
}

void
HeaderFooterWidgetPrivate::calculateGeometry(const QRect& rect)
{
  auto width = rect.size().width();
  m_width = width;

  m_footerHeight = m_footer->sizeHint().height();
  m_headerHeight = m_header->sizeHint().height();
  m_height = rect.height();
  m_footer->calculateGeometry(
    QRect(0, rect.height() - m_footerHeight, width, m_footerHeight));
  m_header->calculateGeometry(QRect(0, 0, width, m_headerHeight));

  m_availableRect.moveTop(m_headerHeight);
  m_availableRect.moveBottom(m_footerHeight);
  m_availableRect.setHeight(rect.height() - (m_headerHeight + m_footerHeight));
  m_availableRect.setWidth(rect.width());

  m_layout->setContentsMargins(
    0, m_headerHeight + 1, width, m_height - m_headerHeight - m_footerHeight);
}
