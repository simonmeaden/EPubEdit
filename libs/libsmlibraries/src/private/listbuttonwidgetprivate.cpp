#include "listbuttonwidgetprivate.h"
#include "abstractdockwidget.h"
#include "abstractdockitem.h"
#include "x11colors.h"

//====================================================================
//=== ListButtonWidgetPrivate
//====================================================================
ListButtonWidgetPrivate::ListButtonWidgetPrivate(AbstractDockWidget* parent,
                                                 AbstractDockItem* dockItem,
                                                 WidgetItem* widgetItem)
  : ButtonWidgetPrivate(parent, widgetItem)
  , m_dockItem(dockItem)
{
}

const QList<ListItem*>&
ListButtonWidgetPrivate::items()
{
  return m_items;
}

ListItem*
ListButtonWidgetPrivate::addItem(const QString& text)
{
  auto item = new ListItem(m_dockItem);
  if (m_items.isEmpty()) {
    setText(text);
  }
  item->setText(text);
  q_ptr->connect(item,
                 &ListItem::itemClicked,
                 qobject_cast<ListButtonWidget*>(q_ptr),
                 &ListButtonWidget::cleanItems);
  m_items.append(item);
  calcMinimumSize();
  emit q_ptr->widgetChanged();
  return item;
}

ListItem*
ListButtonWidgetPrivate::addItem(const QIcon& icon,
                                 const QSize& iconSize,
                                 const QString& text)
{
  auto item = new ListItem(m_dockItem);
  if (m_items.isEmpty()) {
    setText(text);
  }
  item->setText(text);
  item->setIcon(icon, iconSize);
  q_ptr->connect(item,
                 &ListItem::itemClicked,
                 qobject_cast<ListButtonWidget*>(q_ptr),
                 &ListButtonWidget::cleanItems);
  m_items.append(item);
  calcMinimumSize();
  emit q_ptr->widgetChanged();
  return item;
}

void
ListButtonWidgetPrivate::setItems(const QStringList& items)
{
  m_items.clear();
  for (auto& text : items) {
    addItem(text);
  }
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

void
ListButtonWidgetPrivate::setItems(const QList<ListItem*>& items)
{
  m_items = items;
  calcMinimumSize();
  emit q_ptr->widgetChanged();
}

void
ListButtonWidgetPrivate::showList()
{
  auto scrollWidth = 0;
  m_scrollArea = new ListScroller(m_parentWidget);
  scrollWidth = m_scrollArea->verticalScrollBar()->sizeHint().width();
  m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  q_ptr->connect(m_scrollArea,
                 &ListScroller::listClicked,
                 qobject_cast<ListButtonWidget*>(q_ptr),
                 &ListButtonWidget::itemClicked);

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
  auto widgetSize = widgetRect.size();
  widgetSize.rwidth() += scrollWidth;
  widgetRect.setSize(widgetSize);

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

bool
ListButtonWidgetPrivate::isVisible()
{
  if (m_scrollArea)
    return m_scrollArea->isVisible();
  return false;
}

void
ListButtonWidgetPrivate::setSpacer(int spacer)
{
  m_spacer = spacer;
}

void
ListButtonWidgetPrivate::hideWidget()
{
  if (m_scrollArea)
    return m_scrollArea->hide();
}

void
ListButtonWidgetPrivate::cleanItems()
{
  // clean up list display.
  m_listDisplay->deleteLater();
  m_listDisplay = nullptr;
  m_scrollArea->deleteLater();
  m_scrollArea = nullptr;
}

WidgetItem*
ListButtonWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new ListButtonWidget(m_parentWidget, m_dockItem);
  }

  auto button = qobject_cast<ListButtonWidget*>(widget);
  if (button) {
    ButtonWidgetPrivate::clone(button);
    setItems(m_items);
  }
  return button;
}

void
ListButtonWidgetPrivate::setDockItem(AbstractDockItem* dockItem)
{
  m_dockItem = dockItem;
}
