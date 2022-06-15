#include "dockwidget.h"
#include "widgetitem.h"

#include <algorithm>

#include <QDebug>

#include "dockitem.h"
#include "widgetitem.h"

//====================================================================
//=== DockWidget
//====================================================================
DockWidget::DockWidget(QWidget* parent)
  : AbstractDockWidget{ parent }
{
  setContentsMargins(0, 0, 0, 0);
  setAutoFillBackground(true);
  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);

  initGui();
}

QWidget*
DockWidget::widget() const
{
  return m_widget;
}

QWidget*
DockWidget::setWidget(QWidget* centralWidget)
{
  if (centralWidget == m_widget)
    return centralWidget;

  auto oldWidget = m_widget;
  if (oldWidget) {
    m_layout->removeWidget(oldWidget);
  }
  m_widget = centralWidget;
  m_layout->addWidget(m_widget);
  return oldWidget;
}

void
DockWidget::calculateGeometry(const QRect& rect)
{
  auto eastWidth = 0;
  auto eastHeight = rect.height();
  auto eastX = 0;
  auto eastY = 0;
  auto westWidth = 0;
  auto westHeight = rect.height();
  auto westX = 0;
  auto westY = 0;
  auto northHeight = 0;
  auto northToolbarHeight = 0;
  auto headerY = 0;
  auto headerHeight = 0;
  auto northWidth = rect.width();
  auto northX = 0;
  auto southWidth = rect.width();
  auto southHeight = 0;
  auto southX = 0;
  auto southY = rect.height();
  auto footerHeight = 0;
  auto footerY = 0;
  auto southToolbarHeight = 0;
  auto southToolbarY = 0;

  if (m_northToolbar) {
    northToolbarHeight = m_northToolbar->sizeHint().height();
    northHeight += northToolbarHeight;
  }

  if (m_header) {
    headerHeight = m_header->sizeHint().height();
    northHeight += headerHeight;
    headerY = northToolbarHeight;
  }

  if (m_footer) {
    footerHeight = m_footer->sizeHint().height();
    southHeight = footerHeight;
    southY -= footerHeight;
    footerY = southY;
  }

  if (m_southToolbar) {
    southToolbarHeight = m_southToolbar->sizeHint().height();
    southHeight += southToolbarHeight;
    southY -= southToolbarHeight;
    southToolbarY = southY;
  }

  if (m_eastToolbar) {
    eastWidth += m_eastToolbar->sizeHint().width();
  }

  if (m_westToolbar) {
    westWidth += m_westToolbar->sizeHint().width();
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
        westHeight -= southHeight;
        break;
      }
      case VEdge: {
        m_southWest->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, 0, southHeight));
        westHeight -= southHeight;
        break;
      }
      case HEdge: {
        m_southWest->calculateGeometry(
          QRect(rect.x(), rect.height() - southHeight, westWidth, 0));
        southX = westWidth;
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

  if (m_northToolbar) {
    m_northToolbar->calculateGeometry(
      QRect(northX, 0, northWidth, northToolbarHeight));
  }

  if (m_header) {
    m_header->calculateGeometry(
      QRect(northX, headerY, northWidth, headerHeight));
  }

  if (m_southToolbar) {
    m_southToolbar->calculateGeometry(
      QRect(southX, southToolbarY, southWidth, southToolbarHeight));
  }

  if (m_footer) {
    m_footer->calculateGeometry(
      QRect(southX, footerY, southWidth, footerHeight));
  }

  if (m_northEast) {
    switch (m_northEast->type()) {
      case Box: {
        m_northEast->calculateGeometry(QRect(eastX, 0, eastWidth, northHeight));
        break;
      }
      case VEdge: {
        m_northEast->calculateGeometry(QRect(rect.y(), 0, 0, northHeight));
        break;
      }
      case HEdge: {
        m_northEast->calculateGeometry(QRect(eastX, 0, eastWidth, 0));
        break;
      }
      case None:
        break;
    }
  }

  if (m_northWest) {
    switch (m_northWest->type()) {
      case Box: {
        m_northWest->calculateGeometry(QRect(westX, 0, westWidth, northHeight));
        break;
      }
      case VEdge: {
        m_northWest->calculateGeometry(QRect(westX, 0, 0, northHeight));
        break;
      }
      case HEdge: {
        m_northWest->calculateGeometry(QRect(westX, 0, westWidth, 0));
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

  if (m_widget) {
    m_layout->setContentsMargins(
      westWidth + 1, northHeight + 1, eastWidth - 1, southHeight - 1);
  }
}

void
DockWidget::initGui()
{
  auto f = font();
  f.setPointSize(8);
  setFont(f);

  m_layout = new QHBoxLayout;
  setLayout(m_layout);
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
      break;
  }
  return toolbar;
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

// WidgetItem*
// DockWidget::addToolbarIconButton(DockPosition toolbarPos,
//                                  WidgetPosition pos,
//                                  QImage icon,
//                                  const QString& tooltip)
//{
//   auto toolbar = toolbarAt(toolbarPos);
//   if (toolbar) {
//     auto widget = toolbar->addIconButton(pos, icon, tooltip);
//     return widget;
//   }
//   return nullptr;
// }

// WidgetItem*
// DockWidget::addFooterIconButton(WidgetPosition pos,
//                                 QImage icon,
//                                 const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (m_footer)
//     widget = m_footer->addIconButton(pos, icon, tooltip);
//   return widget;
// }

// WidgetItem*
// DockWidget::addHeaderIconButton(WidgetPosition pos,
//                                 QImage icon,
//                                 const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (m_header)
//     widget = m_header->addIconButton(pos, icon, tooltip);
//   return widget;
// }

// WidgetItem*
// DockWidget::addToolbarIconTextButton(DockPosition toolbarPos,
//                                      WidgetPosition position,
//                                      QImage icon,
//                                      const QString& text,
//                                      Arrangement textPos,
//                                      const QString& tooltip)
//{
//   auto toolbar = toolbarAt(toolbarPos);

//  if (toolbar) {
//    auto widget =
//      toolbar->addIconTextButton(position, icon, text, textPos, tooltip);
//    return widget;
//  }
//  return nullptr;
//}

// WidgetItem*
// DockWidget::addHeaderIconTextButton(WidgetPosition position,
//                                     QImage icon,
//                                     const QString& text,
//                                     Arrangement textPos,
//                                     const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (m_header)
//     widget =
//       m_header->addIconTextButton(position, icon, text, textPos, tooltip);

//  return widget;
//}

// WidgetItem*
// DockWidget::addFooterIconTextButton(WidgetPosition position,
//                                     QImage icon,
//                                     const QString& text,
//                                     Arrangement textPos,
//                                     const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (m_footer)
//     widget =
//       m_footer->addIconTextButton(position, icon, text, textPos, tooltip);
//   return widget;
// }

// WidgetItem*
// DockWidget::addToolbarTextButton(DockPosition toolbarPos,
//                                  WidgetPosition position,
//                                  const QString& text,
//                                  const QString& tooltip)
//{
//   auto toolbar = toolbarAt(toolbarPos);
//   if (!toolbar) {
//     toolbar = addToolbar(toolbarPos);
//   }

//  if (toolbar) {
//    auto widget = toolbar->addTextButton(position, text, tooltip);
//    return widget;
//  }
//  return nullptr;
//}

// WidgetItem*
// DockWidget::addHeaderTextButton(WidgetPosition position,
//                                 const QString& text,
//                                 const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (!m_header) {
//     m_header = new DockHeader(this);
//   }
//   widget = m_header->addTextButton(position, text, tooltip);
//   return widget;
// }

// WidgetItem*
// DockWidget::addFooterTextButton(WidgetPosition position,
//                                 const QString& text,
//                                 const QString& tooltip)
//{
//   WidgetItem* widget = nullptr;
//   if (!m_footer) {
//     m_footer = new DockFooter(this);
//   }
//   widget = m_footer->addTextButton(position, text, tooltip);
//   return widget;
// }

// WidgetItem*
// DockWidget::addToolbarSpacer(DockPosition toolbarPos, WidgetPosition
// position)
//{
//   auto toolbar = toolbarAt(toolbarPos);
//   if (!toolbar) {
//     toolbar = addToolbar(toolbarPos);
//   }

//  if (toolbar) {
//    auto widget = toolbar->addSeperator(position);
//    return widget;
//  }
//  return nullptr;
//}

// WidgetItem*
// DockWidget::addHeaderSpacer(WidgetPosition position)
//{
//   WidgetItem* widget = nullptr;
//   if (!m_header) {
//     m_header = new DockHeader(this);
//   }
//   widget = m_header->addSeperator(position);
//   return widget;
// }

// WidgetItem*
// DockWidget::addFooterSpacer(WidgetPosition position)
//{
//   WidgetItem* widget = nullptr;
//   if (!m_footer) {
//     m_footer = new DockFooter(this);
//   }
//   widget = m_footer->addSeperator(position);
//   return widget;
// }

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

// bool
// DockWidget::dockItemHoverCheck(DockItem* item, QPoint pos)
//{
//   if (item) {
//     auto widgets = item->widgets();
//     for (auto& w : widgets) {
//       if (w->rect().contains(pos)) {
//         if (m_hoverItem && w != m_hoverItem) {
//           m_hoverItem->setHoverOver(false);
//           QToolTip::hideText();
//           m_hoverItem = nullptr;
//         }
//         w->setHoverOver(true);
//         QToolTip::showText(mapToGlobal(pos), w->tooltip(), this, w->rect());
//         m_hoverItem = w;
//         repaint();
//         return true;
//       }
//     }
//   }
//   return false;
// }

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

// void
// DockWidget::mouseClickCheck(DockItem* item, QPoint pos)
//{
//   auto widgets = item->widgets();

//  for (auto& w : widgets) {
//    if (w->rect().contains(pos)) {
//      if (w->isEnabled()) {
//        switch (w->type()) {
//          case Button: {
//            emit w->widgetClicked();
//            w->setSelected(true);
//            break;
//          }
//          case List: {
//            //            emit w->widgetClicked();
//            auto listWidget = qobject_cast<ListWidget*>(w);
//            w->setSelected(true);
//            if (listWidget) {
//              listWidget->show(pos, rect());
//            }
//            break;
//          }
//          default:
//            break;
//        }
//      }
//    }
//  }
//}

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
