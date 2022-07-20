#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QEvent>
#include <QHoverEvent>
#include <QIcon>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QToolTip>
#include <QWidget>

#include "widgetwrapper.h"

/*!
 * \class BaseWidget basewidget.h basewidget.cpp
 * \brief This class is the base class of all the ToolbarWidget/HeaderWidget and
 * FooterWidget classes and does most of the background work of those classes.
 */

/*!
 * \class ToolbarWidget basewidget.h basewidget.cpp
 * \brief This class is the base class of all the ToolbarWidget/HeaderWidget and
 * FooterWidget classes and does most of the background work of those classes.
 */
class BaseWidget : public QWidget
{
  Q_OBJECT
public:
  explicit BaseWidget(WidgetPosition position, QWidget* parent = nullptr);
  ~BaseWidget();

  /*!
   * \brief Adds an icon only button to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return the index of the stored wrapper.
   */
  WidgetWrapper* addIconButton(WidgetPosition pos,
                               QImage icon,
                               const QString& tooltip = QString());
  /*!
   * \brief Adds an icon with text button to the Header/Footer/Toolbar widget
   * and an optional tooltip.
   *
   * \return the index of the stored wrapper.
   */
  WidgetWrapper* addIconTextButton(
    WidgetPosition pos,
    QImage icon,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());
  /*!
   * \brief Adds an text only button to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return the index of the stored wrapper.
   */
  WidgetWrapper* addTextButton(WidgetPosition pos,
                               const QString& text,
                               const QString& tooltip = QString());

  WidgetWrapper* addSpacer(WidgetPosition);

  /*!
   * \brief Adds a custom widget to the Header/Footer/Toolbar widget.
   *
   *
   */
  WidgetWrapper* addCustomWidget(CustomWidgetWrapper* w);

  /*!
   * \brief Sets the text in the Header/Footer/Toolbar widget.
   *
   * The text is set to the supplied value ONLY if the widget is
   * of an appropriate type, otherwise it is ignored. For example
   * an ICON_WIDGET has no text.
   */
  void setText(int index, const QString& text);
  /*!
   * \brief Sets the tooltip in the widget.
   */
  void setToolTip(int index, const QString& tooltip);

  /*!
   * \brief Returns and removes the widget at the index.
   *
   * If the index value is invalid then a nullptr is returned, otherwise
   * the widget at that index is returned. The widget is no longer stored
   * in the list.
   *
   * \note The index indicates the position in which the widget was added,
   * not necessarily the position within the list.
   */
  WidgetWrapper* takeAt(int index);

  /*!
   * \brief Returns the widget at the index.
   *
   * If the index value is invalid then a nullptr is returned, otherwise
   * the widget at that index is returned. The widget is still stored
   * in the list.
   *
   * \note The index indicates the position in which the widget was added,
   * not necessarily the position within the list.
   */
  WidgetWrapper* at(int index);

  /*!
   * \brief Replaces the existing widget with another.
   *
   * The index must exist within the stored list. Returns true if
   * the replacement was successful, otherwise returns false.
   *
   * \return
   */
  bool replace(int index, WidgetWrapper* w);

  //  int spacer() const;
  //  void setSpacer(int newSpacer);

  bool widgetEnabled(int index);
  bool enableWidget(int index, bool value);

  bool selected(int index);
  bool setSelected(int index, bool value);

  QMargins widgetMargins(int index);
  void setWidgetMargins(int index, int left, int top, int right, int bottom);

  void setPreferredSize(int width, int height);
  void setPreferredSize(QSize size);
  void setPreferredWidth(int width);
  void setPreferredHeight(int height);
  QSize sizeHint() const;

  WidgetPosition widgetPosition() const;

  int maxWidth() const;
  int maxHeight() const;

signals:
  void buttonClicked(int index);

protected:
  WidgetPosition m_widgetPosition;
  QList<WidgetWrapper*> m_widgets;
  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QColor m_textColor;
  QColor m_spacerColor;

  QRect m_frameRect;
  int m_width;
  int m_height;
  //  int m_spacer;
  int m_maxWidgetWidth = 0;
  int m_maxWidgetHeight = 0;

  void paintEvent(QPaintEvent* event);
  void hoverEnterEvent(QHoverEvent* event);
  void hoverLeaveEvent(QHoverEvent*);
  void hoverMoveEvent(QHoverEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent*);
  bool event(QEvent* event);
  //  void paintBackground(QPainter& painter, WidgetWrapper* wrapper);
  //  void paintBtnIcon(QPainter& painter, WidgetWrapper* w);
  //  void paintBtnText(QPainter& painter, WidgetWrapper* bw);
  void resizeEvent(QResizeEvent* event);

private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int SPACER_TOP = 1;
  static const int SPACER_BOTTOM = 1;

  WidgetWrapper* createWidgetWrapper(WidgetType type,
                                     WidgetPosition pos,
                                     QImage image,
                                     const QString& text,
                                     Arrangement textPos,
                                     const QString& tooltip);
  //  void paintPixmap(QPainter& painter, QPixmap& pixmap, const QRect&
  //  iconRect); void paintBackground(QPainter& painter, QRect rect, QBrush&
  //  color); void paintSpacer(QPainter& painter, WidgetWrapper* w);
  int halfDifference(int large, int small) { return int((large - small) / 2); }
  QMargins calcMaxMargins();
  QSize calcMaxContentsSize();
  void calcWidgetSizes();
};

/*!
 * \class FooterWidget basewidget.h basewidget.cpp
 * \brief The FooterWidget class is a special case of BaseWidget that is
 *        fixed at the bottom
 *
 * Please note that FooterWidget buttons only allow icons, any text is ignored.
 */
class FooterWidget : public BaseWidget
{

public:
  explicit FooterWidget(QWidget* parent = nullptr);

protected:
private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
};
/*!
 * \class HeaderWidget basewidget.h basewidget.cpp
 * \brief The FooterWidget class is a special case of BaseWidget that is
 *        fixed at the top
 *
 * Please note that HeaderWidget buttons only allow icons, any text is ignored.
 */

class HeaderWidget : public BaseWidget
{
  Q_OBJECT
public:
  explicit HeaderWidget(QWidget* parent = nullptr);

signals:

protected:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;

private:
};

class ToolbarWidget : public BaseWidget
{
public:
  explicit ToolbarWidget(WidgetPosition position, QWidget* parent = nullptr);

  static const int WIDTH = 60;
  static const int HEIGHT = 60;

  WidgetPosition position();
  void setPosition(WidgetPosition position);

protected:
private:
};

// class LeftToolbarWidget : public ToolbarWidget
//{
// public:
//   explicit LeftToolbarWidget(QWidget* parent = nullptr);

// protected:
// private:
// };

// class RightToolbarWidget : public ToolbarWidget
//{
// public:
//   explicit RightToolbarWidget(QWidget* parent = nullptr);

// protected:
// private:
// };

#endif // BASEWIDGET_H
