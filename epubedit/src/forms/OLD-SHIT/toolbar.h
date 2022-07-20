#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPainter>
#include <QHoverEvent>
#include "borderlayout.h"

class Toolbar : public QWidget
{
  Q_OBJECT
public:
  explicit Toolbar(QWidget* parent = nullptr);

signals:

protected:
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  void hoverEnter(QHoverEvent* event);
  void hoverLeave(QHoverEvent* event);
  void hoverMove(QHoverEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  bool event(QEvent* event);

private:
  QBrush m_back, m_hoverBack;
  QRect m_frameRect;

  static const int WIDTH = 50;
  static const int BUTTON_HEIGHT = 50;
};

#endif // TOOLBAR_H
