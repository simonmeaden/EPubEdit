#ifndef DOCKWIDGETPRIVATE_H
#define DOCKWIDGETPRIVATE_H

#include "abstractdockwidgetprivate.h"
#include "docker/dockwidget.h"

class DockFooter;
class DockHeader;
class DockCorner;
class DockToolbar;

class DockWidgetPrivate : public AbstractDockWidgetPrivate
{
  Q_DECLARE_PUBLIC(DockWidget)
public:
  DockWidgetPrivate(DockWidget* parent);

//  QWidget* widget() const;
//  QWidget* setWidget(QWidget* widget);

  DockToolbar* addToolbar(DockPosition position);
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

  bool moveToolbar(DockPosition newPosition, DockPosition oldPosition);

  void setCorner(DockPosition position, CornerType type);
  void setCorners(CornerType northWest,
                  CornerType northEast,
                  CornerType southWest,
                  CornerType southEast);

  void setCornerSize(DockPosition position, int width, int height);
  void setCornerWidth(DockPosition position, int width);
  void setCornerHeight(DockPosition position, int height);

  void paint(QPainter& painter);
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);
  void mousePress(QPoint pos);

  DockHeader* header();
  DockFooter* footer();
  DockToolbar* northToolbar();
  DockToolbar* southToolbar();
  DockToolbar* westToolbar();
  DockToolbar* eastToolbar();

  void widgetWasClicked(QPoint);
  void setCorner(DockCorner* corner);
  DockCorner* corner(DockPosition position);

  AbstractDockWidget* clone(AbstractDockWidget* widget);

  //  void initGui();
  void calculateGeometry(const QRect& rect);
  DockToolbar* toolbarAt(DockPosition position);
  DockToolbar* toolbarTakeAt(DockPosition position);
  bool setToolbarAt(DockPosition position, DockToolbar* toolbar = nullptr);

private:
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
};

#endif // DOCKWIDGETPRIVATE_H
