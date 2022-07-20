#include "widgetwrapper.h"
#include "basewidget.h"
#include "x11colors.h"

//====================================================================
//=== WidgetWrapper
//====================================================================
WidgetWrapper::WidgetWrapper(QObject* parent)
  : QObject(parent)
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_margins(QMargins(LEFTMARGIN, TOPMARGIN, RIGHTMARGIN, BOTTOMMARGIN))
{}

bool
WidgetWrapper::isEnabled() const
{
  return m_enabled;
}

void
WidgetWrapper::setEnabled(bool newEnabled)
{
  m_enabled = newEnabled;
}

const QRect&
WidgetWrapper::rect() const
{
  return m_rect;
}

void
WidgetWrapper::setRect(const QRect& newRect)
{
  m_rect = newRect;
}

WidgetType
WidgetWrapper::type() const
{
  return m_type;
}

void
WidgetWrapper::setType(WidgetType newType)
{
  m_type = newType;
}

WidgetPosition
WidgetWrapper::position() const
{
  return m_position;
}

void
WidgetWrapper::setPosition(WidgetPosition newPosition)
{
  m_position = newPosition;
  emit widgetChanged();
}

Arrangement
WidgetWrapper::arrangement() const
{
  return m_arrangement;
}

void
WidgetWrapper::setArrangement(Arrangement newArrangement)
{
  m_arrangement = newArrangement;
  emit widgetChanged();
}

const QString&
WidgetWrapper::tooltip() const
{
  return m_tooltip;
}

void
WidgetWrapper::setTooltip(const QString& newTooltip)
{
  m_tooltip = newTooltip;
}

bool
WidgetWrapper::isHoverOver() const
{
  return m_hoverOver;
}

void
WidgetWrapper::setHoverOver(bool newHoverOver)
{
  m_hoverOver = newHoverOver;
}

void
WidgetWrapper::paintBackground(QPainter& painter)
{
  painter.save();
  if (isHoverOver()) {
    painter.fillRect(m_rect, m_hoverBackColor);
  } else if (isSelected()) {
    painter.fillRect(m_rect, m_selectedColor);
  } else {
    painter.fillRect(m_rect, m_backColor);
  }
  painter.restore();
}

const QBrush&
WidgetWrapper::backColor() const
{
  return m_backColor;
}

void
WidgetWrapper::setBackColor(const QBrush& newBackColor)
{
  m_backColor = newBackColor;
}

const QBrush&
WidgetWrapper::hoverBackColor() const
{
  return m_hoverBackColor;
}

void
WidgetWrapper::setHoverBackColor(const QBrush& newHoverBackColor)
{
  m_hoverBackColor = newHoverBackColor;
}

const QBrush&
WidgetWrapper::selectedColor() const
{
  return m_selectedColor;
}

void
WidgetWrapper::setSelectedColor(const QBrush& newSelectedColor)
{
  m_selectedColor = newSelectedColor;
}

const QSize&
WidgetWrapper::content() const
{
  return m_content;
}

int
WidgetWrapper::halfDifference(int large, int small)
{
  return int((large - small) / 2);
}

bool
WidgetWrapper::isSelected() const
{
  return m_selected;
}

void
WidgetWrapper::setSelected(bool newSelected)
{
  m_selected = newSelected;
}

const QMargins&
WidgetWrapper::margins() const
{
  return m_margins;
}

void
WidgetWrapper::setMargins(const QMargins& newMargins)
{
  m_margins = newMargins;
  emit widgetChanged();
}

int
WidgetWrapper::leftMargin()
{
  return m_margins.left();
}

int
WidgetWrapper::rightMargin()
{
  return m_margins.right();
}

int
WidgetWrapper::topMargin()
{
  return m_margins.top();
}

int
WidgetWrapper::bottomMargin()
{
  return m_margins.bottom();
}

