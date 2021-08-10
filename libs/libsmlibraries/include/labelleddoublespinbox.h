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
#ifndef LABELLEDDOUBLESPINBOX_H
#define LABELLEDDOUBLESPINBOX_H

#include "abstractlabelledspinbox.h"
#include "sm_widgets_global.h"

/*!
   \file labelleddoublespinbox.h labelleddoublespinbox.cpp
   \class LabelledDoubleSpinBox labelleddoublespinbox.h
   \brief The LabelledDoubleSpinBox is a wrapper for QLabel and QDoubleSpinBox
   that provides a labelled double spin box. \since 5.7.0 \license The MIT
   License \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the QDoubleSpinBox are copied across to this widget.
*/
class LabelledDoubleSpinBox : public AbstractLabelledSpinBox
{
  /*!
      \property LabelledDoubleSpinBox::value

     \brief This property holds the value of the spin box.

     @reimplements QSpinBox::value
     @accessor %value(), %setValue()
        @notifier valueChanged(int)
  */
  Q_PROPERTY(
    double value READ value WRITE setValue NOTIFY valueChanged USER true)

  /*!
     \property LabelledDoubleSpinBox::maximum

     \brief This property holds the maximum value of the spin box.

     @reimplements QSpinBox::maximum
     @accessor %maximum(void), %setMaximum()
  */
  Q_PROPERTY(double maximum READ maximum WRITE setMaximum)

  /*!
     \property LabelledDoubleSpinBox::minimum

     \brief This property holds the minimum value of the spin box.

     @reimplements QSpinBox::minimum
     @accessor %minimum(), %setMinimum()
  */
  Q_PROPERTY(double minimum READ minimum WRITE setMinimum)

  /*!
     \property LabelledDoubleSpinBox::prefix

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
    QWidget* newWidget(QWidget* parent) const
    {
      return new QDoubleSpinBox(parent);
    }
  };

  LabelledDoubleSpinBox(WidgetFactory const& factory = WidgetFactory(),
                        const QString& labelText = QString(),
                        QWidget* parent = nullptr);

  /// \endcond

public:
  //! Constructor for  LabelledDoubleSpinBox.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.
  LabelledDoubleSpinBox(const QString& labelText = QString(),
                        QWidget* parent = nullptr);

  //! @reimplements QDoubleSpinBox::value() const.
  double value() const;

  //! @reimplements QDoubleSpinBox::setValue().
  void setValue(double v);

  //! @reimplements QDoubleSpinBox::maximum() const.
  double maximum() const;

  //! @reimplements QDoubleSpinBox::setMaximum().
  void setMaximum(double max);

  //! @reimplements QDoubleSpinBox::minimum() const.
  double minimum() const;

  //! @reimplements QDoubleSpinBox::setMinimum().
  void setMinimum(double min);

  //! @reimplements QDoubleSpinBox::setRange().
  void setRange(double min, double max);

  //! @reimplements QDoubleSpinBox::decimals().
  int decimals();

  //! @reimplements QDoubleSpinBox::setDecimals().
  void setDecimals(int precision);

  //! @reimplements QDoubleSpinBox::singleStep() const.
  double singleStep() const;

  //! @reimplements QDoubleSpinBox::singleStep() const.
  void setSingleStep(double step);

  //! @reimplements QDoubleSpinBox::prefix().
  QString prefix() const;

  //! @reimplements QSpinBox::setPrefix().
  void setPrefix(const QString& labelText);

signals:
  /*!
     \fn LabelledDoubleSpinBox::valueChanged(double value)

     @from QSpinBox::value.
     @notprop value.
  */
  void valueChanged(double);

private:
  void initGui(const QString& text = QString());
};

#endif // LABELLEDDOUBLESPINBOX_H
