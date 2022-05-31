#include "dockwidget.h"

#include <QDebug>

using namespace std;

//====================================================================
//=== DockWidget
//====================================================================
DockWidget::DockWidget(QWidget* parent)
  : QWidget{ parent }
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_spacerColor(QColorConstants::X11::DimGrey)
{
  setContentsMargins(0, 0, 0, 0);
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);

  initGui();
}

QWidget*
DockWidget::centreWidget() const
{
  return m_centralWidget;
}

void
DockWidget::setCentalWidget(QWidget* centralWidget)
{
  m_centralWidget = centralWidget;
  //  m_layout->setCentralWidget(m_centralWidget);
}

void
DockWidget::calculateGeometry(const QRect& rect)
{
  auto centreHeight = rect.height();
  auto centreWidth = rect.width();
  auto eastWidth = 0;
  auto oldEastWidth = eastWidth;
  auto eastHeight = centreHeight;
  auto eastX = 0;
  auto eastY = 0;
  auto westWidth = 0;
  auto oldWestWidth = westWidth;
  auto westHeight = centreHeight;
  auto westX = 0;
  auto westY = 0;
  auto northHeight = 0;
  auto oldNorthHeight = northHeight;
  auto northWidth = centreWidth;
  auto northX = 0;
  auto northY = 0;
  auto southHeight = 0;
  auto oldSouthHeight = southHeight;
  auto southWidth = centreWidth;
  auto southX = 0;
  auto southY = 0;

  //  QLayout::setGeometry(rect);

  QSize sizehint;

  if (m_header) {
    sizehint = m_header->sizeHint();
    northHeight += sizehint.height();
    centreHeight -= sizehint.height();
  }

  if (m_northToolbar) {
    sizehint = m_northToolbar->sizeHint();
    northHeight += sizehint.height();
    centreHeight -= sizehint.height();
  }

  if (m_footer) {
    sizehint = m_footer->sizeHint();
    southHeight += sizehint.height();
    centreHeight -= sizehint.height();
  }

  if (m_southToolbar) {
    sizehint = m_southToolbar->sizeHint();
    southHeight += sizehint.height();
    centreHeight -= sizehint.height();
  }

  if (m_eastToolbar) {
    sizehint = m_eastToolbar->sizeHint();
    eastWidth += sizehint.width();
    centreWidth -= sizehint.width();
  }

  if (m_westToolbar) {
    sizehint = m_westToolbar->sizeHint();
    westWidth += sizehint.width();
    centreWidth -= sizehint.width();
  }

  if (m_northEast) {
    switch (m_northEast->type()) {
      case Box: {
        m_northEast->calculateGeometry(
          QRect(rect.x() + rect.width() - eastWidth,
                rect.y(),
                eastWidth,
                northHeight));
        northWidth -= eastWidth;
        eastHeight -= northHeight;
        eastX = rect.width() - eastWidth;
        eastY = northHeight;
        break;
      }
      case VEdge: {
        m_northEast->calculateGeometry(
          QRect(rect.width(), rect.y(), 0, northHeight));
        eastHeight -= northHeight;
        eastX = rect.width() - eastWidth;
        eastY = northHeight;
        break;
      }
      case HEdge: {
        m_northEast->calculateGeometry(
          QRect(rect.width() - eastWidth, rect.y(), eastWidth, 0));
        northWidth -= eastWidth;
        eastX = rect.width() - eastWidth;
        break;
      }
      case None:
        break;
    }
  }

  if (m_northWest) {
    switch (m_northWest->type()) {
      case Box: {
        m_northWest->calculateGeometry(
          QRect(rect.x(), rect.y(), westWidth, northHeight));
        northWidth -= westWidth;
        northX = westWidth;
        westHeight -= northHeight;
        westY = northHeight;
        break;
      }
      case VEdge: {
        m_northWest->calculateGeometry(
          QRect(rect.x(), rect.y(), 0, northHeight));
        northX = westWidth;
        break;
      }
      case HEdge: {
        m_northWest->calculateGeometry(QRect(rect.x(), rect.y(), westWidth, 0));
        northWidth -= westWidth;
        northX = westWidth;
        break;
      }
      case None:
        break;
    }
  }

  if (m_southEast) {
    switch (m_southEast->type()) {
      case Box: {
        m_southEast->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, eastWidth, southHeight));
        southWidth -= eastWidth;
        eastHeight -= southHeight;
        break;
      }
      case VEdge: {
        m_southEast->calculateGeometry(
          QRect(rect.width(), rect.height() - southHeight, 0, southHeight));
        eastHeight -= southHeight;
        break;
      }
      case HEdge: {
        m_southEast->calculateGeometry(QRect(
          rect.width() - eastWidth, rect.height() - southHeight, eastWidth, 0));
        southWidth -= eastWidth;
        break;
      }
      case None:
        break;
    }
  }

  if (m_southWest) {
    switch (m_southWest->type()) {
      case Box: {
        m_southWest->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, westWidth, southHeight));
        southWidth -= westWidth;
        southX = westWidth;
        southY = rect.height() - southHeight;
        westHeight -= southHeight;
        break;
      }
      case VEdge: {
        m_southWest->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, 0, southHeight));
        southY = rect.height() - southHeight;
        westHeight -= southHeight;
        break;
      }
      case HEdge: {
        m_southWest->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, westWidth, 0));
        southX = westWidth;
        southY = rect.height() - southHeight;
        southWidth -= westWidth;
        break;
      }
      case None:
        break;
    }
  }

  if (m_westToolbar) {
    m_westToolbar->calculateGeometry(
      QRect(westX, westY, westWidth, westHeight));
  }

  if (m_eastToolbar) {
    m_eastToolbar->calculateGeometry(
      QRect(eastX, eastY, eastWidth, eastHeight));
  }

  int h = 0;
  if (m_northToolbar) {
    h = m_northToolbar->height();
    m_northToolbar->calculateGeometry(QRect(northX, northY, northWidth, h));
  }

  if (m_header) {
    m_header->calculateGeometry(QRect(northX, h, northWidth, northHeight - h));
  }

  if (m_southToolbar) {
    h = m_southToolbar->height();
    m_southToolbar->calculateGeometry(QRect(southX, southY, southWidth, h));
  }

  if (m_footer) {
    m_footer->calculateGeometry(
      QRect(southX, southY + h, southWidth, southHeight - h));
  }

  if (m_centralWidget) {
    m_centralWidget->setGeometry(westWidth, westY, centreWidth, centreHeight);
  }

  if (m_northEast) {
    switch (m_northEast->type()) {
      case Box: {
        m_northEast->calculateGeometry(
          QRect(eastX, northY, eastWidth, northHeight));
        break;
      }
      case VEdge: {
        m_northEast->calculateGeometry(QRect(rect.y(), northY, 0, northHeight));
        break;
      }
      case HEdge: {
        m_northEast->calculateGeometry(QRect(eastX, northY, eastWidth, 0));
        break;
      }
      case None:
        break;
    }
  }

  if (m_northWest) {
    switch (m_northWest->type()) {
      case Box: {
        m_northWest->calculateGeometry(
          QRect(westX, northY, westWidth, northHeight));
        break;
      }
      case VEdge: {
        m_northWest->calculateGeometry(QRect(westX, northY, 0, northHeight));
        break;
      }
      case HEdge: {
        m_northWest->calculateGeometry(QRect(westX, northY, westWidth, 0));
        break;
      }
      case None:
        break;
    }
  }

  if (m_southEast) {
    switch (m_southEast->type()) {
      case Box: {
        m_southEast->calculateGeometry(
          QRect(eastX, southY, eastWidth, southHeight));
        break;
      }
      case VEdge: {
        m_southEast->calculateGeometry(QRect(rect.y(), southY, 0, southHeight));
        break;
      }
      case HEdge: {
        m_southEast->calculateGeometry(QRect(eastX, southY, eastWidth, 0));
        break;
      }
      case None:
        break;
    }
  }

  if (m_southWest) {
    switch (m_southWest->type()) {
      case Box: {
        m_southWest->calculateGeometry(
          QRect(westX, southY, westWidth, southHeight));
        break;
      }
      case VEdge: {
        m_southWest->calculateGeometry(QRect(westX, southY, 0, southHeight));
        break;
      }
      case HEdge: {
        m_southWest->calculateGeometry(QRect(westX, southY, westWidth, 0));
        break;
      }
      case None:
        break;
    }
  }

  if ((oldNorthHeight != northHeight) && (oldSouthHeight != southHeight) &&
      (oldEastWidth != eastWidth) && (oldWestWidth != westWidth))
    emit geometryChanged(northWidth, southWidth, eastWidth, westWidth);
}