void
WidgetWrapper::setLeftMargin(int margin)
{
  m_margins.setLeft(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setRightMargin(int margin)
{
  m_margins.setRight(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setTopMargin(int margin)
{
  m_margins.setTop(margin);
  emit widgetChanged();
}

void
WidgetWrapper::setBottomMargin(int margin)
{
  m_margins.setBottom(margin);
  emit widgetChanged();
}

//====================================================================
//=== ButtonWrapper
//====================================================================
ButtonWrapper::ButtonWrapper(QObject* parent)
  : WidgetWrapper(parent)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

const QRect&
ButtonWrapper::iconRect() const
{
  return m_iconRect;
}

void
ButtonWrapper::setIconRect(const QRect& newIconRect)
{
  m_iconRect = newIconRect;
}

const QIcon&
ButtonWrapper::icon() const
{
  return m_icon;
}

void
ButtonWrapper::setIcon(const QIcon& newIcon)
{
  m_icon = newIcon;
}

const QSize&
ButtonWrapper::iconSize() const
{
  return m_iconSize;
}

void
ButtonWrapper::setIconSize(const QSize& newIconSize)
{
  m_iconSize = newIconSize;
  emit widgetChanged();
}

int
ButtonWrapper::spacer() const
{
  return m_spacer;
}

void
ButtonWrapper::setSpacer(int newSpacer)
{
  m_spacer = newSpacer;
}

const QSize
ButtonWrapper::calcSize()
{
  int width = 0, height = 0, value = 0;
  int iconWidth = 0, iconHeight = 0, textWidth = 0, textHeight = 0;
  auto fm = qobject_cast<BaseWidget*>(parent())->fontMetrics();

  if (!icon().isNull()) {
    iconWidth = iconSize().width();
    iconHeight = iconSize().height();
  }

  if (!text().isEmpty()) {
    textWidth = fm.horizontalAdvance(text());
    textHeight = fm.height();
  }

  switch (arrangement()) {
    case TextAboveIcon:
    case TextBelowIcon: {
      value = (iconWidth > textWidth ? iconWidth : textWidth);
      width = (value > width ? value : width);
      m_content.setWidth(value);
      value = iconHeight + spacer() + textHeight;
      m_content.setHeight(value);
      height = (value > height ? value : height);
      break;
    }
    case TextToLeft:
    case TextToRight: {
      value = iconWidth + spacer() + textWidth;
      m_content.setWidth(value);
      width = (value > width ? value : width);
      value = (iconHeight > textHeight ? iconHeight : textHeight);
      height = (value > height ? value : height);
      m_content.setHeight(height);
      break;
    }
    case IconOnly: {
      width = (iconWidth > width ? iconWidth : width);
      m_content.setWidth(width);
      height = (iconHeight > height ? iconHeight : height);
      m_content.setHeight(height);
      break;
    }
    case TextOnly:
      width = (textWidth > width ? textWidth : width);
      m_content.setWidth(width);
      height = (textHeight > height ? textHeight : height);
      m_content.setHeight(height);
      break;
    case NoArrangement:
      break;
  }
  return QSize(width, height);
}

void
ButtonWrapper::paintWidget(QPainter& painter)
{
  painter.save();
  paintBackground(painter);

  if (!m_icon.isNull()) {
    auto pixmap = icon().pixmap(m_iconSize,
                                isEnabled() ? QIcon::Normal : QIcon::Disabled,
                                isSelected() ? QIcon::On : QIcon::Off);
    painter.drawPixmap(m_iconRect.topLeft(), pixmap);
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
ButtonWrapper::resizeEvent(QRect& r, int& min, int& max)
{
  auto p = qobject_cast<BaseWidget*>(parent());
  auto widgetPosition = p->widgetPosition();
  auto iconHeight = 0;
  auto iconWidth = 0;
  auto iconLeft = 0;
  auto iconTop = 0;
  auto textHeight = 0;
  auto textWidth = 0;
  auto textLeft = 0;
  auto textTop = 0;
  auto fm = p->fontMetrics();

  iconHeight = m_iconSize.height();
  iconWidth = m_iconSize.width();
  textHeight = fm.height();
  textWidth = fm.horizontalAdvance(m_text);

  switch (arrangement()) {
    case TextBelowIcon: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          if (position() == Top) {
            r.setTop(min);
            iconTop = min + halfDifference(p->maxHeight(), m_content.height());
            min += p->maxHeight();
          } else if (position() == Bottom) {
            max -= p->maxHeight();
            r.setTop(max);
            iconTop = max + halfDifference(p->maxHeight(), m_content.height());
          }
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          textTop = iconTop + iconHeight + spacer();
          textLeft = halfDifference(p->maxWidth(), textWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case Top:
        case Bottom: {
          if (position() == Left) {
            r.setLeft(min);
            iconLeft = min + halfDifference(p->maxWidth(), m_content.width());
            textLeft = min + halfDifference(p->maxWidth(), textWidth);
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + halfDifference(p->maxWidth(), m_content.width());
            textLeft = max + halfDifference(p->maxWidth(), textWidth);
          }
          iconTop = halfDifference(p->maxHeight(), m_content.height());
          textTop = iconTop + iconHeight + spacer();
          min += p->maxWidth();
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextAboveIcon: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          if (position() == Top) {
            QRect().setLeft(min);
            textLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (position() == Bottom) {
            max -= p->maxWidth();
            r.setLeft(max);
            textLeft = max + leftMargin();
          }
          textTop = halfDifference(p->maxHeight(), textHeight);
          iconTop = textTop + textHeight + spacer();
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case Top:
        case Bottom: {
          if (position() == Left) {
            QRect().setLeft(min);
            textLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
            textLeft = max + leftMargin();
          }
          textTop = halfDifference(p->maxHeight(), textHeight);
          iconTop = textTop + textHeight + spacer();
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextToRight: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          if (position() == Top) {
            r.setTop(min);
            iconLeft = min + halfDifference(p->maxHeight(), iconHeight);
            min += p->maxWidth();
          } else if (position() == Bottom) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + halfDifference(p->maxHeight(), iconHeight);
          }
          iconTop = max + halfDifference(p->maxHeight(), iconHeight);
          textLeft = iconLeft + iconWidth + spacer();
          textTop = halfDifference(p->maxHeight(), textHeight);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case Top:
        case Bottom: {
          if (position() == Left) {
            r.setLeft(min);
            iconLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + leftMargin();
          }
          iconTop = halfDifference(p->maxHeight(), iconHeight);
          textLeft = iconLeft + iconWidth + spacer();
          textTop = halfDifference(p->maxHeight(), textHeight);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case TextToLeft: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          if (position() == Top || position() == Bottom) {
            iconLeft = min + leftMargin();
            iconTop = halfDifference(p->maxHeight(), iconHeight);
            textLeft = iconLeft + iconWidth + spacer();
            textTop = halfDifference(p->maxHeight(), textHeight);
            setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
            setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
            if (widgetPosition == Left || widgetPosition == Top) {
              r.setTop(min);
              min += p->maxHeight();
            } else {
              r.setTop(max);
              max -= p->maxWidth();
            }
          } else {
            iconLeft = leftMargin();
            iconTop = min + halfDifference(p->maxHeight(), iconHeight);
            textLeft = iconLeft + iconWidth + spacer();
            textTop = min + halfDifference(p->maxHeight(), textHeight);
            setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
            setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
            if (widgetPosition == Left || widgetPosition == Top) {
              r.setTop(min);
              min += p->maxHeight();
            } else {
              r.setTop(max);
              max -= p->maxWidth();
            }
          }
          break;
        }
        case Top:
        case Bottom: {
          if (position() == Left) {
            r.setLeft(min);
            textLeft = min + leftMargin();
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
            textLeft = max + leftMargin();
          }
          textTop = halfDifference(p->maxHeight(), textHeight);
          iconTop = halfDifference(p->maxHeight(), iconHeight);
          iconLeft = textLeft + textWidth + spacer();
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    case IconOnly: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          if (position() == Top) {
            r.setTop(min);
            iconTop = min + halfDifference(p->maxHeight(), iconHeight);
            min += p->maxHeight();
          } else if (position() == Bottom) {
            max -= p->maxHeight();
            r.setTop(max);
            iconTop = max + halfDifference(p->maxHeight(), iconHeight);
          }
          iconLeft = halfDifference(p->maxWidth(), iconWidth);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          break;
        }
        case Top:
        case Bottom: {
          if (position() == Left) {
            r.setLeft(min);
            iconLeft = min + halfDifference(p->maxWidth(), iconWidth);
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
            iconLeft = max + halfDifference(p->maxWidth(), iconWidth);
          }
          iconTop = halfDifference(p->maxHeight(), iconHeight);
          setIconRect(QRect(iconLeft, iconTop, iconWidth, iconHeight));
          break;
        }
      }
      break;
    }
    case TextOnly: {
      switch (widgetPosition) {
        case Left:
        case Right: {
          textLeft = halfDifference(p->maxWidth(), textWidth);
          if (position() == Top) {
            textTop = min + halfDifference(p->maxHeight(), textHeight);
            r.setTop(min);
            min += p->maxWidth();
          } else if (position() == Bottom) {
            max -= p->maxWidth();
            textTop = max + halfDifference(p->maxHeight(), textHeight);
            r.setTop(max);
          }
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
        case Top:
        case Bottom: {
          textTop = halfDifference(p->maxHeight(), textHeight);
          textLeft = min + halfDifference(p->maxWidth(), textWidth);
          if (position() == Left) {
            r.setLeft(min);
            min += p->maxWidth();
          } else if (position() == Right) {
            max -= p->maxWidth();
            r.setLeft(max);
          }
          setTextRect(QRect(textLeft, textTop, textWidth, textHeight));
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  r.setWidth(p->maxWidth());
  r.setHeight(p->maxHeight());
}

const QColor&
ButtonWrapper::textColor() const
{
  return m_textColor;
}

void
ButtonWrapper::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QString&
ButtonWrapper::text() const
{
  return m_text;
}

void
ButtonWrapper::setText(const QString& newText)
{
  m_text = newText;
  emit widgetChanged();
}

const QRect&
ButtonWrapper::textRect() const
{
  return m_textRect;
}

void
ButtonWrapper::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

//====================================================================
//=== SpacerWrapper
//====================================================================
SpacerWrapper::SpacerWrapper(QObject* parent)
  : WidgetWrapper(parent)
  , m_color(QColorConstants::X11::DimGray)
{}

const QSize
SpacerWrapper::calcSize()
{
  auto p = qobject_cast<BaseWidget*>(parent());
  return QSize(p->width(), p->height());
}

const QColor&
SpacerWrapper::color() const
{
  return m_color;
}

void
SpacerWrapper::setColor(const QColor& newColor)
{
  m_color = newColor;
}

int
SpacerWrapper::thickness() const
{
  return m_thickness;
}

void
SpacerWrapper::setThickness(int newThickness)
{
  m_thickness = newThickness;
}

void
SpacerWrapper::paintWidget(QPainter& painter)
{
  painter.save();
  paintBackground(painter);
  auto pen = painter.pen();
  pen.setColor(m_color);
  pen.setWidth(thickness());
  painter.setPen(pen);
  auto t = m_rect.top() + m_margins.top();
  painter.drawLine(m_rect.left(), t, m_rect.right(), t);
  painter.restore();
}

void
SpacerWrapper::resizeEvent(QRect& r, int& min, int& max)
{
  auto p = qobject_cast<BaseWidget*>(parent());
  auto widgetPosition = p->widgetPosition();

  switch (widgetPosition) {
    case Left:
    case Right: {
      r.setX(0);
      auto h = topMargin() + thickness() + bottomMargin();
      if (position() == Top) {
        r.setTop(min);
        min += h;
      } else if (position() == Bottom) {
        max -= h;
        r.setTop(max);
      }
      r.setWidth(p->maxWidth());
      r.setHeight(h);
      break;
    }
    case Top:
    case Bottom: {
      // TODO
      break;
    }
  }
}
