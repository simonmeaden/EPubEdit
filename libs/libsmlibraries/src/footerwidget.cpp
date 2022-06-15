#include "footerwidget.h"
#include "abstractdockwidget.h"
#include "dockitem.h"
#include "dockwidget.h"
#include "widgetitem.h"


FooterWidget::FooterWidget(QWidget *parent)
  : AbstractDockWidget{parent}
{
  setContentsMargins(0, 0, 0, 0);

  auto f = font();
  f.setPointSize(8);
  setFont(f);

  m_layout = new QHBoxLayout;
  m_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_layout);

  m_footer = new DockFooter(this);
}

DockItem *FooterWidget::footer() const
{
  return m_footer;
}

QWidget *FooterWidget::setCentralWidget(QWidget *centralWidget)
{
  if (centralWidget == m_centralWidget)
    return centralWidget;

  auto oldWidget = m_centralWidget;
  if (oldWidget) {
    m_layout->removeWidget(oldWidget);
  }
  m_centralWidget = centralWidget;
  m_layout->addWidget(m_centralWidget);
  return oldWidget;
}

void FooterWidget::hideWidget()
{
  m_layout->removeWidget(m_centralWidget);
  if (m_showFooterOnHide) {
    m_hiddenSize = QSize(m_width, m_footerHeight);
    m_visibleSize = QSize(m_width, m_height);
  } else {
    m_hiddenSize = QSize(m_width, 0);
    m_visibleSize = QSize(m_width, m_height);
  }
  adjustSize();
  emit sizeChanged(this, m_hiddenSize);
}

void FooterWidget::showWidget()
{
  m_centralWidget->show();
  m_layout->addWidget(m_centralWidget);
  adjustSize();
  emit sizeChanged(this, m_visibleSize);
}

bool FooterWidget::isWidgetVisible()
{
  return m_centralWidget->isVisible();
}

bool FooterWidget::showFooterOnHide() const
{
  return m_showFooterOnHide;
}

void FooterWidget::setShowFooterOnHide(bool show)
{
  m_showFooterOnHide = show;
}

void FooterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  if (m_footer)
    m_footer->paint(painter);
}

void
FooterWidget::hoverEnterEvent(QHoverEvent* event)
{
  auto p =  event->pos();

  if (m_footer) {
    if (dockItemHoverCheck(m_footer, p)) {
      return;
    }
  }
}

void
FooterWidget::hoverLeaveEvent(QHoverEvent*)
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
FooterWidget::hoverMoveEvent(QHoverEvent* event)
{
  auto p = event->pos();

  if (m_footer) {
    if (dockItemHoverCheck(m_footer, p)) {
      return;
    }
  }
}

void
FooterWidget::mousePressEvent(QMouseEvent* event)
{
  auto pos = event->pos();

  if (m_footer) {
    mouseClickCheck(m_footer, pos);
  }
}

void
FooterWidget::mouseReleaseEvent(QMouseEvent*)
{
  // TODO possibly remove if not used.
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
FooterWidget::calculateGeometry(const QRect& rect)
{
  auto northHeight = 0;
  auto northWidth = rect.size().width();
  m_width = northWidth;

  m_footerHeight = m_footer->sizeHint().height();
  m_height = rect.height();
  northHeight += m_footerHeight;
  m_footer->calculateGeometry(QRect(0, rect.height() - m_footerHeight, northWidth, m_footerHeight));

  if (m_centralWidget) {
    if (m_centralWidget->isVisible()) {
      m_layout->setContentsMargins(0, northHeight + 1, 0, 0);
    } else {
      m_layout->setContentsMargins(0, northHeight + 1, 0, 0);
    }
  } else {
    m_layout->setContentsMargins(0, northHeight + 1, 0, 0);
  }
}
