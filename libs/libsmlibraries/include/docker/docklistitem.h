#ifndef DOCKLISTITEM_H
#define DOCKLISTITEM_H

#include <QObject>
#include <QIcon>
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>

/*!
 * \ingroup docker
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
  //! Constructor for ListItem
  ListItem(QObject* parent);

  //! Returns the item text
  const QString& text() const;
  //! Sets the item text.
  void setText(const QString& newText);

  //! Returns the item icon
  const QIcon& icon() const;
  //! Sets the item icon
  void setIcon(const QIcon& icon, const QSize& iconSize);

  //! Returns the item display area
  const QRect& rect() const;
  //! Sets the item display area
  void setRect(const QRect& rect);

  //! returns the preferred icon size/
  const QSize& iconSize() const;

  //! Returns the left offset of the text.
  int textLeft() const;
  //! Sets the left offset of the text.
  void setTextLeft(int textLeft);

//  //! Returns the text descent
//  int descent() const;
//  // Sets the text descent
//  void setDescent(int newDescent);

signals:
  //! The itemClicked signal is triggered when the menu list is clicked.
  void itemClicked();

private:
  QString m_text;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_rect;
//  int m_descent;
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
