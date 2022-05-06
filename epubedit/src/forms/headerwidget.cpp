#include "forms/headerwidget.h"
#include "x11colors.h"

//====================================================================
//=== HeaderWidget
//====================================================================
HeaderWidget::HeaderWidget(QWidget* parent)
  : BaseWidget(parent)
{
  setFixedHeight(HEIGHT);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
}

void
HeaderWidget::resizeEvent(QResizeEvent* event)
{
  auto s = event->size();
  auto left = 0;
  // MARGIN allows a bit of spare space at the bottom
  auto right = event->size().width() - MARGIN;
//  auto fm = this->fontMetrics();
  QRect r(0, 0, s.width(), s.height());
  m_frameRect = r;

  for (auto& wrapper : m_widgets) {
    switch (wrapper->type) {
      case WidgetWrapper::BUTTON: {
        auto bw = static_cast<ButtonWrapper*>(wrapper);
        bw->iconRect = QRect();
        bw->textRect = QRect();

        if (bw->text.isEmpty()) { // Just an icon
          auto iconWidth = bw->icon.width();
          bw->rect.setWidth(iconWidth + MARGINS);
          bw->iconRect.setWidth(iconWidth);

          auto h = bw->icon.height();
          auto y = bw->rect.y() + (bw->rect.height() - h) / 2;
          bw->iconRect.setY(y);
          bw->iconRect.setHeight(bw->icon.height());

          if (bw->pos == WidgetWrapper::LEFT) {
            bw->iconRect.setX(left + MARGIN);
            left += iconWidth + MARGINS;
          } else if (wrapper->pos == WidgetWrapper::RIGHT) {
            bw->iconRect.setX(right - iconWidth - MARGIN);
            right -= (iconWidth + MARGINS);
          }
          bw->textRect = QRect();
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
