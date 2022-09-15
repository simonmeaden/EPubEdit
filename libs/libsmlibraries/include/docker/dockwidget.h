#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDialog>
#include <QIcon>
#include <QLayout>
#include <QList>
#include <QListWidget>
#include <QMap>
#include <QMenu>
#include <QPainter>
#include <QPair>
#include <QResizeEvent>
#include <QToolTip>
#include <QWidget>
#include <QWidgetItem>

#include "utilities/x11colors.h"

#include "docker/abstractdockwidget.h"

class Config;
class DockToolbar;
class DockFooter;
class DockHeader;
class DockCorner;

class DockCorner;
class AbstractDockItem;
class WidgetItem;

/*!
 * \ingroup docker
 * \class DockWidget dockwidget.h "docker/dockwidget.h"
 * \brief The DockWidget defines an QWidget that emulates the QtCreator
 * Toolbar.
 *
 * It allows up to four toolbars at the est, West, North and South positions
 * plus a single header and a single footer. Various widgets can be added to
 * the toolbars and footers.
 */
class DockWidgetPrivate;
class DockWidget : public AbstractDockWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockWidget)
public:
  /*!
   * Construct a DockWidget that is a child of parent.
   */
  explicit DockWidget(QWidget* parent);

//  /*!
//   * \brief Returns the central widget.
//   *
//   * \sa setWidget(QWidget*)
//   */
//  QWidget* widget();

//  /*!
//   * \brief Sets the central widget of the DockWidget.
//   *
//   * If the new QWidget is replacing an existing widget a pointer tothe old
//   * QWidget is returned, otherwise a nullptr is returned. The old QWidget is
//   * NOT deleted.
//   *
//   * \note If the QWidget is replaced with itself, the pointer is returned but
//   * nothing else is done. You will need to delete it yourself.
//   *
//   * \sa widget()
//   */
//  QWidget* setWidget(QWidget* widget);

  /*!
   * \brief Adds a new ToolbarWidget at the new position, if that position is
   * empty.
   *
   * If a ToolbarWidget already exists at that position then this is ignored.
   * If it a toolbar is to be replaced call removeToolbar(WidgetPosition)
   * first.
   */
  DockToolbar* addToolbar(DockPosition position);

  /*!
   * \brief Removes and deletes an existing ToolbarWidget at position, if it
   * exists, otherwise nothing is done.
   */
  void removeToolbar(DockPosition position);
  /*!
   * \brief Removes and deletes the specified ToolbarWidget at toolbar, if it
   * exists, otherwise nothing is done.
   */
  void removeToolbar(DockToolbar* toolbar);
  /*!
   * \brief Hides an existing ToolbarWidget at position, if it
   * exists, otherwise nothing is done.
   */
  void hideToolbar(DockPosition position);
  /*!
   * \brief Hides the specified ToolbarWidget at toolbar, if it
   * exists, otherwise nothing is done.
   */
  void hideToolbar(DockToolbar* toolbar);
  /*!
   * \brief Shows an existing ToolbarWidget at position, if it
   * exists, otherwise nothing is done.
   */
  void showToolbar(DockPosition position);
  /*!
   * \brief Shows the specified ToolbarWidget at toolbar, if it
   * exists, otherwise nothing is done.
   */
  void showToolbar(DockToolbar* toolbar);

  //! Creates and returns DockFooter object for the DockWidget
  DockFooter* addFooter();
  //! Removes the DockFooter object for the DockWidget
  void removeFooter();
  //! Hides the DockFooter object for the DockWidget
  void hideFooter();
  //! Shows the DockFooter object for the DockWidget
  void showFooter();

  //! Creates and returns DockHeader object for the DockWidget
  DockHeader* addHeader();
  //! Removes the DockHeader object for the DockWidget
  void removeHeader();
  //! Hides the DockHeader object for the DockWidget
  void hideHeader();
  //! Shows the DockHeader object for the DockWidget
  void showHeader();

  /*!
   * \brief Moves the ToolbarWidget at oldPosition to newPosition, if it
   * exists and if there is no ToolbarWidget already at newPosition.
   *
   * If there is no ToolbarWidget at oldPosition, or there is already a
   * ToolbarWidget at newPosition nothing will happen, otherwise the move is
   * made.
   *
   * If there is already an existing ToolbarWidget at newPosition you will
   * need to call removeToolbar(Widgetposition) first.
   */
  bool moveToolbar(DockPosition newPosition, DockPosition oldPosition);

  /*!
   * \brief Creates a Corner of type at position
   */
  void setCorner(DockPosition position, CornerType type);
  /*!
   * \brief Creates a Corner of type at each of the four corners.
   */
  void setCorners(CornerType northWest,
                  CornerType northEast,
                  CornerType southWest,
                  CornerType southEast);

  //! Sets the width and height of the corner at position.
  void setCornerSize(DockPosition position, int width, int height);
  //! Sets the size of the corner at position.
  void setCornerSize(DockPosition position, QSize size);
  //! Sets the width of the corner at position.
  void setCornerWidth(DockPosition position, int width);
  //! Sets the height of the corner at position.
  void setCornerHeight(DockPosition position, int height);

  //! \reimplements{QWidget::paintEvent}
  void paintEvent(QPaintEvent* event) override;
  //! Implements a Hover Enter event
  void hoverEnterEvent(QHoverEvent* event);
  //! Implements a Hover Leave event
  void hoverLeaveEvent(QHoverEvent*);
  //! Implements a Hover Move event
  void hoverMoveEvent(QHoverEvent* event);
  //! \reimplements{QWidget::mousePressEvent}
  void mousePressEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::event}
  bool event(QEvent* event) override;

  //! Returns the DockHeader attached to this DockWidget or nullptr if not
  //! created.
  AbstractDockItem* header();
  //! Returns the DockFooter attached to this DockWidget or nullptr if not
  //! created.
  AbstractDockItem* footer();
  //! Returns the top DockToolbar attached to this DockWidget or nullptr if not
  //! created.
  AbstractDockItem* northToolbar();
  //! Returns the bottom DockToolbar attached to this DockWidget or nullptr if
  //! not created.
  AbstractDockItem* southToolbar();
  //! Returns the left DockToolbar attached to this DockWidget or nullptr if not
  //! created.
  AbstractDockItem* westToolbar();
  //! Returns the right DockToolbar attached to this DockWidget or nullptr if
  //! not created.
  AbstractDockItem* eastToolbar();

