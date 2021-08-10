#ifndef HEXFORMATDELEGATE_H
#define HEXFORMATDELEGATE_H
/*
  Copyright 2020 Simon Meaden

  Permission is hereby granted, free of charge, to any person obtaining a copy
                                                of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                          copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

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
#include <QStyledItemDelegate>
#include "sm_widgets_global.h"


/*!
   \file hexformatdelegate.h hexformatdelegate.cpp
   \class HexFormatDelegate hexformatdelegate.h
   \brief A Qt Delegate that formats numerical text values as hexadecimal.
   \since 5.14.0

   A simple delegate for formatting integer numbers as hexadecimal.
*/
class SM_WIDGETS_SHARED_EXPORT HexFormatDelegate : public QStyledItemDelegate
{

public:
  //! Constructor for HexFormatDelegate
  explicit HexFormatDelegate(QObject* parent = nullptr);

  //! @reimplements QStyledItemDelegate::displaytext
  //! Overrides the default method to return hexadecimal values.
  QString displayText(const QVariant& value, const QLocale& locale) const;

};

#endif // HEXFORMATDELEGATE_H
