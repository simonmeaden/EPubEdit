#include "docker/private/headerwidgetprivate.h"
#include "docker/dockheader.h"
#include "docker/widgetitem.h"

HeaderWidgetPrivate::HeaderWidgetPrivate(HeaderWidget* parent)
  : AbstractDockWidgetPrivate(parent)
  , m_header(new DockHeader(parent))
{
}

DockHeader*
HeaderWidgetPrivate::header()
{
  if (m_header)
    return m_header;
  return nullptr;
}

//QWidget*
//HeaderWidgetPrivate::setWidget(QWidget* widget)
//{
//  if (widget == m_widget)
//    return widget;

//  auto oldWidget = m_widget;
//  if (oldWidget) {
//    m_layout->removeWidget(oldWidget);
//  }
//  m_widget = widget;
//  m_layout->addWidget(m_widget);
//  return oldWidget;
//}

void
HeaderWidgetPrivate::hideWidget()
{
  m_centralWidget->hide();
  m_layout->removeWidget(m_centralWidget);
  m_hiddenSize = QSize(m_width, 0);
  m_visibleSize = QSize(m_width, m_height);
}

void
HeaderWidgetPrivate::showWidget()
{
  m_centralWidget->show();
  m_layout->addWidget(m_centralWidget);
}

bool
HeaderWidgetPrivate::isWidgetVisible()
{
  return m_centralWidget->isVisible();
}

AbstractDockWidget*
HeaderWidgetPrivate::clone(AbstractDockWidget* widget)
{
  HeaderWidget* w = qobject_cast<HeaderWidget*>(widget);
  if (!w)
    w = new HeaderWidget(m_parent);
  AbstractDockWidgetPrivate::clone(w);
  m_header->clone(w->header());
  return w;
}

void
HeaderWidgetPrivate::widgetWasClicked(QPoint)
{
  checkForOpenListWidgets(m_header);
}

void
HeaderWidgetPrivate::paint(QPainter& painter)
{
  if (m_header)
    m_header->paint(painter);
}

void
HeaderWidgetPrivate::hoverEnter(QPoint pos)
{
  if (m_header) {
    if (dockItemHoverCheck(m_header, pos)) {
      q_ptr->repaint();
      return;
    }
  }
}

void
HeaderWidgetPrivate::hoverLeave()
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
HeaderWidgetPrivate::hoverMove(QPoint pos)
{
  if (m_header) {
    if (dockItemHoverCheck(m_header, pos)) {
      q_ptr->repaint();
      return;
    }
  }
}

void
HeaderWidgetPrivate::mousePress(QMouseEvent *event)
{
  if (m_header) {
    mousePressCheck(m_header, event);
  }
}

void HeaderWidgetPrivate::mouseMove(QMouseEvent *event)
{
  if (m_header) {
    mouseMoveCheck(m_header, event);
  }
}

void HeaderWidgetPrivate::mouseRelease(QMouseEvent *event)
{
  if (m_header) {
    mouseReleaseCheck(m_header, event);
  }
}

void
HeaderWidgetPrivate::calculateGeometry(const QRect& rect)
{
  auto northHeight = 0;
  auto northWidth = rect.size().width();
  m_width = northWidth;

  m_headerHeight = m_header->sizeHint().height();
  m_height = rect.height();
  northHeight += m_headerHeight;
  m_header->calculateGeometry(QRect(0, 0, northWidth, m_headerHeight));

  m_availableRect.setHeight(rect.height() - m_headerHeight);
  m_availableRect.setWidth(rect.width());
  m_availableRect.moveTop(m_headerHeight);

  m_layout->setContentsMargins(0, northHeight + 1, 0, 0);
}
