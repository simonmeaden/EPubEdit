#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "docker/widgetitem.h"

/*!
 * \ingroup docker
 * \class ListWidget  widgetitem.h widgetitem.cpp
 * \brief The ListWidget class supplies a button with a drop down menu, also
 * displays the selected text or the first if none selected..
 *
 * ListWidget is a subclass of WidgetItem that supplies a drop down
 * menu when it is clicked. This is only available to headers and footers at
 * present.
 *
 * Preferably addListWidget(...) of the DockHeader or DockFooter should be used
 * to create the button as it assures the button is properly created.
 *
 * \sa ListWidget
 */
class ListItem;
class ListWidgetPrivate;
class ListWidget : public WidgetItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(ListWidget)
public:
  //! Constructor for ListWidget
  ListWidget(AbstractDockWidget* parent, AbstractDockItem* dockItem);

  //! Returns a list of the dropdown list ListItem's
  const QList<ListItem*>& items();

  //! Adds a text list item to the buttons dropdown list
  void addItem(const QString& text);
  //! Adds a text and icon  list item to the buttons dropdown list
  void addItem(const QIcon& icon, const QSize& iconSize, const QString& text);
  //! Sets a list of text only items for the dropdown list.
  void setItems(const QStringList& items);
  //! Sets a list of items for the dropdown list.
  void setItems(const QList<ListItem*>& items);

  //! Returns the currently selected item.
  int currentItem();
  //! Sets the currently selected item.
  void setCurrentItem(int index);

  //! Shows the dropdown list.
  void showList();

  //! Sets the list widgets display text
  void setText(const QString text);
  //! Sets the size of the spacer between the icon and the text.
  //!
  //! Does nothing if there is no icon.
  void setSpacer(int spacer);

  //! Sets the text for the item at index.
  void setText(int index, const QString text);
  //! Sets the icon for the item at index.
  void setIcon(int index, const QIcon& icon, const QSize& iconSize);
  //! Sets the text and the icon for the item at index.
  void setItem(int index,
               const QIcon& icon,
               const QSize& iconSize,
               const QString text);

  //! Returns the display area of the ListWidget
  const QRect& textRect();
  //! Sets the display area of the ListWidget
  void setTextRect(const QRect& textRect);

  //! Returns the size hint.
  QSize sizeHint();

  //! Checks whether the widget is visible, true if it is otherwise false.
  bool isVisible();
  //! Hides the list widget
  void hideWidget() override;

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(QObject* item = nullptr) override;

signals:
  //! sent when an item in the dropdown list is selected.
  void itemClicked(int index, const QString& text);

protected:
  /// \cond DO_NOT_DOCUMENT
  ListWidget(ListWidgetPrivate& d);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect) override;
  const QSize calcMinimumSize() override;
  /// \endcond DO_NOT_DOCUMENT

private:
  void cleanItems();
  void itemWasClicked();
  void paint(QPainter& painter) override;
};
#endif // LISTWIDGET_H
