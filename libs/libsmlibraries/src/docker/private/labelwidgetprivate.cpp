#include "docker/private/labelwidgetprivate.h"
#include "docker/abstractdockwidget.h"
#include "utilities/MathUtilities.h"
#include "utilities/x11colors.h"

LabelWidgetPrivate::LabelWidgetPrivate(AbstractDockWidget* parent,
                                       WidgetItem* qptr)
  : WidgetItemPrivate(parent, qptr)
  , m_textColor(QColorConstants::X11::LightGrey)
{
}

const QString&
LabelWidgetPrivate::text()
{
  return m_text;
}

void
LabelWidgetPrivate::setText(const QString& text)
{
  m_text = text;
}

const QColor&
LabelWidgetPrivate::textColor()
{
  return m_textColor;
}

void
LabelWidgetPrivate::setTextColor(const QColor& textColor)
{
  m_textColor = textColor;
}

WidgetItem*
LabelWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new LabelWidget(m_parentWidget);
  }

  auto label = qobject_cast<LabelWidget*>(widget);
  if (label) {
    WidgetItemPrivate::clone(label);
    label->setText(m_text);
    label->setTextColor(m_textColor);
  }
  return label;
}

const QSize LabelWidgetPrivate::calcMinimumSize()
{
  int textWidth = 0, textHeight = 0;

  if (!m_text.isEmpty()) {
    auto fm = m_parentWidget->fontMetrics();
    textWidth = fm.horizontalAdvance(text());
    textHeight = fm.height();
  }

  m_minContentSize.setWidth(textWidth);
  m_minContentSize.setHeight(textHeight);
  m_textRect = QRect(0, 0, textWidth, textHeight);

  return m_minContentSize;

}

void LabelWidgetPrivate::paint(QPainter &painter)
{
  painter.save();
  paintBackground(painter);

  if (!m_text.isEmpty()) {
    auto pen = painter.pen();
    pen.setColor(m_textColor);
    painter.setPen(pen);
    painter.drawText(m_textRect.left(), m_textRect.bottom(), m_text);
  }

  painter.restore();
}

void LabelWidgetPrivate::setGeometry(const QRect &widgetRect, const QRect &contentsRect)
{
  WidgetItemPrivate::setGeometry(widgetRect);

  if (!m_text.isEmpty()) {
    auto x = Math::halfDifference(contentsRect.width(), m_textRect.width());
    m_textRect.moveLeft(contentsRect.x() + x);
    auto y = Math::halfDifference(contentsRect.height(), m_textRect.height());
    m_textRect.moveTop(contentsRect.y() + y);
  }
}

QSize LabelWidgetPrivate::sizeHint()
{
  return m_minContentSize.grownBy(m_margins);
}
