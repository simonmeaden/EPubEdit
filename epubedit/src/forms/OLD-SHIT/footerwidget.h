#ifndef FOOTERWIDGET_H
#define FOOTERWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QHoverEvent>
#include <QMouseEvent>
#include <QPainter>

#include "basewidget.h"

/*!
 * \class FooterWidget footerwidget.h footerwidget.cpp
 * \brief The FooterWidget class
 *
 * Please note that FooterWidget buttons only allow icons, any text is ignored.
 */
class FooterWidget : public BaseWidget
{

public:
  explicit FooterWidget(QWidget* parent = nullptr);

protected:
//  void resizeEvent(QResizeEvent* event);
//  void paintEvent(QPaintEvent* event);

//signals:
//  void buttonClicked(int index);

private:

  static const int HEIGHT = 25;
  static const int WIDTH = 25;
  static const int ICON_MARGIN = 5;
  static const int ICON_MARGINS = ICON_MARGIN * 2;
};

#endif // FOOTERWIDGET_H
