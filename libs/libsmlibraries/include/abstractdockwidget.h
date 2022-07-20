#ifndef ABSTRACTDOCKWIDGET_H
#define ABSTRACTDOCKWIDGET_H

#include <QHBoxLayout>
#include <QToolTip>
#include <QWidget>

#include "docktypes.h"

class AbstractDockItem;
class WidgetItem;
class DockHeader;

/*!
 * \class AbstractDockWidget  abstractdockwidget.h
 * "include/abstractdockwidget.h" \brief AbstractDockWidget is the base class
 * forall major widgets such as HeaderWidget.
 *
 * \sa DockWidget
 * \sa HeaderWidget
 * \sa FooterWidget
 */
class AbstractDockWidgetPrivate;
class AbstractDockWidget : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractDockWidget)
public:
  /*!
   * Construct an AbstractDockWidget that is a child of parent.
   */
  explicit AbstractDockWidget(QWidget* parent = nullptr);

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

  const QRect& availableRect() const;

  void clone(AbstractDockWidget* widget);

  AbstractDockWidgetPrivate* d_ptr;

signals:

protected:
  AbstractDockWidget(AbstractDockWidgetPrivate& d);
  AbstractDockWidget(AbstractDockWidgetPrivate &d, QWidget *parent);

  //! Checks whether the mouse is hovering over a WidgetItem.
  bool dockItemHoverCheck(AbstractDockItem* item, QPoint pos);
  void mouseClickCheck(AbstractDockItem* item, QPoint pos);
  void checkForOpenListWidgets(AbstractDockItem* item);

private:

  friend class WidgetItem;
  friend class ListButtonWidget;
  friend class ListWidget;
  void initGui();
};

#endif // ABSTRACTDOCKWIDGET_H
