#include "dockitem.h"
#include "widgetitem.h"
#include "abstractdockwidget.h"

//====================================================================
//=== DockItem
//====================================================================
DockItem::DockItem(DockPosition position, AbstractDockWidget *parent)
  : QObject{ parent }
  , m_parent(parent)
  , m_dockPosition(position)
  , m_fontMetrics(parent->fontMetrics())
{
  switch (m_dockPosition) {
    case West:
    case East: {
      m_width = WIDTH;
      m_maxWidgetWidth = m_width;
      break;
    }
    case North:
    case South: {
      m_height = HEIGHT;
      m_maxWidgetHeight = m_height;
      break;
    }
    default:
      break;
  }
}

DockItem::~DockItem()
{
  WidgetItem* l;
  while ((l = takeAt(0)))
    delete l;
}

WidgetItem*
DockItem::addIconButton(WidgetPosition pos,
                        QIcon icon,
                        QSize iconSize,
                        const QString& tooltip)
{
  WidgetItem* w =
    createButtonItem(Button, pos, icon, iconSize, QString(), IconOnly, tooltip);
  calcMaxWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addIconTextButton(WidgetPosition pos,
                            QIcon icon,
                            QSize iconSize,
                            const QString& text,
                            Arrangement textPos,
                            const QString& tooltip)
{
  WidgetItem* w =
    createButtonItem(Button, pos, icon, iconSize, text, textPos, tooltip);
  calcMaxWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addTextButton(WidgetPosition pos,
                        const QString& text,
                        const QString& tooltip)
{
  WidgetItem* w =
    createButtonItem(Button, pos, QIcon(), QSize(), text, TextOnly, tooltip);
  calcMaxWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addTextLabel(WidgetPosition pos,
                       const QString& text,
                       const QString& tooltip)
{
  auto widget = new LabelWidget(m_parent);
  widget->setType(Label);
  widget->setWidgetPosition(pos);
  widget->setText(text);
  widget->setTooltip(tooltip);
  m_widgets.append(widget);
  return widget;
}

WidgetItem*
DockHeader::addListWidget(WidgetPosition pos, const QString& tooltip)
{
  auto widget = new ListWidget(m_parent, this);
  widget->setTooltip(tooltip);
  int height = 0;
  auto textRect = widget->textRect();
  textRect.setHeight(m_fontMetrics.height());
  widget->setTextRect(textRect);
  height =
    widget->topMargin() + m_fontMetrics.height() + widget->bottomMargin();
  m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);

  widget->setType(List);
  widget->setWidgetPosition(pos);
  m_widgets.append(widget);

  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  connect(
    widget, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return widget;
}

WidgetItem*
DockItem::addSeperator(WidgetPosition pos)
{
  WidgetItem* widget = new SeperatorWidget(m_parent);

  widget->setType(Seperator);
  widget->setWidgetPosition(pos);
  m_widgets.append(widget);

  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  connect(
    widget, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return widget;
}

WidgetItem*
DockItem::addCustomWidget(CustomWidget* w)
{
  m_widgets.append(w);
  calcMaxWidgetSizes();
  return w;
}

void
DockItem::setText(int index, const QString& text)
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
DockItem::setToolTip(int index, const QString& tooltip)
{
  auto w = at(index);
  if (w)
    w->setTooltip(tooltip);
}

WidgetItem*
DockItem::createButtonItem(WidgetType type,
                           WidgetPosition pos,
                           QIcon icon,
                           QSize iconSize,
                           const QString& text,
                           Arrangement textPos,
                           const QString& tooltip)
{
  WidgetItem* wrapper = nullptr;

  switch (type) {
    case Button: {
      auto bw = new ButtonWidget(m_parent);
      bw->setIcon(icon);
      bw->setIconSize(iconSize);
      bw->setText(text);
      bw->setArrangement(textPos);
      bw->setTooltip(tooltip);
      wrapper = bw;
      int height = 0;
      int width = 0;

      switch (textPos) {
        case TextAboveAndBelow:
          // TODO
          break;
        case TextAboveIcon:
        case TextBelowIcon: {
          width = m_fontMetrics.horizontalAdvance(text);
          width =
            (width > bw->iconSize().width() ? width : bw->iconSize().width());
          m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
          height = bw->topMargin() + m_fontMetrics.height() + bw->spacer() +
                   bw->iconSize().height() + bw->bottomMargin();
          m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
          break;
        }
        case TextToRight:
        case TextToLeft: {
          width = bw->leftMargin() + m_fontMetrics.horizontalAdvance(text) +
                  bw->spacer() + bw->iconSize().width() + bw->rightMargin();
          m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
          height = bw->topMargin() + height + bw->bottomMargin();
          m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
          break;
        }
        case IconOnly: {
          width = bw->iconSize().width();
          m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
          height =
            bw->topMargin() + bw->iconSize().height() + bw->bottomMargin();
          m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
          break;
        }
        case TextOnly: {
          width = m_fontMetrics.horizontalAdvance(text);
          m_maxWidgetWidth = std::max(width, m_maxWidgetWidth);
          height =
            bw->topMargin() + m_fontMetrics.height() + bw->bottomMargin();
          m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);
          break;
        }
        case NoArrangement:
          break;
      }

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
DockItem::takeAt(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetItem* layoutStruct = m_widgets.takeAt(index);
    return layoutStruct;
  }
  return nullptr;
}

WidgetItem*
DockItem::at(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    WidgetItem* layoutStruct = m_widgets.at(index);
    return layoutStruct;
  }
  return nullptr;
}

bool
DockItem::replace(int index, WidgetItem* w)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.replace(index, w);
    return true;
  }
  return false;
}

bool
DockItem::widgetEnabled(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isEnabled();
  }
  return false;
}

bool
DockItem::enableWidget(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setEnabled(value);
    //    repaint();
    return true;
  }
  return false;
}

