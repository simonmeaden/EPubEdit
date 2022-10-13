#include "docker/private/buttonwidgetprivate.h"
#include "docker/abstractdockwidget.h"
#include "utilities/MathUtilities.h"
#include "utilities/x11colors.h"

int ButtonWidgetPrivate::BUTTON_ROUND = 3;

ButtonWidgetPrivate::ButtonWidgetPrivate(AbstractDockWidget* parent,
                                         WidgetItem* qptr)
  : WidgetItemPrivate(parent, qptr)
  , m_textColor(QColorConstants::X11::LightGrey)
{
}

const QSize&
ButtonWidgetPrivate::iconSize()
{
  return m_iconSize;
}

void
ButtonWidgetPrivate::setIconSize(const QSize& newIconSize)
{
  m_iconSize = newIconSize;
  emit q_ptr->widgetChanged();
}

const QString&
ButtonWidgetPrivate::text()
{
  return m_text;
}

void
ButtonWidgetPrivate::setText(const QString& newText)
{
  m_text = newText;
  emit q_ptr->widgetChanged();
  //  m_parentWidget->repaint(m_rect);
}

int
ButtonWidgetPrivate::spacer()
{
  return m_spacer;
}

void
ButtonWidgetPrivate::setSpacer(int newSpacer)
{
  m_spacer = newSpacer;
  //  m_parentWidget->repaint(m_rect);
}

const QColor&
ButtonWidgetPrivate::textColor()
{
  return m_textColor;
}

void
ButtonWidgetPrivate::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

Arrangement
ButtonWidgetPrivate::arrangement()
{
  return m_arrangement;
}

void
ButtonWidgetPrivate::setArrangement(Arrangement newArrangement)
{
  m_arrangement = newArrangement;
  emit q_ptr->widgetChanged();
  //  m_parentWidget->repaint(m_rect);
}

WidgetItem*
ButtonWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new ButtonWidget(m_parentWidget);
  }

  auto button = qobject_cast<ButtonWidget*>(widget);
  if (button) {
    WidgetItemPrivate::clone(widget);
    button->setText(m_text);
    button->setTextColor(m_textColor);
    button->setPixmap(m_pixmap);
//    button->setIconSize(m_iconSize);
    button->setSpacer(m_spacer);
    button->setArrangement(m_arrangement);
  }
  return button;
}

