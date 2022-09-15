#include "docker/dockcorner.h"
#include "docker/abstractdockwidget.h"
#include "docker/docktypes.h"
#include "docker/private/abstractdockitemprivate.h"
#include "docker/widgetitem.h"

//====================================================================
//=== DockCornerPrivate
//====================================================================

class DockCornerPrivate : public AbstractDockItemPrivate
{
  Q_DECLARE_PUBLIC(DockCorner)
public:
  DockCornerPrivate(CornerType type,
                    DockPosition position,
                    AbstractDockWidget* parent,
                    AbstractDockItem* item)
    : AbstractDockItemPrivate(position, parent, item)
    , m_type(type)
  {
  }

  DockPosition dockPosition() { return m_dockPosition; }
  void setDockPosition(DockPosition position) { m_dockPosition = position; }

  enum CornerType type() const { return m_type; }

  QSize sizeHint() const
  {
    // The actual sizes are calculated within
    return QSize(0, 0);
  }

  void paint(QPainter& painter)
  {
    AbstractDockItemPrivate::paint(painter);
    painter.fillRect(m_rect, m_parentDocker->backColor());
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
  }

  void clone(QObject* item)
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

protected:
  void setType(enum CornerType newType) { m_type = newType; }

private:
  enum CornerType m_type;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
};

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
DockCorner::clone(QObject* item)
{
  Q_D(DockCorner);
  auto anobject = qobject_cast<DockCorner*>(item);
  if (anobject)
    d->clone(anobject);
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
