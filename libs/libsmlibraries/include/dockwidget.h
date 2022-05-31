#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QIcon>
#include <QLayout>
#include <QList>
#include <QMap>
#include <QPainter>
#include <QResizeEvent>
#include <QToolTip>
#include <QWidget>
#include <QWidgetItem>

#include <algorithm>

//#include "borderlayout.h"
#include "x11colors.h"

class Config;
class DockToolbar;
class DockFooter;
class DockHeader;
class DockWidget;
typedef QSharedPointer<Config> PConfig;

enum WidgetType
{
  Button,
  List,
  Spacer,
  Custom,
};

/*!
 * \brief Defines where the DockToolbar appears within the DockWidget.
 *
 * Values are North (the top), South (the bottom), East (the right) and
 * West (the left). There can be one toolbar at each position but the
 * single optional footer and the the single optional header will always
 * appear below the toolbars at these positions.
 *
 * \note that DockHeader's always appear at the top and DockFooter's
 * always at the bottom and there can only be one of each.
 */

enum DockPosition
{
  West,
  North,
  South,
  East,
  NorthEast, //!< Only corner widgets.
  NorthWest, //!< Only corner widgets.
  SouthEast, //!< Only corner widgets.
  SouthWest, //!< Only corner widgets.
  Center
};

/*!
 * \enum The WidgetPosition defines where the widget appears within the
 * toolbar/header/footer.
 */
enum WidgetPosition
{
  Start, //!< The top or left position
  End,   //!< The bottom or right position
};

/*!
 * \enum The Arrangement enum defines where, if any, text will appear with
 * respect to the icon, if any.
 *
 * \note These will generally only be used internally as the direct construction
 * methods such as addToolbarIconTextWidget(...) should generally be used.
 */
enum Arrangement
{
  TextAboveIcon, //!< Text will appear above the icon
  TextBelowIcon, //!< Text will appear below the icon
  TextToRight,   //!< Text will appear to the right of the icon
  TextToLeft,    //!< Text will appear to the left of the icon
  IconOnly,      //!< No text, only an icon.
  TextOnly,      //!< No icon, only text.
  NoArrangement, //!< Should never actually happen
};

/*!
 * \enum The CornerType enum defines the type of Corner on a DockWidget..
 *
 * \note These will generally only be used internally as the direct construction
 * methods such as addToolbarIconTextWidget(...) should generally be used.
 */
enum CornerType
{
  Box,   //!< A blank box is formed at the corner.
  VEdge, //!< East/West DockToolbar's have priority at corners.
  HEdge, //!< North/South DockToolbar's, DockHeader's and DockFooter's have
         //!< priority at corners.
  None, /*!< DockToolbars, DockHeader's and DockFooter's have priority depending
          on order of creation.*/
};

class DockCorner;
class DockItem;


class WidgetItem : public QObject
{
  Q_OBJECT
public:
  WidgetItem(DockWidget* parent);

signals:
  void widgetClicked();
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

  Arrangement arrangement() const;
  void setArrangement(Arrangement newArrangement);

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
  DockWidget* m_parent;
  QFontMetrics m_fontMetrics;
  QRect m_rect;
  WidgetType m_type = Button;
  WidgetPosition m_widgetPosition = Start;
  Arrangement m_arrangement = IconOnly;
  QString m_tooltip;
  bool m_hoverOver = false;
  bool m_selected = false;
  bool m_enabled = true;
  QMargins m_margins;
  QSize m_minContentSize;

  int halfDifference(int large, int small);
};

class SpacerWidget : public WidgetItem
{
  Q_OBJECT
public:
  SpacerWidget(DockWidget* parent);

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
  ButtonWidget(DockWidget* parent);

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

  // WidgetWrapper interface
  const QSize calcMinimumSize();
  void paint(QPainter& painter);
  //  void resizeEvent(QRect& r, int& min, int& max);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);

private:
  QRect m_iconRect;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_textRect;
  QString m_text;
  int m_spacer = TEXT_SPACER;
  QColor m_textColor;
};

/*!
 * \brief The DockWidget defines an QWidget that emulates the QtCreator Toolbar.
 *
 * It allows up to four toolbars at the est, West, North and South positions
 * plus a single header and a single footer. Various widgets can be added to the
 * toolbars and footers.
 */
