#include "forms/headerwidget.h"
#include "x11colors.h"

//====================================================================
//=== HeaderWidget
//====================================================================
HeaderWidget::HeaderWidget(QWidget* parent)
  : BaseWidget(Top, parent)
{
  setPreferredSize();
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);
}

//void
//HeaderWidget::resizeEvent(QResizeEvent* event)
//{
//  auto s = event->size();
//  // MARGIN allows a bit of spare space at the left and right
//  auto left = MARGIN;
//  auto right = event->size().width() - MARGINS;
//  //  auto fm = this->fontMetrics();
//  QRect r(0, 0, s.width(), s.height());
//  m_frameRect = r;

//  for (auto& wrapper : m_widgets) {
//    switch (wrapper->type) {
//      case IconButton: {
//        auto bw = static_cast<IconButtonWrapper*>(wrapper);
//        bw->iconRect = QRect();
//        auto iconWidth = bw->iconSize.width();

//        if (bw->pos == WidgetWrapper::LEFT) {
//          bw->rect.setY(0);
//          bw->rect.setX(left);
//          bw->rect.setHeight(r.height());
//          bw->rect.setWidth(MARGINS + iconWidth);
//          bw->iconRect.setX(bw->rect.x() + MARGIN);
//          bw->iconRect.setY(MARGIN);
//          bw->iconRect.setWidth(iconWidth);
//          bw->iconRect.setHeight(bw->iconSize.height());
//          left += iconWidth + MARGINS;
//        } else if (wrapper->pos == WidgetWrapper::RIGHT) {
//          bw->rect.setY(0);
//          bw->rect.setX(right - (MARGINS + iconWidth));
//          bw->rect.setHeight(r.height());
//          bw->rect.setWidth(MARGINS + iconWidth);
//          bw->iconRect.setX(bw->rect.x() + MARGIN);
//          bw->iconRect.setY(MARGIN);
//          bw->iconRect.setWidth(iconWidth);
//          bw->iconRect.setHeight(bw->iconSize.height());
//          right -= bw->rect.width();
//        }

//        break;
//      }
//      case CUSTOM: {
//        break;
//      }
//      default:
//        break;
//    }
//  }

  QWidget::resizeEvent(event);
}

void
HeaderWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  QPainter painter(this);

  painter.fillRect(m_frameRect, m_backColor);

  for (auto& w : m_widgets) {
    switch (w->type) {
      case IconButton::IconButton: {
        paintBackground(painter, w);
        paintBtnIcon(painter, w);
      }
      default: {
      }
    }
  }
}
