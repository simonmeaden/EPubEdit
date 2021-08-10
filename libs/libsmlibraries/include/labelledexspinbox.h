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
#ifndef LABELLEDEXSPINBOX_H
#define LABELLEDEXSPINBOX_H

#include "abstractlabelledspinbox.h"
#include "exspinbox.h"
#include "sm_widgets_global.h"

/*!
   \file labelledexspinbox.h labelledexspinbox.cpp
   \class LabelledExSpinBox labelledexspinbox.h
   \brief The LabelledExSpinBox is a wrapper for QLabel and ExSpinBox that
          provides a labelled extended spin box.
   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the QSpinBox are copied across to this widget.

   By default the ExSpinBox type is ExSpinBox::Hexadecimal.
*/
class SM_WIDGETS_SHARED_EXPORT LabelledExSpinBox
  : public AbstractLabelledSpinBox
{
  /*!
     \property LabelledExSpinBox::displayDigits

     \brief This property holds the number of displayable digits of the spin
     box.

     This is not necessarily the number of digits displayed unless
     showLeadingZeroes is true.


     @accessor %displayDigits()
  */
  Q_PROPERTY(int displayDigits READ displayDigits)

  /*!
     \property LabelledExSpinBox::displayType

     \brief This property holds the type for the spin box to display.
     By default ExSpinBox::Hexadecimal.

     @accessor %displayType(void), %setDisplayType()
     @notifier displayTypeChanged()
  */
  Q_PROPERTY(ExSpinBox::DisplayType displayType READ displayType WRITE
               setDisplayType NOTIFY displayTypeChanged(ExSpinBox::DisplayType))

  /*!
     \property LabelledExSpinBox::showLeadingZeroes

     \brief This property holds whether the spin box shows leading zeroes.

     If true leading zeroes are added until numberOfDigits characters are
     reached.

     @accessor %showLeadingZeroes(void), %setShowLeadingZeroes()
  */
  Q_PROPERTY(
    bool showLeadingZeroes READ showLeadingZeroes WRITE setShowLeadingZeroes)

  /*!
     \property LabelledExSpinBox::stepType

     \brief This property holds the step type.

     @reimplements QSpinBox::stepType
     @accessor %stepType(), %setStepType(QSpinBox::StepType)
  */
  Q_PROPERTY(QSpinBox::StepType stepType READ stepType WRITE setStepType)

  /*!
     \property LabelledExSpinBox::prefix

     \brief This property holds the spin box's prefix

     @reimplements QSpinBox::prefix
     @accessor %prefix(), %setPrefix()
  */
  Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)

  /*!
     \property LabelledExSpinBox::negBeforePrefix()
     \brief This property holds the display negative sign before prefix flag.

     Default is false. This only applies to a user supplied prefix, not the
     internal "0x" (Hexadecimal), "0" (Octal), "0b" (Binary) prefixes which are
     always supplied.

     If false, the default, and assuming you set a prefix of "£" for a
     hexadecimal spin box then the result would be "£-0xFF" for an int value of
     255. If set to true then "-£0xFF" would result. Of course this is a very
     odd example and I cannot see any reason to display hexadecimal currency.

     @accessor %negBeforePrefix(), %setNegBeforePrefix()
  */
  Q_PROPERTY(bool negBeforePrefix READ negBeforePrefix WRITE setNegBeforePrefix)

  Q_OBJECT

protected:
  /// \cond DO_NOT_DOCUMENT
  struct WidgetFactory : AbstractLabelledWidget::WidgetFactory
  {
    //! Returns a new widget.
    virtual QWidget* newWidget(QWidget* parent) const
    {
      return new ExSpinBox(parent);
    }
  };

  LabelledExSpinBox(WidgetFactory const& factory,
                    const QString& labelText = QString(),
                    QWidget* parent = nullptr);

  /// \endcond

public:
  //! \brief The LabelledExSpinBox is a wrapper for QLabel and QSpinBox that
  //!        provides a labelled extended spin box.
  //!
  //! Extends AbstractLabelledSpinBox to supply a spin box with hexadecimal,
  //! octal and binary display of interger values.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.* /
  explicit LabelledExSpinBox(const QString& labelText = QString(),
                             QWidget* parent = nullptr);

  /*!
     \brief Returns the number of displayable digits in the text string. By
     default 2.
  */
  quint8 displayDigits() const;

  /*!
     \brief Returns the type of the display, by default ExSpinBox Hexadecimal.
  */
  ExSpinBox::DisplayType displayType() const;

  //! @reimplements QSpinBox::singleStep() const.
  int singleStep() const;

  //! @reimplements QSpinBox::setSingleStep().
  void setSingleStep(int step);

  /*!
     \brief Sets the type of the display, by default ExSpinBox Hexadecimal.
  */
  void setDisplayType(ExSpinBox::DisplayType type);

  /*!
     \brief returns whether leading zeroes are displayed.
  */
  bool showLeadingZeroes() const;

  /*!
     \brief Sets whether leading zeroes are displayed.
  */
  void setShowLeadingZeroes(bool showLeadingZeroes);

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

  //! @reimplements QSpinBox::prefix().
  QString prefix() const;

  //! @reimplements QSpinBox::setPrefix().
  void setPrefix(const QString& labelText);

  //! @reimplements QSpinBox::suffix().
  QString suffix();
  //! @reimplements QSpinBox::setSuffix().
  void setSuffix(const QString& text);

  //! Returns the value of the negative before prefix flag.
  bool negBeforePrefix() const;
  //! Sets the value of the negative before prefix flag.
  //!
  //!
  void setNegBeforePrefix(bool negBeforePrefix);

  //! @reimplements QSpinBox::stepType().
  QAbstractSpinBox::StepType stepType() const;
  //! @reimplements QSpinBox::setStepType().
  void setStepType(QAbstractSpinBox::StepType type);
  //! @reimplements QSpinBox::cleanText().
  QString cleanText() const;
  //! @reimplements QSpinBox::setWidgetAlignment().
  void setWidgetAlignment(const Qt::Alignment& widgetAlignment);
  //! @reimplements QSpinBox::widgetAlignment().
  Qt::Alignment widgetAlignment() const;

signals:
  /*!
      \fn LabelledExSpinBox::displayTypeChanged(ExSpinBox::DisplayType)

     This signal is sent when the user changes the display type of the spin box.
  */
  void displayTypeChanged(ExSpinBox::DisplayType);

  /*!
     \fn LabelledExSpinBox::valueChanged(int value)

     @from QSpinBox::value.
     @notprop value.
  */
  void valueChanged(int);

private:
  void initGui(const QString& text = QString());
};

#endif // LABELLEDEXSPINBOX_H
