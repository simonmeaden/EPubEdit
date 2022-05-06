#include "toolbar.h"
#include "x11colors.h"

Toolbar::Toolbar(QWidget* parent)
  : QWidget{ parent }
  , m_back(QColorConstants::X11::DimGrey)
  , m_hoverBack(QColorConstants::X11::DarkGrey)
{
  setFixedWidth(WIDTH);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
}

void
Toolbar::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(m_frameRect, m_back);
  //  if (!m_hoverOverButton) {
  //    painter.fillRect(m_buttonRect, m_back);
  //  } else {
  //    painter.fillRect(m_buttonRect, m_hoverBack);
  //  }

  //  QImage img(":/icons/hideframe.png");
  //  painter.drawImage(m_buttonRect.x() + 3, 3, img);
}

void
Toolbar::resizeEvent(QResizeEvent* event)
{
  auto w = qobject_cast<QWidget*>(parent());
  auto ws = w->size();
  m_frameRect = QRect(0, 0, WIDTH, ws.height());

  QWidget::resizeEvent(event);
}

void
Toolbar::hoverEnter(QHoverEvent* event)
{}

void
Toolbar::hoverLeave(QHoverEvent* event)
{}

void
Toolbar::hoverMove(QHoverEvent* event)
{}

void
Toolbar::mousePressEvent(QMouseEvent* event)
{}

void
Toolbar::mouseReleaseEvent(QMouseEvent* event)
{}

bool
Toolbar::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnter(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverLeave:
      hoverLeave(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(event));
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}
