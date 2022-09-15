#include "listwidgetprivate.h"
#include "docker/docklistitem.h"
//#include "docker/listwidget.h"
#include "docker/abstractdockwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/listwidget.h"
#include "docker/widgetitem.h"
#include "utilities/MathUtilities.h"
#include "utilities/x11colors.h"

//====================================================================
//=== ListWidgetPrivate
//====================================================================
ListWidgetPrivate::ListWidgetPrivate(AbstractDockWidget* parent,
                                     AbstractDockItem* dockItem,
                                     WidgetItem* qptr)
  : WidgetItemPrivate(parent, qptr)
  , m_dockItem(dockItem)
  , m_textColor(QColorConstants::X11::LightGrey)
{
//  addItem(q_ptr->tr("<no document>"));
}

ListWidgetPrivate::~ListWidgetPrivate()
{
  while (!m_items.empty()) {
    auto item = m_items.takeLast();
    delete item;
  }
}

void
ListWidgetPrivate::cleanItems()
{
  // clean up list display.
  m_listDisplay->deleteLater();
  m_listDisplay = nullptr;
  m_scrollArea->deleteLater();
  m_scrollArea = nullptr;
}

void
ListWidgetPrivate::setCurrentItem(int index)
{
  if (index >= 0 && index < m_items.size()) {
    auto item = m_items.at(index);
    setText(item->text());
    m_currentItem = index;
    emit q_ptr->widgetChanged();
  }
}

