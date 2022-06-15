#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDialog>
#include <QIcon>
#include <QLayout>
#include <QList>
#include <QListWidget>
#include <QMap>
#include <QMenu>
#include <QPainter>
#include <QPair>
#include <QResizeEvent>
#include <QToolTip>
#include <QWidget>
#include <QWidgetItem>

#include "x11colors.h"

#include "abstractdockwidget.h"

class Config;
class DockToolbar;
class DockFooter;
class DockHeader;
class DockCorner;


class DockCorner;
class DockItem;
class WidgetItem;

/*!
 * \brief The DockWidget defines an QWidget that emulates the QtCreator
 * Toolbar.
 *
 * It allows up to four toolbars at the est, West, North and South positions
 * plus a single header and a single footer. Various widgets can be added to
 * the toolbars and footers.
 */
class DockWidget : public AbstractDockWidget
{
  Q_OBJECT

public:
  explicit DockWidget(QWidget* parent);

  QWidget* widget() const;

  /*!
   * \brief Sets the central widget of the DockWidget.
   *
   * If the new QWidget is replacing an existing widget a pointer tothe old
   * QWidget is returned, otherwise a nullptr is returned. The old QWidget is
   * NOT deleted.
   *
   * \note If the QWidget is replaced with itself, the pointer is returned but
   * nothing else is done. You will need to delete it yourself.
   */
  QWidget* setWidget(QWidget* centralWidget);

  /*!
   * \brief Adds a new ToolbarWidget at the new position, if that position is
   * empty.
   *
   * If a ToolbarWidget already exists at that position then this is ignored.
   * If it a toolbar is to be replaced call removeToolbar(WidgetPosition)
   * first.
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

  DockFooter* addFooter();
  void removeFooter();
  void hideFooter();
  void showFooter();

  DockHeader* addHeader();
  void removeHeader();
  void hideHeader();
  void showHeader();


  /*!
   * \brief Moves the ToolbarWidget at oldPosition to newPosition, if it
   * exists and if there is no ToolbarWidget already at newPosition.
   *
   * If there is no ToolbarWidget at oldPosition, or there is already a
   * ToolbarWidget at newPosition nothing will happen, otherwise the move is
   * made.
   *
   * If there is already an existing ToolbarWidget at newPosition you will
   * need to call removeToolbar(Widgetposition) first.
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

  void setCorner(DockCorner* corner);
  DockCorner* corner(DockPosition position);

  //  /*!
  //   * \brief Helper method to create a multi image icon from their path
  //   strings
  //   *
  //   * Returns a QPair<QIcon, QSize> containing the new icon and the size of
  //   the
  //   * original image. This assumes that the images are all the same size for
  //   each
  //   * state. The images will be added in the order Normal, Active, Disabled,
  //   * Selected. Only four images are supported.
  //   */
  //  static const QPair<QIcon, QSize> iconFromImages(QStringList paths)
  //  {
  //    QIcon icon;
  //    QSize size;
  //    auto state = QIcon::Normal;
  //    for (auto& path : paths) {
  //      auto img = QImage(path);
  //      size = img.size();
  //      auto pix = QPixmap::fromImage(img);
  //      icon.addPixmap(pix, state);
  //      state =
  //        (state == QIcon::Normal ? QIcon::Active
  //                                : (state == QIcon::Active     ?
  //                                QIcon::Disabled
  //                                   : state == QIcon::Disabled ?
  //                                   QIcon::Selected
  //                                                              :
  //                                                              QIcon::Normal));
  //    }
  //    return qMakePair<QIcon, QSize>(icon, size);
  //  }
  //  /*!
  //   * \brief Helper method to create a multi image icon from their path
  //   strings
  //   *
  //   * Returns a QPair<QIcon, QSize> containing the new icon and the size of
  //   the
  //   * original image. This assumes that the images are all the same size for
  //   each
  //   * state. The images will be added in the order Normal, Active, Disabled,
  //   * Selected. Only four images are supported.
  //   */
  //  static const QPair<QIcon, QSize> iconFromImage(QString path)
  //  {
  //    QIcon icon;
  //    auto img = QImage(path);
  //    auto size = img.size();
  //    auto pix = QPixmap::fromImage(img);
  //    icon.addPixmap(pix, QIcon::Normal);
  //    return qMakePair<QIcon, QSize>(icon, size);
  //  }

signals:

protected:
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
  QHBoxLayout* m_layout = nullptr;
  QWidget* m_widget = nullptr;

  void resizeEvent(QResizeEvent* event) override;

private:

  void initGui();
  void calculateGeometry(const QRect& rect);
//  void mouseClickCheck(DockItem* item, QPoint pos);
//  bool dockItemHoverCheck(DockItem* item, QPoint pos);
  DockToolbar* toolbarAt(DockPosition position);
  DockToolbar* toolbarTakeAt(DockPosition position);
  bool setToolbarAt(DockPosition position, DockToolbar* toolbar = nullptr);
};


#endif // DOCKWIDGET_H
