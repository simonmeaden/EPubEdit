#ifndef LISTWIDGETPRIVATE_H
#define LISTWIDGETPRIVATE_H

#include <QIcon>

#include "widgetitemprivate.h"
#include "docker/docklistitem.h"
#include "docker/listwidget.h"

class ListWidgetPrivate : public WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(ListWidget)
public:
  ListWidgetPrivate(AbstractDockWidget* parent, AbstractDockItem* dockItem, WidgetItem *qptr);
  ~ListWidgetPrivate();

  const QList<ListItem*>& items() ;

  void addItem(const QString& text);
  void addItem(const QIcon& icon, const QSize& iconSize, const QString& text);
  void setItems(const QStringList& items);
  void setItems(const QList<ListItem*>& items);
  int currentItem();
  void setCurrentItem(int index);

  void showList();

  void setText(const QString text);
  void setSpacer(int spacer);

  void setText(int index, const QString text);
  void setIcon(int index, const QIcon& icon, const QSize& iconSize);
  void setItem(int index,
               const QIcon& icon,
               const QSize& iconSize,
               const QString text);

  const QRect& textRect() ;
  void setTextRect(const QRect& newTextRect);
  QSize sizeHint() const;

  bool isVisible() ;
  void hideWidget() override;

  void setDockItem(AbstractDockItem *dockItem);

  WidgetItem* clone(WidgetItem* widget = nullptr);

  void cleanItems();

protected:
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
  const QSize calcMinimumSize();

private:
  AbstractDockItem* m_dockItem = nullptr;
  QRect m_textRect;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_iconRect;
  QRect m_area;
  QString m_text;
  QColor m_textColor;
  int m_spacer = 2;
  int m_currentItem;
  ListScroller* m_scrollArea = nullptr;
  ListDisplay* m_listDisplay = nullptr;
  QList<ListItem*> m_items;
  QRect m_listRect;
};
#endif // LISTWIDGETPRIVATE_H