class DockWidget : public QWidget
{
  Q_OBJECT

public:
  explicit DockWidget(QWidget* parent);

  QWidget* centreWidget() const;
  void setCentalWidget(QWidget* newCentreWidget);

  /*!
   * \brief Adds a new ToolbarWidget at the new position, if that position is
   * empty.
   *
   * If a ToolbarWidget already exists at that position then this is ignored. If
   * it a toolbar is to be replaced call removeToolbar(WidgetPosition) first.
   */
  DockToolbar* addToolbar(DockPosition position);

  /*!
   * \brief Removes and deletes an existing ToolbarWidget at position, if it
   * exists, otherwise nothing is done.
   */
  void removeToolbar(DockPosition position);
  void removeToolbar(DockToolbar* toolbar);
  void hideToolbar(DockPosition position);
  void hideToolbar(DockToolbar* toolbar);
  void showToolbar(DockPosition position);
  void showToolbar(DockToolbar* toolbar);

//  void setToolbarPosition(DockPosition oldPosition, DockPosition newPosition);

  DockFooter* addFooter();
  void removeFooter();
  void hideFooter();
  void showFooter();

  DockHeader* addHeader();
  void removeHeader();
  void hideHeader();
  void showHeader();

  const QBrush& backColor() const;
  void setBackColor(const QBrush& newBackColor);

  const QBrush& hoverBackColor() const;
  void setHoverBackColor(const QBrush& newHoverBackColor);

  const QBrush& selectedColor() const;
  void setSelectedColor(const QBrush& newSelectedColor);

  const QColor& textColor() const;
  void setTextColor(const QColor& newTextColor);

  const QColor& spacerColor() const;
  void setSpacerColor(const QColor& newSpacerColor);

  WidgetItem* addToolbarIconButton(DockPosition toolbarPos,
                                   WidgetPosition pos,
                                   QImage icon,
                                   const QString& tooltip = QString());
  WidgetItem* addFooterIconButton(WidgetPosition pos,
                                  QImage icon,
                                  const QString& tooltip = QString());
  WidgetItem* addHeaderIconButton(WidgetPosition pos,
                                  QImage icon,
                                  const QString& tooltip = QString());
  WidgetItem* addToolbarIconTextButton(DockPosition toolbarPos,
                                       WidgetPosition position,
                                       QImage icon,
                                       const QString& text,
                                       Arrangement textPos,
                                       const QString& tooltip = QString());
  WidgetItem* addHeaderIconTextButton(WidgetPosition position,
                                      QImage icon,
                                      const QString& text,
                                      Arrangement textPos,
                                      const QString& tooltip = QString());
  WidgetItem* addFooterIconTextButton(WidgetPosition position,
                                      QImage icon,
                                      const QString& text,
                                      Arrangement textPos,
                                      const QString& tooltip = QString());
  WidgetItem* addToolbarTextButton(DockPosition toolbarPos,
                                   WidgetPosition position,
                                   const QString& text,
                                   const QString& tooltip = QString());
  WidgetItem* addHeaderTextButton(WidgetPosition position,
                                  const QString& text,
                                  const QString& tooltip = QString());
  WidgetItem* addFooterTextButton(WidgetPosition position,
                                  const QString& text,
                                  const QString& tooltip = QString());
  WidgetItem* addToolbarSpacer(DockPosition toolbarPos,
                               WidgetPosition position);
  WidgetItem* addHeaderSpacer(WidgetPosition position);
  WidgetItem* addFooterSpacer(WidgetPosition position);

  /*!
   * \brief Moves the ToolbarWidget at oldPosition to newPosition, if it exists
   * and if there is no ToolbarWidget already at newPosition.
   *
   * If there is no ToolbarWidget at oldPosition, or there is already a
   * ToolbarWidget at newPosition nothing will happen, otherwise the move is
   * made.
   *
   * If there is already an existing ToolbarWidget at newPosition you will need
   * to call removeToolbar(Widgetposition) first.
   */
  bool moveToolbar(DockPosition newPosition, DockPosition oldPosition);

  void setCorner(DockPosition position, CornerType type);
  void setCorners(CornerType northWest,
                  CornerType northEast,
                  CornerType southWest,
                  CornerType southEast);

  void setCornerSize(DockPosition position, int width, int height);
  void setCornerSize(DockPosition position, QSize size);
  void setCornerWidth(DockPosition position, int width);
  void setCornerHeight(DockPosition position, int height);