bool
DockItem::selected(int index)
{
  if (index >= 0 && index < m_widgets.size()) {
    return m_widgets.at(index)->isSelected();
  }
  return false;
}

bool
DockItem::setSelected(int index, bool value)
{
  if (index >= 0 && index < m_widgets.size()) {
    m_widgets.at(index)->setSelected(value);
    //    repaint();
    return true;
  }
  return false;
}

void
DockItem::calculateGeometry(const QRect& rect)
{
  m_rect = rect;

  auto min = 0;
  auto max = 0;

  switch (m_dockPosition) {
    case East:
    case West: {
      min = rect.y() + 1;
      max = rect.bottom() - TOOLBAR_ENDER;
      for (auto& w : m_widgets) {
        auto size = w->sizeHint();
        auto x = rect.left() + halfDifference(rect.width(), size.width());
        auto widgetHeight = size.height();
        switch (w->widgetPosition()) {
          case Start: {
            auto y = min + halfDifference(widgetHeight, size.height());
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
            auto y = max + halfDifference(widgetHeight, size.height());
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
      max = rect.right() - TOOLBAR_ENDER;
      for (auto& w : m_widgets) {
        auto size = w->sizeHint();
        auto y = rect.top() + halfDifference(rect.height(), size.height());
        auto widgetWidth = size.width();
        switch (w->widgetPosition()) {
          case Start: {
            auto x = min + halfDifference(widgetWidth, size.width());
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
            auto x = max + halfDifference(widgetWidth, size.width());
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
DockItem::widgetMargins(int index)
{
  auto w = at(index);
  if (w) {
    return w->margins();
  }
  return QMargins();
}

void
DockItem::setWidgetMargins(int index, int left, int top, int right, int bottom)
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
DockItem::calcMaxWidgetSizes()
{
  auto maxMargins = calcMaxMargins();
  auto maxContentsSize = calcMaxContentsSize();
  auto w = maxMargins.left() + maxContentsSize.width() + maxMargins.right();
  auto h = maxMargins.top() + maxContentsSize.height() + maxMargins.bottom();

  m_maxWidgetWidth = std::max(w, m_maxWidgetWidth);
  m_maxWidgetHeight = std::max(h, m_maxWidgetHeight);
}

const QRect &DockItem::rect() const
{
  return m_rect;
}

/*
 * Returns the size of the contents of the sub widget, including spacers.
 */
QSize
DockItem::calcMaxContentsSize()
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
DockItem::calcMaxMargins()
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
DockItem::setPreferredSize(int width, int height)
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
DockItem::setPreferredSize(QSize size)
{
  setPreferredSize(size.width(), size.height());
}

void
DockItem::setPreferredWidth(int width)
{
  m_width = width;
}

void
DockItem::setPreferredHeight(int height)
{
  m_height = height;
}

DockPosition
DockItem::dockPosition() const
{
  return m_dockPosition;
}

int
DockItem::maxWidgetWidth() const
{
  return m_maxWidgetWidth;
}

int
DockItem::maxWidgetHeight() const
{
  return m_maxWidgetHeight;
}

int
DockItem::height() const
{
  return m_height;
}

int
DockItem::width() const
{
  return m_width;
}

bool
DockItem::isVisible() const
{
  return m_visible;
}

void
DockItem::setVisible(bool value)
{
  m_visible = value;
}

void
DockItem::show()
{
  setVisible(true);
}

void
DockItem::hide()
{
  setVisible(false);
}

void
DockItem::paint(QPainter& painter)
{
  if (m_visible) {
    painter.fillRect(m_rect, m_parent->backColor());

    for (auto& w : m_widgets) {
      w->paint(painter);
    }
  }
}

const QList<WidgetItem*>&
DockItem::widgets() const
{
  return m_widgets;
}

//====================================================================
//=== DockFooter
//====================================================================
DockFooter::DockFooter(AbstractDockWidget *parent)
  : DockItem(South, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
}

WidgetItem*
DockFooter::addListWidget(WidgetPosition pos, const QString& tooltip)
{
  auto widget = new ListWidget(m_parent, this);
  widget->setTooltip(tooltip);
  int height = 0;
  auto textRect = widget->textRect();
  textRect.setHeight(m_fontMetrics.height());
  widget->setTextRect(textRect);
  height =
    widget->topMargin() + m_fontMetrics.height() + widget->bottomMargin();
  m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);

  widget->setType(List);
  widget->setWidgetPosition(pos);
  m_widgets.append(widget);

  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  connect(
    widget, &WidgetItem::widgetChanged, this, &DockItem::calcMaxWidgetSizes);
  return widget;
}

QSize
DockFooter::sizeHint() const
{
  auto w = m_width;
  auto h = m_height;
  for (auto widget : m_widgets) {
    auto s = widget->sizeHint();
    w += s.width();
    h = (h > s.height() ? h : s.height());
  }
  return QSize(w, h);
}

void
DockFooter::paint(QPainter& painter)
{
  if (m_visible) {
    DockItem::paint(painter);

    auto pen = painter.pen();
    pen.setColor(QColor(55, 56, 56));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
  }
}

//====================================================================
//=== DockHeader
//====================================================================
DockHeader::DockHeader(AbstractDockWidget *parent)
  : DockItem(North, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
}

QSize
DockHeader::sizeHint() const
{
  auto w = m_width;
  auto h = m_height;
  for (auto widget : m_widgets) {
    auto s = widget->sizeHint();
    w += s.width();
    h = (h > s.height() ? h : s.height());
  }
  return QSize(w, h);
}

void
DockHeader::paint(QPainter& painter)
{
  if (m_visible) {
    DockItem::paint(painter);

    auto pen = painter.pen();
    pen.setColor(QColor(55, 56, 56));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
  }
}

//====================================================================
//=== DockToolbar
//====================================================================
DockToolbar::DockToolbar(DockPosition position, AbstractDockWidget *parent)
  : DockItem(position, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
}

DockPosition
DockToolbar::dockPosition()
{
  return m_dockPosition;
}

void
DockToolbar::setDockPosition(DockPosition position)
{
  m_dockPosition = position;
  //  repaint();
}

QSize
DockToolbar::sizeHint() const
{
  auto w = m_width;
  auto h = m_height;
  switch (m_dockPosition) {
    case North:
    case South: {
      for (auto widget : m_widgets) {
        auto s = widget->sizeHint();
        w += s.width();
        h = (h > s.height() ? h : s.height());
      }
      return QSize(w, h);
    }
    case East:
    case West: {
      for (auto widget : m_widgets) {
        auto s = widget->sizeHint();
        w = (w > s.width() ? w : s.width());
        h += s.height();
      }
      return QSize(w, h);
    }
    default:
      break;
  }
  return QSize();
}

void
DockToolbar::paint(QPainter& painter)
{
  if (m_visible) {
    DockItem::paint(painter);

    auto pen = painter.pen();
    pen.setColor(QColor(55, 56, 56));
    pen.setWidth(1);
    painter.setPen(pen);
    switch (m_dockPosition) {
      case West:
      case East:
        painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
        painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
        break;
      case North:
      case South:
        painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
        painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
        break;
      default:
        break;
    }
  }
}

//====================================================================
//=== DockCorner
//====================================================================
DockCorner::DockCorner(CornerType type,
                       DockPosition position,
                       AbstractDockWidget *parent)
  : DockItem(position, parent)
  , m_type(type)
{}

void
DockCorner::paint(QPainter& painter)
{
  DockItem::paint(painter);
  painter.fillRect(m_rect, m_parent->backColor());
  painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
  painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
}

DockPosition
DockCorner::dockPosition()
{
  return m_dockPosition;
}

void
DockCorner::setDockPosition(DockPosition position)
{
  m_dockPosition = position;
}

CornerType
DockCorner::type() const
{
  return m_type;
}

QSize
DockCorner::sizeHint() const
{
  // The actual sizes are calculated within
  return QSize(0, 0);
}

void
DockCorner::setWidth(int newWidth)
{
  m_width = newWidth;
}

int
DockCorner::height() const
{
  return m_height;
}

void
DockCorner::setHeight(int newHeight)
{
  m_height = newHeight;
}

int
DockCorner::width() const
{
  return m_width;
}
