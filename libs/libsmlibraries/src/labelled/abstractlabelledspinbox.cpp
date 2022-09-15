/**
   Copyright 2020 Simon Meaden
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
   "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "labelled/abstractlabelledspinbox.h"

AbstractLabelledSpinBox::AbstractLabelledSpinBox(const QString& text,
                                                 QWidget* parent,
                                                 const WidgetFactory& factory)
  : AbstractLabelledWidget(text, parent, factory)
{}

QString
AbstractLabelledSpinBox::suffix()
{
  return qobject_cast<QSpinBox*>(m_widget)->suffix();
}

void
AbstractLabelledSpinBox::setSuffix(const QString& text)
{
  qobject_cast<QSpinBox*>(m_widget)->setSuffix(text);
}

QAbstractSpinBox::StepType
AbstractLabelledSpinBox::stepType() const
{
  return qobject_cast<QSpinBox*>(m_widget)->stepType();
}

void
AbstractLabelledSpinBox::setStepType(QAbstractSpinBox::StepType type)
{
  qobject_cast<QSpinBox*>(m_widget)->setStepType(type);
}

QString
AbstractLabelledSpinBox::cleanText() const
{
  return qobject_cast<QSpinBox*>(m_widget)->cleanText();
}

void
AbstractLabelledSpinBox::setWidgetAlignment(
  const Qt::Alignment& widgetAlignment)
{
  qobject_cast<QAbstractSpinBox*>(m_widget)->setAlignment(widgetAlignment);
}

Qt::Alignment
AbstractLabelledSpinBox::widgetAlignment() const
{
  return qobject_cast<QAbstractSpinBox*>(m_widget)->alignment();
}
