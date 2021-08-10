/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy
                                                of this software and associated
   documentation files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @author: Simon Meaden

*/
#ifndef QHEXSPINBOX_H
#define QHEXSPINBOX_H

#include <QSpinBox>
#include <QValidator>
#include <QtMath>

#include "sm_widgets_global.h"

// template <typename T>
// typename std::enable_if<std::numeric_limits<T>::is_integer, unsigned
// int>::type numberDigits(T value) {
//  unsigned int digits = 0;
//  if (value < 0) digits = 1;
//  while (value) {
//    value /= 10;
//    ++digits;
//  }
//  return digits;
//}

// template <typename T>
// typename std::enable_if<std::numeric_limits<T>::is_integer, unsigned
// int>::type numberHexDigits(T value) {
//  unsigned int digits = 0;
//  if (value < 0) digits = 1;
//  while (value) {
//    value /= 0x10;
//    ++digits;
//  }
//  return digits;
//}

/*!
   \file exspinbox.h exspinbox.cpp
   \class ExSpinBox exspinbox.h
   \brief A spin box that will display the value in decimal, hexadecimal, octal
   or binary. \since 5.7.0

   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   An extended QSpinBox that can display in various common formats, (hexadecimal,
   octal and binary). ExSpinBox::Decimal in fact just uses the underlying
   QSpinBox so acts in exactly the same way.

   Character case (in the case of hexadecimal) is enforced, uppercase by default,
   and can be changed using the setCharCase() method to either
   ExSpinBox::Uppercase or ExSpinBox::Lowercase.

   The display type can be set using the ExSpinBox::setDisplayType() method to
   ExSpinBox::Decimal, ExSpinBox::Hexadecimal, ExSpinBox::Binary or
   ExSpinBox::Octal.

   By default leading zeroes are not shown. This can be modified setting the
   ExSpinBox::setLeadingZeroes(bool) to true. The actual number of displayed
   characters is set when the minimum and maximum range is set.
*/

class SM_WIDGETS_SHARED_EXPORT ExSpinBox : public QSpinBox
{

   /*!
      \property ExSpinBox::displayType()
      \brief This property holds the display type of the spin box.

      Valid types are ExSpinBox::Decimal, ExSpinBox::Hexadecimal (the default),
      ExSpinBox::Binary or ExSpinBox::Octal.

      @accessor %displayType(), setDisplayType()
      @notifier displayTypeChanged(DisplayType);
   */ Q_PROPERTY(DisplayType displayType READ displayType WRITE setDisplayType
                 NOTIFY displayTypeChanged(DisplayType))

   /*!
      \property ExSpinBox::displayDigits()
      \brief This property holds the number of display digits of the spin box.

      @accessor %displayDigits()
      @notifier displayDigitsChanged(int)
   */
   Q_PROPERTY(int displayDigits READ displayDigits NOTIFY displayDigitsChanged)

   /*!
      \property ExSpinBox::showLeadingZeroes()
      \brief This property holds whether the spin box shows leading zeroes.

      Default is false. If true then leading zeroes will be displayed, up to
      displayDigits() characters will be displayed.

      @accessor %showLeadingZeroes(), setShowLeadingZeroes()
   */
   Q_PROPERTY(
      bool showLeadingZeroes READ showLeadingZeroes WRITE setShowLeadingZeroes)

   /*!
      \property ExSpinBox::charCase()
      \brief This property holds the display case of the spin box.

      Default is DisplayCase::UpperCase. Possible values are
      DisplayCase::Uppercase and DisplayCase::Lowercase. This is only relevant when
      the displayType property is DisplayType::Hexadecimal.

      @accessor %charCase(), setCharCase()
   */
   Q_PROPERTY(DisplayCase charCase READ charCase WRITE setCharCase)

   /*!
      \property ExSpinBox::prefix

      \brief This property holds the spin box's prefix

      @reimplements QSpinBox::prefix
      @accessor %prefix(), %setPrefix()
   */
   Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)

   /*!
      \property ExSpinBox::negBeforePrefix()
      \brief This property holds the display negative sign before prefix flag.

      Default is true. This only applies to a user supplied prefix, not the internal
      "0x" (Hexadecimal), "0" (Octal), "0b" (Binary) prefixes which are always supplied.

      If false, the default, and assuming you set a prefix of "£" for a hexadecimal
      spin box then the result would be "£-0xFF" for an int value of 255. If set to
      true then "-£0xFF" would result which would make more sense in this case.
      Of course this is a very wierd example and I cannot see any reason to display
      hexadecimal currency but hey ho!.

      @accessor %negBeforePrefix(), %setNegBeforePrefix()
   */
   Q_PROPERTY(bool negBeforePrefix READ negBeforePrefix WRITE setNegBeforePrefix)

   Q_OBJECT
