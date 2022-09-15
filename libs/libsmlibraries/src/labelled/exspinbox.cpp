/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy
                                                of this software and associated
   documentation files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @author: Simon Meaden

*/
#include "labelled/exspinbox.h"

/*!
   \brief Default constructor for ExSpinBox.

   By default the type is set to Hexadecimal with a maximum of 0xFF and a
   minimum of 0. Leading zeroes are not displayed.

   \param parent - the parent widget.
*/
ExSpinBox::ExSpinBox(QWidget* parent)
  : QSpinBox(parent)
  , m_displayDigits(2)
  , m_maxMinDigits(0)
  , m_maxMaxDigits(0)
  , m_prefix(QString())
  , m_defPrefix(QString())
  , m_showLeadingZeroes(false)
  , m_type(Hexadecimal)
  , m_case(Uppercase)
  , m_negBeforePrefix(false)
{
  setDefaultPrefix();
  setDefaultMaxMin();
  setWrapping(false);
}

/*!
   \brief Constructor for ExSpinBox that sets the display type.

   The display type will be set to the supplied type with a maximum of 0xFF
   and a minimum of 0. Leading zeroes are not displayed.

   \param parent - the parent widget.
*/
ExSpinBox::ExSpinBox(DisplayType displayType, QWidget* parent)
  : QSpinBox(parent)
  , m_maxMinDigits(0)
  , m_maxMaxDigits(0)
  , m_prefix(QString())
  , m_defPrefix(QString())
  , m_showLeadingZeroes(false)
  , m_type(displayType)
  , m_case(Uppercase)
{
  setDefaultPrefix();
  setDefaultMaxMin();
  setWrapping(false);
  //   connect(this, &ExSpinBox::valueChanged, this, );
}

QValidator::State
ExSpinBox::validate(QString& value, int& pos) const
{
  int length;

  switch (m_type) {
    case Hexadecimal:
    case Binary:
      length = m_displayDigits + 2;
      break;

    case Octal:
      length = m_displayDigits + 1;
      break;

    case Decimal:
      return QSpinBox::validate(value, pos);
  }

  if (pos <= length && valueFromText(value)) {
    return QValidator::Acceptable;
  }

  return QValidator::Invalid;
}

void
ExSpinBox::setDefaultPrefix()
{
  // Sets up the default prefix for display type;
  switch (m_type) {
    case Decimal:
      m_defPrefix.clear();
      break;

    case Hexadecimal:
      m_defPrefix = "0x";
      break;

    case Octal:
      m_defPrefix = "0";
      break;

    case Binary:
      m_defPrefix = "0b";
      break;
  }
}

void
ExSpinBox::setDefaultMaxMin()
{
  switch (m_type) {
    case Decimal:
      setMinimum(0);
      setMaximum(255);
      break;

    case Hexadecimal:
      setMinimum(0);
      setMaximum(0xFF);
      break;

    case Octal:
      setMinimum(0);
      setMaximum(0xFF);
      break;

    case Binary:
      setMinimum(0);
      setMaximum(0xFF);
      break;
  }
}

int
ExSpinBox::valueFromText(const QString& text) const
{
  // Decimal numbers just pass back to parent
  QString t = text;
  if (m_type == Decimal) {
    return QSpinBox::valueFromText(text);
  }

  bool ok = false;

  int i;
  t = text.trimmed();

  // check for negative value and remove it
  bool is_negative = t.contains("-");
  if (is_negative) {
    t.remove(t.indexOf("-"), 1);
  }

  // remove a prefix if it exists.
  if (!m_prefix.isEmpty() && t.startsWith(m_prefix)) {
    t = t.right(t.length() - m_prefix.length());
  }

  if (t.startsWith("0")) {
    if (t.startsWith("0x")) {
      i = t.toInt(&ok, 16); // hex - base 16

    } else if (t.startsWith("0b")) {
      t = t.mid(2);        // toInt() doesn't support the C++17 "0b" prefix.
      i = t.toInt(&ok, 2); // binary - base 2

    } else {
      i = t.toInt(&ok, 8); // octal - base 8
    }

  } else {
    i = t.toInt(&ok); // base 10
  }

  if (is_negative) {
    i = -i;
  }

  if (ok) {
    return i;
  }

  // ignore new value if it is not viable.
  return 0;
}

