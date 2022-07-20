#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "widgetitem.h"

/*!
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
  ListWidget(AbstractDockWidget* parent, AbstractDockItem* dockItem);

  const QList<ListItem*>& items() ;

  void addItem(const QString& text);
  void addItem(const QIcon& icon, const QSize& iconSize, const QString& text);
  void setItems(const QStringList& items);
  void setItems(const QList<ListItem*>& items);
  int currentItem();
  void setCurrentItem(int index);

  void showList();

  //  int spacer() const;
  void setText(const QString text);
  void setSpacer(int spacer);

  void setText(int index, const QString text);
  void setIcon(int index, const QIcon& icon, const QSize& iconSize);
  void setItem(int index,
               const QIcon& icon,
               const QSize& iconSize,
               const QString text);

  const QRect& textRect() ;
  void setTextRect(const QRect& textRect);
  QSize sizeHint();

  bool isVisible() ;
  void hideWidget() override;

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  WidgetItem* clone(WidgetItem* widget = nullptr) override;

  void cleanItems();

  void paint(QPainter& painter) override;

signals:
  void itemClicked(int index, const QString& text);

protected:
  /// \cond DO_NOT_DOCUMENT
  ListWidget(ListWidgetPrivate& d);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect) override;
  const QSize calcMinimumSize() override;
  /// \endcond DO_NOT_DOCUMENT

private:
};
#endif // LISTWIDGET_H
