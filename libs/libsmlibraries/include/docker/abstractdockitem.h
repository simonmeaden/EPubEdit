#ifndef ABSTRACTDOCKITEM_H
#define ABSTRACTDOCKITEM_H

#include <QPainter>
#include <QWidget>

#include "docktypes.h"
#include "multisplitter/cloneableinterface.h"

class AbstractDockWidget;
class WidgetItem;
class CustomWidget;
class ButtonWidget;

class DockHelper
{};

/*!
 * \ingroup docker
 * \class AbstractDockItem abstractdockitem.h
 * "include/docker/abstractdockitem.h" \brief This class is the base class of
 * all the ToolbarWidget/HeaderWidget and FooterWidget classes and does most of
 * the background work of those classes.
 */
class AbstractDockItemPrivate;
class AbstractDockItem
  : public QObject
  , public CloneableObjectInterface
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractDockItem)
public:
  /*!
   * \brief Constructs a DockItem which is a child of parent.
   */
  explicit AbstractDockItem(DockPosition position, AbstractDockWidget* parent);

  /*!
   * \brief Sets the text in the Header/Footer/Toolbar widget.
   *
   * The text is set to the supplied value ONLY if the widget is
   * of an appropriate type, otherwise it is ignored. For example
   * an ICON_WIDGET has no text.
   */
  void setText(int index, const QString& text);
  /*!
   * \brief Sets the tooltip in the widget.
   */
  void setToolTip(int index, const QString& tooltip);

  /*!
   * \brief Returns and removes the widget at the index.
   *
   * If the index value is invalid then a nullptr is returned, otherwise
   * the widget at that index is returned. The widget is no longer stored
   * in the list.
   *
   * \note The index indicates the position in which the widget was added,
   * not necessarily the position within the list.
   */
  WidgetItem* takeAt(int index);

  /*!
   * \brief Returns the widget at the index.
   *
   * If the index value is invalid then a nullptr is returned, otherwise
   * the widget at that index is returned. The widget is still stored
   * in the list.
   *
   * \note The index indicates the position in which the widget was added,
   * not necessarily the position within the list.
   */
  WidgetItem* at(int index);

  //! Returns the number of WidgetItem stored in this AbstractDockItem
  int count();

  /*!
   * \brief Replaces the existing widget with another.
   *
   * The index must exist within the stored list. Returns true if
   * the replacement was successful, otherwise returns false.
   *
   * \return
   */
  bool replace(int index, WidgetItem* w);

  //! Returns true if the specified widget at index is enabled, otherwise
  //! false;
  bool widgetEnabled(int index);
  //! Enables the widget at index if value is true, otherwise disables widget.
  bool enableWidget(int index, bool value);

  //! Returns true if the specified widget at index is selected, otherwise
  //! false;
  bool selected(int index);
  //! Selects the widget at index if value is true, otherwise deselects widget.
  bool setSelected(int index, bool value);

  //! Returns the margins of the selected widget at index.
  QMargins widgetMargins(int index);
  //! Sets the margins of the selected widget at index.
  void setWidgetMargins(int index, int left, int top, int right, int bottom);

  //! Sets the preferred size to width and height.
  void setPreferredSize(int width, int height);
  //! Sets the preferred size to size.
  void setPreferredSize(QSize size);
  //! Sets the preferred width to width.
  void setPreferredWidth(int width);
  //! Sets the preferred height to height.
  void setPreferredHeight(int height);

  //! Returns the DockPosition of the DockItem.
  DockPosition dockPosition() const;

  //  int maxWidgetWidth() const;
  //  int maxWidgetHeight() const;

  //! ReturnsaddItem the height of the DockItem.
  int height();
  //! sets the height of the DockItem.
  void setHeight(int height);
  //! Returns the width of the DockItem.
  int width();
  //! sets the width of the DockItem.
  void setWidth(int width);

  //! Returns the value of the visible flag
  bool isVisible() const;
  //! Sets the value of the visible flag
  void setVisible(bool newShow);
  //! Shows the DockItem. Equivalent of setVisible(true).
  void show();
  //! Hides the DockItem. Equivalent of setVisible(false).
  void hide();

  //! paints the dock item
  virtual void paint(QPainter& painter);

  //! Returns a list of all the WidgetItem's added to this DockItem
  const QList<WidgetItem*>& widgets() const;
  //! Defines the QRect display area of this DockWidget
  const QRect& rect() const;

  //! Returns a size hint for this object.
  virtual QSize sizeHint() const = 0;

  //! Recalculates the geometry of this DockItem and it's children.
  void calculateGeometry(const QRect& rect);
  //! Calculates the maximum widget sizes.
  void calcMaxWidgetSizes();

  const QMargins& margins() const;
  void setMargins(const QMargins& margins);

  int spacer() const;
  void setSpacer(int spacer);

signals:

protected:
  //! protected d_ptr instance
  AbstractDockItemPrivate* d_ptr = nullptr;

  //! protected d_ptr constructoraddItem
  AbstractDockItem(AbstractDockItemPrivate& d);

  /*!
   * \brief Creates a clone of the DockItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(QObject* object) override;

  /*!
   * \brief Adds a prebuilt widget to the DockToolbar/DockHeader/DockFooter.
   *
   * This is used internally to allow  the user to clone the appropriate
   * DockItem. This was originally added to allow the MultiSplitter widget
   * to clone it's various widgets along with their headers and footers
   * without having to totally rebuild them from scatch.
   */
  WidgetItem* addItem(WidgetItem* item);

private:
  QMargins m_margins;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
  friend class SeperatorWidget;
};

#endif // ABSTRACTDOCKITEM_H
