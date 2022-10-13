#ifndef WIDGETITEMPRIVATE_H
#define WIDGETITEMPRIVATE_H

#include <QFontMetrics>
#include <QMargins>
#include <QPainter>
#include <QScrollArea>
#include <QVariant>
#include <QWidget>

#include "docker/docktypes.h"

class WidgetItem;
class AbstractDockWidget;
class ListItem;

/*!
  \ingroup docker
  \file widgetitemprivate.h
  \brief Private d_ptr class for WidgetItem.
  \author Simon Meaden
  \date 2022-06-01
 */
class WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(WidgetItem)
public:
  //! Constructor
  WidgetItemPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  //! parent widget
  AbstractDockWidget* parentWidget() const;
  //! Set the parent widget
  void setParentWidget(AbstractDockWidget* newParentWidget);

  //! widget type
  WidgetType type() const;
  //! set the widget type.
  void setType(WidgetType type);

  //! widget position
  WidgetPosition widgetPosition() const;
  //! ste the widget position.
  void setWidgetPosition(WidgetPosition newPosition);

  //! tooltip text.
  const QString& tooltip() const;
  //! set the tooltip text.
  void setTooltip(const QString& newTooltip);

  //! widget margins
  const QMargins& margins() const;
  //! Set the widget margins
  void setMargins(const QMargins& newMargins);
  //! widget left margin
  int leftMargin();
  //! set the widget left margin
  void setLeftMargin(int margin);
  //! widget right margin
  int rightMargin();
  //! set the widget right margin
  void setRightMargin(int margin);
  //! widget top margin
  int topMargin();
  //! set the widget top margin
  void setTopMargin(int margin);
  //! widget bottom margin
  int bottomMargin();
  //! set the widget bottom margin
  void setBottomMargin(int margin);

  //! widget enabled flag.
  bool isEnabled() const;
  //! set the widget enabled flag
  void setEnabled(bool enabled);

  //! widget selected flag
  bool isSelected() const;
  //! set the widget selected flag
  void setSelected(bool selected);

  //! hide the widget.
  virtual void hideWidget();

  //! user data
  const QVariant& data() const;
  //! set the user data
  void setData(const QVariant& data);

  //! clone the widget
  WidgetItem* clone(WidgetItem* item = nullptr);

  //! set widget geometry
  void setGeometry(const QRect& rect, const QRect& = QRect());

  const QSize calcMinimumSize() const { return QSize(); }
  void paint(QPainter& /*painter*/) {}

  void paintBackground(QPainter& painter);

  const QSize& minContentSize() const;

  bool isHoverOver() const;

  void setHoverOver(bool newHoverOver);

  bool isVisible() const;

  QSize sizeHint() const;

  void widgetWasClicked(QPoint pos);
  const QRect& rect() const;

protected:
  //! pointer to parent
  WidgetItem* q_ptr = nullptr;
  //! parent dock widget
  AbstractDockWidget* m_parentWidget = nullptr;
  //! widget bounds
  QRect m_rect;
  //! widget type
  WidgetType m_type = Button;
  //! widget position, either Start or End
  WidgetPosition m_widgetPosition = Start;
  //! tooltip text
  QString m_tooltip;
  //! hover over flag.
  bool m_hoverOver = false;
  //! widget selected flag
  bool m_selected = false;
  //! widget enabled flag
  bool m_enabled = true;
  //! margins
  QMargins m_margins;
  //! minimum contents size
  QSize m_minContentSize;
  //! user data.
  QVariant m_data;

};

///*!
// * \class LabelWidget  widgetitem.h widgetitem.cpp
// * \brief The LabelWidget class supplies a simple label widget.
// */
// class LabelWidgetPrivate : WidgetItemPrivate
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
