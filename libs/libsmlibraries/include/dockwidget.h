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

//#include "borderlayout.h"
#include "x11colors.h"

class Config;
class DockToolbar;
class DockFooter;
class DockHeader;
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

// enum DockPosition
//{
//   North, //!< The top of the DockWidget.
//   South, //!< The Bottom of the DockWidget.
//   East,  //!< The Right of the DockWidget.
//   West,  //!< The Left of the DockWidget.
//   NorthEast, //!< Only corner widgets.
//   NorthWest, //!< Only corner widgets.
//   SouthEast, //!< Only corner widgets.
//   SouthWest, //!< Only corner widgets.
// };

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
  Box,    //!< A blank box is formed at the corner.
  Height, //!< DockToolbar's have priority at corners.
  Width,  //!< DockHeader's and DockFooter's have priority at corners.
  None,   //!< DockToolbars, DockHeader's and DockFooter's have priority
          //!< depending on order of creation.
};

class DockLayout : public QLayout
{
public:
  enum Position
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

  explicit DockLayout(QWidget* parent,
                      const QMargins& margins = QMargins(),
                      int spacing = -1);
  DockLayout(int spacing = -1);
  ~DockLayout();

  void addItem(QLayoutItem* item) override;
  void addWidget(QWidget* widget, Position position);
  Qt::Orientations expandingDirections() const override;
  bool hasHeightForWidth() const override;
  int count() const override;
  QLayoutItem* itemAt(int index) const override;
  QSize minimumSize() const override;
  void setGeometry(const QRect& rect) override;
  QSize sizeHint() const override;
  QLayoutItem* takeAt(int index) override;

  void add(QLayoutItem* item, Position position);

  CornerType topLeft() const;
  void setTopLeft(CornerType type);
  CornerType topRight() const;
  void setTopRight(CornerType type);
  CornerType bottomLeft() const;
  void setBottomLeft(CornerType type);
  CornerType bottomRight() const;
  void setBottomRight(CornerType type);
  void setCornerType(CornerType type);
  void setCornerType(CornerType topLeft,
                     CornerType topRight,
                     CornerType bottomLeft,
                     CornerType bottomRight);

private:
  struct ItemWrapper
  {
    ItemWrapper(QLayoutItem* i, Position p)
    {
      item = i;
      position = p;
    }

    QLayoutItem* item;
    Position position;
  };

  QList<ItemWrapper*> m_items;
  CornerType m_northWest = None;
  CornerType m_northEast = None;
  CornerType m_southWest = None;
  CornerType m_southEast = None;

  enum SizeType
  {
    MinimumSize,
    SizeHint
  };
  QSize calculateSize(SizeType sizeType) const;
};

class WidgetWrapper : public QObject
{
  Q_OBJECT
public:
  WidgetWrapper(QObject* parent = nullptr);

signals:
  void widgetClicked();
  /*!
   * \brief  This signal is emitted when the widget is changed in any way that
   *         would modify the layout. This could be a size change, a margin
   *         change or an arrangement change.
   */
  void widgetChanged();

public:
  static const int TOPMARGIN = 3;
  static const int BOTTOMMARGIN = 3;
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
  void setRect(const QRect& newRect);

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

  virtual const QSize calcSize() = 0;
  virtual void paintWidget(QPainter& painter) = 0;
  virtual void resizeEvent(QRect& r, int& min, int& max) = 0;

  void paintBackground(QPainter& painter);

  const QBrush& backColor() const;
  void setBackColor(const QBrush& newBackColor);

  const QBrush& hoverBackColor() const;
  void setHoverBackColor(const QBrush& newHoverBackColor);

  const QBrush& selectedColor() const;
  void setSelectedColor(const QBrush& newSelectedColor);

  const QSize& content() const;

protected:
  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QRect m_rect;
  WidgetType m_type = Button;
  WidgetPosition m_widgetPosition = Start;
  Arrangement m_arrangement = IconOnly;
  QString m_tooltip;
  bool m_hoverOver = false;
  bool m_selected = false;
  bool m_enabled = true;
  QMargins m_margins;
  QSize m_content;

  int halfDifference(int large, int small);
};

class SpacerWidget : public WidgetWrapper
{
  Q_OBJECT
public:
  SpacerWidget(QObject* parent = nullptr);

  const QColor& color() const;
  void setColor(const QColor& newColor);

  int thickness() const;
  void setThickness(int newThickness);

  // WidgetWrapper interface
  const QSize calcSize();
  void paintWidget(QPainter& painter);
  void resizeEvent(QRect& r, int& min, int& max);

private:
  QColor m_color;
  int m_thickness = 1;
};

class CustomWidget : public WidgetWrapper
{};

class ButtonWidget : public WidgetWrapper
{
  Q_OBJECT
public:
  ButtonWidget(QObject* parent = nullptr);

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
  const QSize calcSize();
  void paintWidget(QPainter& painter);
  void resizeEvent(QRect& r, int& min, int& max);

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
  explicit DockWidget(QWidget* parent = nullptr);
  //  DockWidget(DockWidget&&) = default;

  QWidget* centreWidget() const;
  void setCentreWidget(QWidget* newCentreWidget);

