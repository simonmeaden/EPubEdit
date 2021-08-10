
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
#ifndef ABSTRACTLABELLEDSPINBOX_H
#define ABSTRACTLABELLEDSPINBOX_H

#include "abstractlabelledwidget.h"
#include <QSpinBox>

/*!
   \file abstractlabelledspinbox.h abstractlabelledspinbox.cpp
   \class AbstractLabelledSpinBox abstractlabelledspinbox.h
   \brief The LabelledSpinWidget abstract base class.
   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   Extends AbstractLabelledWidget to supply common capability to the
    various labelled spin boxes.

   All properties of the QSpinBox  are copied across to this widget.
*/
class AbstractLabelledSpinBox
  : public AbstractLabelledWidget
  , public AlignableWidgetInterface
{
  /*!
     \property AbstractLabelledSpinBox::suffix

     \brief This property holds the suffix of the spin box

     @reimplements QSpinBox::suffix
      @accessor %suffix(), %setSuffix()
  */
  Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)

  Q_OBJECT

public:
  //! Constructs a lineedit with the given parent, and supplied label.
  explicit AbstractLabelledSpinBox(
    const QString& labelText = QString(),
    QWidget* parent = nullptr,
    WidgetFactory const& factory = WidgetFactory());

  //! @reimplements QSpinBox::suffix() const.
  QString suffix();

  //! @reimplements QSpinBox::setSuffix().
  void setSuffix(const QString& labelText);

  //! @reimplements QSpinBox::stepType() const.
  QAbstractSpinBox::StepType stepType() const;

  //! @reimplements QSpinBox::setStepType().
  void setStepType(QAbstractSpinBox::StepType type);

  //! @reimplements QSpinBox::cleanText().
  QString cleanText() const;

  //! @reimplements QSpinBox::setAlignment() const.
  void setWidgetAlignment(const Qt::Alignment& widgetAlignment);

  //! @reimplements QSpinBox::alignment() const.
  Qt::Alignment widgetAlignment() const;

signals:
  /*!
     \fn AbstractLabelledSpinBox::textChanged(const QString& text)

     @from QSpinBox::textChanged.
     @notprop value.
  */
  void textChanged(const QString& labelText);
};
#endif // ABSTRACTLABELLEDSPINBOX_H
