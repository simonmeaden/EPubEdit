#ifndef DOCKLISTITEM_H
#define DOCKLISTITEM_H

#include <QObject>
#include <QIcon>
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>

/*!
 * \class ListItem widgetitem.h "includes/widgetitem.h"
 * \brief The ListItem class is used to store items in ListWidget's and
 * ListButtonWidget's.
 *
 * When an item is added to ListButtonWidget then a ListItem* is returned. This
 * can then be used to connect the ListItem::itemClicked signal to your receiver
 * method.
 *
 * ListItem's are not returned when used in a ListWidget as instead the
 * ListWidget::itemClicked signal has parameters of the int index and QString
 * text of the item.
 *
 * You should never need to create one of these items manually as they are
 * created internally when items are added to the ListButtonWidget.
 *
 * \sa ListWidget
 * \sa ListButtonWidget
 * \sa ListItem::itemClicked()
 */
class ListItem : public QObject
{
  Q_OBJECT
public:
  /// cond DO_NOT_DOCUMENT
  ListItem(QObject* parent);

  const QString& text() const;
  void setText(const QString& newText);

  const QIcon& icon() const;
  void setIcon(const QIcon& icon, const QSize& iconSize);

  const QRect& rect() const;
  void setRect(const QRect& rect);

  const QSize& iconSize() const;

  int textLeft() const;
  void setTextLeft(int textLeft);

  int descent() const;
  void setDescent(int newDescent);

signals:
  //! The itemClicked signal is triggered when the menu list is clicked.
  void itemClicked();

private:
  QString m_text;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_rect;
  int m_descent;
  int m_textLeft;
};

/// \cond DO_NOT_DOCUMENT
class ListDisplay : public QWidget
{
  Q_OBJECT
public:
  ListDisplay(QWidget* parent);

  const QList<ListItem*>& items() const;
  void setItems(const QList<ListItem*>& items, int scrollWidth);

  //! Sets the display margins
  void setMargins(const QMargins& newMargins);

  //! Sets the text spacer size in characters.
  void setSpacer(int newSpacer);

  //! \reimplements{QWidget::sizeHint}
  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent*) override;

  int halfDifference(int large, int small);

private:
  QList<ListItem*> m_items;
  QWidget* m_parent;
  int m_maxItemHeight;
  int m_width;
  int m_height;
  int m_spacer;
  QMargins m_margins;
};

class ListScroller : public QScrollArea
{
  Q_OBJECT
public:
  ListScroller(QWidget* parent);

signals:
  void listClicked(int index, const QString& href);

protected:
  void mousePressEvent(QMouseEvent* event) override;
};

/// \endcond DO_NOT_DOCUMENT
#endif // DOCKLISTITEM_H