const QSize
ButtonWidgetPrivate::calcMinimumSize()
{
  auto width = 0, height = 0, value = 0;
  auto iconWidth = 0, iconHeight = 0, textWidth = 0, textHeight = 0;

  if (!m_pixmap.isNull()) {
    auto size = m_pixmap.size();
    iconWidth = size.width();
    iconHeight = size.height();
  }

  if (!text().isEmpty()) {
    auto fm = m_parentWidget->fontMetrics();
    textWidth = fm.horizontalAdvance(text());
    textHeight = fm.height();
  }

  switch (arrangement()) {
    case TextAboveAndBelow:
      // TODO
      break;
    case TextAboveIcon:
    case TextBelowIcon: {
      value = std::max(iconWidth, textWidth);
      //      width = std::max(value, width);
      m_minContentSize.setWidth(value);
      value = iconHeight + spacer() + textHeight;
      m_minContentSize.setHeight(value);
      //      height = std::max(iconHeight, height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    }
    case TextToLeft:
    case TextToRight: {
      value = iconWidth + spacer() + textWidth;
      m_minContentSize.setWidth(value);
      //      width = std::max(value, width);
      value = std::max(iconHeight, textHeight);
      height = std::max(value, height);
      m_minContentSize.setHeight(height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    }
    case IconOnly: {
      width = std::max(iconWidth, width);
      m_minContentSize.setWidth(width);
      height = std::max(iconHeight, height);
      m_minContentSize.setHeight(height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      break;
    }
    case TextOnly:
      width = std::max(textWidth, width);
      m_minContentSize.setWidth(width);
      height = std::max(textHeight, height);
      m_minContentSize.setHeight(height);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    case NoArrangement:
      break;
  }
  m_minContentSize.rwidth() += m_margins.left();
  m_minContentSize.rwidth() += m_margins.right();
  m_minContentSize.rheight() += m_margins.top();
  m_minContentSize.rheight() += m_margins.bottom();
  return m_minContentSize;
}

void
ButtonWidgetPrivate::paint(QPainter& painter)
{
  painter.save();
  painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing,
                         true);
  paintBackground(painter);

  if (!m_pixmap.isNull()) {
    auto pixmap = QIcon(m_pixmap).pixmap(m_iconSize,
                                isEnabled() ? QIcon::Normal : QIcon::Disabled,
                                isSelected() ? QIcon::On : QIcon::Off);
    painter.drawPixmap(m_iconRect.topLeft(), pixmap);
  }

  if (m_showBorder) {
    painter.save();
    auto pen = painter.pen();
    pen.setColor(QColorConstants::X11::grey53);
    painter.setPen(pen);
    painter.drawRoundedRect(m_rect, BUTTON_ROUND, BUTTON_ROUND);
    painter.restore();
  }

  if (!m_text.isEmpty()) {
    auto pen = painter.pen();
    pen.setColor(m_textColor);
    painter.setPen(pen);
    painter.drawText(m_textRect.left(), m_textRect.bottom(), m_text);
  }

  painter.restore();
}

void
ButtonWidgetPrivate::setGeometry(const QRect& widgetRect,
                                 const QRect& contentsRect)
{
  WidgetItemPrivate::setGeometry(widgetRect);
  auto fm = m_parentWidget->fontMetrics();

  auto x = 0, y = 0, max = 0;

  switch (arrangement()) {
    case TextAboveAndBelow:
      // TODO
      break;
    case TextAboveIcon: {
      if (!m_text.isEmpty()) {
        x = Math::halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(),
                                 m_minContentSize.height());
        m_textRect.moveTop(contentsRect.y() + y);
        max = m_textRect.bottom() + spacer();
      }

      if (!m_pixmap.isNull()) {
        x = Math::halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        m_iconRect.moveTop(max);
      }

      break;
    }
    case TextBelowIcon: {
      if (!m_pixmap.isNull()) {
        x = Math::halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(),
                                 m_minContentSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
        max = m_iconRect.bottom() /*+ spacer()*/;
      }

      if (!m_text.isEmpty()) {
        x = Math::halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        m_textRect.moveTop(max);
      }
      break;
    }
    case TextToRight: {
      if (!m_pixmap.isNull()) {
        x =
          Math::halfDifference(contentsRect.width(), m_minContentSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
        max = m_iconRect.right() + spacer();
      }

      if (!m_text.isEmpty()) {
        m_textRect.moveLeft(max);
        y = Math::halfDifference(contentsRect.height(), fm.height());
        m_textRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case TextToLeft: {
      if (!m_text.isEmpty()) {
        x =
          Math::halfDifference(contentsRect.width(), m_minContentSize.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(), fm.height());
        m_textRect.moveTop(contentsRect.y() + y);
        max = m_textRect.right() + spacer();
      }

      if (!m_pixmap.isNull()) {
        m_iconRect.moveLeft(max);
        y = Math::halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case IconOnly: {
      if (!m_pixmap.isNull()) {
        x = Math::halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case TextOnly: {
      if (!m_text.isEmpty()) {
        x = Math::halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = Math::halfDifference(contentsRect.height(), m_textRect.height());
        m_textRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case NoArrangement:
      //      qWarning();
      break;
  }
}

const QRect&
ButtonWidgetPrivate::iconRect()
{
  return m_iconRect;
}

void
ButtonWidgetPrivate::setIconRect(const QRect& newIconRect)
{
  m_iconRect = newIconRect;
  m_parentWidget->repaint(m_rect);
}

const QRect&
ButtonWidgetPrivate::textRect()
{
  return m_textRect;
}

void
ButtonWidgetPrivate::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

bool
ButtonWidgetPrivate::isShowBorder() const
{
  return m_showBorder;
}

void
ButtonWidgetPrivate::setShowBorder(bool ShowBorder)
{
  m_showBorder = ShowBorder;
}
