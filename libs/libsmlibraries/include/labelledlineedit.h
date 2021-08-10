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
#ifndef LABELLEDLINEEDIT_H
#define LABELLEDLINEEDIT_H

#include "abstractlabelledwidget.h"
#include "sm_widgets_global.h"

#include <QLineEdit>

/*!
   \file labelledlineedit.h labelledlineedit.cpp
   \class LabelledLineEdit labelledlineedit.h
   \brief The LabelledTextField is a wrapper for QLabel and a QLineEdit
          provides a labelled non-editable text field box.

   \since 5.7.0
   \license The MIT License
   \copyright © 2019 - 2020 Simon Meaden. All rights reserved.

   All properties of the QLineEdit are copied across to this widget.

*/
class SM_WIDGETS_SHARED_EXPORT LabelledLineEdit
  : public AbstractLabelledWidget
  , public AlignableWidgetInterface
{

  /*!
     \property LabelledLineEdit::text
     \brief This property holds the line edit's text.

     @reimplements QLineEdit::text

     @accessor %text(void), %setText()
     @notifier textChanged(const QString&)
  */
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

  /*!
     \property LabelledLineEdit::acceptableInput
     \brief This property holds whether the input satisfies the inputMask and
     the validator.

     @reimplements QLineEdit::acceptableInput

     @accessor %hasAcceptableInput()
  */
  Q_PROPERTY(bool acceptableInput READ hasAcceptableInput)

  /*!
     \property LabelledLineEdit::widgetAlignment
     \brief This property holds the alignment of the line edit.

     @reimplements QLineEdit::widgetAlignment

     @accessor %widgetAlignment(), %setWidgetAlignment()
  */
  Q_PROPERTY(
    Qt::Alignment widgetAlignment READ widgetAlignment WRITE setWidgetAlignment)

  /*!
     \property LabelledLineEdit::clearButtonEnabled
     \brief This property holds whether the line edit displays a clear button
     when it is not empty.

     @reimplements QLineEdit::clearButtonEnabled

     @accessor %isClearButtonEnabled(), %setClearButtonEnabled()
  */
  Q_PROPERTY(bool clearButtonEnabled READ isClearButtonEnabled WRITE
               setClearButtonEnabled)

  /*!
     \property LabelledLineEdit::cursorMoveStyle
     \brief This property holds the movement style of cursor in this line edit.

     @reimplements QLineEdit::cursorMoveStyle

     @accessor %cursorMoveStyle(), %setCursorMoveStyle()
  */
  Q_PROPERTY(Qt::CursorMoveStyle cursorMoveStyle READ cursorMoveStyle WRITE
               setCursorMoveStyle)

  /*!
     \property LabelledLineEdit::cursorPosition
     \brief This property holds the current cursor position for this line edit.

     @reimplements QLineEdit::cursorPosition

     @accessor %cursorPosition(), %setCursorPosition()
  */
  Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition)

  /*!
     \property LabelledLineEdit::displayText
     \brief This property holds the displayed text.

     @reimplements QLineEdit::displayText

     @accessor %displayText()
  */
  Q_PROPERTY(QString displayText READ displayText)

  /*!
     \property LabelledLineEdit::dragEnabled
     \brief This property holds whether the lineedit starts a drag if the user
     presses and moves the mouse on some selected text.

     @reimplements QLineEdit::dragEnabled

     @accessor %dragEnabled(), %setDragEnabled()
  */
  Q_PROPERTY(int dragEnabled READ dragEnabled WRITE setDragEnabled)

  /*!
     \property LabelledLineEdit::echoMode
     \brief This property holds the line edit's echo mode.

     @reimplements QLineEdit::echoMode

     @accessor %echoMode(), %setEchoMode()
  */
  Q_PROPERTY(QLineEdit::EchoMode echoMode READ echoMode WRITE setEchoMode)

  /*!
     \property LabelledLineEdit::selectedText
     \brief This property holds whether there is any text selected.

     @reimplements QLineEdit::selectedText

      @accessor %hasSelectedText()
  */
  Q_PROPERTY(bool selectedText READ hasSelectedText)

  /*!
     \property LabelledLineEdit::inputMask
     \brief This property holds the validation input mask.

     @reimplements QLineEdit::inputMask

     @accessor %inputMask(), %setInputMask(const QString &inputMask)
  */
  Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask)

  /*!
     \property LabelledLineEdit::maxLength
     \brief This property holds the maximum permitted length of the text.

     @reimplements QLineEdit::maxLength

     @accessor %maxLength(), %setMaxLength()
  */
  Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)

  /*!
     \property LabelledLineEdit::modified
     \brief This property holds whether the line edit's contents has been
     modified by the user.

     @reimplements QLineEdit::modified

     @accessor %isModified(), %setModified()
  */
  Q_PROPERTY(int modified READ isModified WRITE setModified)

  /*!
     \property LabelledLineEdit::placeholderText()
     \brief Sets a placeholderText text shown when no valid index is set

     @reimplements QLineEdit::placeholderText

     @accessor %placeholderText(void), %setPlaceholderText()
  */
  Q_PROPERTY(
    QString placeholderText READ placeholderText WRITE setPlaceholderText)

  /*!
     \property LabelledLineEdit::readOnly()
     \brief This property holds whether the line edit is read only.

     @reimplements QLineEdit::readOnly

     @accessor %isReadOnly(), %setReadOnly()
  */
  Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

  /*!
     \property LabelledLineEdit::redoAvailable()
     \brief This property holds whether redo is available.

     @reimplements QLineEdit::redoAvailable

     @accessor %isRedoAvailable()
  */
  Q_PROPERTY(bool redoAvailable READ isRedoAvailable)

  /*!
     \property LabelledLineEdit::undoAvailable()
     \brief This property holds whether undo is available.

     @reimplements QLineEdit::undoAvailable

     @accessor %isUndoAvailable()
  */
  Q_PROPERTY(bool undoAvailable READ isUndoAvailable)

  Q_OBJECT