void
ListWidgetPrivate::addItem(const QString& text)
{
  auto item = new ListItem(m_dockItem);
  if (m_items.isEmpty()) {
    setText(text);
  }
  item->setText(text);
  q_ptr->connect(item,
                 &ListItem::itemClicked,
                 dynamic_cast<ListWidget*>(q_ptr),
                 &ListWidget::itemWasClicked);
  m_items.append(item);
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

void
ListWidgetPrivate::addItem(const QIcon& icon,
                           const QSize& iconSize,
                           const QString& text)
{
  auto item = new ListItem(m_dockItem);
  if (m_items.isEmpty()) {
    setText(text);
  }
  item->setText(text);
  item->setIcon(icon, iconSize);
  q_ptr->connect(
    item, &ListItem::itemClicked, dynamic_cast<ListWidget*>(q_ptr), &ListWidget::cleanItems);
  q_ptr->connect(
    item, &ListItem::itemClicked, dynamic_cast<ListWidget*>(q_ptr), &ListWidget::cleanItems);
  m_items.append(item);
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

void
ListWidgetPrivate::setItems(const QStringList& items)
{
  m_items.clear();
  for (auto& text : items) {
    addItem(text);
  }
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

void
ListWidgetPrivate::setItems(const QList<ListItem*>& items)
{
  m_items = items;
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

const QList<ListItem*>&
ListWidgetPrivate::items()
{
  return m_items;
}

int
ListWidgetPrivate::currentItem()
{
  return m_currentItem;
}

void
ListWidgetPrivate::showList()
{
  auto scrollWidth = 0;
  m_scrollArea = new ListScroller(m_parentWidget);
  scrollWidth = m_scrollArea->verticalScrollBar()->sizeHint().width();
  m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  q_ptr->connect(m_scrollArea,
                 &ListScroller::listClicked,
                 dynamic_cast<ListWidget*>(q_ptr),
                 &ListWidget::itemClicked);
  q_ptr->connect(m_scrollArea,
                 &ListScroller::listClicked,
                 dynamic_cast<ListWidget*>(q_ptr),
                 &ListWidget::itemClicked);

  m_listDisplay = new ListDisplay(m_parentWidget);
  m_listDisplay->setMargins(m_margins);
  m_listDisplay->setSpacer(m_spacer);
  m_listDisplay->setItems(m_items, scrollWidth);

  QPoint position;
  switch (m_dockItem->dockPosition()) {
    case North:
      position = m_rect.bottomLeft();
      break;
    case South:
      position = m_rect.topLeft();
      break;
    default:
      break;
  }

  auto hint = m_listDisplay->sizeHint();
  auto availableRect = m_parentWidget->availableRect();
  auto widgetRect = QRect(position, hint);

  if (availableRect.contains(widgetRect)) {
    m_scrollArea->setGeometry(widgetRect);
  } else {
    auto intersect = availableRect.intersected(widgetRect);
    if (intersect.width() < widgetRect.width()) {
      auto extraWidth = availableRect.width() - intersect.width();
      auto diff = widgetRect.width() - intersect.width();
      if (extraWidth > diff) {
        widgetRect.moveLeft(widgetRect.left() - diff);
      } else if (extraWidth < diff) {
        widgetRect.moveLeft(widgetRect.left() - extraWidth);
      } else {
        widgetRect.moveLeft(widgetRect.left() - extraWidth);
        widgetRect.setWidth(availableRect.width());
      }
    }

    if (intersect.height() < widgetRect.height()) {
      widgetRect.setHeight(intersect.height());
    }

    m_scrollArea->setGeometry(widgetRect);
  }

  m_scrollArea->setWidget(m_listDisplay);
  m_scrollArea->show();
}

void
ListWidgetPrivate::setText(const QString text)
{
  m_text = text;
  m_parentWidget->repaint();
}

void
ListWidgetPrivate::setSpacer(int spacer)
{
  m_spacer = spacer;
}

void
ListWidgetPrivate::setText(int index, const QString text)
{
  if (index > 0 && index < m_items.size()) {
    m_items.at(index)->setText(text);
  }
}

void
ListWidgetPrivate::setIcon(int index, const QIcon& icon, const QSize& iconSize)
{
  if (index > 0 && index < m_items.size()) {
    m_items.at(index)->setIcon(icon, iconSize);
  }
}

void
ListWidgetPrivate::setItem(int index,
                           const QIcon& icon,
                           const QSize& iconSize,
                           const QString text)
{
  if (index > 0 && index < m_items.size()) {
    auto item = m_items.at(index);
    item->setText(text);
    item->setIcon(icon, iconSize);
  }
}

void
ListWidgetPrivate::paint(QPainter& painter)
{
  painter.save();
  paintBackground(painter);

  if (!m_icon.isNull()) {
    auto pixmap = m_icon.pixmap(m_iconSize,
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
ListWidgetPrivate::setGeometry(const QRect& widgetRect,
                               const QRect& contentsRect)
{
  WidgetItemPrivate::setGeometry(widgetRect);
  auto x = 0, y = 0;

  if (!m_icon.isNull()) {
    //    x = Math::halfDifference(contentsRect.width(), m_iconSize.width());
    y = Math::halfDifference(contentsRect.height(), m_iconSize.height());
    m_iconRect.moveTopLeft(
      QPoint(contentsRect.right() - m_iconSize.width(), contentsRect.y() + y));
  }

  if (!m_text.isEmpty()) {
    x = Math::halfDifference(contentsRect.width(), m_textRect.width());
    y = Math::halfDifference(contentsRect.height(), m_textRect.height());
    m_textRect.moveTopLeft(QPoint(contentsRect.x() + x, contentsRect.y() + y));
  }
}

const QSize
ListWidgetPrivate::calcMinimumSize()
{
  auto fm = m_parentWidget->fontMetrics();
  auto maxTextWidth = 0;
  auto textWidth = 0;
  auto iconWidth = 0, iconHeight = 0, textHeight = fm.height();
  auto image = QImage(":/icons/ListButton");
  auto listWidth = 0, listHeight = m_margins.top();

  m_icon = QIcon(QPixmap::fromImage(image));
  m_iconSize = image.size();
  m_iconRect = QRect(0, 0, iconWidth, iconHeight);

  if (!m_icon.isNull()) {
    iconWidth = m_iconSize.width();
    iconHeight = m_iconSize.height();
  }

  auto itemHeight = std::max(textHeight, iconHeight);
  for (auto& item : m_items) {
    auto w = 0;
    if (!item->icon().isNull()) {
      w = item->iconSize().width();
      w += m_spacer;
    }
    textWidth = fm.horizontalAdvance(item->text());
    w += textWidth;
    maxTextWidth = std::max(maxTextWidth, textWidth);
    listWidth = std::max(w, listWidth);
    listHeight += itemHeight;
  }
  m_textRect = QRect(0, 0, maxTextWidth, textHeight);

  listHeight += m_margins.bottom();
  listWidth += m_margins.left();
  listWidth += m_margins.right();
  m_listRect = QRect(0, 0, listWidth, listHeight);

  m_minContentSize.setWidth(maxTextWidth + m_spacer + iconWidth);
  m_minContentSize.setHeight(std::max(textHeight, iconHeight));

  return m_minContentSize;
}

const QRect&
ListWidgetPrivate::textRect()
{
  return m_textRect;
}

void
ListWidgetPrivate::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

QSize
ListWidgetPrivate::sizeHint() const
{
  return m_minContentSize.grownBy(m_margins);
}

bool
ListWidgetPrivate::isVisible()
{
  if (m_scrollArea)
    return m_scrollArea->isVisible();
  return false;
}

void
ListWidgetPrivate::hideWidget()
{
  if (m_scrollArea)
    return m_scrollArea->hide();
}

 void ListWidgetPrivate::setDockItem(AbstractDockItem *dockItem)
{
   m_dockItem = dockItem;
 }

WidgetItem*
ListWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new ListWidget(m_parentWidget, m_dockItem);
  }

  auto list = dynamic_cast<ListWidget*>(widget);
  if (list) {
    WidgetItemPrivate::clone(list);
    list->setText(m_text);
    list->setItems(m_items);
    list->setSpacer(m_spacer);
  }
  return list;
}
