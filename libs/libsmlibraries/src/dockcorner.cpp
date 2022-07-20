#include "dockcorner.h"
#include "abstractdockwidget.h"
#include "docktypes.h"
#include "private/abstractdockitemprivate.h"
#include "widgetitem.h"

class DockCornerPrivate : public AbstractDockItemPrivate
{
  Q_DECLARE_PUBLIC(DockCorner)
public:
  DockCornerPrivate(CornerType type,
                    DockPosition position,
                    AbstractDockWidget* parent,
                    AbstractDockItem* item);

  DockPosition dockPosition();
  void setDockPosition(DockPosition position);

  enum CornerType type() const;

  QSize sizeHint() const;

  //  int width() const;
  //  void setWidth(int width);

  //  int height() const;
  //  void setHeight(int height);

  void paint(QPainter& painter);

  void clone(AbstractDockItem* item);

protected:
  void setType(enum CornerType newType);

private:
  enum CornerType m_type;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
};

//====================================================================
//=== DockCornerPrivate
//====================================================================
DockCornerPrivate::DockCornerPrivate(CornerType type,
                                     DockPosition position,
                                     AbstractDockWidget* parent,
                                     AbstractDockItem* item)
  : AbstractDockItemPrivate(position, parent, item)
  , m_type(type)
{
}

void
DockCornerPrivate::paint(QPainter& painter)
{
  AbstractDockItemPrivate::paint(painter);
  painter.fillRect(m_rect, m_parentDocker->backColor());
  painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
  painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
}

void
DockCornerPrivate::clone(AbstractDockItem* item)
{
  auto corner = qobject_cast<DockCorner*>(item);
  if (corner) {
    AbstractDockItemPrivate::clone(corner);
    corner->setDockPosition(m_dockPosition);
    corner->setType(m_type);
    corner->setHeight(m_height);
    corner->setWidth(m_width);
  }
}

void
DockCornerPrivate::setType(CornerType newType)
{
  m_type = newType;
}

DockPosition
DockCornerPrivate::dockPosition()
{
  return m_dockPosition;
}

void
DockCornerPrivate::setDockPosition(DockPosition position)
{
  m_dockPosition = position;
}

// CornerType
// DockCornerPrivate::type() const
//{
//   return m_type;
// }

QSize
DockCornerPrivate::sizeHint() const
{
  // The actual sizes are calculated within
  return QSize(0, 0);
}

// void
// DockCornerPrivate::setWidth(int width)
//{
//   m_width = width;
// }

// int
// DockCorner::height()
//{
//   return m_height;
// }

// void
// DockCorner::setHeight(int height)
//{
//   m_height = height;
// }

// int
// DockCornerPrivate::width() const
//{
//   return m_width;
// }

//====================================================================
//=== DockCorner
//====================================================================
DockCorner::DockCorner(CornerType type,
                       DockPosition position,
                       AbstractDockWidget* parent)
  : AbstractDockItem(*new DockCornerPrivate(type, position, parent, this))
{
}

DockCorner::DockCorner(DockCornerPrivate& d)
  : AbstractDockItem(d)
{
}

void
DockCorner::paint(QPainter& painter)
{
  d_ptr->paint(painter);
}

void
DockCorner::clone(AbstractDockItem* item)
{
  Q_D(DockCorner);
  d->clone(item);
}

DockPosition
DockCorner::dockPosition()
{
  return d_ptr->dockPosition();
}

void
DockCorner::setDockPosition(DockPosition position)
{
  Q_D(DockCorner);
  d->m_dockPosition = position;
}

CornerType
DockCorner::type()
{
  Q_D(DockCorner);
  return d->m_type;
}

void
DockCorner::setType(CornerType type)
{
  Q_D(DockCorner);
  d->m_type = type;
}

QSize
DockCorner::sizeHint() const
{
  return d_ptr->sizeHint();
}
