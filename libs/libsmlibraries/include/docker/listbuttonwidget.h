#ifndef LISTBUTTONWIDGET_H
#define LISTBUTTONWIDGET_H

#include "docker/buttonwidget.h"

/*!
 * \ingroup docker
 * \class ListButtonWidget  widgetitem.h widgetitem.cpp
 * \brief The ListButtonWidget class supplies a button with a drop down menu.
 *
 * ListButtonWidget is a subclass of ButtonWidget that supplies a drop down
 * menu when it is clicked.
 *
 * Preferably addIconTextListButton(...) DockHeader, DockFooter or DockToolbar
 * should be used to create the button as it assures the button is properly
 * created.
 *
 * \sa ListWidget
 */
class ListItem;
class ListButtonWidgetPrivate;
class ListButtonWidget : public ButtonWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(ListButtonWidget)
public:
  //! Constructor for ListButtonWidget
  ListButtonWidget(AbstractDockWidget* parent, AbstractDockItem* dockItem);

  //! Returns a list of the dropdown list ListItem's
  const QList<ListItem*>& items();

  //! Adds a text list item to the buttons dropdown list
  ListItem* addItem(const QString& text);
  //! Adds a text and icon  list item to the buttons dropdown list
  ListItem* addItem(const QIcon& icon,
                    const QSize& iconSize,
                    const QString& text);
  //! Sets a list of text only items for the dropdown list.
  void setItems(const QStringList& items);

  //! Shows the dropdown list.
  void showList();

  //! Checks whether the button is visible, true if it is otherwise false.
  bool isVisible() ;

  //! Sets the size of the spacer between the icon and the text.
  //!
  //! Does nothing if there is no icon.
  void setSpacer(int spacer);

  //! Hides the button
  void hideWidget() override;

  //! Hides and cleans up the dropdown list.
  void cleanItems();

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(QObject* widget) override;


signals:
  //! sent when an item in the dropdown list is selected.
  void itemClicked(int index, const QString& text);

protected:
  //! d_ptr constructor.
   ListButtonWidget(ListButtonWidgetPrivate& d);

private:
  void setItems(const QList<ListItem*>& items);
  void itemWasClicked();

};
#endif // LISTBUTTONWIDGET_H
