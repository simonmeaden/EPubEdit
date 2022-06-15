#include "widgetitem.h"
#include "dockitem.h"
#include "dockwidget.h"

#include <algorithm>

#include <QDebug>

//====================================================================
//=== WidgetItem
//====================================================================
WidgetItem::WidgetItem(AbstractDockWidget* parent)
  : QObject(parent)
  , m_parent(parent)
  , m_fontMetrics(parent->fontMetrics())
  , m_margins(QMargins(LEFTMARGIN, TOPMARGIN, RIGHTMARGIN, BOTTOMMARGIN))
{}

bool
WidgetItem::isEnabled() const
{
  return m_enabled;
}

void
WidgetItem::setEnabled(bool newEnabled)
{
  m_enabled = newEnabled;
}

const QRect&
WidgetItem::rect() const
{
  return m_rect;
}

WidgetType
WidgetItem::type() const
{
  return m_type;
}

void
WidgetItem::setType(WidgetType newType)
{
  m_type = newType;
}

WidgetPosition
WidgetItem::widgetPosition() const
{
  return m_widgetPosition;
}

void
WidgetItem::setWidgetPosition(WidgetPosition newPosition)
{
  m_widgetPosition = newPosition;
  emit widgetChanged();
}

const QString&
WidgetItem::tooltip() const
{
  return m_tooltip;
}

void
WidgetItem::setTooltip(const QString& newTooltip)
{
  m_tooltip = newTooltip;
}

bool
WidgetItem::isHoverOver() const
{
  return m_hoverOver;
}

void
WidgetItem::setHoverOver(bool newHoverOver)
{
  m_hoverOver = newHoverOver;
}

void
WidgetItem::setGeometry(const QRect& rect, const QRect& /*widgetRect*/)
{
  m_rect = rect;
}

QSize
WidgetItem::sizeHint() const
{
  return m_minContentSize.grownBy(m_margins);
}

void
WidgetItem::paintBackground(QPainter& painter)
{
  painter.save();
  if (isSelected()) {
    painter.fillRect(m_rect, m_parent->selectedColor());
  } else if (isHoverOver()) {
    painter.fillRect(m_rect, m_parent->hoverBackColor());
  } else {
    painter.fillRect(m_rect, m_parent->backColor());
  }
  painter.restore();
}

const QSize&
WidgetItem::minContentSize() const
{
  return m_minContentSize;
}

int
WidgetItem::halfDifference(int large, int small)
{
  return int((large - small) / 2);
}

bool
WidgetItem::isSelected() const
{
  return m_selected;
}

void
WidgetItem::setSelected(bool newSelected)
{
  m_selected = newSelected;
}

const QMargins&
WidgetItem::margins() const
{
  return m_margins;
}

void
WidgetItem::setMargins(const QMargins& newMargins)
{
  m_margins = newMargins;
  emit widgetChanged();
}

int
WidgetItem::leftMargin()
{
  return m_margins.left();
}

int
WidgetItem::rightMargin()
{
  return m_margins.right();
}

int
WidgetItem::topMargin()
{
  return m_margins.top();
}

int
WidgetItem::bottomMargin()
{
  return m_margins.bottom();
}

void
WidgetItem::setLeftMargin(int margin)
{
  m_margins.setLeft(margin);
  emit widgetChanged();
}

void
WidgetItem::setRightMargin(int margin)
{
  m_margins.setRight(margin);
  emit widgetChanged();
}

void
WidgetItem::setTopMargin(int margin)
{
  m_margins.setTop(margin);
  emit widgetChanged();
}

void
WidgetItem::setBottomMargin(int margin)
{
  m_margins.setBottom(margin);
  emit widgetChanged();
}

//====================================================================
//=== ButtonWrapper
//====================================================================
ButtonWidget::ButtonWidget(AbstractDockWidget* parent)
  : WidgetItem(parent)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