protected:
  /// \cond DO_NOT_DOCUMENT
  struct WidgetFactory : AbstractLabelledWidget::WidgetFactory
  {
    virtual QWidget* newWidget(QWidget* parent) const
    {
      return new QLineEdit(parent);
    }
  };

  explicit LabelledLineEdit(WidgetFactory const& factory,
                            const QString& labelText,
                            const QString& contents,
                            QWidget* parent = nullptr);

  /// \endcond

public:
  //! \brief Constructs a LabelledLineEdit with given labelText and default
  //!        parent.
  //!
  //! \param labelText the label text string.
  //! \param parent the parent widget.
  explicit LabelledLineEdit(const QString& labelText,
                            QWidget* parent = nullptr);
  //! \brief Constructs a LabelledLineEdit with given labelText and lineedit
  //!        contents, and default parent.
  //!
  //! \param labelText the label text string.
  //! \param contents of the QLineEdit.
  //! \param parent the parent widget.
  explicit LabelledLineEdit(const QString& labelText,
                            const QString& contents,
                            QWidget* parent = nullptr);

  //! @reimplements QLineEdit::addAction(QAction, QLineEdit::ActionPosition).
  void addAction(QAction* action, QLineEdit::ActionPosition labelPosition);

  //! @reimplements QLineEdit::addAction(const QIcon&,
  //! QLineEdit::ActionPosition).
  QAction* addAction(const QIcon& icon,
                     QLineEdit::ActionPosition labelPosition);

  //! @reimplements QLineEdit::backspace().
  void backspace();

  //! @reimplements QLineEdit::completer().
  QCompleter* completer() const;

  //! @reimplements QLineEdit::backspace().
  QMenu* createStandardContextMenu();

  //! @reimplements QLineEdit::cursorBackward().
  void cursorBackward(bool mark, int steps = 1);

  //! @reimplements QLineEdit::cursorForward().
  void cursorForward(bool mark, int steps = 1);

  //! @reimplements QLineEdit::cursorPositionAt().
  int cursorPositionAt(const QPoint& pos);

  //! @reimplements QLineEdit::cursorWordBackward().
  void cursorWordBackward(bool mark);

  //! @reimplements QLineEdit::cursorWordForward().
  void cursorWordForward(bool mark);

  //! @reimplements QLineEdit::del().
  void del();

  //! @reimplements QLineEdit::deselect().
  void deselect();

  //! @reimplements QLineEdit::end().
  void end(bool mark);

  //! @reimplements QLineEdit::home().
  void home(bool mark);

  //! @reimplements QLineEdit::insert().
  void insert(const QString& newText);

  //! @reimplements QLineEdit::selectionEnd().
  int selectionEnd() const;

  //! @reimplements QLineEdit::selectionLength().
  int selectionLength() const;

  //! @reimplements QLineEdit::selectionStart().
  int selectionStart() const;

  //! @reimplements QLineEdit::setCompleter().
  void setCompleter(QCompleter* c);

  //! @reimplements QLineEdit::setSelection().
  void setSelection(int start, int length);

  //! @reimplements QLineEdit::setTextMargins().
  void setTextMargins(int left, int top, int right, int bottom);

  //! @reimplements QLineEdit::setValidator().
  void setValidator(const QValidator* v);

  //! @reimplements QLineEdit::textMargins().
  QMargins textMargins() const;

  //! @reimplements QLineEdit::validator().
  const QValidator* validator() const;

  //! @reimplements QLineEdit::text().
  QString text() const;

  //! @reimplements QLineEdit::setText().
  void setText(const QString& text);

  //! @reimplements QLineEdit::hasAcceptableInput().
  bool hasAcceptableInput() const;

  //! @reimplements QLineEdit::setAlignment().
  void setWidgetAlignment(const Qt::Alignment& widgetAlignment) override;

  //! @reimplements QLineEdit::alignment().
  Qt::Alignment widgetAlignment() const override;

  //! @reimplements QLineEdit::isClearButtonEnabled().
  bool isClearButtonEnabled() const;

  //! @reimplements QLineEdit::setClearButtonEnabled().
  void setClearButtonEnabled(bool enable);

  //! @reimplements QLineEdit::cursorMoveStyle().
  Qt::CursorMoveStyle cursorMoveStyle() const;

  //! @reimplements QLineEdit::setCursorMoveStyle().
  void setCursorMoveStyle(Qt::CursorMoveStyle style);

  //! @reimplements QLineEdit::cursorPosition().
  int cursorPosition() const;

  //! @reimplements QLineEdit::setCursorPosition().
  void setCursorPosition(int position);

  //! @reimplements QLineEdit::displayText().
  QString displayText() const;

  //! @reimplements QLineEdit::dragEnabled().
  bool dragEnabled() const;

  //! @reimplements QLineEdit::setDragEnabled().
  void setDragEnabled(bool enabled);

  //! @reimplements QLineEdit::echoMode().
  QLineEdit::EchoMode echoMode() const;

  //! @reimplements QLineEdit::setEchoMode().
  void setEchoMode(QLineEdit::EchoMode echomode);

  //! @reimplements QLineEdit::hasSelectedText().
  bool hasSelectedText() const;

  //! @reimplements QLineEdit::inputMask().
  QString inputMask() const;

  //! @reimplements QLineEdit::setInputMask().
  void setInputMask(const QString& inputMask);

  //! @reimplements QLineEdit::maxLength().
  int maxLength() const;

  //! @reimplements QLineEdit::setMaxLength().
  void setMaxLength(int length);

  //! @reimplements QLineEdit::isModified().
  bool isModified() const;

  //! @reimplements QLineEdit::setModified().
  void setModified(bool modified);

  //! @reimplements QLineEdit::placeholderText().
  QString placeholderText() const;

  //! @reimplements QLineEdit::setPlaceholderText().
  void setPlaceholderText(const QString& placeholderText);

  //! @reimplements QLineEdit::isReadOnly().
  bool isReadOnly() const;

  //! @reimplements QLineEdit::setReadOnly().
  void setReadOnly(bool readonly);

  //! @reimplements QLineEdit::isRedoAvailable().
  bool isRedoAvailable() const;

  //! @reimplements QLineEdit::selectedText().
  QString selectedText() const;

  //! @reimplements QLineEdit::isUndoAvailable().
  bool isUndoAvailable() const;

  //! Checks whether the line edit is empty.
  //! Returns true if it is, otherwise returns false.
  bool isEmpty() const;

  //! Returns the length of the line edit text.
  int size() const;

  //! Returns the length of the line edit text.
  //! This is an alias for size().
  int length() const;

  //! Clears the line edit text.
  void clear();