QString
ExSpinBox::textFromValue(int value) const
{
  // Decimal numbers just pass back to parent
  if (m_type == Decimal) {
    return QSpinBox::textFromValue(value);
  }

  QString numerical = "%1";
  QString result;
  bool is_negative = (value < 0);
  value = abs(value);

  if (m_type == Hexadecimal) {
    if (m_showLeadingZeroes) {
      result = numerical.arg(value, m_displayDigits, 16, QChar('0')).trimmed();

    } else {
      result = numerical.arg(value, m_displayDigits, 16, QChar(' ')).trimmed();
    }

    if (m_case == Uppercase) {
      result = result.toUpper();

    } else { /*if (m_case == Lowercase)*/
      result = result.toLower();
    }

  } else if (m_type == Octal) {
    if (m_showLeadingZeroes) {
      result = numerical.arg(value, m_displayDigits, 8, QChar('0')).trimmed();

    } else {
      result = numerical.arg(value, 0, 8).trimmed();
    }

  } else if (m_type == Binary) {
    if (m_showLeadingZeroes) {
      result = numerical.arg(value, m_displayDigits, 2, QChar('0')).trimmed();

    } else {
      result = numerical.arg(value, 0, 2).trimmed();
    }
  }

  if (is_negative) {
    if (m_negBeforePrefix) {
      result.prepend("-" + m_prefix + m_defPrefix);

    } else {
      result.prepend(m_prefix + "-" + m_defPrefix);
    }

  } else {
    result.prepend(m_prefix + m_defPrefix);
  }

  return result;
}

int
ExSpinBox::calcDisplayDigits(int value, int divisor)
{
  if (value == 0) {
    return 1;
  }

  unsigned int digits = 0;
  int v = abs(value);

  while (v) {
    v /= divisor;
    ++digits;
  }

  return digits;
}

/*!
   \brief Gets the number of digits allowed in the text string. By default 2.

   The value is read only and can only be reset by increasing the range values.

   \return the number of digits.
*/
int
ExSpinBox::displayDigits() const
{
  return m_displayDigits;
}

ExSpinBox::DisplayType
ExSpinBox::displayType() const
{
  return m_type;
}

void
ExSpinBox::setDisplayType(ExSpinBox::DisplayType displayType)
{
  if (displayType == m_type) {
    return;
  }

  m_type = displayType;
  setDefaultPrefix();

  setMinimum(minimum());
  setMaximum(maximum());
  setValue(value());
}

bool
ExSpinBox::showLeadingZeroes() const
{
  return m_showLeadingZeroes;
}

void
ExSpinBox::setShowLeadingZeroes(bool showLeadingZeroes)
{
  m_showLeadingZeroes = showLeadingZeroes;
  setValue(value()); // TODO probably a better way to do this
}

/*!
   \brief This overrides QSpinBox::setRange.

   This will set the number of display digits appropriately.
*/
void
ExSpinBox::setRange(int min, int max)
{
  setMinimum(min);
  setMaximum(max);
  setValue(value());
}

/*!
   \brief This overrides QSpinBox::setMaximum.

   This will set the number of display digits appropriately.
*/
void
ExSpinBox::setMinimum(int minimum)
{
  int numDigits = 0;

  // sets the minimum and calculates how many display characters are necessary.
  switch (m_type) {
    case Hexadecimal:
      numDigits = calcDisplayDigits(minimum, 0x10);
      break;

    case Binary:
      numDigits = calcDisplayDigits(minimum, 2);
      break;

    case Octal:
      numDigits = calcDisplayDigits(minimum, 8);
      break;

    case Decimal:
      numDigits = calcDisplayDigits(minimum, 10);
      break;
  }

  m_maxMinDigits = (numDigits > m_maxMinDigits ? numDigits : m_maxMinDigits);
  m_displayDigits = qMax(m_maxMinDigits, m_maxMaxDigits);

  QSpinBox::setMinimum(minimum);
}

/*!
   \brief This overrides QSpinBox::setMinimum.

   This will set the number of display digits appropriately.
*/
void
ExSpinBox::setMaximum(int maximum)
{
  int numDigits = 0;

  switch (m_type) {
    case Hexadecimal:
      numDigits = calcDisplayDigits(maximum, 0x10);
      break;

    case Binary:
      numDigits = calcDisplayDigits(maximum, 2);
      break;

    case Octal:
      numDigits = calcDisplayDigits(maximum, 8);
      break;

    case Decimal:
      numDigits = calcDisplayDigits(maximum, 10);
      break;
  }

  m_maxMaxDigits = (numDigits > m_maxMaxDigits ? numDigits : m_maxMaxDigits);
  m_displayDigits = qMax(m_maxMinDigits, m_maxMaxDigits);

  QSpinBox::setMaximum(maximum);
}

ExSpinBox::DisplayCase
ExSpinBox::charCase() const
{
  return m_case;
}

void
ExSpinBox::setCharCase(DisplayCase charcase)
{
  m_case = charcase;
  setValue(value());
}

QString
ExSpinBox::prefix()
{
  return m_prefix;
}

void
ExSpinBox::setPrefix(const QString& prefix)
{
  m_prefix = prefix;
  setValue(value());
}

bool
ExSpinBox::negBeforePrefix() const
{
  return m_negBeforePrefix;
}

void
ExSpinBox::setNegBeforePrefix(bool neg_before_prefix)
{
  m_negBeforePrefix = neg_before_prefix;
  setValue(value());
}
