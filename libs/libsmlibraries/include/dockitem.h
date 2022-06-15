#ifndef DOCKITEM_H
#define DOCKITEM_H

#include <QWidget>
#include <QPainter>

#include "docktypes.h"

class AbstractDockWidget;
class WidgetItem;
class CustomWidget;

/*!
 * \class DockItem dockidget.h dockwidget.cpp
 * \brief This class is the base class of all the ToolbarWidget/HeaderWidget
 * and FooterWidget classes and does most of the background work of those
 * classes.
 */
class DockItem : public QObject
{
  Q_OBJECT
public:
  explicit DockItem(DockPosition position, AbstractDockWidget* parent);
  ~DockItem();

  /*!
   * \brief Adds an icon only button to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addIconButton(WidgetPosition pos,
                            QIcon icon,
                            QSize iconSize,
                            const QString& tooltip = QString());

  /*!
   * \brief Adds an icon with text button to the Header/Footer/Toolbar widget
   * and an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addIconTextButton(
    WidgetPosition pos,
    QIcon icon,
    QSize iconSize,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());

  /*!
   * \brief Adds an text only button to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addTextButton(WidgetPosition pos,
                            const QString& text,
                            const QString& tooltip = QString());

  /*!
   * \brief Adds a text label to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addTextLabel(WidgetPosition pos,
                           const QString& text,
                           const QString& tooltip = QString());

  /*!
   * \brief addSpacer
   * \return
   */
  WidgetItem* addSeperator(WidgetPosition);

  /*!
   * \brief Adds a custom widget to the Header/Footer/Toolbar widget.
   *
   *
   */
  WidgetItem* addCustomWidget(CustomWidget* w);

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
  WidgetItem* takeAt(int index);

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
  WidgetItem* at(int index);

  /*!
   * \brief Replaces the existing widget with another.
   *
   * The index must exist within the stored list. Returns true if
   * the replacement was successful, otherwise returns false.
   *
   * \return
   */
  bool replace(int index, WidgetItem* w);

  bool widgetEnabled(int index);
  bool enableWidget(int index, bool value);

  bool selected(int index);
  bool setSelected(int index, bool value);

  void calculateGeometry(const QRect& rect);

  QMargins widgetMargins(int index);
  void setWidgetMargins(int index, int left, int top, int right, int bottom);

  void setPreferredSize(int width, int height);
  void setPreferredSize(QSize size);
  void setPreferredWidth(int width);
  void setPreferredHeight(int height);
  virtual QSize sizeHint() const = 0;

  DockPosition dockPosition() const;

  int maxWidgetWidth() const;
  int maxWidgetHeight() const;

  int height() const;
  int width() const;

  bool isVisible() const;
  void setVisible(bool newShow);
  void show();
  void hide();

  virtual void paint(QPainter& painter);

  const QList<WidgetItem*>& widgets() const;

  void calcMaxWidgetSizes();

  const QRect &rect() const;

signals:
  void buttonClicked(int index);

protected:
  AbstractDockWidget* m_parent;
  DockPosition m_dockPosition;
  QFontMetrics m_fontMetrics;
  QList<WidgetItem*> m_widgets;
  bool m_visible = true;
  QRect m_rect;

  int m_width;
  int m_height;
  int m_maxWidgetWidth = 0;
  int m_maxWidgetHeight = 0;

  //  bool m_itemsSameSize = false;

  QMargins calcMaxMargins();
  QSize calcMaxContentsSize();

private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int SPACER_TOP = 1;
  static const int SPACER_BOTTOM = 1;
  // just places a blank space at the end of the toolbar.
  static const int TOOLBAR_ENDER = 5;

  WidgetItem* createButtonItem(WidgetType type,
                               WidgetPosition pos,
                               QIcon icon,
                               QSize iconSize,
                               const QString& text,
                               Arrangement textPos,
                               const QString& tooltip);
  int halfDifference(int large, int small) { return int((large - small) / 2); }
};

/*!
 * \class DockFooter basewidget.h basewidget.cpp
 * \brief The DockFooter class is a special case of BaseWidget that is
 *        fixed at the bottom
 *
 * Please note that DockFooter buttons only allow icons, any text is ignored.
 */
class DockFooter : public DockItem
{
public:
  explicit DockFooter(AbstractDockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  /*!
   * \brief Adds a ListWidget to the Footer widget with an optional tooltip.
   *
   * Items must be added to the list later as each item can have an optional
   * QIcon. Use ListWidget::addItem(QString), ListWidget::addItem(QIcon,
   * QString) or ListWidget::setItems(QStringList) to add items to the
   * ListWidget. ListWidget::setText(int, QString), ListWidget::setIcon(int,
   * QIcon) and ListWidget::setItem(int, QString, QIcon) to set or reset the
   * values of particular items.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addListWidget(WidgetPosition pos,
                            const QString& tooltip = QString());

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;
};

/*!
 * \class HeaderWidget basewidget.h basewidget.cpp
 * \brief The FooterWidget class is a special case of BaseWidget that is
 *        fixed at the top
 *
 * Please note that HeaderWidget buttons only allow icons, any text is
 * ignored.
 */

class DockHeader : public DockItem
{
public:
  explicit DockHeader(AbstractDockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  /*!
   * \brief Adds a ListWidget to the Header widget with an optional tooltip.
   *
   * Items must be added to the list later as each item can have an optional
   * QIcon. Use ListWidget::addItem(QString), ListWidget::addItem(QIcon,
   * QString) or ListWidget::setItems(QStringList) to add items to the
   * ListWidget. ListWidget::setText(int, QString), ListWidget::setIcon(int,
   * QIcon) and ListWidget::setItem(int, QString, QIcon) to set or reset the
   * values of particular items.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addListWidget(WidgetPosition pos,
                            const QString& tooltip = QString());

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;
};

class DockToolbar : public DockItem
{
public:
  explicit DockToolbar(DockPosition position, AbstractDockWidget* parent);

  static const int WIDTH = 60;
  static const int HEIGHT = 60;

  DockPosition dockPosition();
  void setDockPosition(DockPosition position);

  QSize sizeHint() const;

  void paint(QPainter& painter);

protected:
private:
};

class DockCorner : public DockItem
{
public:
  DockCorner(CornerType type, DockPosition position, AbstractDockWidget* parent);

  //  void resizeEvent(QResizeEvent* event);

  DockPosition dockPosition();
  void setDockPosition(DockPosition position);

  CornerType type() const;

  QSize sizeHint() const;

  int width() const;
  void setWidth(int newWidth);

  int height() const;
  void setHeight(int newHeight);

  void paint(QPainter& painter);

private:
  CornerType m_type;
  int m_width = -1;
  int m_height = -1;
};


#endif // DOCKITEM_H