signals:
  /*!
     \fn LabelledLineEdit::cursorPositionChanged()
     \brief This signal is emitted whenever the cursor moves.
     The previous position is given by old, and the new position by new.

     @from QLineEdit::cursorPositionChanged.
  */
  void cursorPositionChanged(int oldPos, int newPos);

  /*!
     \fn LabelledLineEdit::editingFinished()
     \brief This signal is emitted when the Return or Enter key is pressed or
     the line edit loses focus.

     @from QLineEdit::editingFinished.
  */
  void editingFinished();

  /*!
     \fn LabelledLineEdit::returnPressed()
     \brief This signal is emitted when the Return or Enter key is pressed.

     @from QLineEdit::returnPressed.
  */
  void returnPressed();

  /*!
     \fn LabelledLineEdit::selectionChanged()
     \brief This signal is emitted whenever the selection changes.

     @from QLineEdit::selectionChanged.
  */
  void selectionChanged();

  /*!
     \fn LabelledLineEdit::textChanged(const QString&)
     \brief This signal is emitted whenever the text changes. The text
     argument is the new text.

     @from QLineEdit::textEdited.
  */
  void textChanged(const QString&);

  /*!
     \fn LabelledLineEdit::textEdited(const QString&)
     \brief This signal is emitted whenever the text is edited. The text
     argument is the new text.

     @from QLineEdit::textEdited.
  */
  void textEdited(const QString&);

  /*!
     \fn LabelledLineEdit::inputRejected()
     \brief This signal is emitted when the user presses a key that is not
     considered to be acceptable input.

     @from QLineEdit::inputRejected.
  */
  void inputRejected();

private:
  void initGui(QString contents = QString());
  //   void textHasChanged(const QString& text);
  //   void textHasEdited(const QString& text);
};

#endif // LABELLEDLINEEDIT_H
