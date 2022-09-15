/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
#include "labelled/labelleddoublespinbox.h"

LabelledDoubleSpinBox::LabelledDoubleSpinBox(const QString& labelText,
                                             QWidget* parent)
  : LabelledDoubleSpinBox(LabelledDoubleSpinBox::WidgetFactory(),
                          labelText,
                          parent)
{}

LabelledDoubleSpinBox::LabelledDoubleSpinBox(WidgetFactory const& factory,
                                             const QString& labelText,
                                             QWidget* parent)
  : AbstractLabelledSpinBox(labelText, parent, factory)
{
  LabelledDoubleSpinBox::initGui();
}

double
LabelledDoubleSpinBox::value() const
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->value();
}

void
LabelledDoubleSpinBox::setValue(double val)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setValue(val);
}

double
LabelledDoubleSpinBox::maximum() const
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->maximum();
}

void
LabelledDoubleSpinBox::setMaximum(double max)

{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setMaximum(max);
}

double
LabelledDoubleSpinBox::minimum() const
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->minimum();
}

void
LabelledDoubleSpinBox::setMinimum(double min)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setMinimum(min);
}

// QString LabelledDoubleSpinBox::suffix()
//{
//  return qobject_cast<QSpinBox*>(m_widget)->suffix();
//}

// void LabelledDoubleSpinBox::setSuffix(const QString& text)
//{
//  qobject_cast<QSpinBox*>(m_widget)->setSuffix(text);
//}

void
LabelledDoubleSpinBox::setRange(double min, double max)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setRange(min, max);
}

int
LabelledDoubleSpinBox::decimals()
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->decimals();
}

void
LabelledDoubleSpinBox::setDecimals(int precision)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setDecimals(precision);
}

double
LabelledDoubleSpinBox::singleStep() const
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->singleStep();
}

void
LabelledDoubleSpinBox::setSingleStep(double step)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setSingleStep(step);
}

QString
LabelledDoubleSpinBox::prefix() const
{
  return qobject_cast<QDoubleSpinBox*>(m_widget)->prefix();
}

void
LabelledDoubleSpinBox::setPrefix(const QString& text)
{
  qobject_cast<QDoubleSpinBox*>(m_widget)->setPrefix(text);
}

void
LabelledDoubleSpinBox::initGui(const QString& /*text*/)
{
  connect(qobject_cast<QDoubleSpinBox*>(m_widget),
          qOverload<double>(&QDoubleSpinBox::valueChanged),
          this,
          &LabelledDoubleSpinBox::valueChanged);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  connect(reinterpret_cast<QDoubleSpinBox*>(m_widget),
          qOverload<const QString&>(&QDoubleSpinBox::textChanged),
          this,
          &LabelledDoubleSpinBox::textChanged);
#else
  connect(reinterpret_cast<QDoubleSpinBox*>(m_widget),
          qOverload<const QString&>(&QDoubleSpinBox::valueChanged),
          this,
          &LabelledDoubleSpinBox::labelTextChanged);
#endif
}