void
DockWidget::initGui()
{
  auto f = font();
  f.setPointSize(8);
  setFont(f);

  //  m_layout = new DockLayout(this);
  //  m_layout->setSpacing(0);
  //  setLayout(m_layout);

  //  connect(this, &DockLayout::geometryChanged, this,
  //  &DockWidget::geometryChanged);
}

DockToolbar*
DockWidget::addToolbar(DockPosition position)
{
  auto toolbar = toolbarAt(position);
  if (toolbar)
    return toolbar;

  toolbar = new DockToolbar(position, this);
  setToolbarAt(position, toolbar);

  repaint();
  return toolbar;
}

void
DockWidget::removeToolbar(DockPosition position)
{
  toolbarTakeAt(position);
}

void
DockWidget::removeToolbar(DockToolbar* toolbar)
{
  if (toolbar) {
    toolbarTakeAt(toolbar->dockPosition());
    toolbar = nullptr;
    repaint();
  }
}

void
DockWidget::hideToolbar(DockPosition position)
{
  auto toolbar = toolbarAt(position);
  if (toolbar) {
    toolbar->setVisible(false);
  }
}

void
DockWidget::hideToolbar(DockToolbar* toolbar)
{
  if (toolbar) {
    toolbar->setVisible(false);
  }
}

