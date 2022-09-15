#include "docker/listbuttonwidget.h"
#include "docker/docklistitem.h"
#include "private/listbuttonwidgetprivate.h"

//====================================================================
//=== ListButtonWidget
//====================================================================
ListButtonWidget::ListButtonWidget(AbstractDockWidget* parent,
                                   AbstractDockItem* dockItem)
  : ButtonWidget(*new ListButtonWidgetPrivate(parent, dockItem, this))
{
  qRegisterMetaType<ListButtonWidget*>();
  dynamic_cast<ListButtonWidgetPrivate*>(d_ptr)->setDockItem(dockItem);
}

ListButtonWidget::ListButtonWidget(ListButtonWidgetPrivate& d)
  : ButtonWidget(d)
{
}

const QList<ListItem*>&
ListButtonWidget::items()
{
  Q_D(ListButtonWidget);
  return d->items();
}

ListItem*
ListButtonWidget::addItem(const QString& text)
{
  Q_D(ListButtonWidget);
  return d->addItem(text);
}

ListItem*
ListButtonWidget::addItem(const QIcon& icon,
                          const QSize& iconSize,
                          const QString& text)
{
  Q_D(ListButtonWidget);
  return d->addItem(icon, iconSize, text);
}

void
ListButtonWidget::setItems(const QStringList& items)
{
  Q_D(ListButtonWidget);
  d->setItems(items);
}

void
ListButtonWidget::setItems(const QList<ListItem*>& items)
{
  Q_D(ListButtonWidget);
  d->setItems(items);
}

void
ListButtonWidget::showList()
{
  Q_D(ListButtonWidget);
  d->showList();
}

bool
ListButtonWidget::isVisible()
{
  Q_D(ListButtonWidget);
  return d->isVisible();
}

void
ListButtonWidget::setSpacer(int spacer)
{
  Q_D(ListButtonWidget);
 d->setSpacer(spacer);
}

void
ListButtonWidget::hideWidget()
{
  Q_D(ListButtonWidget);
  d->hideWidget();
}

void
ListButtonWidget::cleanItems()
{
  Q_D(ListButtonWidget);
  // clean up list display.
  d->cleanItems();
}

void ListButtonWidget::clone(QObject *widget)
{
  Q_D(ListButtonWidget);
  auto anobject = qobject_cast<ListButtonWidget*>(widget);
  if (anobject)
    d->clone(anobject);
}

void ListButtonWidget::itemWasClicked()
{
  Q_D(ListButtonWidget);
  auto o = sender();
  auto item = dynamic_cast<ListItem*>(o);
  if (o) {
    auto index = d->m_items.indexOf(item);
    auto text = item->text();
    emit itemClicked(index, text);
  }
}
