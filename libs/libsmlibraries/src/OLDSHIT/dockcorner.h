#ifndef DOCKCORNER_H
#define DOCKCORNER_H

#include "abstractdockitem.h"

/*!
 * \class DockCorner dockitem.h "includes/dockitem"
 * \brief The DockCorner class defines the appearance of the corners of the dockwidget.
 */
class DockCornerPrivate;
class DockCorner : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockCorner)
public:
  DockCorner(CornerType type,
             DockPosition position,
             AbstractDockWidget* parent);

  DockPosition dockPosition();
  void setDockPosition(DockPosition position);

  CornerType type();
  void setType(CornerType type);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  void clone(AbstractDockItem *item);


protected:
  DockCorner(DockCornerPrivate& d);

private:

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
};

#endif // DOCKCORNER_H
