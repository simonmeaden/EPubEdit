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
#ifndef LABELLEDSPINBOX_H
#define LABELLEDSPINBOX_H

#include "abstractlabelledspinbox.h"
#include "sm_widgets_global.h"

/*!
   \file labelledspinbox.h labelledspinbox.cpp
   \class LabelledSpinBox labelledspinbox.h
   \brief The LabelledSpinBox is a wrapper for QLabel and QSpinBox that
          provides a labelled spin box.
   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the QSpinBox  are copied across to this widget.
*/
class SM_WIDGETS_SHARED_EXPORT LabelledSpinBox : public AbstractLabelledSpinBox
{
  /*!
      \property LabelledSpinBox::value

     \brief This property holds the value of the spin box.

     @reimplements QSpinBox::value
     @accessor %value(), %setValue()
        @notifier valueChanged(int)
  */
  Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)

  /*!
     \property LabelledSpinBox::maximum

     \brief This property holds the maximum value of the spin box.

     @reimplements QSpinBox::maximum
     @accessor %maximum(void), %setMaximum()
  */
  Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

  /*!
     \property LabelledSpinBox::minimum

     \brief This property holds the minimum value of the spin box.

     @reimplements QSpinBox::minimum
     @accessor %minimum(), %setMinimum()
  */
  Q_PROPERTY(int minimum READ minimum WRITE setMinimum)

  /*!
     \property LabelledSpinBox::displayIntegerBase

     \brief This property holds the base used to display the value of the spin
      box

     @reimplements QSpinBox::displayIntegerBase
     @accessor %displayIntegerBase(), %setDisplayIntegerBase()
  */
  Q_PROPERTY(
    int displayIntegerBase READ displayIntegerBase WRITE setDisplayIntegerBase)

  /*!
     \property LabelledSpinBox::singleStep

     \brief This property holds the step value

     @reimplements QSpinBox::singleStep
     @accessor %singleStep(), %setSingleStep()
  */
  Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)

  /*!
     \property LabelledSpinBox::prefix

     \brief This property holds the spin box's prefix

     @reimplements QSpinBox::prefix
     @accessor %prefix(), %setPrefix()
  */
  Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)

  Q_OBJECT

protected:
  /// \cond DO_NOT_DOCUMENT

  struct WidgetFactory : AbstractLabelledWidget::WidgetFactory
  {
    QWidget* newWidget(QWidget* parent) const { return new QSpinBox(parent); }
  };

  LabelledSpinBox(WidgetFactory const& factory,
                  const QString& labelText = QString(),
                  QWidget* parent = nullptr);

  /// \endcond

public:
  //! \brief Constructor for LabelledSpinBox.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.
  explicit LabelledSpinBox(const QString& labelText = QString(),
                           QWidget* parent = nullptr);

  //! @reimplements QSpinBox::value() const.
  int value() const;

  //! @reimplements QSpinBox::setValue().
  void setValue(int v);

  //! @reimplements QSpinBox::maximum() const.
  int maximum() const;

  //! @reimplements QSpinBox::setMaximum().
  void setMaximum(int max);

  //! @reimplements QSpinBox::minimum() const.
  int minimum() const;

  //! @reimplements QSpinBox::setMinimum().
  void setMinimum(int min);

  //! @reimplements QSpinBox::setRange().
  void setRange(int min, int max);

  //! @reimplements QSpinBox::singleStep() const.
  int singleStep() const;

  //! @reimplements QSpinBox::setSingleStep().
  void setSingleStep(int step);

  //! @reimplements QSpinBox::displayIntegerBase() const.
  int displayIntegerBase() const;

  //! @reimplements QSpinBox::setDisplayIntegerBase().
  void setDisplayIntegerBase(int base);

  //! @reimplements QSpinBox::prefix().
  QString prefix() const;

  //! @reimplements QSpinBox::setPrefix().
  void setPrefix(const QString& labelText);

signals:
  /*!
     \fn LabelledSpinBox::valueChanged(int value)

     @from QSpinBox::value.
     @notprop value.
  */
  void valueChanged(int);

private:
  void initGui(const QString& text = QString());
};

#endif // LABELLEDSPINBOX_H
