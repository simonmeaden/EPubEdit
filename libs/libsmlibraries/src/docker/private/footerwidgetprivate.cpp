#include "docker/private/footerwidgetprivate.h"
#include "docker/dockfooter.h"
#include "docker/widgetitem.h"

FooterWidgetPrivate::FooterWidgetPrivate(FooterWidget* parent)
  : AbstractDockWidgetPrivate(parent)

{
  m_footer = new DockFooter( q_ptr);
}

DockFooter*
FooterWidgetPrivate::footer()
{
  if (m_footer)
    return m_footer;
  return nullptr;
}

//QWidget*
//FooterWidgetPrivate::setWidget(QWidget* widget)
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
FooterWidgetPrivate::hideWidget()
{
  m_centralWidget->hide();
  m_layout->removeWidget(m_centralWidget);
  m_hiddenSize = QSize(m_width, 0);
  m_visibleSize = QSize(m_width, m_height);
}

void
FooterWidgetPrivate::showWidget()
{
  m_centralWidget->show();
  m_layout->addWidget(m_centralWidget);
}

bool
FooterWidgetPrivate::isWidgetVisible()
{
  return m_centralWidget->isVisible();
}

void
FooterWidgetPrivate::paint(QPainter& painter)
{
  if (m_footer)
    m_footer->paint(painter);
}

void
FooterWidgetPrivate::hoverEnter(QPoint pos)
{
  if (m_footer) {
    if (dockItemHoverCheck(m_footer, pos)) {
      return;
    }
  }
}

void
FooterWidgetPrivate::hoverLeave()
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
FooterWidgetPrivate::hoverMove(QPoint pos)
{
  if (m_footer) {
    if (dockItemHoverCheck(m_footer, pos)) {
      return;
    }
  }
}

void
FooterWidgetPrivate::mousePress(QMouseEvent *event)
{
  if (m_footer) {
    mousePressCheck(m_footer, event);
  }
}

void FooterWidgetPrivate::mouseRelease(QMouseEvent *event)
{
  if (m_footer) {
    mouseReleaseCheck(m_footer, event);
  }
}

void FooterWidgetPrivate::mouseMove(QMouseEvent *event)
{
  if (m_footer) {
    mouseMoveCheck(m_footer, event);
  }
}

void
FooterWidgetPrivate::widgetWasClicked(QPoint)
{
  checkForOpenListWidgets(m_footer);
}

AbstractDockWidget *FooterWidgetPrivate::clone(AbstractDockWidget *widget)
{
  FooterWidget* w = qobject_cast<FooterWidget*>(widget);
  if (!w)
    w = new FooterWidget(m_parent);
  AbstractDockWidgetPrivate::clone(w);
  m_footer->clone(w->footer());
  return w;
}

void
FooterWidgetPrivate::calculateGeometry(const QRect& rect)
{
  auto southHeight = 0;
  auto southWidth = rect.size().width();
  m_width = southWidth;

  m_footerHeight = m_footer->sizeHint().height();
  m_height = rect.height();
  southHeight += m_footerHeight;
  m_footer->calculateGeometry(
    QRect(0, rect.height() - m_footerHeight, southWidth, m_footerHeight));

  m_availableRect.moveBottom(m_footerHeight);
  m_availableRect.setHeight(rect.height() - m_footerHeight);
  m_availableRect.setWidth(rect.width());

  m_layout->setContentsMargins(0, southHeight + 1, 0, 0);
}
