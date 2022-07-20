#ifndef WIDGETENUM_H
#define WIDGETENUM_H

enum WidgetType
{
  Button,
  List,
  Spacer,
  Custom,
};

enum WidgetPosition
{
  Left,   // ONLY USED FOR FOOTERS AND HEADERS
  Right,  // ONLY USED FOR FOOTERS AND HEADERS
  Top,    // ONLY USED FOR TOOLBARS
  Bottom, // ONLY USED FOR TOOLBARS
};

enum Arrangement
{
  TextAboveIcon,
  TextBelowIcon,
  TextToRight,
  TextToLeft,
  IconOnly,
  TextOnly,
  NoArrangement,
};

#endif // WIDGETENUM_H
