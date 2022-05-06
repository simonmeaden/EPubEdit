#include "basewidget.h"
#include "x11colors.h"

BaseWidget::BaseWidget(QWidget *parent)
  : QWidget{parent}
  , m_backColor(QColorConstants::X11::DimGrey)
  , m_hoverBackColor(QColorConstants::X11::DarkGrey)
  , m_textColor(QColorConstants::X11::WhiteSmoke)
{

}

BaseWidget::~BaseWidget()
{
  WidgetWrapper* l;
  while ((l = takeAt(0)))
    delete l;
}

int
BaseWidget::addWidget(WidgetWrapper::Type type,
                         WidgetWrapper::Position pos,
                         QImage icon)
{
  WidgetWrapper* wrapper = createWidgetWrapper(type, pos, icon, QString());
  return m_widgets.indexOf(wrapper);
}

int
BaseWidget::addWidget(WidgetWrapper::Type type,
                         WidgetWrapper::Position pos,
                         QImage icon,
                         const QString& text)
{
  WidgetWrapper* wrapper = createWidgetWrapper(type, pos, icon, text);
  return m_widgets.indexOf(wrapper);
}

int
BaseWidget::addWidget(WidgetWrapper::Type type,
                         WidgetWrapper::Position pos,
                         const QString& text)
{
  WidgetWrapper* wrapper = createWidgetWrapper(type, pos, QImage(), text);
  return m_widgets.indexOf(wrapper);
}
void
BaseWidget::setTooltip(int index, const QString& tooltip)
{
  m_widgets.at(index)->tooltip = tooltip;
}

WidgetWrapper*
BaseWidget::createWidgetWrapper(WidgetWrapper::Type type,
                                   WidgetWrapper::Position pos,
                                   QImage icon,
                                   const QString& text)
{
  WidgetWrapper* wrapper;

  switch (type) {
    case WidgetWrapper::BUTTON: {
      auto buttonWrapper = new ButtonWrapper;
      buttonWrapper->icon = icon;
      buttonWrapper->text = text;
      wrapper = buttonWrapper;
      break;
    }
    case WidgetWrapper::CUSTOM: {
      // Probably never happen.
      wrapper = new WidgetWrapper;
    }
  }

  wrapper->type = type;
  wrapper->pos = pos;
  wrapper->tooltip = QString();
  m_widgets.append(wrapper);

  return wrapper;
}

WidgetWrapper*
BaseWidget::takeAt(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.takeAt(index);
    return layoutStruct;
  }
  return nullptr;
}

WidgetWrapper*
BaseWidget::at(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetWrapper* layoutStruct = m_widgets.at(index);
    return layoutStruct;
  }
  return nullptr;
}

void
BaseWidget::paintBackground(QPainter& painter, WidgetWrapper* wrapper)
{
  if (wrapper->hoverOver) {
    painter.fillRect(wrapper->rect, m_hoverBackColor);
  } else {
    painter.fillRect(wrapper->rect, m_backColor);
  }
}

void
BaseWidget::paintBtnIcon(QPainter& painter, ButtonWrapper* bw)
{
  if (!bw->icon.isNull()) {
    if (bw->text.isEmpty()) {
      auto y =
        bw->iconRect.y() + ((bw->iconRect.height() - bw->icon.height()) / 2);
      painter.drawImage(bw->iconRect.x(), y, bw->icon);
    } else {
      auto y =
        bw->iconRect.y() + ((bw->iconRect.height() - bw->icon.height()) / 2);
      painter.drawImage(bw->iconRect.x(), y, bw->icon);
    }
  }
}

void
BaseWidget::paintBtnText(QPainter& painter, ButtonWrapper* bw)
{
  if (!bw->text.isEmpty()) {
    auto pen = painter.pen();
    pen.setColor(m_textColor);
    painter.setPen(pen);
    if (bw->text.isNull()) {
      painter.drawText(bw->textRect.x(), bw->textRect.y(), bw->text);
    } else {
      painter.drawText(bw->textRect.x(), bw->textRect.y(), bw->text);
    }
  }
}

void
BaseWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(m_frameRect, m_backColor);

  for (auto& w : m_widgets) {
    switch (w->type) {
      case WidgetWrapper::Type::BUTTON: {
        paintBackground(painter, w);
        paintBtnIcon(painter, static_cast<ButtonWrapper*>(w));
        paintBtnText(painter, static_cast<ButtonWrapper*>(w));
      }
      default: {
      }
    }
  }
}

void
BaseWidget::hoverEnter(QHoverEvent* event)
{
  for (auto& w : m_widgets) {
    if (w->rect.contains(event->pos())) {
      w->hoverOver = true;
      repaint();
    } else {
      w->hoverOver = false;
      repaint();
    }
  }
}

void
BaseWidget::hoverLeave(QHoverEvent* /*event*/)
{
  for (auto& w : m_widgets) {
    w->hoverOver = false;
  }
  repaint();
}

void
BaseWidget::hoverMove(QHoverEvent* event)
{
  for (auto& w : m_widgets) {
    if (w->rect.contains(event->pos())) {
      w->hoverOver = true;
      repaint();
    } else {
      w->hoverOver = false;
      repaint();
    }
  }
}

void
BaseWidget::mousePressEvent(QMouseEvent* event)
{
  for (auto& w : m_widgets) {
    if (w->rect.contains(event->pos())) {
      // TODO handle button click
    }
  }
  repaint();
}

void
BaseWidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{}

bool
BaseWidget::event(QEvent* event)
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
