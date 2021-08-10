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
#ifndef LABELLEDTEXTFIELD_H
#define LABELLEDTEXTFIELD_H

#include "abstractlabelledwidget.h"
#include "sm_widgets_global.h"
#include <QLabel>
#include <QPicture>
#include <QPixmap>

/*!
   \file labelledtextfield.h labelledtextfield.cpp
   \class LabelledTextField labelledtextfield.h
   \brief The LabelledTextField is a wrapper for two QLabels and provides a
   labelled non-editable text field box. \since 5.7.0 \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the text QLabel are copied across to this widget.
*/
class SM_WIDGETS_SHARED_EXPORT LabelledTextField
  : public AbstractLabelledWidget
  , public AlignableWidgetInterface
  , public TextFormatInterface
{
  /*!
     \property LabelledTextField::text
     \brief This property holds the field's text

     @reimplements QLabel::text

     @accessor %text(), %setText(const QString&)
     @notifier textChanged()
  */
  Q_PROPERTY(QString text READ text WRITE setText)

  /*!
     \property LabelledTextField::textFormat
     \brief This property holds the field's text format

     @reimplements QLabel::textFormat

     @accessor %textFormat(), %setTextFormat(Qt::TextFormat)
  */
  Q_PROPERTY(Qt::TextFormat textFormat READ textFormat WRITE setTextFormat)

  /*!
     \property LabelledTextField::pixmap
     \brief This property holds the field's pixmap.

     @reimplements QLabel::pixmap
     @accessor %pixmap(), %setPixmap(const QPixmap&)
  */
  Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)

  /*!
     \property LabelledTextField::scaledContents
     \brief This property holds whether the field will scale its contents to
     fill all a vailable space.

     @reimplements QLabel::scaledContents
     @accessor %hasScaledContents(), %setScaledContents(bool)
  */
  Q_PROPERTY(bool scaledContents READ hasScaledContents WRITE setScaledContents)

  /*!
     \property LabelledTextField::widgetAlignment
     \brief This property holds the alignment of the field's contents

     @reimplements QLabel::alignment
     @accessor %alignment(), %setAlignment(Qt::Alignment)
  */
  Q_PROPERTY(
    Qt::Alignment widgetAlignment READ widgetAlignment WRITE setWidgetAlignment)

  /*!
     \property LabelledTextField::wordWrap
     \brief This property holds the field's word-wrapping policy

     @reimplements QLabel::wordWrap
     @accessor %wordWrap(), %setWordWrap(bool)
  */
  Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)

  /*!
     \property LabelledTextField::margin
     \brief This property holds the width of the field's margin

     @reimplements QLabel::margin
     @accessor %margin(), %setMargin(int)
  */
  Q_PROPERTY(int margin READ margin WRITE setMargin)

  /*!
     \property LabelledTextField::indent
     \brief This property holds the field's text indent in pixels

     @reimplements QLabel::indent
     @accessor %indent(), %setIndent(int)
  */
  Q_PROPERTY(int indent READ indent WRITE setIndent)

  /*!
     \property LabelledTextField::openExternalLinks
     \brief Specifies whether the field should automatically open links using
     QDesktopServices::openUrl() instead of emitting the linkActivated()
     signal.

     @reimplements QLabel::openExternalLinks
     @accessor %openExternalLinks(), %setOpenExternalLinks(bool)
  */
  Q_PROPERTY(
    bool openExternalLinks READ openExternalLinks WRITE setOpenExternalLinks)

  /*!
     \property LabelledTextField::textInteractionFlags
     \brief Specifies how the field should interact with user input if it
     displays text.

     @reimplements QLabel::textInteractionFlags
     @accessor %textInteractionFlags(),
     %setTextInteractionFlags(Qt::TextInteractionFlags)
  */
  Q_PROPERTY(Qt::TextInteractionFlags textInteractionFlags READ
               textInteractionFlags WRITE setTextInteractionFlags)

  /*!
     \property LabelledTextField::hasSelectedText
     \brief This property holds whether the field has any text selected

     @reimplements QLabel::hasSelectedText
     @accessor %hasSelectedText()
  */
  Q_PROPERTY(bool hasSelectedText READ hasSelectedText)

  /*!
     \property LabelledTextField::selectedText
     \brief This property holds the field's selected text

     @reimplements QLabel::selectedText
      @accessor %selectedText()
  */
  Q_PROPERTY(QString selectedText READ selectedText)

  Q_OBJECT

