#ifndef WIDGETITEM_H
#define WIDGETITEM_H

#include <QAction>
#include <QMargins>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>

#include "docktypes.h"

class DockWidget;

class AbstractDockWidget;
class AbstractDockItem;
class ListScroller;
class ListDisplay;

/*!
 * \class WidgetItem widgetitem.h widgetitem.cpp
 * \brief The WidgetItem class is the base class for all dock widget items.
 *
 * WidgetItem provide an abstract class for creation of WidgetItem's.
 */
class WidgetItemPrivate;
class WidgetItem : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(WidgetItem)
public:
  /*!
   * Construct a WidgetItem that is a child of parent.
   *
   * parent connot be a nullptr, the item must have a parent widget.
   */
  explicit WidgetItem(AbstractDockWidget* parent);

  //! Returns the WidgetType of the widget.
  WidgetType type() const;

  //! Sets the type of the widget.
  void setType(WidgetType newType);

  //! Returns the docking position of the widget.
  WidgetPosition widgetPosition() const;
  //! Sets the docking position of the widget.
  void setWidgetPosition(WidgetPosition newPosition);

  //! Returns the value of the optional tooltip, empty if not specified.
  const QString& tooltip() const;
  //! Sets the value of the optional tooltip
  void setTooltip(const QString& newTooltip);

  //! Returns the widget margins
  const QMargins& margins() const;
  //! Sets the widgets margins
  void setMargins(const QMargins& margins);
  //! Returns the left margin
  int leftMargin();
  //! Sets the widgets left margin
  void setLeftMargin(int margin);
  //! Returns the right margin
  int rightMargin();
  //! Sets the widgets right margin
  void setRightMargin(int margin);
  //! Returns the top margin
  int topMargin();
  //! Sets the widgets top margin
  void setTopMargin(int margin);
  //! Returns the bottom margin
  int bottomMargin();
  //! Sets the widgets bottom margin
  void setBottomMargin(int margin);

  //!  Returns the value of the enabled flag
  bool isEnabled() const;
  //! Sets the value of the enabled flag.
  void setEnabled(bool newEnabled);

  //!  Returns the value of the selected flag
  bool isSelected() const;
  //! Sets the value of the selected flag.
  void setSelected(bool selected);

  /*!
   * \brief Hides the widget.
   *
   *  \sa showWidget
   */
  virtual void hideWidget();

  //! Retrieves a QVariant data object from the store.
  const QVariant& data() const;
  //! Sets a QVariant data object from the store.
  void setData(const QVariant& data);

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   * All subclasses of WidgetItem should create their own clone() methods.
   */
  virtual WidgetItem* clone(WidgetItem* item = nullptr);

  bool isHoverOver() const;
  void setHoverOver(bool newHoverOver);
  bool isVisible() const;
  const QRect& rect() const;

  virtual void paint(QPainter& painter) = 0;

  QSize sizeHint() const;

  virtual void setGeometry(const QRect& rect, const QRect& = QRect());
  virtual const QSize calcMinimumSize() = 0;

signals:
  /*!
   * \brief This signal is emmited whnever the item is clicked.
   *
   * Not all widgets will emit this signal when clicked. SeperatorWidget's and
   * LabelWidgets are examples of these.
   */
  void widgetClicked(QPoint pos);
  /*!
   * \brief  This signal is emitted when the widget is changed in any way that
   *         would modify the layout. This could be a size change, a margin
   *         change or an arrangement change.
   */
  void widgetChanged();

protected:
  /// \cond DO_NOT_DOCUMENT
  WidgetItem(WidgetItemPrivate& d);
  WidgetItemPrivate* d_ptr;

  void paintBackground(QPainter& painter);
  const QSize& minContentSize() const;
  int halfDifference(int large, int small);
  void widgetWasClicked(QPoint pos);

  static const int TEXT_SPACER = 2;
  /// \endcond DO_NOT_DOCUMENT

  friend class AbstractDockItem;
  friend class AbstractDockWidget;
  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
  friend class DockHeader;
  friend class DockFooter;
  friend class DockToolbar;
};

/*!
 * \class CustomWidget  widgetitem.h widgetitem.cpp
 * \brief The CustomWidget class is an abstract WidgetItem class.
 *
 * The CustomWidget class is an abstract class intended to be used as a
 * base class of anycuston widgets.
 *
 *
 */
class CustomWidget : public WidgetItem
{
public:
protected:
  /*! /brief Holds the recommended size for the widget
   *  If the value of this property is an invalid size, no size is recommended.
   *  */
  virtual QSize sizeHint() const = 0;
};

#endif // WIDGETITEM_H
