#ifndef DOCKTYPES_H
#define DOCKTYPES_H

#include <QSharedPointer>

/*!
 * \enum WidgetType
 * Defines the various types of WidgetItem's.
 */
enum WidgetType
{
  Button,     //!< A ButtonWidget with text, an optional QIcon and tool tip.
  MenuButton, //!< A ListButtonWidget with text, an optional QIcon and tool tip
              //!< plus a dropdown menu.
  List,       //!< A Dropdown list of strings.
  Label,      //!< A simple label
  Seperator,  //!< A seperator bar
  Custom,     //!< Allows  custom widgets to be created
};

/*!
 * \enum WidgetPosition
 *
 * The WidgetPosition defines where the widget appears within the
 * toolbar/header/footer. The two options are Start and End.
 */
enum WidgetPosition
{
  Start, //!< The top or left position
  End,   //!< The bottom or right position
};

/*!
 * \enum Arrangement
 *
 * The Arrangement enum defines where, if any, text will appear with
 * respect to the icon, if any.
 *
 * \note These will generally only be used internally as the direct construction
 * methods such as addToolbarIconTextWidget(...) should generally be used.
 */
enum Arrangement
{
  TextAboveIcon,     //!< Text will appear above the icon
  TextBelowIcon,     //!< Text will appear below the icon
  TextAboveAndBelow, //!< Text will appear above and below the icon
  TextToRight,       //!< Text will appear to the right of the icon
  TextToLeft,        //!< Text will appear to the left of the icon
  IconOnly,          //!< No text, only an icon.
  TextOnly,          //!< No icon, only text.
  NoArrangement,     //!< Should never actually happen
};

/*!
 * \enum CornerType
 * The CornerType enum defines the type of Corner on a DockWidget..
 *
 * \note These will generally only be used internally as the direct construction
 * methods such as addToolbarIconTextWidget(...) should generally be used.
 */
enum CornerType
{
  Box,   //!< A blank box is formed at the corner.
  VEdge, //!< East/West DockToolbar's have priority at corners.
  HEdge, //!< North/South DockToolbar's, DockHeader's and DockFooter's have
         //!< priority at corners.
  None, /*!< DockToolbars, DockHeader's and DockFooter's have priority depending
          on order of creation.*/
};

/*!
 * \brief Defines where the DockToolbar appears within the DockWidget.
 *
 * Valid values are North (the top), South (the bottom), East (the right) and
 * West (the left). The other values are only used internally for the corners
 * and the central widget and will be ignored if manually entered.
 * There can be one toolbar at each position but the
 * single optional footer and the the single optional header will always
 * appear below the toolbars at these positions.
 *
 * \note that DockHeader's always appear at the top and DockFooter's
 * always at the bottom and there can only be one of each.
 */
enum DockPosition
{
  West,      //!< The West or Left side position
  North,     //!< The North or Top position
  South,     //!< The South or Bottom position.
  East,      //!< The East or Right position.
  NorthEast, //!< Only corner widgets.
  NorthWest, //!< Only corner widgets.
  SouthEast, //!< Only corner widgets.
  SouthWest, //!< Only corner widgets.
  Center     //!< The centre widget position.
};

#endif // DOCKTYPES_H