protected:
  /// \cond DO_NOT_DOCUMENT
  struct WidgetFactory : AbstractLabelledWidget::WidgetFactory
  {
    virtual QWidget* newWidget(QWidget* parent) const
    {
      return new QLabel(parent);
    }
  };

  explicit LabelledTextField(WidgetFactory const& factory,
                             const QString& labelText,
                             const QString& contents = QString(),
                             QWidget* parent = nullptr);
  /// \endcond

public:
  //! Constructs a text field with the given parent, and supplied label and
  //! empty text properties.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.
  explicit LabelledTextField(const QString& labelText,
                             QWidget* parent = nullptr);
  //! Constructs a text field with the given parent, and supplied label and text
  //! properties.
  //!
  //! \param labelText the label text string.
  //! \param contents of the QLineEdit.
  //! \param parent the parent widget.
  explicit LabelledTextField(const QString& labelText,
                             const QString& contents = QString(),
                             QWidget* parent = nullptr);

  //! Returns the QLabel widget.
  QLabel* textfield();

  //! @reimplements QLabel::text() const.
  QString text() const;

  //! @reimplements QLabel::setText().
  void setText(const QString& text);

  //! @reimplements QLabel::textFormat() const.
  Qt::TextFormat textFormat() const;

  //! @reimplements QLabel::setTextFormat().
  void setTextFormat(Qt::TextFormat format);

  //! @reimplements QLabel::pixmap() const.
  QPixmap pixmap() const;

  //! @reimplements QLabel::setPixmap().
  void setPixmap(const QPixmap& pixmap);

#ifndef QT_NO_PICTURE
  //! @reimplements QLabel::picture() const.
  QPicture picture() const;
#endif

#if QT_CONFIG(movie)
  //! @reimplements QLabel::movie() const.
  QMovie* movie() const;

  //! @reimplements QLabel::setMovie().
  void setMovie(QMovie* movie);
#endif

  //! @reimplements QLabel::setPicture().
  void setPicture(const QPicture& picture);

  //! @reimplements QLabel::hasScaledContents() const.
  bool hasScaledContents() const;

  //! @reimplements QLabel::setScaledContents().
  void setScaledContents(bool scaled);

  //! @reimplements QLabel::alignment() const.
  Qt::Alignment widgetAlignment() const override;

  //! @reimplements QLabel::setAlignment().
  void setWidgetAlignment(const Qt::Alignment& widgetAlignment) override;

  //! @reimplements QLabel::wordWrap() const.
  bool wordWrap() const;

  //! @reimplements QLabel::setWordWrap().
  void setWordWrap(bool on);

  //! @reimplements QLabel::indent() const.
  int indent() const;

  //! @reimplements QLabel::setIndent().
  void setIndent(int);

  //! @reimplements QLabel::margin() const.
  int margin() const;

  //! @reimplements QLabel::setMargin().
  void setMargin(int);

  //! @reimplements QLabel::openExternalLinks() const.
  bool openExternalLinks() const;

  //! @reimplements QLabel::setOpenExternalLinks().
  void setOpenExternalLinks(bool open);

  //! @reimplements QLabel::setTextInteractionFlags().
  void setTextInteractionFlags(Qt::TextInteractionFlags flags);

  //! @reimplements QLabel::textInteractionFlags() const.
  Qt::TextInteractionFlags textInteractionFlags() const;

  //! @reimplements QLabel::setSelection().
  void setSelection(int start, int length);

  //! @reimplements QLabel::text() hasSelectedText.
  bool hasSelectedText() const;

  //! @reimplements QLabel::selectedText() const.
  QString selectedText() const;

  //! @reimplements QLabel::selectionStart() const.
  int selectionStart() const;

  //! @reimplements QLabel::heightForWidth() const.
  int heightForWidth(int height) const override;

  //! @reimplements QLabel::setNum(int).
  void setNum(int);

  //! @reimplements QLabel::setNum(double).
  void setNum(double);

  //! @reimplements QLabel::clear().
  void clearField();

  //   //! @reimplements QFrame::sizeHint().
  //   QSize sizeHint() const;

  //   //! @reimplements QFrame::minimumSizeHint().
  //   QSize minimumSizeHint() const;
signals:
  /*!
      \fn LabelledTextField::linkActivated(const QString&)
      \brief This signal is emitted when the user clicks a link.
     The URL referred to by the anchor is passed in link.

     @from QLabel::linkActivated.
  */
  void linkActivated(const QString& link);

  /*!
     \fn LabelledTextField::linkHovered(const QString&)
     \brief This signal is emitted when the user hovers over a link.
     The URL referred to by the anchor is passed in link.

     @from QLabel::linkHovered.
  */
  void linkHovered(const QString& link);

private:
  void initGui(const QString& contents = QString());
};

#endif // LABELLEDTEXTFIELD_H
