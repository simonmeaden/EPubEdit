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
#include "labelled/labelledlineedit.h"

LabelledLineEdit::LabelledLineEdit(const QString& labelText, QWidget* parent)
  : LabelledLineEdit(LabelledLineEdit::WidgetFactory(),
                     labelText,
                     QString(),
                     parent)
{}

LabelledLineEdit::LabelledLineEdit(const QString& labelText,
                                   const QString& contents,
                                   QWidget* parent)
  : LabelledLineEdit(LabelledLineEdit::WidgetFactory(),
                     labelText,
                     contents,
                     parent)
{}

LabelledLineEdit::LabelledLineEdit(const WidgetFactory& factory,
                                   const QString& labelText,
                                   const QString& contents,
                                   QWidget* parent)
  : AbstractLabelledWidget(labelText, parent, factory)
{
  LabelledLineEdit::initGui(contents);
}

void
LabelledLineEdit::addAction(QAction* action,
                            QLineEdit::ActionPosition labelPosition)
{
  qobject_cast<QLineEdit*>(m_widget)->addAction(action, labelPosition);
}

QAction*
LabelledLineEdit::addAction(const QIcon& icon,
                            QLineEdit::ActionPosition labelPosition)
{
  return qobject_cast<QLineEdit*>(m_widget)->addAction(icon, labelPosition);
}

void
LabelledLineEdit::backspace()
{
  qobject_cast<QLineEdit*>(m_widget)->backspace();
}

QCompleter*
LabelledLineEdit::completer() const
{
  return qobject_cast<QLineEdit*>(m_widget)->completer();
}

QMenu*
LabelledLineEdit::createStandardContextMenu()
{
  return qobject_cast<QLineEdit*>(m_widget)->createStandardContextMenu();
}

void
LabelledLineEdit::cursorBackward(bool mark, int steps)
{
  qobject_cast<QLineEdit*>(m_widget)->cursorBackward(mark, steps);
}

void
LabelledLineEdit::cursorForward(bool mark, int steps)
{
  qobject_cast<QLineEdit*>(m_widget)->cursorForward(mark, steps);
}

int
LabelledLineEdit::cursorPositionAt(const QPoint& pos)
{
  return qobject_cast<QLineEdit*>(m_widget)->cursorPositionAt(pos);
}

void
LabelledLineEdit::cursorWordBackward(bool mark)
{
  qobject_cast<QLineEdit*>(m_widget)->cursorWordBackward(mark);
}

void
LabelledLineEdit::cursorWordForward(bool mark)
{
  qobject_cast<QLineEdit*>(m_widget)->cursorWordForward(mark);
}

void
LabelledLineEdit::del()
{
  qobject_cast<QLineEdit*>(m_widget)->del();
}

void
LabelledLineEdit::deselect()
{
  qobject_cast<QLineEdit*>(m_widget)->deselect();
}

void
LabelledLineEdit::end(bool mark)
{
  return qobject_cast<QLineEdit*>(m_widget)->end(mark);
}

void
LabelledLineEdit::home(bool mark)
{
  qobject_cast<QLineEdit*>(m_widget)->home(mark);
}

void
LabelledLineEdit::insert(const QString& newText)
{
  qobject_cast<QLineEdit*>(m_widget)->insert(newText);
}

int
LabelledLineEdit::selectionEnd() const
{
  return qobject_cast<QLineEdit*>(m_widget)->selectionEnd();
}

int
LabelledLineEdit::selectionLength() const
{
  return qobject_cast<QLineEdit*>(m_widget)->selectionLength();
}

int
LabelledLineEdit::selectionStart() const
{
  return qobject_cast<QLineEdit*>(m_widget)->selectionStart();
}

void
LabelledLineEdit::setCompleter(QCompleter* c)
{
  qobject_cast<QLineEdit*>(m_widget)->setCompleter(c);
}

void
LabelledLineEdit::setSelection(int start, int length)
{
  qobject_cast<QLineEdit*>(m_widget)->setSelection(start, length);
}

void
LabelledLineEdit::setTextMargins(int left, int top, int right, int bottom)
{
  qobject_cast<QLineEdit*>(m_widget)->setTextMargins(left, top, right, bottom);
}

void
LabelledLineEdit::setValidator(const QValidator* v)
{
  qobject_cast<QLineEdit*>(m_widget)->setValidator(v);
}

QMargins
LabelledLineEdit::textMargins() const
{
  return qobject_cast<QLineEdit*>(m_widget)->textMargins();
}

const QValidator*
LabelledLineEdit::validator() const
{
  return qobject_cast<QLineEdit*>(m_widget)->validator();
}

QString
LabelledLineEdit::text() const
{
  return qobject_cast<QLineEdit*>(m_widget)->text();
}

void
LabelledLineEdit::setText(const QString& text)
{
  qobject_cast<QLineEdit*>(m_widget)->setText(text);
}

bool
LabelledLineEdit::hasAcceptableInput() const
{
  return qobject_cast<QLineEdit*>(m_widget)->hasAcceptableInput();
}

void
LabelledLineEdit::setWidgetAlignment(const Qt::Alignment& widgetAlignment)
{
  qobject_cast<QLineEdit*>(m_widget)->setAlignment(widgetAlignment);
}

Qt::Alignment
LabelledLineEdit::widgetAlignment() const
{
  return qobject_cast<QLineEdit*>(m_widget)->alignment();
}

bool
LabelledLineEdit::isClearButtonEnabled() const
{
  return qobject_cast<QLineEdit*>(m_widget)->isClearButtonEnabled();
}

