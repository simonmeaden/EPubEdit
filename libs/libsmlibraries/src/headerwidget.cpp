#include "headerwidget.h"
#include "abstractdockwidget.h"
#include "dockitem.h"
#include "dockwidget.h"
#include "widgetitem.h"

HeaderWidget::HeaderWidget(QWidget* parent)
  : AbstractDockWidget{ parent }
{
  setContentsMargins(0, 0, 0, 0);

  auto f = font();
  f.setPointSize(8);
  setFont(f);

  m_layout = new QHBoxLayout;
  m_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(m_layout);

  m_header = new DockHeader(this);
}

DockItem*
HeaderWidget::header() const
{
  return m_header;
}

QWidget*
HeaderWidget::setCentralWidget(QWidget* centralWidget)
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

void
HeaderWidget::hideWidget()
{
  m_centralWidget->hide();
  m_layout->removeWidget(m_centralWidget);
  if (m_showHeaderOnHide) {
    m_hiddenSize = QSize(m_width, m_headerHeight);
    m_visibleSize = QSize(m_width, m_height);
  } else {
    m_hiddenSize = QSize(m_width, 0);
    m_visibleSize = QSize(m_width, m_height);
  }
  adjustSize();
  emit sizeChanged(this, m_hiddenSize);
}

void
HeaderWidget::showWidget()
{
  m_centralWidget->show();
  m_layout->addWidget(m_centralWidget);
  adjustSize();
  emit sizeChanged(this, m_visibleSize);
}

bool
HeaderWidget::isWidgetVisible()
{
  return m_centralWidget->isVisible();
}

void
HeaderWidget::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);

  if (m_header)
    m_header->paint(painter);
}

void
HeaderWidget::hoverEnterEvent(QHoverEvent* event)
{
  auto p = event->pos();

  if (m_header) {
    if (dockItemHoverCheck(m_header, p)) {
      return;
    }
  }
}

void
HeaderWidget::hoverLeaveEvent(QHoverEvent*)
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
HeaderWidget::hoverMoveEvent(QHoverEvent* event)
{
  auto p = /*mapToGlobal(*/ event->pos() /*)*/;

  if (m_header) {
    if (dockItemHoverCheck(m_header, p)) {
      return;
    }
  }
}

void
HeaderWidget::mousePressEvent(QMouseEvent* event)
{
  //  auto pos = mapToGlobal(event->pos());
  auto pos = event->pos();

  if (m_header) {
    mouseClickCheck(m_header, pos);
  }
  // TODO the rest of the widgets.
}

void
HeaderWidget::mouseReleaseEvent(QMouseEvent*)
{
  // TODO possibly remove if not used.
}

bool
HeaderWidget::event(QEvent* event)
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
HeaderWidget::resizeEvent(QResizeEvent* /*event*/)
{
  calculateGeometry(rect());
}

bool
HeaderWidget::showHeaderOnHide() const
{
  return m_showHeaderOnHide;
}

void
HeaderWidget::setShowHeaderOnHide(bool show)
{
  m_showHeaderOnHide = show;
}

void
HeaderWidget::calculateGeometry(const QRect& rect)
{
  auto northHeight = 0;
  auto northWidth = rect.size().width();
  m_width = northWidth;

  m_headerHeight = m_header->sizeHint().height();
  m_height = rect.height();
  northHeight += m_headerHeight;
  m_header->calculateGeometry(QRect(0, 0, northWidth, m_headerHeight));

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
