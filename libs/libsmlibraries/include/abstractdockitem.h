#ifndef ABSTRACTDOCKITEM_H
#define ABSTRACTDOCKITEM_H

#include <QPainter>
#include <QWidget>

#include "docktypes.h"

class AbstractDockWidget;
class WidgetItem;
class CustomWidget;
class ButtonWidget;

/*!
 * \class DockItem dockitem.h "include/dockitem.h"
 * \brief This class is the base class of all the ToolbarWidget/HeaderWidget
 * and FooterWidget classes and does most of the background work of those
 * classes.
 */
class AbstractDockItemPrivate;
class AbstractDockItem : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractDockItem)
public:
  /*!
   * \brief Constructs a DockItem which is a child of parent.
   */
  explicit AbstractDockItem(DockPosition position, AbstractDockWidget* parent);

  /*!
   * \brief Adds an icon only ButtonWidget to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconButton(WidgetPosition pos,
                              QIcon icon,
                              QSize iconSize,
                              const QString& tooltip = QString());
  /*!
   * \brief Adds an icon only ListButtonWidget to the Header/Footer/Toolbar
   * widget with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconListButton(WidgetPosition pos,
                                  QIcon icon,
                                  QSize iconSize,
                                  const QString& tooltip = QString());

  /*!
   * \brief Adds an icon with text ButtonWidget to the Header/Footer/Toolbar
   * widget and an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconTextButton(
    WidgetPosition pos,
    QIcon icon,
    QSize iconSize,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());
  /*!
   * \brief Adds an icon with text ListButtonWidget to the Header/Footer/Toolbar
   * widget and an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconTextListButton(
    WidgetPosition pos,
    QIcon icon,
    QSize iconSize,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());

  /*!
   * \brief Adds an text only ButtonWidget to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addTextButton(WidgetPosition pos,
                              const QString& text,
                              const QString& tooltip = QString());
  /*!
   * \brief Adds an text only ListButtonWidget to the Header/Footer/Toolbar
   * widget with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addTextListButton(WidgetPosition pos,
                                  const QString& text,
                                  const QString& tooltip = QString());

  /*!
   * \brief Adds a text label to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addTextLabel(WidgetPosition pos,
                           const QString& text,
                           const QString& tooltip = QString());

  /*!
   * \brief addSpacer
   * \return
   */
  WidgetItem* addSeperator(WidgetPosition);

  /*!
   * \brief Adds a custom widget to the Header/Footer/Toolbar widget.
   */
  WidgetItem* addCustomWidget(CustomWidget* w);

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

  //!< Returns true if the specified widget at index is enabled, otherwise
  //!< false;
  bool widgetEnabled(int index);
  //!< Enables the widget at index if value is true, otherwise disables widget.
  bool enableWidget(int index, bool value);

  //!< Returns true if the specified widget at index is selected, otherwise
  //!< false;
  bool selected(int index);
  //!< Selects the widget at index if value is true, otherwise deselects widget.
  bool setSelected(int index, bool value);

  //!< Returns the margins of the selected widget at index.
  QMargins widgetMargins(int index);
  //!< Sets the margins of the selected widget at index.
  void setWidgetMargins(int index, int left, int top, int right, int bottom);

  //!< Sets the preferred size to width and height.
  void setPreferredSize(int width, int height);
  //!< Sets the preferred size to size.
  void setPreferredSize(QSize size);
  //!< Sets the preferred width to width.
  void setPreferredWidth(int width);
  //!< Sets the preferred height to height.
  void setPreferredHeight(int height);

  DockPosition dockPosition() const;

//  int maxWidgetWidth() const;
//  int maxWidgetHeight() const;

  int height();
  void setHeight(int height);
  int width() ;
  void setWidth(int width);

  bool isVisible() const;
  void setVisible(bool newShow);
  void show();
  void hide();

  virtual void paint(QPainter& painter);

  const QList<WidgetItem*>& widgets() const;

//  void calcMaxWidgetSizes();

  const QRect& rect() const;

  virtual QSize sizeHint() const = 0;

  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int SPACER_TOP = 1;
  static const int SPACER_BOTTOM = 1;
  // just places a blank space at the end of the toolbar.
  static const int TOOLBAR_ENDER = 5;

  void calculateGeometry(const QRect& rect);
  void calcMaxWidgetSizes();

signals:

protected:
  AbstractDockItem(AbstractDockItemPrivate& d);
  AbstractDockItemPrivate* d_ptr;

  /*!
   * \brief Creates a clone of the DockItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(AbstractDockItem* item);

//  QMargins calcMaxMargins();
//  QSize calcMaxContentsSize();

  /*!
   * \internal
   * \brief Adds a prebuilt widget to the DockToolbar/DockHeader/DockFooter.
   *
   * This is used internally to allow  the user to clone the appropriate
   * DockItem. This was originally added to allow the MultiSplitter widget
   * to clone it's various widgets along with their headers and footers
   * without having to totally rebuild them from scatch.
   */
  WidgetItem* addItem(WidgetItem* item);

private:


  friend class HeaderWidget;
  friend class FooterWidget;
  friend class HeaderFooterWidget;
  friend class DockWidget;
  friend class SeperatorWidget;
};

#endif // ABSTRACTDOCKITEM_H
