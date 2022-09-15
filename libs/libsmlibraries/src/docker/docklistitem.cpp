#include "docker/docklistitem.h"
#include "utilities/MathUtilities.h"

//====================================================================
//=== ListItem
//====================================================================
ListItem::ListItem(QObject* parent)
  : QObject(parent)
{
}

const QString&
ListItem::text() const
{
  return m_text;
}

void
ListItem::setText(const QString& newText)
{
  m_text = newText;
}

const QIcon&
ListItem::icon() const
{
  return m_icon;
}

void
ListItem::setIcon(const QIcon& icon, const QSize& iconSize)
{
  m_icon = icon;
  m_iconSize = iconSize;
}

const QRect&
ListItem::rect() const
{
  return m_rect;
}

void
ListItem::setRect(const QRect& rect)
{
  m_rect = rect;
}

const QSize&
ListItem::iconSize() const
{
  return m_iconSize;
}

int
ListItem::textLeft() const
{
  return m_textLeft;
}

void
ListItem::setTextLeft(int textLeft)
{
  m_textLeft = textLeft;
}

//int
//ListItem::descent() const
//{
//  return m_descent;
//}

//void
//ListItem::setDescent(int descent)
//{
//  this->m_descent = descent;
//}

//====================================================================
//=== ListDisplay
//====================================================================
ListDisplay::ListDisplay(QWidget* parent)
  : QWidget(parent)
  , m_parent(parent)
{
}

const QList<ListItem*>&
ListDisplay::items() const
{
  return m_items;
}

void
ListDisplay::setItems(const QList<ListItem*>& items, int scrollWidth)
{
  m_items = items;
  auto fm = m_parent->fontMetrics();
  m_maxItemHeight = 0; // height of text initially
  m_width = 0;
  int maxTextPos = 0;
  for (auto& item : m_items) {
    auto width = 0;
    auto textPos = m_margins.left();

    if (!item->icon().isNull()) {
      m_maxItemHeight = std::max(m_maxItemHeight, item->iconSize().height());
      if (!item->text().isEmpty()) {
        textPos += item->iconSize().width();
        textPos += m_spacer;
        width = textPos;
      }
      maxTextPos = std::max(maxTextPos, textPos);
    }

    if (!item->text().isEmpty()) {
      auto boundingRect = fm.boundingRect(item->text());
      m_maxItemHeight = std::max(m_maxItemHeight, boundingRect.height());
      width += boundingRect.width() + m_margins.right();
    }

    width += scrollWidth;

    m_width = std::max(m_width, width);
  }

  auto top = m_margins.top();
  for (auto& item : m_items) {
    item->setRect(QRect(m_margins.left(), top, m_width, m_maxItemHeight));
    top += m_maxItemHeight;
    top += m_spacer;
    item->setTextLeft(maxTextPos);
  }

  m_height = top + m_margins.bottom();
}

void
ListDisplay::setMargins(const QMargins& newMargins)
{
  m_margins = newMargins;
}

void
ListDisplay::setSpacer(int newSpacer)
{
  m_spacer = newSpacer;
}

QSize
ListDisplay::sizeHint() const
{
  return QSize(m_width, m_height);
}

void
ListDisplay::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);
  auto fm = m_parent->fontMetrics();

  for (auto& item : m_items) {
    auto rect = item->rect();
    if (!item->icon().isNull()) {
      auto pixmap = item->icon().pixmap(item->iconSize(), QIcon::Normal);
      painter.drawPixmap(
        rect.left(),
        rect.top() + Math::halfDifference(m_maxItemHeight, item->iconSize().height()),
        pixmap);
    }

    if (!item->text().isEmpty()) {
      painter.drawText(item->textLeft(),
                       rect.bottom() - fm.descent() -
                         Math::halfDifference(m_maxItemHeight, fm.height()),
                       item->text());
    }
  }
}

//====================================================================
//=== ListScroller
//====================================================================
ListScroller::ListScroller(QWidget* parent)
  : QScrollArea(parent)
{
  setBackgroundRole(QPalette::QPalette::Midlight);
}

void
ListScroller::mousePressEvent(QMouseEvent* event)
{
  auto pos = event->pos();
  auto widget = this->widget();
  auto listDisplay = qobject_cast<ListDisplay*>(widget);
  auto items = listDisplay->items();
  for (auto& item : items) {
    if (item->rect().contains(pos)) {
      emit item->itemClicked();
      hide();
      break;
    }
  }
}
