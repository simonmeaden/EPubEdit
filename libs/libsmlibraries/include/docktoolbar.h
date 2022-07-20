#ifndef DOCKTOOLBAR_H
#define DOCKTOOLBAR_H

#include <QFontMetrics>

#include "abstractdockitem.h"

/*!
 * \class DockToolbar dockitem.h "includes/dockitem.h"
 * \brief The DockToolbar class defines a DockToolbar object.
 */
class DockToolbarPrivate;
class DockToolbar : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockToolbar)
public:
  explicit DockToolbar(DockPosition position,
                       AbstractDockWidget* parent);

  static const int WIDTH = 60;
  static const int HEIGHT = 60;

  DockPosition dockPosition();
  void setDockPosition(DockPosition position);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  /*!
   * \brief Creates a clone of the DockToolbar into item if item is a
   * DockToolbar.
   */
  void clone(AbstractDockItem* item);

protected:
  DockToolbar(DockToolbarPrivate& d);

private:
  friend class HeaderWidget;
  friend class FooterWidget;
  friend class DockWidget;
};

#endif // DOCKTOOLBAR_H
