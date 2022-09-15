#ifndef LISTBUTTONWIDGETPRIVATE_H
#define LISTBUTTONWIDGETPRIVATE_H

#include "docker/private/buttonwidgetprivate.h"
#include "docker/docklistitem.h"
#include "docker/listbuttonwidget.h"

class ListButtonWidgetPrivate : public ButtonWidgetPrivate
{
  Q_DECLARE_PUBLIC(ListButtonWidget)
public:
  ListButtonWidgetPrivate(AbstractDockWidget* parent,
                          AbstractDockItem* dockItem,
                          WidgetItem* widgetItem);

  const QList<ListItem*>& items();

  ListItem* addItem(const QString& text);
  ListItem* addItem(const QIcon& icon,
                    const QSize& iconSize,
                    const QString& text);
  void setItems(const QStringList& items);

  void setItems(const QList<ListItem*>& items);

  void showList();

  bool isVisible();

  void setSpacer(int spacer);

  void hideWidget();

  void cleanItems();

  WidgetItem* clone(WidgetItem* widget);

  void setDockItem(AbstractDockItem *dockItem);

protected:
  /// \endcond DO_NOT_DOCUMENT

private:
  AbstractDockItem* m_dockItem = nullptr;
  QList<ListItem*> m_items;
  ListScroller* m_scrollArea = nullptr;
  ListDisplay* m_listDisplay = nullptr;
  int m_spacer = 2;
};

#endif // LISTBUTTONWIDGETPRIVATE_H
