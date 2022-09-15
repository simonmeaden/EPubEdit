#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <QChar>

/*!
  \ingroup utilities
  \file characters.h
  \brief A holding place for reusable unicode characters.
  \author Simon Meaden
  \date 2022-08-10

  A partial set of useful unicode characters.

  \note Some of the characters have multiple names for the same character. An example
        is the forward slash character which has two names FORWARDSLASH and SOLIDUS.
        This is because it is actually called a solidus character in unicode but in
        many internal programming uses the name FORWARDSLASH is more descriptive of
        it's actual use.
 */
namespace Characters {
//! ⯅ North pointing triangle
constexpr static QChar N_TRIANGLE = QChar(0x2BC5);
//! ⯆ South pointing triangle
constexpr static QChar S_TRIANGLE = QChar(0x2BC6);
//! ⯇ West pointing triangle
constexpr static QChar W_TRIANGLE = QChar(0x2BC7);
//! ⯈ East pointing triangle
constexpr static QChar E_TRIANGLE = QChar(0x2BC8);

//! ⭠ West pointing arrow
constexpr static QChar W_ARROW = QChar(0x2B60);
//! ⭡ North pointing arrow
constexpr static QChar N_ARROW = QChar(0x2B61);
//! ⭢ East pointing arrow
constexpr static QChar E_ARROW = QChar(0x2B62);
//! ⭣ South pointing arrow
constexpr static QChar S_ARROW = QChar(0x2B63);
//! ⭤ Horizontal double headed arrow
constexpr static QChar EW_ARROW = QChar(0x2B64);
//! ⭥ Vertical double headed arrow
constexpr static QChar NS_ARROW = QChar(0x2B65);
//! ⭦ NorthWest pointing arrow
constexpr static QChar NW_ARROW = QChar(0x2B66);
//! ⭧ NorthEast pointing arrow
constexpr static QChar NE_ARROW = QChar(0x2B67);
//! ⭨ SouthEast pointing arrow
constexpr static QChar SE_ARROW = QChar(0x2B68);
//! ⭩ SouthWest pointing arrow
constexpr static QChar SW_ARROW = QChar(0x2B69);
//! ⭪ West pointing dashed arrow
constexpr static QChar W_DASH_ARROW = QChar(0x2B6A);
//! ⭫ North pointing dashed arrow
constexpr static QChar N_DASH_ARROW = QChar(0x2B6B);
//! ⭬ East pointing dashed arrow
constexpr static QChar E_DASH_ARROW = QChar(0x2B6C);
//! ⭭ South pointing dashed arrow
constexpr static QChar S_DASH_ARROW = QChar(0x2B6D);
//! ⭮ Clockwise open circle arrow
constexpr static QChar CW_ARROW = QChar(0x2B6E);
//! ⭯ Anticlockwise open circle arrow
constexpr static QChar ACW_ARROW = QChar(0x2B6F);
//! ⭰ West pointing arrow to bar
constexpr static QChar W_ARROW_BAR = QChar(0x2B70);
//! ⭱ North pointing arrowv
constexpr static QChar N_ARROW_BAR = QChar(0x2B71);
//! ⭲ East pointing arrow to bar
constexpr static QChar E_ARROW_BAR = QChar(0x2B72);
//! ⭳ South pointing arrow to bar
constexpr static QChar S_ARROW_BAR = QChar(0x2B73);
//! ⭶ NorthWest pointing arrow to bar
constexpr static QChar NW_ARROW_BAR = QChar(0x2B76);
//!	⭷ NorthEast pointing arrow to bar
constexpr static QChar NE_ARROW_BAR = QChar(0x2B77);
//! ⭸ SouthEast pointing arrow to bar
constexpr static QChar SE_ARROW_BAR = QChar(0x2B78);
//! ⭹ SouthWest pointing arrow to bar
constexpr static QChar SW_ARROW_BAR = QChar(0x2B79);

//! ' ' space character
constexpr static QChar SPACE = QChar(0x20);
//! = equals/assignment character
constexpr static QChar EQUALS = QChar(0x3D);
//! = assignment character. Identical to EQUALS.
constexpr static QChar ASSIGNMENT = EQUALS;
//! - minus/hyphen character
constexpr static QChar MINUS = QChar(0x3D);
//! - minus/hyphen character
constexpr static QChar HYPHEN = MINUS;
//! ␤ Newline character - used when requiring a newline in text.
constexpr static QChar NEWLINE = '\n';
//! ␤ Newline character
constexpr static QChar NEWLINE_SYMBOL = QChar(0x2424);
//! ⮒ Left Newline character
constexpr static QChar NEWLINE_LEFT_SYMBOL = QChar(0x2B92);
//! ⮒ Right Newline character
constexpr static QChar NEWLINE_RIGHT_SYMBOL = QChar(0x2B93);
//! ␤ Newline character - used when requiring a tab in text.
constexpr static QChar TAB = '\t';
//! ⭿ Vertical Tab character
constexpr static QChar VERTICAL_TAB_SYMBOL = QChar(0x2B7F);
//! ⭾ Horizontal Tab character
constexpr static QChar HORIZONTAL_TAB_SYMBOL = QChar(0x2B7E);
//! < less than character
constexpr static QChar LT = QChar(0x3C);
//! > greater than character
constexpr static QChar GT = QChar(0x3E);
//! ? question mark character
constexpr static QChar QUESTIONMARK = QChar(0x3F);
//! ! exclamation mark character
constexpr static QChar EXCLAMATIONMARK = QChar(0x21);
//! ' apostrophe/single quote character
constexpr static QChar APOSTROPHE = QChar(0x27);
//! ' apostrophe/single quote character
constexpr static QChar SQUOTE = APOSTROPHE;
//! " double quote character
constexpr static QChar QUOTATION = QChar(0x22);
//! / solidus/forward slash character
constexpr static QChar SOLIDUS = QChar(0x2F);
constexpr static QChar FORWARDSLASH = SOLIDUS;
//! \ reverse solidus/ backslash character
constexpr static QChar R_SOLIDUS = QChar(0x5C);
constexpr static QChar BACKSLASH = R_SOLIDUS;
//! , comma character
constexpr static QChar COMMA = QChar(0x2C);
//! : colon character
constexpr static QChar COLON = QChar(0x3A);
//! ; semicolon character
constexpr static QChar SEMICOLON = QChar(0x2C);
//! ` Grave Accent
constexpr static QChar GRAVE_ACCENT = QChar(0x60);
//! ´ Acute Accent
constexpr static QChar ACUTE_ACCENT = QChar(0xB4);

}; // end of namespace characters.

#endif // CHARACTERS_H
