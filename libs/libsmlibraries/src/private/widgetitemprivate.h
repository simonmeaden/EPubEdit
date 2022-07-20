#ifndef WIDGETITEMPRIVATE_H
#define WIDGETITEMPRIVATE_H

#include <QFontMetrics>
#include <QMargins>
#include <QPainter>
#include <QScrollArea>
#include <QVariant>
#include <QWidget>

#include "docktypes.h"

class WidgetItem;
class AbstractDockWidget;
class ListItem;

class WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(WidgetItem)
public:
  WidgetItemPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  AbstractDockWidget* parentWidget() const;
  void setParentWidget(AbstractDockWidget* newParentWidget);

  WidgetType type() const;
  void setType(WidgetType type);

  WidgetPosition widgetPosition() const;
  void setWidgetPosition(WidgetPosition newPosition);

  const QString& tooltip() const;
  void setTooltip(const QString& newTooltip);

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
  void setEnabled(bool enabled);

  bool isSelected() const;
  void setSelected(bool selected);

  virtual void hideWidget();

  const QVariant& data() const;
  void setData(const QVariant& data);

  WidgetItem* clone(WidgetItem* item = nullptr);

  void setGeometry(const QRect& rect, const QRect& = QRect());

  const QSize calcMinimumSize() const { return QSize(); }
  void paint(QPainter& /*painter*/) {}

  void paintBackground(QPainter& painter);

  const QSize& minContentSize() const;

  int halfDifference(int large, int small);

  bool isHoverOver() const;

  void setHoverOver(bool newHoverOver);

  bool isVisible() const;

  QSize sizeHint() const;

  void widgetWasClicked(QPoint pos);
  const QRect& rect() const;

  WidgetItem* q_ptr;
  AbstractDockWidget* m_parentWidget;
  QFontMetrics m_fontMetrics;
  QFont m_font;
  QRect m_rect;
  WidgetType m_type = Button;
  WidgetPosition m_widgetPosition = Start;
  QString m_tooltip;
  bool m_hoverOver = false;
  bool m_selected = false;
  bool m_enabled = true;
  QMargins m_margins;
  QSize m_minContentSize;
  QVariant m_data;

  static const int TOPMARGIN = 1;
  static const int BOTTOMMARGIN = 1;
  static const int LEFTMARGIN = 3;
  static const int RIGHTMARGIN = 3;
};

///*!
// * \class LabelWidget  widgetitem.h widgetitem.cpp
// * \brief The LabelWidget class supplies a simple label widget.
// */
//class LabelWidgetPrivate : WidgetItemPrivate
//{
//  LabelWidgetPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

//  const QString& text() const { return m_text; }

//  void setText(const QString& text)
//  {
//    m_text = text;
//    calcMinimumSize();
//  }

//  const QColor& textColor() const;
//  void setTextColor(const QColor& newTextColor);

//  WidgetItem* clone(WidgetItem* widget = nullptr);

//  const QSize calcMinimumSize() const;
//  void paint(QPainter& painter);
//  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
//  QSize sizeHint() const;

//  QRect m_textRect;
//  QString m_text;
//  QColor m_textColor;
//};
#endif // WIDGETITEMPRIVATE_H