  /*!
   * \brief Adds a new ToolbarWidget at the new position, if that position is
   * empty.
   *
   * If a ToolbarWidget already exists at that position then this is ignored. If
   * it a toolbar is to be replaced call removeToolbar(WidgetPosition) first.
   */
  void addToolbar(DockLayout::Position position);
  /*!
   * \brief Removes and deletes an existing ToolbarWidget at position, if it
   * exists, otherwise nothing is done.
   */
  void removeToolbar(DockLayout::Position osition);
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
  void setToolbarPosition(DockLayout::Position oldPosition,
                          DockLayout::Position newPosition);

  void addFooter();
  void addHeader();

  WidgetWrapper* addToolbarIconButton(DockLayout::Position toolbarPos,
                                      WidgetPosition pos,
                                      QImage icon,
                                      const QString& tooltip = QString());
  WidgetWrapper* addFooterIconButton(WidgetPosition pos,
                                     QImage icon,
                                     const QString& tooltip = QString());
  WidgetWrapper* addHeaderIconButton(WidgetPosition pos,
                                     QImage icon,
                                     const QString& tooltip = QString());
  WidgetWrapper* addToolbarIconTextButton(DockLayout::Position toolbarPos,
                                          WidgetPosition position,
                                          QImage icon,
                                          const QString& text,
                                          Arrangement textPos,
                                          const QString& tooltip = QString());
  WidgetWrapper* addHeaderIconTextButton(WidgetPosition position,
                                         QImage icon,
                                         const QString& text,
                                         Arrangement textPos,
                                         const QString& tooltip = QString());
  WidgetWrapper* addFooterIconTextButton(WidgetPosition position,
                                         QImage icon,
                                         const QString& text,
                                         Arrangement textPos,
                                         const QString& tooltip = QString());
  WidgetWrapper* addToolbarTextButton(DockLayout::Position toolbarPos,
                                      WidgetPosition position,
                                      const QString& text,
                                      const QString& tooltip = QString());
  WidgetWrapper* addHeaderTextButton(WidgetPosition position,
                                     const QString& text,
                                     const QString& tooltip = QString());
  WidgetWrapper* addFooterTextButton(WidgetPosition position,
                                     const QString& text,
                                     const QString& tooltip = QString());
  WidgetWrapper* addToolbarSpacer(DockLayout::Position toolbarPos,
                                  WidgetPosition position);
  WidgetWrapper* addHeaderSpacer(WidgetPosition position);
  WidgetWrapper* addFooterSpacer(WidgetPosition position);

protected:
  DockLayout* m_layout;

  void setCornerType(CornerType topLeft,
                     CornerType topRight,
                     CornerType bottomLeft,
                     CornerType bottomRight);

private:
  QMap<DockLayout::Position, DockToolbar*> m_toolbars;
  DockFooter* m_footer = nullptr;
  DockHeader* m_header = nullptr;

  QWidget* m_centreWidget;

  void initGui();
  DockToolbar* addNewToolbar(DockLayout::Position position);
};

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
class DockWidgetItem : public QWidget
{
  Q_OBJECT
public:
  explicit DockWidgetItem(DockLayout::Position position,
                          QWidget* parent = nullptr);
  ~DockWidgetItem();

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
  WidgetWrapper* addCustomWidget(CustomWidget* w);

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

  DockLayout::Position dockPosition() const;

  int maxWidth() const;
  int maxHeight() const;

signals:
  void buttonClicked(int index);

protected:
  DockLayout::Position m_dockPosition;
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
  void resizeEvent(QResizeEvent* event);

private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int SPACER_TOP = 1;
  static const int SPACER_BOTTOM = 1;
  // just places a blank space at the end of the toolbar.
  static const int TOOLBAR_ENDER = 5;

  WidgetWrapper* createWidgetWrapper(WidgetType type,
                                     WidgetPosition pos,
                                     QImage image,
                                     const QString& text,
                                     Arrangement textPos,
                                     const QString& tooltip);
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
class DockFooter : public DockWidgetItem
{
public:
  explicit DockFooter(QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event);

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

class DockHeader : public DockWidgetItem
{
public:
  explicit DockHeader(QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event);

private:
  static const int HEIGHT = 25;
  static const int WIDTH = 25;
};

class DockToolbar : public DockWidgetItem
{
public:
  explicit DockToolbar(DockLayout::Position position,
                       QWidget* parent = nullptr);

  static const int WIDTH = 60;
  static const int HEIGHT = 60;

  DockLayout::Position dockPosition();
  void setDockPosition(DockLayout::Position position);

protected:
  void paintEvent(QPaintEvent* event);

private:
};

class DockWidthCorner : public DockWidgetItem
{
public:
  DockWidthCorner(DockLayout::Position position, QWidget* parent = nullptr);

  // WidgetWrapper interface
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

protected:
  DockLayout::Position dockPosition();
  void setDockPosition(DockLayout::Position position);

private:
};

class DockHeightCorner : public DockWidgetItem
{
public:
  DockHeightCorner(DockLayout::Position position, QWidget* parent = nullptr);

  // WidgetWrapper interface
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

protected:
  DockLayout::Position dockPosition();
  void setDockPosition(DockLayout::Position position);

private:
};

class DockBoxCorner : public DockWidgetItem
{
public:
  DockBoxCorner(DockLayout::Position position, QWidget* parent = nullptr);

  // WidgetWrapper interface
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);

protected:
  DockLayout::Position dockPosition();
  void setDockPosition(DockLayout::Position position);

private:
};

#endif // DOCKWIDGET_H
