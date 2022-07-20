#include "toolbarwidget.h"
#include "x11colors.h"

//====================================================================
//=== Toolbar
//====================================================================
ToolbarWidget::ToolbarWidget(WidgetPosition position, QWidget* parent)
  : BaseWidget(position, parent)
{
  setPreferredSize();
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

//void
//ToolbarWidget::resizeEvent(QResizeEvent* event)
//{
//  auto s = event->size();
//  auto top = 0;
//  // MARGIN allows a bit of spare space at the bottom
//  auto bottom = event->size().height() - MARGIN;
//  auto fm = this->fontMetrics();
//  QRect r(0, 0, s.width(), s.height());
//  m_frameRect = r;

//  for (auto& wrapper : m_widgets) {
//    auto iconHeight = 0;
//    auto iconWidth = 0;
//    auto iconLeft = 0;
//    auto textHeight = 0;
//    auto textWidth = 0;
//    auto height = 0;
//    auto textBottom = 0;
//    auto textLeft = 0;
//    auto iconTop = 0;
//    auto fm = fontMetrics();

//    switch (wrapper->type) {
//      case Button: {
//        auto bw = static_cast<ButtonWrapper*>(wrapper);
//        iconHeight = bw->iconSize.height();
//        iconWidth = bw->iconSize.width();
//        iconLeft = int((width() - iconWidth) / 2);
//        textHeight = fm.height();
//        textWidth = fm.horizontalAdvance(bw->text);
//        textLeft = int((width() - textWidth) / 2);
//        switch (bw->textPos) {
//          case LabelAlignment::TextAboveIcon: {
//            textBottom = m_margins.top() + textHeight;
//            iconTop = textBottom + m_spacer;
//            break;
//          }
//          case LabelAlignment::TextBelowIcon: {
//            iconTop = m_margins.top();
//            textBottom = iconTop + iconHeight + m_spacer + textHeight;
//            break;
//          }
//          default:
//            break;
//        }
//        height = m_margins.top() + iconHeight + spacer() + textHeight +
//                 m_margins.bottom();
//        break;
//      }
//      case ICON_BUTTON: {
//        auto iw = static_cast<ButtonWrapper*>(wrapper);
//        iconHeight = iw->iconSize.height();
//        break;
//      }
//      case TEXT_BUTTON: {
//        textHeight = fontMetrics().height();
//        break;
//      }
//      default: {
//        break;
//      }
//    }

//    //    if (bw->text.isEmpty()) { // Just an icon
//    //      bw->iconSize.width();
//    //      height = m_margins.top() + iconHeight + m_margins.bottom();

//    //      switch (bw->textPos) {
//    //        case WidgetWrapper::TEXTABOVE: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTBELOW: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTRIGHT: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTLEFT: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::NOTEXT: {
//    //          break;
//    //        }
//    //        default:
//    //          break;
//    //      }
//    //    } else {
//    //      switch (bw->textPos) {
//    //        case WidgetWrapper::TEXTABOVE: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTBELOW: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTRIGHT: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::TEXTLEFT: {
//    //          break;
//    //        }
//    //        case WidgetWrapper::NOTEXT: {
//    //          break;
//    //        }
//    //        default:
//    //          break;
//    //      }
//    //    }
//    //    switch (wrapper->type) {
//    //      case BUTTON: {
//    //        auto bw = static_cast<ButtonWrapper*>(wrapper);
//    //        bw->iconRect = QRect();
//    //        bw->textRect = QRect();

//    //        int iconHeight = 0;
//    //        int textHeight = 0;
//    //        int height = 0;
//    //        int width = 0;
//    //        int textBottom = 0;
//    //        int textLeft = 0;
//    //        int iconTop = 0;
//    //        int iconLeft = 0;

//    //        if (bw->text.isEmpty()) { // Just an icon
//    //          bw->iconSize.width();
//    //          height = m_margins.top() + iconHeight + m_margins.bottom();

//    //          switch (bw->textPos) {
//    //            case WidgetWrapper::TEXTABOVE: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTBELOW: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTRIGHT: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTLEFT: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::NOTEXT: {
//    //              break;
//    //            }
//    //            default:
//    //              break;
//    //          }
//    //        } else {
//    //          switch (bw->textPos) {
//    //            case WidgetWrapper::TEXTABOVE: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTBELOW: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTRIGHT: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::TEXTLEFT: {
//    //              break;
//    //            }
//    //            case WidgetWrapper::NOTEXT: {
//    //              break;
//    //            }
//    //            default:
//    //              break;
//    //          }
//    //        }
//    //        bw->rect.setHeight(m_margins.top() + iconHeight +
//    //        m_margins.bottom()); bw->iconRect.setHeight(iconHeight);

//    //        auto w = bw->iconSize.width();
//    //        auto x = bw->rect.x() + (bw->rect.width() - w) / 2;
//    //        bw->iconRect.setX(x);
//    //        bw->iconRect.setWidth(bw->iconSize.width());

//    //        if (bw->pos == WidgetWrapper::TOP) {
//    //          bw->iconRect.setY(top + m_margins.top());
//    //          top += iconHeight + MARGINS;
//    //        } else if (wrapper->pos == WidgetWrapper::BOTTOM) {
//    //          bw->iconRect.setY(bottom - iconHeight - MARGIN);
//    //          bottom -= (iconHeight + MARGINS);
//    //        }
//    //        bw->textRect = QRect();
//    //      }
//    //        else
//    //        { // Both icon and text
//    //          auto iconHeight = bw->iconSize.width();
//    //          auto textHeight = fm.height();
//    //          bw->rect.setHeight(iconHeight + MARGINS + SPACER + textHeight);
//    //          bw->rect.setWidth(WIDTH);

//    //          auto w = bw->iconSize.width();
//    //          auto x = r.x() + ((bw->rect.width() - w) / 2);
//    //          bw->iconRect.setX(x);
//    //          bw->iconRect.setWidth(bw->iconSize.width());

//    //          w = fm.horizontalAdvance(bw->text);
//    //          x = r.x() + ((bw->rect.width() - w) / 2);
//    //          bw->textRect.setX(x);
//    //          bw->textRect.setWidth(w);

//    //          switch (bw->pos) {
//    //            case WidgetWrapper::TOP: {
//    //              if (bw->textPos == WidgetWrapper::TEXTBELOW) {
//    //                wrapper->rect.setY(top);
//    //                bw->iconRect.setY(top + m_margins.top());
//    //                bw->textRect.setY(top + m_margins.left() + iconHeight +
//    //                                  spacer() + textHeight);
//    //                bw->rect.setHeight(m_margins.top() + textHeight + spacer()
//    //                +
//    //                                   iconHeight + m_margins.bottom());
//    //                bw->textRect.setHeight(textHeight);
//    //                bw->iconRect.setHeight(iconHeight);
//    //                top += bw->rect.height();
//    //              } else if (bw->textPos == WidgetWrapper::TEXTABOVE) {
//    //              }
//    //              break;
//    //            }
//    //            case WidgetWrapper::BOTTOM: {
//    //              bw->iconRect.setY(bottom - (m_margins.top() + textHeight +
//    //                                          spacer() + iconHeight));
//    //              bw->textRect.setY(bottom - MARGIN);
//    //              bottom -= bw->rect.height();
//    //              bw->rect.setY(bottom);
//    //              bw->rect.setHeight(MARGINS + textHeight + spacer() +
//    //              iconHeight); bw->textRect.setHeight(textHeight);
//    //              bw->iconRect.setHeight(iconHeight);
//    //              break;
//    //            }
//    //            default: {
//    //              break;
//    //            }
//    //          }
//    //        }
//    //        break;
//    //    }
//    //    case CUSTOM: {
//    //      break;
//    //    }
//    //    default: {
//    //      break;
//    //    }
//  }
//}

//QWidget::resizeEvent(event);
//}
