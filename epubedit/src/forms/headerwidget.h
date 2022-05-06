#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPainter>
#include <QHoverEvent>

#include "basewidget.h"

class HeaderWidget : public BaseWidget
{
  Q_OBJECT
public:
  explicit HeaderWidget(QWidget* parent = nullptr);

signals:
  void buttonClicked(int index);

protected:
  void resizeEvent(QResizeEvent* event);

  static const int HEIGHT = 17;

private:
//  QRect m_frameRect;

//  void paintBackground(QPainter& painter, WidgetWrapper *w);
//  void paintBtnIcon(QPainter& painter, ButtonWrapper *w);
//  void paintBtnText(QPainter&, ButtonWrapper *w);

};

#endif // HEADERWIDGET_H
