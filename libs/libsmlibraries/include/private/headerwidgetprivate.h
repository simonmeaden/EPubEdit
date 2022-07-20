#ifndef HEADERWIDGETPRIVATE_H
#define HEADERWIDGETPRIVATE_H

#include <QFontMetrics>

#include "dockwidget.h"
#include "headerwidget.h"
#include "private/abstractdockwidgetprivate.h"

class HeaderWidgetPrivate : public AbstractDockWidgetPrivate
{
  Q_DECLARE_PUBLIC(HeaderWidget)
public:
  HeaderWidgetPrivate(HeaderWidget* parent);

  DockHeader* header();
  QWidget* setWidget(QWidget* widget);
  void hideWidget();
  void showWidget();
  bool isWidgetVisible();
  AbstractDockWidget* clone(AbstractDockWidget* widget = nullptr);
  void widgetWasClicked(QPoint);

  DockHeader* m_header;

  void paint(QPainter& painter);
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);
  void mousePress(QPoint pos);
  //  void mouseReleaseEvent(QMouseEvent*) ;
  //  bool event(QEvent* event) override;
  //  void resizeEvent(QResizeEvent* event) ;
  //  void focusInEvent(QFocusEvent* event) ;
  //  void focusOutEvent(QFocusEvent* event) ;

private:
  bool m_hoverIn = false;
  int m_width = 0;
  int m_height = 0;
  int m_headerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;

  void calculateGeometry(const QRect& rect);
};

#endif // HEADERWIDGETPRIVATE_H
