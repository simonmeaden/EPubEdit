#ifndef ABSTRACTDOCKWIDGET_H
#define ABSTRACTDOCKWIDGET_H

#include <QHBoxLayout>
#include <QToolTip>
#include <QWidget>

#include "docktypes.h"
#include "multisplitter/cloneableinterface.h"

class AbstractDockItem;
class WidgetItem;
class DockHeader;

/*!
 * \ingroup docker
 * \class AbstractDockWidget  abstractdockwidget.h
 * "include/abstractdockwidget.h" \brief AbstractDockWidget is the base class
 * forall major widgets such as HeaderWidget.
 *
 * \sa DockWidget
 * \sa HeaderWidget
 * \sa FooterWidget
 */
class AbstractDockWidgetPrivate;
class AbstractDockWidget
  : public QWidget
  , public CloneableWidgetInterface
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractDockWidget)
public:

  //! Returns the background color
  const QBrush& backColor() const;
  //! Sets the background color
  void setBackColor(const QBrush& backColor);

  //! Returns the hover background color
  const QBrush& hoverBackColor() const;
  //! Sets the hover background color
  void setHoverBackColor(const QBrush& hoverBackColor);

  //! Returns the selected background color
  const QBrush& selectedColor() const;
  //! Sets the selected background color
  void setSelectedColor(const QBrush& selectedColor);

  //! Returns the text color
  const QColor& textColor() const;
  //! Sets the text color
  void setTextColor(const QColor& textColor);

  //! Returns the spacer color
  const QColor& spacerColor() const;
  //! Sets the spacer color
  void setSpacerColor(const QColor& spacerColor);

  //! returns the available space within this DockWidget
  const QRect& availableRect() const;

  /*!
   * \brief Clones the values of this widget to the cloned DockWidget passed as
   * a parameter.
   */
  QWidget* clone(QWidget* widget);

  /*!
   * \brief Returns the central widget.
   *
   * \sa setWidget(QWidget*)
   */
  QWidget* centralWidget();

  /*!
   * \brief Sets the central widget of the DockWidget.
   *
   * If the new QWidget is replacing an existing widget a pointer tothe old
   * QWidget is returned, otherwise a nullptr is returned. The old QWidget is
   * NOT deleted.
   *
   * \note If the QWidget is replaced with itself, the pointer is returned but
   * nothing else is done. You will need to delete it yourself.
   *
   * \sa widget()
   */
  QWidget* setCentralWidget(QWidget* widget);

signals:

protected:
  //! protected d_ptr instance
  AbstractDockWidgetPrivate* d_ptr;

  //! protected d_ptr constructor
  AbstractDockWidget(AbstractDockWidgetPrivate& d);
  //! protected d_ptr constructor
  AbstractDockWidget(AbstractDockWidgetPrivate& d, QWidget* parent);

  //! Checks whether the mouse is hovering over a WidgetItem.
  bool dockItemHoverCheck(AbstractDockItem* item, QPoint pos);
  //! Check carried out when a mouse is clicked.
  void mouseClickCheck(AbstractDockItem* item,
                       QMouseEvent *event);
  //! Checks whether a ListWidget or ListButtonWidget is open and closes it if
  //! it is.
  void checkForOpenListWidgets(AbstractDockItem* item);

private:
  friend class WidgetItem;
  friend class ListButtonWidget;
  friend class ListWidget;
  void initGui();
};

#endif // ABSTRACTDOCKWIDGET_H