void
LabelledLineEdit::setClearButtonEnabled(bool enable)
{
  qobject_cast<QLineEdit*>(m_widget)->setClearButtonEnabled(enable);
}

Qt::CursorMoveStyle
LabelledLineEdit::cursorMoveStyle() const
{
  return qobject_cast<QLineEdit*>(m_widget)->cursorMoveStyle();
}

void
LabelledLineEdit::setCursorMoveStyle(Qt::CursorMoveStyle style)
{
  qobject_cast<QLineEdit*>(m_widget)->setCursorMoveStyle(style);
}

int
LabelledLineEdit::cursorPosition() const
{
  return qobject_cast<QLineEdit*>(m_widget)->cursorPosition();
}

void
LabelledLineEdit::setCursorPosition(int position)
{
  qobject_cast<QLineEdit*>(m_widget)->setCursorPosition(position);
}

QString
LabelledLineEdit::displayText() const
{
  return qobject_cast<QLineEdit*>(m_widget)->displayText();
}

bool
LabelledLineEdit::dragEnabled() const
{
  return qobject_cast<QLineEdit*>(m_widget)->dragEnabled();
}

void
LabelledLineEdit::setDragEnabled(bool enabled)
{
  qobject_cast<QLineEdit*>(m_widget)->setDragEnabled(enabled);
}

QLineEdit::EchoMode
LabelledLineEdit::echoMode() const
{
  return qobject_cast<QLineEdit*>(m_widget)->echoMode();
}

void
LabelledLineEdit::setEchoMode(QLineEdit::EchoMode echomode)
{
  qobject_cast<QLineEdit*>(m_widget)->setEchoMode(echomode);
}

bool
LabelledLineEdit::hasSelectedText() const
{
  return qobject_cast<QLineEdit*>(m_widget)->hasSelectedText();
}

QString
LabelledLineEdit::inputMask() const
{
  return qobject_cast<QLineEdit*>(m_widget)->inputMask();
}

void
LabelledLineEdit::setInputMask(const QString& inputMask)
{
  qobject_cast<QLineEdit*>(m_widget)->setInputMask(inputMask);
}

int
LabelledLineEdit::maxLength() const
{
  return qobject_cast<QLineEdit*>(m_widget)->maxLength();
}

void
LabelledLineEdit::setMaxLength(int length)
{
  qobject_cast<QLineEdit*>(m_widget)->setMaxLength(length);
}

bool
LabelledLineEdit::isModified() const
{
  return qobject_cast<QLineEdit*>(m_widget)->isModified();
}

void
LabelledLineEdit::setModified(bool modified)
{
  qobject_cast<QLineEdit*>(m_widget)->setModified(modified);
}

QString
LabelledLineEdit::placeholderText() const
{
  return qobject_cast<QLineEdit*>(m_widget)->placeholderText();
}

void
LabelledLineEdit::setPlaceholderText(const QString& placeholderText)
{
  qobject_cast<QLineEdit*>(m_widget)->setPlaceholderText(placeholderText);
}

bool
LabelledLineEdit::isReadOnly() const
{
  return qobject_cast<QLineEdit*>(m_widget)->isReadOnly();
}

void
LabelledLineEdit::setReadOnly(bool readonly)
{
  qobject_cast<QLineEdit*>(m_widget)->setReadOnly(readonly);
}

bool
LabelledLineEdit::isRedoAvailable() const
{
  return qobject_cast<QLineEdit*>(m_widget)->isRedoAvailable();
}

QString
LabelledLineEdit::selectedText() const
{
  return qobject_cast<QLineEdit*>(m_widget)->selectedText();
}

bool
LabelledLineEdit::isUndoAvailable() const
{
  return qobject_cast<QLineEdit*>(m_widget)->isUndoAvailable();
}

bool
LabelledLineEdit::isEmpty() const
{
  return qobject_cast<QLineEdit*>(m_widget)->text().isEmpty();
}

int
LabelledLineEdit::size() const
{
  return qobject_cast<QLineEdit*>(m_widget)->text().size();
}

int
LabelledLineEdit::length() const
{
  return qobject_cast<QLineEdit*>(m_widget)->text().size();
}

void
LabelledLineEdit::clear()
{
  qobject_cast<QLineEdit*>(m_widget)->clear();
}

void
LabelledLineEdit::initGui(QString contents)
{
  qobject_cast<QLineEdit*>(m_widget)->setText(contents);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::cursorPositionChanged,
          this,
          &LabelledLineEdit::cursorPositionChanged);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::editingFinished,
          this,
          &LabelledLineEdit::editingFinished);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::inputRejected,
          this,
          &LabelledLineEdit::inputRejected);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::returnPressed,
          this,
          &LabelledLineEdit::returnPressed);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::editingFinished,
          this,
          &LabelledLineEdit::selectionChanged);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::textChanged,
          this,
          &LabelledLineEdit::textChanged);
  //   connect(qobject_cast<QLineEdit*>(m_widget),
  //           &QLineEdit::textChanged,
  //           this,
  //           &LabelledLineEdit::textHasChanged);
  connect(qobject_cast<QLineEdit*>(m_widget),
          &QLineEdit::textEdited,
          this,
          &LabelledLineEdit::textEdited);
  //   connect(qobject_cast<QLineEdit*>(m_widget),
  //           &QLineEdit::textEdited,
  //           this,
  //           &LabelledLineEdit::textHasEdited);
}

// void LabelledLineEdit::textHasChanged(const QString& text)
//{
//   emit textChanged(text);
//}

// void LabelledLineEdit::textHasEdited(const QString& text)
//{
//   emit textEdited(text);
//}
