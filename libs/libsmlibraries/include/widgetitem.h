#ifndef WIDGETITEM_H
#define WIDGETITEM_H

#include <QAction>
#include <QMargins>
#include <QMenu>
#include <QObject>
#include <QWidget>

#include "docktypes.h"

class DockWidget;

class AbstractDockWidget;
class DockItem;

class MenuItem
{
public:
  const QString& text() const { return m_text; }
  void setText(const QString& newText) { m_text = newText; }
  const QIcon& icon() const { return m_icon; }
  void setIcon(const QIcon& icon, const QSize& iconSize)
  {
    m_icon = icon;
    m_iconSize = iconSize;
  }
  const QRectF& rect() const { return m_rect; }
  void setRect(const QRectF& rect) { m_rect = rect; }

  const QSize& iconSize() const { return m_iconSize; }

private:
  QString m_text;
  QIcon m_icon;
  QSize m_iconSize;
  QRectF m_rect;
};

class WidgetItem : public QObject
{
  Q_OBJECT
public:
  WidgetItem(AbstractDockWidget* parent);

signals:
//  void widgetClicked();
  void widgetClicked(QPoint pos);
  /*!
   * \brief  This signal is emitted when the widget is changed in any way that
   *         would modify the layout. This could be a size change, a margin
   *         change or an arrangement change.
   */
  void widgetChanged();

public:
  static const int TOPMARGIN = 1;
  static const int BOTTOMMARGIN = 1;
  static const int LEFTMARGIN = 3;
  static const int RIGHTMARGIN = 3;
  static const int TEXT_SPACER = 2;

  const QMargins& margins() const;
  void setMargins(const QMargins& newMargins);
  int leftMargin();
  void setLeftMargin(int margin);
  int rightMargin();
  void setRightMargin(int margin);
  int topMargin();
  void setTopMargin(int margin);
  int bottomMargin();
  void setBottomMargin(int margin);

  bool isEnabled() const;
  void setEnabled(bool newEnabled);

  bool isSelected() const;
  void setSelected(bool newSelected);

  const QRect& rect() const;

  virtual void setGeometry(const QRect& rect, const QRect& = QRect());

  WidgetType type() const;
  void setType(WidgetType newType);

  WidgetPosition widgetPosition() const;
  void setWidgetPosition(WidgetPosition newPosition);

  const QString& tooltip() const;
  void setTooltip(const QString& newTooltip);

  bool isHoverOver() const;
  void setHoverOver(bool newHoverOver);

  virtual const QSize calcMinimumSize() = 0;

  QSize sizeHint() const;

  virtual void paint(QPainter& painter) = 0;

  void paintBackground(QPainter& painter);

  const QSize& minContentSize() const;

protected:
  AbstractDockWidget* m_parent;
  QFontMetrics m_fontMetrics;
  QRect m_rect;
  WidgetType m_type = Button;
  WidgetPosition m_widgetPosition = Start;
  QString m_tooltip;
  bool m_hoverOver = false;
  bool m_selected = false;
  bool m_enabled = true;
  QMargins m_margins;
  QSize m_minContentSize;

  int halfDifference(int large, int small);
};

class SeperatorWidget : public WidgetItem
{
  Q_OBJECT
public:
  SeperatorWidget(AbstractDockWidget* parent);

  const QColor& color() const;
  void setColor(const QColor& newColor);

  int thickness() const;
  void setThickness(int newThickness);

  // WidgetWrapper interface
  const QSize calcMinimumSize();
  void paint(QPainter& painter);

private:
  QColor m_color;
  int m_thickness = 1;
};

class CustomWidget : public WidgetItem
{
public:
  virtual QSize sizeHint() const = 0;

  static WidgetItem* create(const QString& type);
  static WidgetItem* create(QWidget* sister);
};

class ButtonWidget : public WidgetItem
{
  Q_OBJECT
public:
  ButtonWidget(AbstractDockWidget* parent);
//  ~ButtonWidget();

  const QRect& iconRect() const;
  void setIconRect(const QRect& newIconRect);

  const QIcon& icon() const;
  void setIcon(const QIcon& newIcon);

  const QSize& iconSize() const;
  void setIconSize(const QSize& newIconSize);

  const QRect& textRect() const;

  void setTextRect(const QRect& newTextRect);

  const QString& text() const;
  void setText(const QString& newText);

  int spacer() const;
  void setSpacer(int newSpacer);

  const QColor& textColor() const;
  void setTextColor(const QColor& newTextColor);

  Arrangement arrangement() const;
  void setArrangement(Arrangement newArrangement);

  // WidgetWrapper interface
  const QSize calcMinimumSize();
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);

  void setIconState(QIcon::Mode mode);

//  const QList<MenuItem*>& items() const;
//  void addItem(const QString& text);
//  void addItem(const QIcon& icon, const QSize& iconSize, const QString& item);
//  void setItems(const QStringList& items);
//  void setItems(const QList<MenuItem*>& items);
//  int currentItem();
//  void setCurrentItem(int index);

private:
  Arrangement m_arrangement = IconOnly;
  QRect m_iconRect;
  QIcon m_icon;
  QIcon::Mode m_iconMode = QIcon::Normal;
  QSize m_iconSize;
  QRect m_textRect;
  QString m_text;
  int m_spacer = TEXT_SPACER;
  QColor m_textColor;
//  QList<MenuItem*> m_items;
//  int m_currentIndex = -1;
};

class LabelWidget : public WidgetItem
{
  Q_OBJECT
public:
  LabelWidget(AbstractDockWidget* parent);

  const QString& text() const;
  void setText(const QString& newText);

  const QColor& textColor() const;
  void setTextColor(const QColor& newTextColor);

  // WidgetWrapper interface
  const QSize calcMinimumSize();
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
  QSize sizeHint() const;

private:
  QRect m_textRect;
  QString m_text;
  QColor m_textColor;
};

class ListAction : public QAction
{
  Q_OBJECT
public:
  ListAction(const QString& text, QObject* parent)
    : QAction(text, parent)
  {}
  ListAction(const QIcon& icon, const QString& text, QObject* parent)
    : QAction(icon, text, parent)
  {}
};

class ListWidget : public WidgetItem
{
  Q_OBJECT
public:
  ListWidget(AbstractDockWidget* parent, DockItem* dockItem);
  ~ListWidget();

  const QList<MenuItem*>& items() const;
  void addItem(const QString& text);
  void addItem(const QIcon& icon, const QSize& iconSize, const QString& item);
  void setItems(const QStringList& items);
  void setItems(const QList<MenuItem*>& items);
  int currentItem();
  void setCurrentItem(int index);

  //  int spacer() const;
  void setText(const QString text);
  void setSpacer(int newSpacer);

  void setText(int index, const QString text);
  void setIcon(int index, const QIcon& icon, const QSize& iconSize);
  void setItem(int index,
               const QIcon& icon,
               const QSize& iconSize,
               const QString text);

  void show(const QPoint& pos, const QRect& frameRect);

  const QRect& textRect() const;
  void setTextRect(const QRect& newTextRect);
  QSize sizeHint() const;

signals:
  void itemClicked(int index, const QString& text);

protected:
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
  const QSize calcMinimumSize();
  //  void itemWasClicked(QListWidgetItem* item);

private:
  QRect m_listRect;
  DockItem* m_dockItem = nullptr;
  QRect m_textRect;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_iconRect;
  QRect m_area;
  QString m_text;
  QColor m_textColor;
  int m_spacer = TEXT_SPACER;
  QList<MenuItem*> m_items;
  int m_currentItem;

  void menuClicked(QAction* action);
};

#endif // WIDGETITEM_H