//ButtonWidget::~ButtonWidget()
//{
//  while (!m_items.empty()) {
//    auto item = m_items.takeLast();
//    delete item;
//  }
//}

Arrangement
ButtonWidget::arrangement() const
{
  return m_arrangement;
}

void
ButtonWidget::setArrangement(Arrangement newArrangement)
{
  m_arrangement = newArrangement;
  emit widgetChanged();
  m_parent->repaint(m_rect);
}

const QRect&
ButtonWidget::iconRect() const
{
  return m_iconRect;
}

void
ButtonWidget::setIconRect(const QRect& newIconRect)
{
  m_iconRect = newIconRect;
  m_parent->repaint(m_rect);
}

const QIcon&
ButtonWidget::icon() const
{
  return m_icon;
}

void
ButtonWidget::setIcon(const QIcon& newIcon)
{
  m_icon = newIcon;
  m_parent->repaint(m_rect);
}

const QSize&
ButtonWidget::iconSize() const
{
  return m_iconSize;
}

void
ButtonWidget::setIconSize(const QSize& newIconSize)
{
  m_iconSize = newIconSize;
  emit widgetChanged();
  m_parent->repaint(m_rect);
}

int
ButtonWidget::spacer() const
{
  return m_spacer;
}

void
ButtonWidget::setSpacer(int newSpacer)
{
  m_spacer = newSpacer;
  m_parent->repaint(m_rect);
}