void
DockWidget::showToolbar(DockPosition position)
{
  auto toolbar = toolbarAt(position);
  showToolbar(toolbar);
}

void
DockWidget::showToolbar(DockToolbar* toolbar)
{
  if (toolbar) {
    toolbar->setVisible(true);
  }
}

bool
DockWidget::moveToolbar(DockPosition newPosition, DockPosition oldPosition)
{
  auto oldbar = toolbarAt(oldPosition);
  if (!oldbar)
    return false;

  auto newbar = toolbarAt(newPosition);
  if (newbar)
    return false;

  setToolbarAt(oldPosition);
  auto b = setToolbarAt(newPosition, oldbar);
  newbar->deleteLater();
  return b;
}

void
DockWidget::resizeEvent(QResizeEvent* /*event*/)
{
  calculateGeometry(rect());
}

DockToolbar*
DockWidget::toolbarAt(DockPosition position)
{
  switch (position) {
    case North:
      return m_northToolbar;
    case South:
      return m_southToolbar;
    case East:
      return m_eastToolbar;
    case West:
      return m_westToolbar;
    default:
      return nullptr;
  }
}

DockToolbar*
DockWidget::toolbarTakeAt(DockPosition position)
{
  DockToolbar* toolbar = nullptr;
  switch (position) {
    case North:
      toolbar = m_northToolbar;
      m_northToolbar->deleteLater();
      m_northToolbar = nullptr;
      break;
    case South:
      toolbar = m_southToolbar;
      m_southToolbar->deleteLater();
      m_southToolbar = nullptr;
      break;
    case East:
      toolbar = m_eastToolbar;
      m_eastToolbar->deleteLater();
      m_eastToolbar = nullptr;
      break;
    case West:
      toolbar = m_westToolbar;
      m_westToolbar->deleteLater();
      m_westToolbar = nullptr;
      break;
    default:
      return toolbar;
  }
  return nullptr;
}

bool
DockWidget::setToolbarAt(DockPosition position, DockToolbar* toolbar)
{
  switch (position) {
    case North:
      m_northToolbar = toolbar;
      break;
    case South:
      m_southToolbar = toolbar;
      break;
    case East:
      m_eastToolbar = toolbar;
      break;
    case West:
      m_westToolbar = toolbar;
      break;
    default:
      return false;
  }
  return true;
}

