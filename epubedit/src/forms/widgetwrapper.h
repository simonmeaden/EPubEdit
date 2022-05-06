#ifndef WIDGETWRAPPER_H
#define WIDGETWRAPPER_H

#include <QWidget>

struct WidgetWrapper
{
  enum Type
  {
    BUTTON,
    CUSTOM,
  };
  enum Position
  {
    LEFT,   // ONLY USED FOR FOOTERS AND HEADERS
    RIGHT,  // ONLY USED FOR FOOTERS AND HEADERS
    TOP,    // ONLY USED FOR TOOLBARS
    BOTTOM, // ONLY USED FOR TOOLBARS
  };

  WidgetWrapper()
    : type(BUTTON)
    , pos(LEFT)
  {}
  QRect rect;
  QString tooltip;
  bool hoverOver = false;
  Type type;
  Position pos;
};

struct ButtonWrapper : public WidgetWrapper
{
  QRect iconRect;
  QRect textRect;
  QImage icon;
  QString text;
};

#endif // WIDGETWRAPPER_H
