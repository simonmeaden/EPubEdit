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
#include "labelled/labelledspinbox.h"

LabelledSpinBox::LabelledSpinBox(const QString& labelText, QWidget* parent)
  : LabelledSpinBox(LabelledSpinBox::WidgetFactory(), labelText, parent)
{}

LabelledSpinBox::LabelledSpinBox(const WidgetFactory& factory,
                                 const QString& labelText,
                                 QWidget* parent)
  : AbstractLabelledSpinBox(labelText, parent, factory)
{
  LabelledSpinBox::initGui();
}

int
LabelledSpinBox::value() const
{
  return qobject_cast<QSpinBox*>(m_widget)->value();
}

void
LabelledSpinBox::setValue(int val)
{
  qobject_cast<QSpinBox*>(m_widget)->setValue(val);
}

int
LabelledSpinBox::maximum() const
{
  return qobject_cast<QSpinBox*>(m_widget)->maximum();
}

void
LabelledSpinBox::setMaximum(int max)

{
  qobject_cast<QSpinBox*>(m_widget)->setMaximum(max);
}

int
LabelledSpinBox::minimum() const
{
  return qobject_cast<QSpinBox*>(m_widget)->minimum();
}

void
LabelledSpinBox::setMinimum(int min)
{
  qobject_cast<QSpinBox*>(m_widget)->setMinimum(min);
}

void
LabelledSpinBox::setRange(int min, int max)
{
  qobject_cast<QSpinBox*>(m_widget)->setRange(min, max);
}

int
LabelledSpinBox::singleStep() const
{
  return qobject_cast<QSpinBox*>(m_widget)->singleStep();
}

void
LabelledSpinBox::setSingleStep(int step)
{
  qobject_cast<QSpinBox*>(m_widget)->setSingleStep(step);
}

QString
LabelledSpinBox::prefix() const
{
  return qobject_cast<QSpinBox*>(m_widget)->prefix();
}

void
LabelledSpinBox::setPrefix(const QString& text)
{
  qobject_cast<QSpinBox*>(m_widget)->setPrefix(text);
}

int
LabelledSpinBox::displayIntegerBase() const
{
  return qobject_cast<QSpinBox*>(m_widget)->displayIntegerBase();
}

void
LabelledSpinBox::setDisplayIntegerBase(int base)
{
  qobject_cast<QSpinBox*>(m_widget)->setDisplayIntegerBase(base);
}

void
LabelledSpinBox::initGui(const QString& /*text*/)
{
  connect(qobject_cast<QSpinBox*>(m_widget),
          qOverload<int>(&QSpinBox::valueChanged),
          this,
          &LabelledSpinBox::valueChanged);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  connect(reinterpret_cast<QSpinBox*>(m_widget),
          qOverload<const QString&>(&QSpinBox::textChanged),
          this,
          &LabelledSpinBox::textChanged);
#else
  connect(reinterpret_cast<QSpinBox*>(m_widget),
          qOverload<const QString&>(&QSpinBox::valueChanged),
          this,
          &LabelledSpinBox::labelTextChanged);
#endif
}