public:
   /*!
      \brief Defines the possible values for the display type.
   */
   enum DisplayType
   {
      Decimal = 0, //!< Values will be displayed in Decimal format "255".
      Hexadecimal, //!< Values will be displayed in Hexadecimal format "0xFF".
      Binary,      //!< Values will be displayed in Binary format "0b11111111".
      Octal,       //!< Values will be displayed in Octal format "0777".
   };
   /*!
      \brief Defines the possible values for the hexadecimal display case.
   */
   enum DisplayCase
   {
      Uppercase = 0, //!< Hexadecimal characters A-F display in uppercase.
      Lowercase,     //!< Hexadecimal characters a-f display in lowercase.
   };

   //! Default constructor for ExSpinBox, with optional parent set.
   explicit ExSpinBox(QWidget* parent = 0);
   //! Constructor for ExSpinBox, specifying the display type, and optional parent.
   //! \param displayType the ExSpinBox::DisplayType.
   //! \param parent the parent widget.
   explicit ExSpinBox(DisplayType displayType, QWidget* parent = 0);

   /*!
      \brief Returns the number of digits allowed in the text string. By default 2.

      The value is read only and can only be reset by increasing the range values.

      \return the number of digits.
   */
   int displayDigits() const;

   //! Returns the type of the display, by default ExSpinBox Hexadecimal.
   DisplayType displayType() const;

   //! Sets the DisplayType of the display.
   void setDisplayType(DisplayType displayType);

   //! Returns whether leading zeroes are displayed.
   bool showLeadingZeroes() const;

   //! Sets whether leading zeroes are displayed.
   void setShowLeadingZeroes(bool showLeadingZeroes);

   //! @reimplements QSpinBox::setRange().
   void setRange(int min, int max);

   //! @reimplements QSpinBox::setMinimum().
   void setMinimum(int min);

   //! @reimplements QSpinBox::setMaximum().
   void setMaximum(int max);

   //! Returns whether the spin box is in uppercase or lowercase.
   //!
   //! This is only relevant when the spin box is having  the type ExSpinBox::Hexadecimal.
   DisplayCase charCase() const;

   //! Sets whether the spin box is in uppercase or lowercase.
   //!
   //! This is only relevant when the spin box is having  the type ExSpinBox::Hexadecimal.
   void setCharCase(DisplayCase charcase);

   //! @reimplements QSpinBox::prefix().
   QString prefix();

   //! @reimplements QSpinBox::setPrefix().
   void setPrefix(const QString& prefix);

   //! Returns the value of the negative before prefix property.
   bool negBeforePrefix() const;

   //! Sets the value of the negative before prefix property.
   void setNegBeforePrefix(bool negBeforePrefix);

signals:
   /*!
      \fn ExSpinBox::displayTypeChanged(DisplayType)

      This signal is sent whenever display type changes.

      Note: Notifier signal for property displayType.
   */
   void displayTypeChanged(DisplayType);
   /*!
      \fn ExSpinBox::displayDigitsChanged(int index)

      This signal is sent whenever displayDigits changes.

      Note: Notifier signal for property displayDigits.
   */
   void displayDigitsChanged(int);

private:
   int m_displayDigits, m_maxMinDigits, m_maxMaxDigits;
   QString m_prefix, m_defPrefix;
   bool m_showLeadingZeroes;
   DisplayType m_type;
   DisplayCase m_case;
   bool m_negBeforePrefix;

   QValidator::State validate(QString& value, int& pos) const override;
   int valueFromText(const QString& textString) const override;
   QString textFromValue(int val) const override;

   void setDefaultPrefix();
   void setDefaultMaxMin();
   void setDisplayFault();
   void clearErrorFault();
   static int calcDisplayDigits(int value, int divisor);

private:
};

#endif // QHEXSPINBOX_H
