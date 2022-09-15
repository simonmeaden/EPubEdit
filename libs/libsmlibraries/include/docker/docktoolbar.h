#ifndef DOCKTOOLBAR_H
#define DOCKTOOLBAR_H

#include <QFontMetrics>

#include "docker/abstractdockitem.h"

/*!
 * \ingroup docker
 * \class DockToolbar docktoolbar.h "includes/docker/docktoolbar.h"
 * \brief The DockToolbar class defines a DockToolbar object.
 *
 * DockToolbar's are similar to QToolbars in that you can add certain types of
 * widget to them, primarily certain types of ButtonWidget, a LabelWidget or a
 * ListWidget.
 *
 * \sa ButtonWidget
 * \sa ListButtonWidget
 * \sa LabelWidget
 * \sa ListWidget
 * \sa SeperatorWidget
 *
 */
class DockToolbarPrivate;
class DockToolbar : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockToolbar)
public:
  //! Constructor for DockToolbar
  explicit DockToolbar(DockPosition position,
                       AbstractDockWidget* parent);


  //! Returns the position of the toolbar.
  DockPosition dockPosition();
  //! sets the position of the toolbar.
  void setDockPosition(DockPosition position);

  //! Returns a size hint for this object.
  QSize sizeHint() const override;

  //! Paints the DockToolbar and all of it's child widgets.
  void paint(QPainter& painter) override;

  /*!
   * \brief Creates a clone of the DockToolbar into item if item is a
   * DockToolbar.
   */
  void clone(QObject *item) override;

protected:
  //! d_ptr constructor.
  DockToolbar(DockToolbarPrivate& d);

private:
  static const int WIDTH = 60;
  static const int HEIGHT = 60;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class DockWidget;
};

#endif // DOCKTOOLBAR_H