signals:

protected:
  //! d_ptr constructor.
  DockWidget(DockWidgetPrivate& d);

  /// \cond DO_NOT_DOCUMENT
  DockFooter* m_footer = nullptr;
  DockHeader* m_header = nullptr;
  DockCorner* m_northWest = nullptr;
  DockCorner* m_northEast = nullptr;
  DockCorner* m_southWest = nullptr;
  DockCorner* m_southEast = nullptr;
  DockToolbar* m_northToolbar = nullptr;
  DockToolbar* m_southToolbar = nullptr;
  DockToolbar* m_eastToolbar = nullptr;
  DockToolbar* m_westToolbar = nullptr;
  /// \endcond

  //! \reimplements{QWidget::resizeEvent.}
  void resizeEvent(QResizeEvent* event) override;
  //! called when the widget was clicked.
  void widgetWasClicked(QPoint pos);
  //! Sets the corner.
  void setCorner(DockCorner* corner);
  //! Returns the corner in position.
  DockCorner* corner(DockPosition position);

  /*!
   * \brief Creates a clone of the DockWidget, passing it's variable
   * and settings into the supplied 'master'. If the widget parameter is
   * nullptr then it will create a clone of itself.
   */
  QWidget* clone(QWidget* widget) override;

private:
  //  void initGui();
  void calculateGeometry(const QRect& rect);
  DockToolbar* toolbarAt(DockPosition position);
  DockToolbar* toolbarTakeAt(DockPosition position);
  bool setToolbarAt(DockPosition position, DockToolbar* toolbar = nullptr);
};

#endif // DOCKWIDGET_H
