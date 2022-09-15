#include "docker/private/abstractdockitemprivate.h"
#include "docker/abstractdockwidget.h"
#include "docker/buttonwidget.h"
#include "docker/labelwidget.h"
#include "docker/listbuttonwidget.h"
#include "docker/seperatorwidget.h"
#include "docker/widgetitem.h"
#include "utilities/MathUtilities.h"

AbstractDockItemPrivate::AbstractDockItemPrivate(DockPosition position,
                                                 AbstractDockWidget* parent,
                                                 AbstractDockItem* qptr)
  : q_ptr(qptr)
  , m_parentDocker(parent)
  , m_dockPosition(position)
{
  switch (m_dockPosition) {
    case West:
    case East: {
      m_width = AbstractDockItem::WIDTH;
      m_maxWidgetWidth = m_width;
      break;
    }
    case North:
    case South: {
      m_height = AbstractDockItem::HEIGHT;
      m_maxWidgetHeight = m_height;
      break;
    }
    default:
      break;
  }
}

ButtonWidget*
AbstractDockItemPrivate::addIconButton(WidgetPosition pos,
                                       QIcon icon,
                                       QSize iconSize,
                                       const QString& tooltip)
{
  ButtonWidget* w =
    createButtonItem(Button, pos, icon, iconSize, QString(), IconOnly, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

ButtonWidget*
AbstractDockItemPrivate::addIconListButton(WidgetPosition pos,
                                           QIcon icon,
                                           QSize iconSize,
                                           const QString& tooltip)
{
  ButtonWidget* w = createButtonItem(
    MenuButton, pos, icon, iconSize, QString(), IconOnly, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

ButtonWidget*
AbstractDockItemPrivate::addIconTextButton(WidgetPosition pos,
                                           QIcon icon,
                                           QSize iconSize,
                                           const QString& text,
                                           Arrangement textPos,
                                           const QString& tooltip)
{
  ButtonWidget* w =
    createButtonItem(Button, pos, icon, iconSize, text, textPos, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

ButtonWidget*
AbstractDockItemPrivate::addIconTextListButton(WidgetPosition pos,
                                               QIcon icon,
                                               QSize iconSize,
                                               const QString& text,
                                               Arrangement textPos,
                                               const QString& tooltip)
{
  ButtonWidget* w =
    createButtonItem(MenuButton, pos, icon, iconSize, text, textPos, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

ButtonWidget*
AbstractDockItemPrivate::addTextButton(WidgetPosition pos,
                                       const QString& text,
                                       const QString& tooltip)
{
  ButtonWidget* w =
    createButtonItem(Button, pos, QIcon(), QSize(), text, TextOnly, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

ButtonWidget*
AbstractDockItemPrivate::addTextListButton(WidgetPosition pos,
                                           const QString& text,
                                           const QString& tooltip)
{
  ButtonWidget* w = createButtonItem(
    MenuButton, pos, QIcon(), QSize(), text, TextOnly, tooltip);
  calcMaxWidgetSizes();
  q_ptr->connect(w,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return w;
}

WidgetItem*
AbstractDockItemPrivate::addItem(WidgetItem* item)
{
  m_widgets.append(item);
  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  q_ptr->connect(item,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return item;
}

WidgetItem*
AbstractDockItemPrivate::addTextLabel(WidgetPosition pos,
                                      const QString& text,
                                      const QString& tooltip)
{
  auto widget = new LabelWidget(m_parentDocker);
  widget->setType(Label);
  widget->setWidgetPosition(pos);
  widget->setText(text);
  widget->setTooltip(tooltip);
  m_widgets.append(widget);
  return widget;
}

WidgetItem*
AbstractDockItemPrivate::addSeperator(WidgetPosition pos)
{
  WidgetItem* widget = new SeperatorWidget(m_parentDocker);

  widget->setType(Seperator);
  widget->setWidgetPosition(pos);
  m_widgets.append(widget);

  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  q_ptr->connect(widget,
                 &WidgetItem::widgetChanged,
                 q_ptr,
                 &AbstractDockItem::calcMaxWidgetSizes);
  return widget;
}

WidgetItem*
AbstractDockItemPrivate::addCustomWidget(CustomWidget* item)
{
  return addItem(item);
}

void
AbstractDockItemPrivate::setText(int index, const QString& text)
{
  auto w = at(index);
  if (w) {
    switch (w->type()) {
      case Button: {
        auto tb = qobject_cast<ButtonWidget*>(w);
        tb->setText(text);
        break;
      }
      default: {
      }
    }
  }
}

void
AbstractDockItemPrivate::setToolTip(int index, const QString& tooltip)
{
  auto w = at(index);
  if (w)
    w->setTooltip(tooltip);
}

void
AbstractDockItemPrivate::calculateMaxSize(ButtonWidget* bw,
                                          QString text,
                                          Arrangement textPos)
{
  int width = 0;
  int height = 0;
  auto fm = m_parentDocker->fontMetrics();
  switch (textPos) {
    case TextAboveAndBelow:
      // TODO
      break;
    case TextAboveIcon:
    case TextBelowIcon: {
      width = fm.horizontalAdvance(text);
      width = (width > bw->iconSize().width() ? width : bw->iconSize().width());
      m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
      height = bw->topMargin() + fm.height() + bw->spacer() +
               bw->iconSize().height() + bw->bottomMargin();
      m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
      break;
    }
    case TextToRight:
    case TextToLeft: {
      width = bw->leftMargin() + fm.horizontalAdvance(text) +
              bw->spacer() + bw->iconSize().width() + bw->rightMargin();
      m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
      height = bw->topMargin() + height + bw->bottomMargin();
      m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
      break;
    }
    case IconOnly: {
      width = bw->iconSize().width();
      m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
      height = bw->topMargin() + bw->iconSize().height() + bw->bottomMargin();
      m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
      break;
    }
    case TextOnly: {
      width = fm.horizontalAdvance(text);
      m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
      height = bw->topMargin() + fm.height() + bw->bottomMargin();
      m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
      break;
    }
    case NoArrangement:
      break;
  }
}

void
AbstractDockItemPrivate::setupButton(ButtonWidget* bw,
                                     QIcon icon,
                                     QSize iconSize,
                                     const QString& text,
                                     Arrangement textPos,
                                     const QString& tooltip)
{
  bw->setIcon(icon);
  bw->setIconSize(iconSize);
  bw->setText(text);
  bw->setArrangement(textPos);
  bw->setTooltip(tooltip);

//  calculateMaxSize(bw, text, textPos);
}

ButtonWidget*
AbstractDockItemPrivate::createButtonItem(WidgetType type,
                                          WidgetPosition pos,
                                          QIcon icon,
                                          QSize iconSize,
                                          const QString& text,
                                          Arrangement textPos,
                                          const QString& tooltip)
{
  ButtonWidget* wrapper = nullptr;

  switch (type) {
    case Button: {
      auto buttonWidget = new ButtonWidget(m_parentDocker);
      setupButton(buttonWidget, icon, iconSize, text, textPos, tooltip);
      wrapper = buttonWidget;
      break;
    }
    case MenuButton: {
      auto listButton = new ListButtonWidget(m_parentDocker, q_ptr);
      setupButton(listButton, icon, iconSize, text, textPos, tooltip);
      wrapper = listButton;
      break;
    }
    default:
      //      wrapper = WidgetItem::create();
      break;
  }

  if (wrapper) {
    wrapper->setType(type);
    wrapper->setWidgetPosition(pos);
    m_widgets.append(wrapper);

    setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  }

  return wrapper;
}

WidgetItem*
AbstractDockItemPrivate::takeAt(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetItem* layoutStruct = m_widgets.takeAt(index);
    return layoutStruct;
  }
  return nullptr;
}

WidgetItem*
AbstractDockItemPrivate::at(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetItem* layoutStruct = m_widgets.at(index);
    return layoutStruct;
  }
  return nullptr;
}

int
AbstractDockItemPrivate::count()
{
  return m_widgets.count();
}

bool
AbstractDockItemPrivate::replace(int index, WidgetItem* w)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.replace(index, w);
    return true;
  }
  return false;
}

bool
AbstractDockItemPrivate::widgetEnabled(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isEnabled();
  }
  return false;
}

bool
AbstractDockItemPrivate::enableWidget(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setEnabled(value);
    //    repaint();
    return true;
  }
  return false;
}

bool
AbstractDockItemPrivate::selected(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isSelected();
  }
  return false;
}

bool
AbstractDockItemPrivate::setSelected(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setSelected(value);
    //    repaint();
    return true;
  }
  return false;
}

void
AbstractDockItemPrivate::calculateGeometry(const QRect& rect)
{
  m_rect = rect;

  auto min = 0;
  auto max = 0;

  switch (m_dockPosition) {
    case East:
    case West: {
      min = rect.y() + 1;
      max = rect.bottom() - AbstractDockItem::TOOLBAR_ENDER;
      for (auto& w : m_widgets) {
        auto size = w->sizeHint();
        auto x = rect.left() + Math::halfDifference(rect.width(), size.width());
        auto widgetHeight = size.height();
        switch (w->widgetPosition()) {
          case Start: {
            auto y = min + Math::halfDifference(widgetHeight, size.height());
            auto contentsRect = QRect(x, y, size.width(), size.height());
            auto widgetRect = contentsRect;
            widgetRect.setWidth(rect.width());
            widgetRect.moveLeft(rect.left());
            w->setGeometry(widgetRect, contentsRect);
            min += widgetHeight;
            break;
          }
          case End: {
            max -= widgetHeight;
            auto y = max + Math::halfDifference(widgetHeight, size.height());
            auto contentsRect = QRect(x, y, size.width(), size.height());
            auto widgetRect = contentsRect;
            widgetRect.setWidth(rect.width());
            widgetRect.moveLeft(rect.left());
            w->setGeometry(widgetRect, contentsRect);
          }
        }
      }
      break;
    }
    case North:
    case South: {
      min = rect.x() + 1;
      max = rect.right() - AbstractDockItem::TOOLBAR_ENDER;
      for (auto& w : m_widgets) {
        auto size = w->sizeHint();
        auto y = rect.top() + Math::halfDifference(rect.height(), size.height());
        auto widgetWidth = size.width();
        switch (w->widgetPosition()) {
          case Start: {
            auto x = min + Math::halfDifference(widgetWidth, size.width());
            auto contentsRect = QRect(x, y, size.width(), size.height());
            auto widgetRect = contentsRect;
            widgetRect.setHeight(rect.height());
            widgetRect.moveTop(rect.top());
            w->setGeometry(widgetRect, contentsRect);
            min += widgetWidth;
            break;
          }
          case End: {
            max -= widgetWidth;
            auto x = max + Math::halfDifference(widgetWidth, size.width());
            auto contentsRect = QRect(x, y, size.width(), size.height());
            auto widgetRect = contentsRect;
            widgetRect.moveTop(rect.top());
            widgetRect.setHeight(rect.height());
            w->setGeometry(widgetRect, contentsRect);
          }
        }
      }
      break;
    }
    default:
      break;
  }
}

QMargins
AbstractDockItemPrivate::widgetMargins(int index)
{
  auto w = at(index);
  if (w) {
    return w->margins();
  }
  return QMargins();
}

void
AbstractDockItemPrivate::setWidgetMargins(int index,
                                          int left,
                                          int top,
                                          int right,
                                          int bottom)
{
  auto w = at(index);
  if (w) {
    w->setLeftMargin(left);
    w->setTopMargin(top);
    w->setRightMargin(right);
    w->setBottomMargin(bottom);
  }
}

/*
 * Scans the sub widgets to find the maximum widget size. This means that all
 * the sub-widgets are the same size, at least if they are buttons. Other
 * sub-widgets may or may not affect this value.
 */
void
AbstractDockItemPrivate::calcMaxWidgetSizes()
{
  auto maxMargins = calcMaxMargins();
  auto maxContentsSize = calcMaxContentsSize();
  auto w = maxMargins.left() + maxContentsSize.width() + maxMargins.right();
  auto h = maxMargins.top() + maxContentsSize.height() + maxMargins.bottom();

  m_maxWidgetWidth = std::max(w, m_maxWidgetWidth);
  m_maxWidgetHeight = std::max(h, m_maxWidgetHeight);
}

const QRect&
AbstractDockItemPrivate::rect() const
{
  return m_rect;
}

void
AbstractDockItemPrivate::clone(AbstractDockItem* item)
{
  item->d_ptr->m_dockPosition = m_dockPosition;
  for (auto& widgetItem : m_widgets) {
    auto widget = widgetItem->createFromName(widgetItem);
    item->addItem(widget);
  }
  item->d_ptr->m_visible = m_visible;
  item->d_ptr->m_rect = m_rect;
  item->d_ptr->m_width = m_width;
  item->d_ptr->m_height = m_height;
  item->d_ptr->m_maxWidgetHeight = m_maxWidgetHeight;
  item->d_ptr->m_maxWidgetWidth = m_maxWidgetWidth;
}

/*
 * Returns the size of the contents of the sub widget, including spacers.
 */
QSize
AbstractDockItemPrivate::calcMaxContentsSize()
{
  auto width = 0;
  auto height = 0;

  for (auto& w : m_widgets) {
    auto size = w->calcMinimumSize();
    width = std::max(size.width(), width);
    height = std::max(size.height(), height);
  }
  return QSize(width, height);
}

QMargins
AbstractDockItemPrivate::calcMaxMargins()
{
  QMargins maxMargins;
  for (auto& w : m_widgets) {
    maxMargins.setLeft(w->leftMargin() > maxMargins.left() ? w->leftMargin()
                                                           : maxMargins.left());
    maxMargins.setRight(w->rightMargin() > maxMargins.right()
                          ? w->rightMargin()
                          : maxMargins.right());
    maxMargins.setTop(w->topMargin() > maxMargins.top() ? w->topMargin()
                                                        : maxMargins.top());
    maxMargins.setBottom(w->bottomMargin() > maxMargins.bottom()
                           ? w->bottomMargin()
                           : maxMargins.bottom());
  }
  return maxMargins;
}

void
AbstractDockItemPrivate::setPreferredSize(int width, int height)
{
  m_width = width;
  m_height = height;
  if (m_dockPosition == East || m_dockPosition == West) {
    m_maxWidgetWidth = m_width;
  } else if (m_dockPosition == North || m_dockPosition == South) {
    m_maxWidgetHeight = m_height;
  }
}

void
AbstractDockItemPrivate::setPreferredSize(QSize size)
{
  setPreferredSize(size.width(), size.height());
}

void
AbstractDockItemPrivate::setPreferredWidth(int width)
{
  m_width = width;
}

void
AbstractDockItemPrivate::setPreferredHeight(int height)
{
  m_height = height;
}

DockPosition
AbstractDockItemPrivate::dockPosition() const
{
  return m_dockPosition;
}

void
AbstractDockItemPrivate::setDockPosition(DockPosition position)
{
  m_dockPosition = position;
}

int
AbstractDockItemPrivate::maxWidgetWidth() const
{
  return m_maxWidgetWidth;
}

int
AbstractDockItemPrivate::maxWidgetHeight() const
{
  return m_maxWidgetHeight;
}

int
AbstractDockItemPrivate::height() const
{
  return m_height;
}

int
AbstractDockItemPrivate::width() const
{
  return m_width;
}

void
AbstractDockItemPrivate::setWidth(int width)
{
  m_width = width;
}

void
AbstractDockItemPrivate::setHeight(int height)
{
  m_height = height;
}

bool
AbstractDockItemPrivate::isVisible() const
{
  return m_visible;
}

void
AbstractDockItemPrivate::setVisible(bool value)
{
  m_visible = value;
}

void
AbstractDockItemPrivate::show()
{
  setVisible(true);
}

void
AbstractDockItemPrivate::hide()
{
  setVisible(false);
}

void
AbstractDockItemPrivate::paint(QPainter& painter)
{
  if (m_visible) {
    painter.fillRect(m_rect, m_parentDocker->backColor());

    for (auto& w : m_widgets) {
      w->paint(painter);
    }
  }
}

const QList<WidgetItem*>&
AbstractDockItemPrivate::widgets() const
{
  return m_widgets;
}
