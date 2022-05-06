#include "toolbarwidget.h"
#include "x11colors.h"

//====================================================================
//=== Toolbar
//====================================================================
ToolbarWidget::ToolbarWidget(QWidget* parent)
  : BaseWidget(parent)
{
  setFixedWidth(WIDTH);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

ToolbarWidget::~ToolbarWidget()
{
  WidgetWrapper* l;
  while ((l = takeAt(0)))
    delete l;
}

void
ToolbarWidget::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  auto top = 0;
  // MARGIN allows a bit of spare space at the bottom
  auto bottom = event->size().height() - MARGIN;
  auto fm = this->fontMetrics();
  QRect r(0, 0, s.width(), s.height());
  m_frameRect = r;

  for (auto& wrapper : m_widgets) {
    switch (wrapper->type) {
      case WidgetWrapper::BUTTON: {
        auto bw = static_cast<ButtonWrapper*>(wrapper);
        bw->iconRect = QRect();
        bw->textRect = QRect();

        if (bw->text.isEmpty()) { // Just an icon
          auto iconHeight = bw->icon.height();
          bw->rect.setHeight(iconHeight + MARGINS);
          bw->iconRect.setHeight(iconHeight);

          auto w = bw->icon.width();
          auto x = bw->rect.x() + (bw->rect.width() - w) / 2;
          bw->iconRect.setX(x);
          bw->iconRect.setWidth(bw->icon.width());

          if (bw->pos == WidgetWrapper::TOP) {
            bw->iconRect.setY(top + MARGIN);
            top += iconHeight + MARGINS;
          } else if (wrapper->pos == WidgetWrapper::BOTTOM) {
            bw->iconRect.setY(bottom - iconHeight - MARGIN);
            bottom -= (iconHeight + MARGINS);
          }
          bw->textRect = QRect();
        } else { // Both icon and text
          auto iconHeight = bw->icon.height();
          auto textHeight = fm.height();
          bw->rect.setHeight(iconHeight + MARGINS + SPACER + textHeight);
          bw->rect.setWidth(WIDTH);

          auto w = bw->icon.width();
          auto x = r.x() + ((bw->rect.width() - w) / 2);
          bw->iconRect.setX(x);
          bw->iconRect.setWidth(bw->icon.width());

          w = fm.horizontalAdvance(bw->text);
          x = r.x() + ((bw->rect.width() - w) / 2);
          bw->textRect.setX(x);
          bw->textRect.setWidth(w);

          switch (bw->pos) {
            case WidgetWrapper::TOP: {
              wrapper->rect.setY(top);
              bw->iconRect.setY(top + MARGIN);
              bw->textRect.setY(top + MARGIN + iconHeight + SPACER +
                                textHeight);
              bw->rect.setHeight(MARGINS + textHeight + SPACER + iconHeight);
              bw->textRect.setHeight(textHeight);
              bw->iconRect.setHeight(iconHeight);
              top += bw->rect.height();
              break;
            }
            case WidgetWrapper::BOTTOM: {
              bw->iconRect.setY(bottom -
                                (MARGIN + textHeight + SPACER + iconHeight));
              bw->textRect.setY(bottom - MARGIN);
              bottom -= bw->rect.height();
              bw->rect.setY(bottom);
              bw->rect.setHeight(MARGINS + textHeight + SPACER + iconHeight);
              bw->textRect.setHeight(textHeight);
              bw->iconRect.setHeight(iconHeight);
              break;
            }
            default: {
              break;
            }
          }
        }
        break;
      }
      case WidgetWrapper::CUSTOM: {
        break;
      }
    }
  }

  QWidget::resizeEvent(event);
}

void
ToolbarWidget::hoverEnter(QHoverEvent* event)
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
ToolbarWidget::hoverLeave(QHoverEvent* /*event*/)
{
  for (auto& w : m_widgets) {
    w->hoverOver = false;
  }
  repaint();
}

void
ToolbarWidget::hoverMove(QHoverEvent* event)
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
ToolbarWidget::mousePressEvent(QMouseEvent* event)
{}

void
ToolbarWidget::mouseReleaseEvent(QMouseEvent* event)
{}

bool
ToolbarWidget::event(QEvent* event)
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
