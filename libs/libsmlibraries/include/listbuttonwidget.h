#ifndef LISTBUTTONWIDGET_H
#define LISTBUTTONWIDGET_H

#include "buttonwidget.h"

/*!
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
  ListButtonWidget(AbstractDockWidget* parent, AbstractDockItem* dockItem);

  const QList<ListItem*>& items();

  ListItem* addItem(const QString& text);
  ListItem* addItem(const QIcon& icon,
                    const QSize& iconSize,
                    const QString& text);
  void setItems(const QStringList& items);

  void showList();

  bool isVisible() ;

  void setSpacer(int spacer);

  void hideWidget() override;

  void cleanItems();

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  WidgetItem* clone(WidgetItem* widget) override;

signals:
  void itemClicked(int index, const QString& text);

protected:
  /// \cond DO_NOT_DOCUMENT
  ListButtonWidget(ListButtonWidgetPrivate& d);

  void setItems(const QList<ListItem*>& items);
  /// \endcond DO_NOT_DOCUMENT

private:
};
#endif // LISTBUTTONWIDGET_H
