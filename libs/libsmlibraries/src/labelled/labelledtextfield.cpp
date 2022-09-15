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
#include "labelled/labelledtextfield.h"

LabelledTextField::LabelledTextField(const QString& labelText, QWidget* parent)
  : LabelledTextField(LabelledTextField::WidgetFactory(),
                      labelText,
                      QString(),
                      parent)
{}

LabelledTextField::LabelledTextField(const QString& labelText,
                                     const QString& contents,
                                     QWidget* parent)
  : LabelledTextField(LabelledTextField::WidgetFactory(),
                      labelText,
                      contents,
                      parent)
{}

LabelledTextField::LabelledTextField(const WidgetFactory& factory,
                                     const QString& labelText,
                                     const QString& contents,
                                     QWidget* parent)
  : AbstractLabelledWidget(labelText, parent, factory)
{
  LabelledTextField::initGui(contents);
}

QLabel*
LabelledTextField::textfield()
{
  return qobject_cast<QLabel*>(m_widget);
}

QString
LabelledTextField::text() const
{
  return qobject_cast<QLabel*>(m_widget)->text();
}

void
LabelledTextField::setText(const QString& text)
{
  qobject_cast<QLabel*>(m_widget)->setText(text);
}

Qt::TextFormat
LabelledTextField::textFormat() const
{
  return qobject_cast<QLabel*>(m_widget)->textFormat();
}

void
LabelledTextField::setTextFormat(Qt::TextFormat format)
{
  qobject_cast<QLabel*>(m_widget)->setTextFormat(format);
}

QPixmap
LabelledTextField::pixmap() const
{
#if QT_DEPRECATED_SINCE(5, 15)
  return qobject_cast<QLabel*>(m_widget)->pixmap(Qt::ReturnByValue);
#else
  return qobject_cast<QLabel*>(m_widget)->pixmap(Qt::ReturnByValueConstant);
#endif
}

void
LabelledTextField::setPixmap(const QPixmap& pixmap)
{
  qobject_cast<QLabel*>(m_widget)->setPixmap(pixmap);
}

#ifndef QT_NO_PICTURE
QPicture
LabelledTextField::picture() const
{
#if QT_DEPRECATED_SINCE(5, 15)
  return qobject_cast<QLabel*>(m_widget)->picture(Qt::ReturnByValue);
#else
  return qobject_cast<QLabel*>(m_widget)->picture(Qt::ReturnByValueConstant);
#endif
}

void
LabelledTextField::setPicture(const QPicture& picture)
{
  qobject_cast<QLabel*>(m_widget)->setPicture(picture);
}
#endif

#if QT_CONFIG(movie)
QMovie*
LabelledTextField::movie() const
{
  return qobject_cast<QLabel*>(m_widget)->movie();
}

void
LabelledTextField::setMovie(QMovie* movie)
{
  qobject_cast<QLabel*>(m_widget)->setMovie(movie);
}
#endif

bool
LabelledTextField::hasScaledContents() const
{
  return qobject_cast<QLabel*>(m_widget)->hasScaledContents();
}

void
LabelledTextField::setScaledContents(bool scaled)
{
  qobject_cast<QLabel*>(m_widget)->setScaledContents(scaled);
}

Qt::Alignment
LabelledTextField::widgetAlignment() const
{
  return qobject_cast<QLabel*>(m_widget)->alignment();
}

void
LabelledTextField::setWidgetAlignment(const Qt::Alignment& alignment)
{
  qobject_cast<QLabel*>(m_widget)->setAlignment(alignment);
}

bool
LabelledTextField::wordWrap() const
{
  return qobject_cast<QLabel*>(m_widget)->wordWrap();
}

void
LabelledTextField::setWordWrap(bool on)
{
  qobject_cast<QLabel*>(m_widget)->setWordWrap(on);
}

int
LabelledTextField::indent() const
{
  return qobject_cast<QLabel*>(m_widget)->indent();
}

void
LabelledTextField::setIndent(int indent)
{
  qobject_cast<QLabel*>(m_widget)->setIndent(indent);
}

int
LabelledTextField::margin() const
{
  return qobject_cast<QLabel*>(m_widget)->margin();
}

void
LabelledTextField::setMargin(int margin)
{
  qobject_cast<QLabel*>(m_widget)->setMargin(margin);
}

bool
LabelledTextField::openExternalLinks() const
{
  return qobject_cast<QLabel*>(m_widget)->openExternalLinks();
}

void
LabelledTextField::setOpenExternalLinks(bool open)
{
  qobject_cast<QLabel*>(m_widget)->setOpenExternalLinks(open);
}

void
LabelledTextField::setSelection(int start, int length)
{
  qobject_cast<QLabel*>(m_widget)->setSelection(start, length);
}

bool
LabelledTextField::hasSelectedText() const
{
  return qobject_cast<QLabel*>(m_widget)->hasSelectedText();
}

QString
LabelledTextField::selectedText() const
{
  return qobject_cast<QLabel*>(m_widget)->selectedText();
}

int
LabelledTextField::selectionStart() const
{
  return qobject_cast<QLabel*>(m_widget)->selectionStart();
}

int
LabelledTextField::heightForWidth(int height) const
{
  return qobject_cast<QWidget*>(m_widget)->heightForWidth(height);
}

void
LabelledTextField::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
  qobject_cast<QLabel*>(m_widget)->setTextInteractionFlags(flags);
}

Qt::TextInteractionFlags
LabelledTextField::textInteractionFlags() const
{
  return qobject_cast<QLabel*>(m_widget)->textInteractionFlags();
}

void
LabelledTextField::setNum(int num)
{
  qobject_cast<QLabel*>(m_widget)->setNum(num);
}

void
LabelledTextField::setNum(double num)
{
  qobject_cast<QLabel*>(m_widget)->setNum(num);
}

void
LabelledTextField::clearField()
{
  qobject_cast<QLabel*>(m_widget)->clear();
}

void
LabelledTextField::initGui(const QString& text)
{
  setText(text);
  connect(qobject_cast<QLabel*>(m_widget),
          &QLabel::linkHovered,
          this,
          &LabelledTextField::linkHovered);
  connect(qobject_cast<QLabel*>(m_widget),
          &QLabel::linkActivated,
          this,
          &LabelledTextField::linkActivated);
}