  void paintEvent(QPaintEvent* event) override;
  void hoverEnterEvent(QHoverEvent* event);
  void hoverLeaveEvent(QHoverEvent*);
  void hoverMoveEvent(QHoverEvent* event);
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  bool event(QEvent* event) override;

  void hoverWidgetEvent();

  void setCorner(DockCorner* corner);
  DockCorner* corner(DockPosition position);


signals:
  void geometryChanged(int north, int south, int east, int west);

protected:
  //  DockLayout* m_layout;

  void resizeEvent(QResizeEvent* event) override;

private:
  //  QMap<DockPosition, DockToolbar*> m_toolbars;
  DockFooter* m_footer = nullptr;
  DockHeader* m_header = nullptr;
  DockCorner* m_northWest = nullptr;
  DockCorner* m_northEast = nullptr;
  DockCorner* m_southWest = nullptr;
  DockCorner* m_southEast = nullptr;
  DockToolbar* m_northToolbar = nullptr;
  DockToolbar* m_southToolbar = nullptr;
  DockToolbar* m_eastToolbar = nullptr;
  DockToolbar* m_westToolbar = nullptr;
  QWidget* m_centralWidget = nullptr;

  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QColor m_textColor;
  QColor m_spacerColor;

  WidgetItem* m_hoverItem = nullptr;

  void initGui();
  void calculateGeometry(const QRect& rect);
  void mouseClickCheck(DockItem* item, QPoint pos);
  bool dockItemHoverCheck(DockItem* item, QPoint pos);
  DockToolbar* toolbarAt(DockPosition position);
  DockToolbar* toolbarTakeAt(DockPosition position);
  bool setToolbarAt(DockPosition position, DockToolbar* toolbar = nullptr);
};

/*!
 * \class DockItem dockidget.h dockwidget.cpp
 * \brief This class is the base class of all the ToolbarWidget/HeaderWidget and
 * FooterWidget classes and does most of the background work of those classes.
 */
class DockItem : public QObject
{
  Q_OBJECT
public:
  explicit DockItem(DockPosition position, DockWidget* parent);
  ~DockItem();

  /*!
   * \brief Adds an icon only button to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return the index of the stored wrapper.
   */
  WidgetItem* addIconButton(WidgetPosition pos,
                            QImage icon,
                            const QString& tooltip = QString());
  /*!
   * \brief Adds an icon with text button to the Header/Footer/Toolbar widget
   * and an optional tooltip.
   *
   * \return the index of the stored wrapper.
   */
  WidgetItem* addIconTextButton(
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
  WidgetItem* addTextButton(WidgetPosition pos,
                            const QString& text,
                            const QString& tooltip = QString());

  WidgetItem* addSpacer(WidgetPosition);

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

signals:
  void buttonClicked(int index);

protected:
  DockWidget* m_parent;
  DockPosition m_dockPosition;
  QFontMetrics m_fontMetrics;
  QList<WidgetItem*> m_widgets;
  bool m_visible = true;
  QRect m_rect;

  int m_width;
  int m_height;
  int m_maxWidgetWidth = 0;
  int m_maxWidgetHeight = 0;

  bool m_itemsSameSize = false;

  QMargins calcMaxMargins();
  QSize calcMaxContentsSize();
  void calcWidgetSizes();

private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int SPACER_TOP = 1;
  static const int SPACER_BOTTOM = 1;
  // just places a blank space at the end of the toolbar.
  static const int TOOLBAR_ENDER = 5;

  WidgetItem* createWidgetItem(WidgetType type,
                               WidgetPosition pos,
                               QImage image,
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
  explicit DockFooter(DockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;
};

/*!
 * \class HeaderWidget basewidget.h basewidget.cpp
 * \brief The FooterWidget class is a special case of BaseWidget that is
 *        fixed at the top
 *
 * Please note that HeaderWidget buttons only allow icons, any text is ignored.
 */

class DockHeader : public DockItem
{
public:
  explicit DockHeader(DockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;
};

class DockToolbar : public DockItem
{
public:
  explicit DockToolbar(DockPosition position, DockWidget* parent);

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
  DockCorner(CornerType type, DockPosition position, DockWidget* parent);

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

#endif // DOCKWIDGET_H