DockFooter*
DockWidget::addFooter()
{
  if (!m_footer) {
    m_footer = new DockFooter(this);
    repaint();
    return m_footer;
  }
  return nullptr;
}

void
DockWidget::removeFooter()
{
  if (m_footer) {
    m_footer->deleteLater();
    m_footer = nullptr;
    repaint();
  }
}

void
DockWidget::hideFooter()
{
  if (m_footer)
    m_footer->setVisible(false);
}

void
DockWidget::showFooter()
{
  if (m_footer)
    m_footer->setVisible(true);
}

DockHeader*
DockWidget::addHeader()
{
  if (!m_header) {
    m_header = new DockHeader(this);
    repaint();
    return m_header;
  }
  return nullptr;
}

void
DockWidget::removeHeader()
{
  if (m_header) {
    m_header->deleteLater();
    m_header = nullptr;
    repaint();
  }
}

void
DockWidget::hideHeader()
{
  if (m_header)
    m_header->setVisible(false);
}

void
DockWidget::showHeader()
{
  if (m_header)
    m_header->setVisible(true);
}

WidgetItem*
DockWidget::addToolbarIconButton(DockPosition toolbarPos,
                                 WidgetPosition pos,
                                 QImage icon,
                                 const QString& tooltip)
{
  auto toolbar = toolbarAt(toolbarPos);
  if (toolbar) {
    auto widget = toolbar->addIconButton(pos, icon, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetItem*
DockWidget::addFooterIconButton(WidgetPosition pos,
                                QImage icon,
                                const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (m_footer)
    widget = m_footer->addIconButton(pos, icon, tooltip);
  return widget;
}

WidgetItem*
DockWidget::addHeaderIconButton(WidgetPosition pos,
                                QImage icon,
                                const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (m_header)
    widget = m_header->addIconButton(pos, icon, tooltip);
  return widget;
}

WidgetItem*
DockWidget::addToolbarIconTextButton(DockPosition toolbarPos,
                                     WidgetPosition position,
                                     QImage icon,
                                     const QString& text,
                                     Arrangement textPos,
                                     const QString& tooltip)
{
  auto toolbar = toolbarAt(toolbarPos);

  if (toolbar) {
    auto widget =
      toolbar->addIconTextButton(position, icon, text, textPos, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetItem*
DockWidget::addHeaderIconTextButton(WidgetPosition position,
                                    QImage icon,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (m_header)
    widget =
      m_header->addIconTextButton(position, icon, text, textPos, tooltip);

  return widget;
}

WidgetItem*
DockWidget::addFooterIconTextButton(WidgetPosition position,
                                    QImage icon,
                                    const QString& text,
                                    Arrangement textPos,
                                    const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (m_footer)
    widget =
      m_footer->addIconTextButton(position, icon, text, textPos, tooltip);
  return widget;
}

WidgetItem*
DockWidget::addToolbarTextButton(DockPosition toolbarPos,
                                 WidgetPosition position,
                                 const QString& text,
                                 const QString& tooltip)
{
  auto toolbar = toolbarAt(toolbarPos);
  if (!toolbar) {
    toolbar = addToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget = toolbar->addTextButton(position, text, tooltip);
    return widget;
  }
  return nullptr;
}

WidgetItem*
DockWidget::addHeaderTextButton(WidgetPosition position,
                                const QString& text,
                                const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (!m_header) {
    m_header = new DockHeader(this);
  }
  widget = m_header->addTextButton(position, text, tooltip);
  return widget;
}

WidgetItem*
DockWidget::addFooterTextButton(WidgetPosition position,
                                const QString& text,
                                const QString& tooltip)
{
  WidgetItem* widget = nullptr;
  if (!m_footer) {
    m_footer = new DockFooter(this);
  }
  widget = m_footer->addTextButton(position, text, tooltip);
  return widget;
}

WidgetItem*
DockWidget::addToolbarSpacer(DockPosition toolbarPos, WidgetPosition position)
{
  auto toolbar = toolbarAt(toolbarPos);
  if (!toolbar) {
    toolbar = addToolbar(toolbarPos);
  }

  if (toolbar) {
    auto widget = toolbar->addSpacer(position);
    return widget;
  }
  return nullptr;
}

WidgetItem*
DockWidget::addHeaderSpacer(WidgetPosition position)
{
  WidgetItem* widget = nullptr;
  if (!m_header) {
    m_header = new DockHeader(this);
  }
  widget = m_header->addSpacer(position);
  return widget;
}

WidgetItem*
DockWidget::addFooterSpacer(WidgetPosition position)
{
  WidgetItem* widget = nullptr;
  if (!m_footer) {
    m_footer = new DockFooter(this);
  }
  widget = m_footer->addSpacer(position);
  return widget;
}

void
DockWidget::setCorners(CornerType northWest,
                       CornerType northEast,
                       CornerType southWest,
                       CornerType southEast)
{
  if (northWest != None) {
    m_northWest = new DockCorner(northWest, NorthEast, this);
  }
  if (northEast != None) {
    m_northEast = new DockCorner(northEast, NorthWest, this);
  }
  if (southWest != None) {
    m_southWest = new DockCorner(southWest, SouthEast, this);
  }
  if (southEast != None) {
    m_southEast = new DockCorner(southEast, SouthWest, this);
  }
}

void
DockWidget::setCornerSize(DockPosition position, int width, int height)
{
  auto c = corner(position);
  if (c) {
    c->setWidth(width);
    c->setHeight(height);
  }
}

void
DockWidget::setCornerSize(DockPosition position, QSize size)
{
  setCornerSize(position, size.width(), size.height());
}

void
DockWidget::setCornerWidth(DockPosition position, int width)
{
  auto c = corner(position);
  if (c) {
    c->setWidth(width);
  }
}

void
DockWidget::setCornerHeight(DockPosition position, int height)
{
  auto c = corner(position);
  if (c) {
    c->setHeight(height);
  }
}

void
DockWidget::paintEvent(QPaintEvent* /*event*/)
{
  QPainter painter(this);

  if (m_northToolbar)
    m_northToolbar->paint(painter);

  if (m_southToolbar)
    m_southToolbar->paint(painter);

  if (m_eastToolbar)
    m_eastToolbar->paint(painter);

  if (m_westToolbar)
    m_westToolbar->paint(painter);

  if (m_footer)
    m_footer->paint(painter);

  if (m_header)
    m_header->paint(painter);

  if (m_northEast)
    m_northEast->paint(painter);

  if (m_northWest)
    m_northWest->paint(painter);

  if (m_southEast)
    m_southEast->paint(painter);

  if (m_southWest)
    m_southWest->paint(painter);
}

bool
DockWidget::dockItemHoverCheck(DockItem* item, QPoint pos)
{
  auto widgets = item->widgets();
  for (auto& w : widgets) {
    if (w->rect().contains(pos)) {
      if (m_hoverItem && w != m_hoverItem) {
        m_hoverItem->setHoverOver(false);
        QToolTip::hideText();
        m_hoverItem = nullptr;
      }
      w->setHoverOver(true);
      QToolTip::showText(mapToGlobal(pos), w->tooltip(), this, w->rect());
      m_hoverItem = w;
      repaint();
      return true;
    }
  }
  return false;
}

void
DockWidget::hoverEnterEvent(QHoverEvent* event)
{
  auto p = /*mapToGlobal(*/ event->pos() /*)*/;

  if (m_footer) {
    if (dockItemHoverCheck(m_footer, p)) {
      return;
    }
  }
  if (m_header) {
    if (dockItemHoverCheck(m_header, p)) {
      return;
    }
  }
  if (dockItemHoverCheck(m_eastToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_westToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_southToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_northToolbar, p)) {
    return;
  }
}

void
DockWidget::hoverLeaveEvent(QHoverEvent* /*event*/)
{
  if (m_hoverItem) {
    m_hoverItem->setHoverOver(false);
    QToolTip::hideText();
    m_hoverItem = nullptr;
  }
}

void
DockWidget::hoverMoveEvent(QHoverEvent* event)
{
  auto p = /*mapToGlobal(*/ event->pos() /*)*/;

  if (m_footer) {
    if (dockItemHoverCheck(m_footer, p)) {
      return;
    }
  }
  if (m_header) {
    if (dockItemHoverCheck(m_header, p)) {
      return;
    }
  }
  if (dockItemHoverCheck(m_eastToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_westToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_southToolbar, p)) {
    return;
  }

  if (dockItemHoverCheck(m_northToolbar, p)) {
    return;
  }
}

void
DockWidget::mouseClickCheck(DockItem* item, QPoint pos)
{
  auto widgets = item->widgets();

  for (auto& w : widgets) {
    if (w->rect().contains(pos)) {
      if (w->isEnabled()) {
        switch (w->type()) {
          case Button: {
            emit w->widgetClicked();
            break;
          }
          default:
            break;
        }
      }
    }
  }
}

void
DockWidget::mousePressEvent(QMouseEvent* event)
{
  //  auto pos = mapToGlobal(event->pos());
  auto pos = event->pos();

  if (m_footer) {
    mouseClickCheck(m_footer, pos);
  }
  if (m_header) {
    mouseClickCheck(m_header, pos);
  }
  // TODO the rest of the widgets.
}

void
DockWidget::mouseReleaseEvent(QMouseEvent* /*event*/)
{
  // TODO possibly remove if not used.
}

bool
DockWidget::event(QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnterEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverLeave:
      hoverLeaveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    case QEvent::HoverMove:
      hoverMoveEvent(static_cast<QHoverEvent*>(event));
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}

const QColor&
DockWidget::textColor() const
{
  return m_textColor;
}

void
DockWidget::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QColor&
DockWidget::spacerColor() const
{
  return m_spacerColor;
}

void
DockWidget::setSpacerColor(const QColor& newSpacerColor)
{
  m_spacerColor = newSpacerColor;
}

const QBrush&
DockWidget::backColor() const
{
  return m_backColor;
}

void
DockWidget::setBackColor(const QBrush& newBackColor)
{
  m_backColor = newBackColor;
}

const QBrush&
DockWidget::hoverBackColor() const
{
  return m_hoverBackColor;
}

void
DockWidget::setHoverBackColor(const QBrush& newHoverBackColor)
{
  m_hoverBackColor = newHoverBackColor;
}

const QBrush&
DockWidget::selectedColor() const
{
  return m_selectedColor;
}

void
DockWidget::setSelectedColor(const QBrush& newSelectedColor)
{
  m_selectedColor = newSelectedColor;
}

void
DockWidget::setCorner(DockPosition position, CornerType type)
{
  switch (position) {
    case NorthEast: {
      m_northEast = new DockCorner(type, position, this);
      break;
    }
    case NorthWest: {
      m_northWest = new DockCorner(type, position, this);
      break;
    }
    case SouthEast: {
      m_southEast = new DockCorner(type, position, this);
      break;
    }
    case SouthWest: {
      m_southWest = new DockCorner(type, position, this);
      break;
    }
    default:
      break;
  }
}

DockCorner*
DockWidget::corner(DockPosition position)
{
  switch (position) {
    case NorthEast:
      return m_northEast;
    case NorthWest:
      return m_northWest;
    case SouthEast:
      return m_southEast;
    case SouthWest:
      return m_southWest;
    default:
      return nullptr;
  }
}

void
DockWidget::setCorner(DockCorner* corner)
{
  switch (corner->dockPosition()) {
    case NorthEast:
      m_northEast = corner;
      break;
    case NorthWest:
      m_northWest = corner;
      break;
    case SouthEast:
      m_southEast = corner;
      break;
    case SouthWest:
      m_southWest = corner;
      break;
    default:
      break;
  }
}

//====================================================================
//=== WidgetWrapper
//====================================================================
WidgetItem::WidgetItem(DockWidget* parent)
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

// void
// WidgetItem::setRect(const QRect& newRect)
//{
//   m_rect = newRect;
// }

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

Arrangement
WidgetItem::arrangement() const
{
  return m_arrangement;
}

void
WidgetItem::setArrangement(Arrangement newArrangement)
{
  m_arrangement = newArrangement;
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
  if (isHoverOver()) {
    painter.fillRect(m_rect, m_parent->hoverBackColor());
  } else if (isSelected()) {
    painter.fillRect(m_rect, m_parent->selectedColor());
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
ButtonWidget::ButtonWidget(DockWidget* parent)
  : WidgetItem(parent)
  , m_textColor(QColorConstants::X11::LightGrey)
{}

const QRect&
ButtonWidget::iconRect() const
{
  return m_iconRect;
}

void
ButtonWidget::setIconRect(const QRect& newIconRect)
{
  m_iconRect = newIconRect;
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
}

const QSize
ButtonWidget::calcMinimumSize()
{
  int width = 0, height = 0, value = 0;
  int iconWidth = 0, iconHeight = 0, textWidth = 0, textHeight = 0;

  if (!icon().isNull()) {
    iconWidth = iconSize().width();
    iconHeight = iconSize().height();
  }

  if (!text().isEmpty()) {
    textWidth = m_fontMetrics.horizontalAdvance(text());
    textHeight = m_fontMetrics.height();
  }

  switch (arrangement()) {
    case TextAboveIcon:
    case TextBelowIcon: {
      value = (iconWidth > textWidth ? iconWidth : textWidth);
      width = max(value, width);
      m_minContentSize.setWidth(value);
      value = iconHeight + spacer() + textHeight;
      m_minContentSize.setHeight(value);
      height = max(iconHeight, height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    }
    case TextToLeft:
    case TextToRight: {
      value = iconWidth + spacer() + textWidth;
      m_minContentSize.setWidth(value);
      width = max(value, width);
      value = (iconHeight > textHeight ? iconHeight : textHeight);
      height = max(iconHeight, height);
      m_minContentSize.setHeight(height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    }
    case IconOnly: {
      width = max(iconWidth, width);
      m_minContentSize.setWidth(width);
      height = max(iconHeight, height);
      m_minContentSize.setHeight(height);
      m_iconRect = QRect(0, 0, iconWidth, iconHeight);
      break;
    }
    case TextOnly:
      width = max(textWidth, width);
      m_minContentSize.setWidth(width);
      height = max(textHeight, height);
      m_minContentSize.setHeight(height);
      m_textRect = QRect(0, 0, textWidth, textHeight);
      break;
    case NoArrangement:
      break;
  }
  return QSize(width, height);
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

  int x = 0, y = 0, max = 0;

  switch (arrangement()) {
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

//====================================================================
//=== SpacerWrapper
//====================================================================
SpacerWidget::SpacerWidget(DockWidget* parent)
  : WidgetItem(parent)
  , m_color(QColorConstants::X11::DimGray)
{}

const QSize
SpacerWidget::calcMinimumSize()
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
SpacerWidget::color() const
{
  return m_color;
}

void
SpacerWidget::setColor(const QColor& newColor)
{
  m_color = newColor;
}

int
SpacerWidget::thickness() const
{
  return m_thickness;
}

void
SpacerWidget::setThickness(int newThickness)
{
  m_thickness = newThickness;
}

void
SpacerWidget::paint(QPainter& painter)
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

//====================================================================
//=== BaseDockWidget
//====================================================================
DockItem::DockItem(DockPosition position, DockWidget* parent)
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
DockItem::addIconButton(WidgetPosition pos, QImage icon, const QString& tooltip)
{
  WidgetItem* w =
    createWidgetItem(Button, pos, icon, QString(), IconOnly, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addIconTextButton(WidgetPosition pos,
                            QImage icon,
                            const QString& text,
                            Arrangement textPos,
                            const QString& tooltip)
{
  WidgetItem* w = createWidgetItem(Button, pos, icon, text, textPos, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addTextButton(WidgetPosition pos,
                        const QString& text,
                        const QString& tooltip)
{
  WidgetItem* w =
    createWidgetItem(Button, pos, QImage(), text, TextOnly, tooltip);
  calcWidgetSizes();
  connect(w, &WidgetItem::widgetChanged, this, &DockItem::calcWidgetSizes);
  return w;
}

WidgetItem*
DockItem::addSpacer(WidgetPosition pos)
{
  WidgetItem* w =
    createWidgetItem(Spacer, pos, QImage(), QString(), IconOnly, QString());
  return w;
}

WidgetItem*
DockItem::addCustomWidget(CustomWidget* w)
{
  m_widgets.append(w);
  calcWidgetSizes();
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
DockItem::createWidgetItem(WidgetType type,
                           WidgetPosition pos,
                           QImage image,
                           const QString& text,
                           Arrangement textPos,
                           const QString& tooltip)
{
  WidgetItem* wrapper = nullptr;

  switch (type) {
    case Button: {
      auto bw = new ButtonWidget(m_parent);
      bw->setIcon(QIcon(QPixmap::fromImage(image)));
      bw->setIconSize(image.size());
      bw->setText(text);
      bw->setArrangement(textPos);
      bw->setTooltip(tooltip);
      wrapper = bw;
      int height = 0;
      int width = 0;

      switch (textPos) {
        case TextAboveIcon:
        case TextBelowIcon: {
          width = m_fontMetrics.horizontalAdvance(text);
          width =
            (width > bw->iconSize().width() ? width : bw->iconSize().width());
          m_maxWidgetWidth = max(width, m_maxWidgetWidth);
          height = bw->topMargin() + m_fontMetrics.height() + bw->spacer() +
                   bw->iconSize().height() + bw->bottomMargin();
          m_maxWidgetHeight = max(height, m_maxWidgetHeight);
          break;
        }
        case TextToRight:
        case TextToLeft: {
          width = bw->leftMargin() + m_fontMetrics.horizontalAdvance(text) +
                  bw->spacer() + bw->iconSize().width() + bw->rightMargin();
          m_maxWidgetWidth = max(width, m_maxWidgetWidth);
          height = bw->topMargin() + height + bw->bottomMargin();
          m_maxWidgetHeight = max(height, m_maxWidgetHeight);
          break;
        }
        case IconOnly: {
          width = bw->iconSize().width();
          m_maxWidgetWidth = max(width, m_maxWidgetWidth);
          height =
            bw->topMargin() + bw->iconSize().height() + bw->bottomMargin();
          m_maxWidgetHeight = max(height, m_maxWidgetHeight);
          ;
          break;
        }
        case TextOnly: {
          width = m_fontMetrics.horizontalAdvance(text);
          m_maxWidgetWidth = max(width, m_maxWidgetWidth);
          height =
            bw->topMargin() + m_fontMetrics.height() + bw->bottomMargin();
          m_maxWidgetHeight = max(height, m_maxWidgetHeight);
          ;
          break;
        }
        case NoArrangement:
          break;
      }

      break;
    }
    case List: {
      // TODO
      wrapper = nullptr;
      break;
    }
    case Spacer: {
      auto sp = new SpacerWidget(m_parent);
      wrapper = sp;

      break;
    }
    case Custom:
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
    case NorthEast:
    case NorthWest:
    case SouthEast:
    case SouthWest:
    case Center:
      break;
    case East:
    case West: {
      min = rect.y() + 1;
      max = rect.bottom() - TOOLBAR_ENDER;
      for (auto& w : m_widgets) {
        auto size = w->sizeHint();
        auto x = rect.left() + halfDifference(rect.width(), size.width());
        auto widgetHeight =
          (m_itemsSameSize ? m_maxWidgetHeight : size.height());
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
        auto widgetWidth = (m_itemsSameSize ? m_maxWidgetWidth : size.width());
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
DockItem::calcWidgetSizes()
{
  auto maxMargins = calcMaxMargins();
  auto maxContentsSize = calcMaxContentsSize();
  auto w = maxMargins.left() + maxContentsSize.width() + maxMargins.right();
  auto h = maxMargins.top() + maxContentsSize.height() + maxMargins.bottom();

  m_maxWidgetWidth = max(w, m_maxWidgetWidth);
  m_maxWidgetHeight = max(h, m_maxWidgetHeight);
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
    width = max(size.width(), width);
    height = max(size.height(), height);
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
DockFooter::DockFooter(DockWidget* parent)
  : DockItem(South, parent)
{
  setPreferredSize(WIDTH, HEIGHT);
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
DockHeader::DockHeader(DockWidget* parent)
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
DockToolbar::DockToolbar(DockPosition position, DockWidget* parent)
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
                       DockWidget* parent)
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