const QSize
ButtonWidget::calcMinimumSize()
{
  auto width = 0, height = 0, value = 0;
  auto iconWidth = 0, iconHeight = 0, textWidth = 0, textHeight = 0;

  if (!icon().isNull()) {
    iconWidth = iconSize().width();
    iconHeight = iconSize().height();
  }

  if (!text().isEmpty()) {
    textWidth = m_fontMetrics.horizontalAdvance(text());
    textHeight = m_fontMetrics.height();
  }

  switch (arrangement()) {
    case TextAboveAndBelow:
      // TODO
      break;
    case TextAboveIcon:
    case TextBelowIcon: {
      value = (iconWidth > textWidth ? iconWidth : textWidth);
      width = std::max(value, width);
      m_minContentSize.setWidth(value);
      value = iconHeight + spacer() + textHeight;
      m_minContentSize.setHeight(value);
      height = std::max(iconHeight, height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    }
    case TextToLeft:
    case TextToRight: {
      value = iconWidth + spacer() + textWidth;
      m_minContentSize.setWidth(value);
      width = std::max(value, width);
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
  return m_minContentSize;
}

void
ButtonWidget::paint(QPainter& painter)
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
ButtonWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  WidgetItem::setGeometry(widgetRect);

  auto x = 0, y = 0, max = 0;

  switch (arrangement()) {
    case TextAboveAndBelow:
      // TODO
      break;
    case TextAboveIcon: {
      if (!m_text.isEmpty()) {
        x = halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_minContentSize.height());
        m_textRect.moveTop(contentsRect.y() + y);
        max = m_textRect.bottom() + spacer();
      }

      if (!m_icon.isNull()) {
        x = halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        m_iconRect.moveTop(max);
      }

      break;
    }
    case TextBelowIcon: {
      if (!m_icon.isNull()) {
        x = halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_minContentSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
        max = m_iconRect.bottom() /*+ spacer()*/;
      }

      if (!m_text.isEmpty()) {
        x = halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        m_textRect.moveTop(max);
      }
      break;
    }
    case TextToRight: {
      if (!m_icon.isNull()) {
        x = halfDifference(contentsRect.width(), m_minContentSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
        max = m_iconRect.right() + spacer();
      }

      if (!m_text.isEmpty()) {
        m_textRect.moveLeft(max);
        y = halfDifference(contentsRect.height(), m_fontMetrics.height());
        m_textRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case TextToLeft: {
      if (!m_text.isEmpty()) {
        x = halfDifference(contentsRect.width(), m_minContentSize.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_fontMetrics.height());
        m_textRect.moveTop(contentsRect.y() + y);
        max = m_textRect.right() + spacer();
      }

      if (!m_icon.isNull()) {
        m_iconRect.moveLeft(max);
        y = halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case IconOnly: {
      if (!m_icon.isNull()) {
        x = halfDifference(contentsRect.width(), m_iconSize.width());
        m_iconRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_iconSize.height());
        m_iconRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case TextOnly: {
      if (!m_text.isEmpty()) {
        x = halfDifference(contentsRect.width(), m_textRect.width());
        m_textRect.moveLeft(contentsRect.x() + x);
        y = halfDifference(contentsRect.height(), m_textRect.height());
        m_textRect.moveTop(contentsRect.y() + y);
      }
      break;
    }
    case NoArrangement:
      qWarning();
      break;
  }
}

void
ButtonWidget::setIconState(QIcon::Mode mode)
{
  if (!m_icon.isNull()) {
    m_iconMode = mode;
  }
}

const QColor&
ButtonWidget::textColor() const
{
  return m_textColor;
}

void
ButtonWidget::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QString&
ButtonWidget::text() const
{
  return m_text;
}

void
ButtonWidget::setText(const QString& newText)
{
  m_text = newText;
  emit widgetChanged();
}

const QRect&
ButtonWidget::textRect() const
{
  return m_textRect;
}

void
ButtonWidget::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

//void
//ButtonWidget::addItem(const QString& text)
//{
//  auto item = new MenuItem;
//  item->setText(text);
//  m_items.append(item);
//  calcMinimumSize();
//  emit widgetChanged();
//}

//void
//ButtonWidget::addItem(const QIcon& icon,
//                    const QSize& iconSize,
//                    const QString& text)
//{
//  auto item = new MenuItem;
//  item->setText(text);
//  item->setIcon(icon, iconSize);
//  m_items.append(item);
//  calcMinimumSize();
//  emit widgetChanged();
//}

//void
//ButtonWidget::setItems(const QStringList& items)
//{
//  m_items.clear();
//  for (auto& text : items) {
//    addItem(text);
//  }
//  calcMinimumSize();
//  emit widgetChanged();
//}

//void
//ButtonWidget::setItems(const QList<MenuItem*>& items)
//{
//  m_items = items;
//  calcMinimumSize();
//  emit widgetChanged();
//}

//int
//ButtonWidget::currentItem()
//{
//  return m_currentIndex;
//}

//void
//ButtonWidget::menuClicked(QAction* action)
//{
//  m_currentIndex = action->data().toInt();
//  auto text = action->text();
//  auto item = m_items.at(m_currentIndex);
//  emit itemClicked(m_currentIndex, item->text);
//}

//void
//ButtonWidget::setCurrentItem(int index)
//{
//  if (index >= 0 && index < m_items.size()) {
//    auto item = m_items.at(index);
//    m_currentIndex = index;
//    emit widgetChanged();
//  }
//}

//====================================================================
//=== LabelWidget
//====================================================================
LabelWidget::LabelWidget(AbstractDockWidget* parent)
  : WidgetItem(parent)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

const QString&
LabelWidget::text() const
{
  return m_text;
}

void
LabelWidget::setText(const QString& newText)
{
  m_text = newText;
  calcMinimumSize();
}

const QColor&
LabelWidget::textColor() const
{
  return m_textColor;
}

void
LabelWidget::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QSize
LabelWidget::calcMinimumSize()
{
  int textWidth = 0, textHeight = 0;

  if (!m_text.isEmpty()) {
    textWidth = m_fontMetrics.horizontalAdvance(text());
    textHeight = m_fontMetrics.height();
  }

  m_minContentSize.setWidth(textWidth);
  m_minContentSize.setHeight(textHeight);
  m_textRect = QRect(0, 0, textWidth, textHeight);

  return m_minContentSize;
}

void
LabelWidget::paint(QPainter& painter)
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

void
LabelWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  WidgetItem::setGeometry(widgetRect);

  if (!m_text.isEmpty()) {
    auto x = halfDifference(contentsRect.width(), m_textRect.width());
    m_textRect.moveLeft(contentsRect.x() + x);
    auto y = halfDifference(contentsRect.height(), m_textRect.height());
    m_textRect.moveTop(contentsRect.y() + y);
  }
}

QSize
LabelWidget::sizeHint() const
{
  return m_minContentSize.grownBy(m_margins);
}

//====================================================================
//=== ListWidget
//====================================================================
ListWidget::ListWidget(AbstractDockWidget* parent, DockItem* dockItem)
  : WidgetItem(parent)
  , m_dockItem(dockItem)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

ListWidget::~ListWidget()
{
  while (!m_items.empty()) {
    auto item = m_items.takeLast();
    delete item;
  }
}

const QList<MenuItem*>&
ListWidget::items() const
{
  return m_items;
}

void
ListWidget::addItem(const QString& text)
{
  auto item = new MenuItem;
  if (m_items.isEmpty()) {
    setText(text);
  }
  item->setText(text);
  m_items.append(item);
  calcMinimumSize();
  emit widgetChanged();
}

void
ListWidget::addItem(const QIcon& icon,
                    const QSize& iconSize,
                    const QString& text)
{
  auto item = new MenuItem;
  item->setText(text);
  item->setIcon(icon, iconSize);
  m_items.append(item);
  calcMinimumSize();
  emit widgetChanged();
}

void
ListWidget::setItems(const QStringList& items)
{
  m_items.clear();
  for (auto& text : items) {
    addItem(text);
  }
  calcMinimumSize();
  emit widgetChanged();
}

void
ListWidget::setItems(const QList<MenuItem*>& items)
{
  m_items = items;
  calcMinimumSize();
  emit widgetChanged();
}

int
ListWidget::currentItem()
{
  return m_currentItem;
}

void
ListWidget::menuClicked(QAction* action)
{
  m_currentItem = action->data().toInt();
  auto text = action->text();
  setText(text);
  emit itemClicked(m_currentItem, text);
}

void
ListWidget::setCurrentItem(int index)
{
  if (index >= 0 && index < m_items.size()) {
    auto item = m_items.at(index);
    setText(item->text());
    m_currentItem = index;
    emit widgetChanged();
  }
}

void
ListWidget::setText(const QString text)
{
  m_text = text;
  m_parent->repaint();
}

void
ListWidget::setText(int index, const QString text)
{
  if (index > 0 && index < m_items.size()) {
    m_items.at(index)->setText(text);
  }
}

void
ListWidget::setIcon(int index, const QIcon& icon, const QSize& iconSize)
{
  if (index > 0 && index < m_items.size()) {
    m_items.at(index)->setIcon(icon, iconSize);
  }
}

void
ListWidget::setItem(int index,
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
ListWidget::show(const QPoint& pos, const QRect& frameRect)
{
  int index = 0;
  QMenu* menu = new QMenu(m_parent);
  for (auto& item : m_items) {
    if (item->icon().isNull()) {
      auto action = new ListAction(item->text(), m_parent);
      action->setData(index++);
      menu->addAction(action);
    } else {
      auto action = new ListAction(item->icon(), item->text(), m_parent);
      action->setData(index++);
      menu->addAction(action);
    }
  }
  connect(menu, &QMenu::triggered, this, &ListWidget::menuClicked);

  auto size = m_listRect.size();
  QPoint position;
  switch (m_dockItem->dockPosition()) {
      //    case West:
      //      position.setX(frameRect.right() - size.width());
      //      position.setY(pos.y());
      //      // TODO
      //      break;
    case North:
      position.setX(pos.x());
      position.setY(frameRect.top());
      break;
    case South:
      position.setX(pos.x());
      position.setY(frameRect.bottom() - size.height());
      break;
      //    case East:
      //      position.setX(pos.x());
      //      position.setY(pos.y());
      //      break;
    default:
      break;
  }

  menu->popup(m_parent->mapToGlobal(position));
}

void
ListWidget::paint(QPainter& painter)
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
ListWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  WidgetItem::setGeometry(widgetRect);
  auto x = 0, y = 0;

  if (!m_icon.isNull()) {
    //    x = halfDifference(contentsRect.width(), m_iconSize.width());
    y = halfDifference(contentsRect.height(), m_iconSize.height());
    m_iconRect.moveTopLeft(
      QPoint(contentsRect.right() - m_iconSize.width(), contentsRect.y() + y));
  }

  if (!m_text.isEmpty()) {
    x = halfDifference(contentsRect.width(), m_textRect.width());
    y = halfDifference(contentsRect.height(), m_textRect.height());
    m_textRect.moveTopLeft(QPoint(contentsRect.x() + x, contentsRect.y() + y));
  }
}

const QSize
ListWidget::calcMinimumSize()
{
  auto maxTextWidth = 0;
  auto textWidth = 0;
  auto iconWidth = 0, iconHeight = 0, textHeight = m_fontMetrics.height();
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
    textWidth = m_fontMetrics.horizontalAdvance(item->text());
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

  //  auto top = halfDifference(m_rect.height(), iconHeight);
  //  m_iconRect.moveTopLeft(QPoint(rightMargin() - iconWidth, top));

  //  top = halfDifference(m_rect.height(), textHeight);
  //  m_textRect.moveTopLeft(QPoint(leftMargin(), top));

  return m_minContentSize;
}

const QRect&
ListWidget::textRect() const
{
  return m_textRect;
}

void
ListWidget::setTextRect(const QRect& newTextRect)
{
  m_textRect = newTextRect;
}

QSize
ListWidget::sizeHint() const
{
  return m_minContentSize.grownBy(m_margins);
}

//====================================================================
//=== SpacerWrapper
//====================================================================
SeperatorWidget::SeperatorWidget(AbstractDockWidget* parent)
  : WidgetItem(parent)
  , m_color(QColorConstants::X11::DimGray)
{}

const QSize
SeperatorWidget::calcMinimumSize()
{
  auto p = qobject_cast<DockItem*>(parent());
  auto dockPosition = p->dockPosition();
  auto ps = p->sizeHint();

  switch (dockPosition) {
    case East:
    case West: {
      auto pw = ps.width();
      m_minContentSize.setWidth(pw);
      m_minContentSize.setHeight(topMargin() + thickness() + bottomMargin());
      break;
    }
    case North:
    case South: {
      auto ph = ps.height();
      m_minContentSize.setWidth(leftMargin() + thickness() + rightMargin());
      m_minContentSize.setHeight(ph);
      break;
    }
    default:
      break;
  }
  return m_minContentSize;
}

const QColor&
SeperatorWidget::color() const
{
  return m_color;
}

void
SeperatorWidget::setColor(const QColor& newColor)
{
  m_color = newColor;
}

int
SeperatorWidget::thickness() const
{
  return m_thickness;
}

void
SeperatorWidget::setThickness(int newThickness)
{
  m_thickness = newThickness;
}

void
SeperatorWidget::paint(QPainter& painter)
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

//====================================================================
//=== CustomWidget
//====================================================================
WidgetItem*
CustomWidget::create(const QString& type)
{
  void* myClassPtr = nullptr;
  int id = QMetaType::type(type.toStdString().c_str());
  if (id) {
    myClassPtr = QMetaType::create(id);
  }
  return static_cast<WidgetItem*>(myClassPtr);
}

WidgetItem*
CustomWidget::create(QWidget* sister)
{
  void* myClassPtr = nullptr;
  int id = QMetaType::type(sister->metaObject()->className());
  if (id) {
    myClassPtr = QMetaType::create(id, sister);
  }
  return static_cast<WidgetItem*>(myClassPtr);
}
