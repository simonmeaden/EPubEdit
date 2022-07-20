#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

//#include <QWidget>
//#include <QEvent>
//#include <QHoverEvent>
//#include <QMouseEvent>
//#include <QPaintEvent>
//#include <QPainter>

#include "basewidget.h"

class ToolbarWidget : public BaseWidget
{
public:
  explicit ToolbarWidget(WidgetPosition position = WidgetPosition::Left,
                         QWidget* parent = nullptr);

  static const int WIDTH = 60;
  static const int HEIGHT = 60;

protected:
//  void resizeEvent(QResizeEvent* event);
//  void hoverEnter(QHoverEvent* event);
  //  void hoverLeave(QHoverEvent*);
  //  void hoverMove(QHoverEvent* event);
  //  void mousePressEvent(QMouseEvent* event);
  //  void mouseReleaseEvent(QMouseEvent* event);
  //  bool event(QEvent* event);

private:
  //  void paintBackground(QPainter& painter, WidgetWrapper* wrapper);
  //  void paintBtnIcon(QPainter& painter, ButtonWrapper* bw);
  //  void paintBtnText(QPainter& painter, ButtonWrapper* bw);
};

#endif // TOOLBARWIDGET_H
