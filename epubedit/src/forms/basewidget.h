#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include "widgetwrapper.h"

class BaseWidget : public QWidget
{
  Q_OBJECT
public:
  explicit BaseWidget(QWidget *parent = nullptr);
  ~BaseWidget();

  int addWidget(WidgetWrapper::Type type,
                WidgetWrapper::Position pos,
                QImage icon);
  int addWidget(WidgetWrapper::Type type,
                WidgetWrapper::Position pos,
                QImage icon,
                const QString& text);
  int addWidget(WidgetWrapper::Type type,
                WidgetWrapper::Position pos,
                const QString& text);
  void setTooltip(int index, const QString& tooltip);

  WidgetWrapper* takeAt(int index);
  WidgetWrapper* at(int index);

signals:

protected:
  QList<WidgetWrapper*> m_widgets;
  QBrush m_backColor, m_hoverBackColor;
  QColor m_textColor;
  QRect m_frameRect;

  static const int MARGIN = 5;
  static const int MARGINS = MARGIN * 2;
  static const int SPACER = 2;

  void paintEvent(QPaintEvent* event);
  void hoverEnter(QHoverEvent* event);
  void hoverLeave(QHoverEvent*);
  void hoverMove(QHoverEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent*);
  bool event(QEvent* event);

private:
  void paintBackground(QPainter& painter, WidgetWrapper* wrapper);
  void paintBtnIcon(QPainter& painter, ButtonWrapper* bw);
  void paintBtnText(QPainter& painter, ButtonWrapper* bw);
  WidgetWrapper* createWidgetWrapper(WidgetWrapper::Type type,
                                     WidgetWrapper::Position pos,
                                     QImage icon,
                                     const QString& text);

private:

};

#endif // BASEWIDGET_H
