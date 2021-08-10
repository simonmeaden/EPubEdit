/**
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
#include "labelledexspinbox.h"

LabelledExSpinBox::LabelledExSpinBox(const QString& text, QWidget* parent)
  : LabelledExSpinBox(LabelledExSpinBox::WidgetFactory(), text, parent)
{
  initGui();
}

LabelledExSpinBox::LabelledExSpinBox(const WidgetFactory& factory,
                                     const QString& text,
                                     QWidget* parent)
  : AbstractLabelledSpinBox(text, parent, factory)
{
  LabelledExSpinBox::initGui();
}

quint8
LabelledExSpinBox::displayDigits() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->displayDigits();
}

ExSpinBox::DisplayType
LabelledExSpinBox::displayType() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->displayType();
}

void
LabelledExSpinBox::setDisplayType(ExSpinBox::DisplayType type)
{
  qobject_cast<ExSpinBox*>(m_widget)->setDisplayType(type);
}

bool
LabelledExSpinBox::showLeadingZeroes() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->showLeadingZeroes();
}

void
LabelledExSpinBox::setShowLeadingZeroes(bool showLeadingZeroes)
{
  qobject_cast<ExSpinBox*>(m_widget)->setShowLeadingZeroes(showLeadingZeroes);
}

QString
LabelledExSpinBox::suffix()
{
  return qobject_cast<QSpinBox*>(m_widget)->suffix();
}

void
LabelledExSpinBox::setSuffix(const QString& text)
{
  qobject_cast<QSpinBox*>(m_widget)->setSuffix(text);
}

QAbstractSpinBox::StepType
LabelledExSpinBox::stepType() const
{
  return qobject_cast<QSpinBox*>(m_widget)->stepType();
}

void
LabelledExSpinBox::setStepType(QAbstractSpinBox::StepType type)
{
  qobject_cast<QSpinBox*>(m_widget)->setStepType(type);
}

QString
LabelledExSpinBox::cleanText() const
{
  return qobject_cast<QSpinBox*>(m_widget)->cleanText();
}

void
LabelledExSpinBox::setWidgetAlignment(const Qt::Alignment& widgetAlignment)
{
  qobject_cast<QAbstractSpinBox*>(m_widget)->setAlignment(widgetAlignment);
}

Qt::Alignment
LabelledExSpinBox::widgetAlignment() const
{
  return qobject_cast<QAbstractSpinBox*>(m_widget)->alignment();
}

int
LabelledExSpinBox::value() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->value();
}

void
LabelledExSpinBox::setValue(int val)
{
  qobject_cast<ExSpinBox*>(m_widget)->setValue(val);
}

int
LabelledExSpinBox::maximum() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->maximum();
}

void
LabelledExSpinBox::setMaximum(int max)

{
  qobject_cast<ExSpinBox*>(m_widget)->setMaximum(max);
}

int
LabelledExSpinBox::minimum() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->minimum();
}

void
LabelledExSpinBox::setMinimum(int min)
{
  qobject_cast<ExSpinBox*>(m_widget)->setMinimum(min);
}

void
LabelledExSpinBox::setRange(int min, int max)
{
  qobject_cast<ExSpinBox*>(m_widget)->setRange(min, max);
}

int
LabelledExSpinBox::singleStep() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->singleStep();
}

void
LabelledExSpinBox::setSingleStep(int step)
{
  qobject_cast<ExSpinBox*>(m_widget)->setSingleStep(step);
}

QString
LabelledExSpinBox::prefix() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->prefix();
}

void
LabelledExSpinBox::setPrefix(const QString& text)
{
  qobject_cast<ExSpinBox*>(m_widget)->setPrefix(text);
}

bool
LabelledExSpinBox::negBeforePrefix() const
{
  return qobject_cast<ExSpinBox*>(m_widget)->negBeforePrefix();
}

void
LabelledExSpinBox::setNegBeforePrefix(bool negBeforePrefix)
{
  qobject_cast<ExSpinBox*>(m_widget)->setNegBeforePrefix(negBeforePrefix);
}

void
LabelledExSpinBox::initGui(const QString&)
{
  connect(qobject_cast<ExSpinBox*>(m_widget),
          qOverload<int>(&QSpinBox::valueChanged),
          this,
          &LabelledExSpinBox::valueChanged);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  connect(qobject_cast<ExSpinBox*>(m_widget),
          qOverload<const QString&>(&QSpinBox::textChanged),
          this,
          &LabelledExSpinBox::textChanged);
#else
  connect(qobject_cast<ExSpinBox*>(m_widget),
          qOverload<const QString&>(&QSpinBox::valueChanged),
          this,
          &LabelledExSpinBox::textChanged);
#endif
  connect(qobject_cast<ExSpinBox*>(m_widget),
          &ExSpinBox::displayTypeChanged,
          this,
          &LabelledExSpinBox::displayTypeChanged);
}
