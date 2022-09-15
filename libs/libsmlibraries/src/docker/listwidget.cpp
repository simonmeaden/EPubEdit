#include "docker/listwidget.h"
#include "docker/docklistitem.h"
#include "private/listwidgetprivate.h"
#include "utilities/x11colors.h"

//====================================================================
//=== ListWidget
//====================================================================
ListWidget::ListWidget(AbstractDockWidget* parent, AbstractDockItem* dockItem)
  : WidgetItem(*new ListWidgetPrivate(parent, dockItem, this))
{
  qRegisterMetaType<ListWidget*>();
  dynamic_cast<ListWidgetPrivate*>(d_ptr)->setDockItem(dockItem);
}

ListWidget::ListWidget(ListWidgetPrivate& d)
  : WidgetItem(d)
{
}

void
ListWidget::cleanItems()
{
  Q_D(ListWidget);
  // clean up list display.
  d->cleanItems();
}

void
ListWidget::setCurrentItem(int index)
{
  Q_D(ListWidget);
  d->setCurrentItem(index);
}

void
ListWidget::addItem(const QString& text)
{
  Q_D(ListWidget);
  d->addItem(text);
}

void
ListWidget::addItem(const QIcon& icon,
                    const QSize& iconSize,
                    const QString& text)
{
  Q_D(ListWidget);
  d->addItem(icon, iconSize, text);
}

void
ListWidget::setItems(const QStringList& items)
{
  Q_D(ListWidget);
  d->setItems(items);
}

void
ListWidget::setItems(const QList<ListItem*>& items)
{
  Q_D(ListWidget);
  d->setItems(items);
}

const QList<ListItem*>&
ListWidget::items()
{
  Q_D(ListWidget);
  return d->items();
}

int
ListWidget::currentItem()
{
  Q_D(ListWidget);
  return d->currentItem();
}

void
ListWidget::showList()
{
  Q_D(ListWidget);
  d->showList();
}

void
ListWidget::setText(const QString text)
{
  Q_D(ListWidget);
  d->setText(text);
}

void
ListWidget::setSpacer(int spacer)
{
  Q_D(ListWidget);
  d->setSpacer(spacer);
}

void
ListWidget::setText(int index, const QString text)
{
  Q_D(ListWidget);
  d->setText(index, text);
}

void
ListWidget::setIcon(int index, const QIcon& icon, const QSize& iconSize)
{
  Q_D(ListWidget);
  d->setIcon(index, icon, iconSize);
}

void
ListWidget::setItem(int index,
                    const QIcon& icon,
                    const QSize& iconSize,
                    const QString text)
{
  Q_D(ListWidget);
  d->setItem(index, icon, iconSize, text);
}

void
ListWidget::paint(QPainter& painter)
{
  Q_D(ListWidget);
  d->paint(painter);
}

void
ListWidget::setGeometry(const QRect& widgetRect, const QRect& contentsRect)
{
  Q_D(ListWidget);
  d->setGeometry(widgetRect, contentsRect);
}

const QSize
ListWidget::calcMinimumSize()
{
  Q_D(ListWidget);
  return d->minContentSize();
}

const QRect&
ListWidget::textRect()
{
  Q_D(ListWidget);
  return d->textRect();
}

void
ListWidget::setTextRect(const QRect& textRect)
{
  Q_D(ListWidget);
  d->setTextRect(textRect);
}

QSize
ListWidget::sizeHint()
{
  Q_D(ListWidget);
  return d->sizeHint();
}

bool
ListWidget::isVisible()
{
  Q_D(ListWidget);
  return d->isVisible();
}

void
ListWidget::hideWidget()
{
  Q_D(ListWidget);
  d->hideWidget();
}

void ListWidget::itemWasClicked()
{
  Q_D(ListWidget);
  auto o = sender();
  auto item = dynamic_cast<ListItem*>(o);
  if (o) {
    auto index = d->m_items.indexOf(item);
    auto text = item->text();
    emit itemClicked(index, text);
  }
}

void ListWidget::clone(QObject *item)
{
  Q_D(ListWidget);
  auto anobject = qobject_cast<ListWidget*>(item);
  if (anobject)
    d->clone(anobject);}
