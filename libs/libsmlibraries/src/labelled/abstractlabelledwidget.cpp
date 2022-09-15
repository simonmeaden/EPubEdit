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
#include "labelled/abstractlabelledwidget.h"

AbstractLabelledWidget::AbstractLabelledWidget(QString text,
                                               QWidget* parent,
                                               const WidgetFactory& factory)
  : QWidget(parent)
  , m_widget(factory.newWidget(this))
  , m_position(Left)
{
  AbstractLabelledWidget::initGui(text);
  m_label->setObjectName("label");
  m_widget->setObjectName("widget");
  positionLabel();
}

AbstractLabelledWidget::~AbstractLabelledWidget() {}

QLabel*
AbstractLabelledWidget::label() const
{
  return m_label;
}

void
AbstractLabelledWidget::setLabel(QLabel* label)
{
  m_label = label;
  update();
}

void
AbstractLabelledWidget::setLabelText(const QString& text)
{
  m_label->setText(text);
}

QString
AbstractLabelledWidget::labelText() const
{
  return m_label->text();
}

AbstractLabelledWidget::LabelPosition
AbstractLabelledWidget::labelPosition() const
{
  return m_position;
}

void
AbstractLabelledWidget::setLabelPosition(
  const AbstractLabelledWidget::LabelPosition& position)
{
  if (position != m_position) {
    m_position = position;
    positionLabel();
  }
}

Qt::Alignment
AbstractLabelledWidget::labelAlignment() const
{
  return m_label->alignment();
}

void
AbstractLabelledWidget::setLabelAlignment(const Qt::Alignment& alignment)
{
  if (alignment != m_label->alignment()) {
    m_label->setAlignment(alignment);
  }
}

int
AbstractLabelledWidget::spacing()
{
  return m_layout->spacing();
}

void
AbstractLabelledWidget::setSpacing(int spacing)
{
  m_layout->setSpacing(spacing);
}

Qt::TextFormat
AbstractLabelledWidget::labelTextFormat() const
{
  return m_label->textFormat();
}

void
AbstractLabelledWidget::setLabelTextFormat(Qt::TextFormat format)
{
  m_label->setTextFormat(format);
}

void
AbstractLabelledWidget::positionLabel()
{
  m_layout->removeWidget(m_label);
  m_layout->removeWidget(m_widget);

  switch (m_position) {
    case Left:
      m_layout->addWidget(m_label, 0, 0);
      m_layout->addWidget(m_widget, 0, 1);
      break;

    case Right:
      m_layout->addWidget(m_label, 0, 1);
      m_layout->addWidget(m_widget, 0, 0);
      break;

    case Above:
      m_layout->addWidget(m_label, 0, 0);
      m_layout->addWidget(m_widget, 1, 0);
      break;

    case Below:
      m_layout->addWidget(m_label, 1, 0);
      m_layout->addWidget(m_widget, 0, 0);
      break;
  }
}

void
AbstractLabelledWidget::initGui(const QString& text)
{
  m_layout = new QGridLayout();
  setLayout(m_layout);

  m_label = new QLabel(text, this);
}

/*!
   \brief Reimplements: QFrame::sizeHint() const.
*/
QSize
AbstractLabelledWidget::sizeHint() const
{
  QSize lsh = m_label->sizeHint();
  QSize wsh = m_widget->sizeHint();
  QMargins m = m_layout->contentsMargins();
  int h, w;

  switch (m_position) {
    case Left:
    case Right:
      h = lsh.height() > wsh.height() ? lsh.height() : wsh.height();
      w = lsh.width() + wsh.width() + m_layout->spacing();
      break;

    case Above:
    case Below:
      h = lsh.height() + wsh.height() + m_layout->spacing();
      w = lsh.width() > wsh.width() ? lsh.width() : wsh.width();
  }

  h += (m.top() + m.bottom());
  w += (m.left() + m.right());

  return QSize(w, h);
}

/*!
   \brief Reimplements an access function for property:
   QWidget::minimumSizeHint.
*/
QSize
AbstractLabelledWidget::minimumSizeHint() const
{
  QSize lsh = m_label->minimumSizeHint();
  QSize wsh = m_widget->minimumSizeHint();
  QMargins m = m_layout->contentsMargins();
  int h, w;

  switch (m_position) {
    case Left:
    case Right:
      h = lsh.height() > wsh.height() ? lsh.height() : wsh.height();
      w = lsh.width() + wsh.width() + m_layout->spacing();
      break;

    case Above:
    case Below:
      h = lsh.height() + wsh.height() + m_layout->spacing();
      w = lsh.width() > wsh.width() ? lsh.width() : wsh.width();
  }

  h += (m.top() + m.bottom());
  w += (m.left() + m.right());

  return QSize(w, h);
}

void
AbstractLabelledWidget::setLabelSizePolicy(QSizePolicy policy)
{
  m_label->setSizePolicy(policy);
}

void
AbstractLabelledWidget::setLabelSizePolicy(QSizePolicy::Policy horizontal,
                                           QSizePolicy::Policy vertical)
{
  m_label->setSizePolicy(horizontal, vertical);
}

void
AbstractLabelledWidget::setWidgetSizePolicy(QSizePolicy policy)
{
  m_widget->setSizePolicy(policy);
}

void
AbstractLabelledWidget::setWidgetSizePolicy(QSizePolicy::Policy horizontal,
                                            QSizePolicy::Policy vertical)
{
  m_widget->setSizePolicy(horizontal, vertical);
}

QSizePolicy
AbstractLabelledWidget::labelSizePolicy()
{
  return m_label->sizePolicy();
}

QSizePolicy
AbstractLabelledWidget::widgetSizePolicy()
{
  return m_widget->sizePolicy();
}

QString
AbstractLabelledWidget::labelStyleSheet()
{
  return m_label->styleSheet();
}

void
AbstractLabelledWidget::setLabelStyleSheet(const QString& styleSheet)
{
  m_label->setStyleSheet(styleSheet);
}

QString
AbstractLabelledWidget::widgetStyleSheet()
{
  return m_widget->styleSheet();
}

void
AbstractLabelledWidget::setWidgetStyleSheet(const QString& styleSheet)
{
  m_widget->setStyleSheet(styleSheet);
}

void
AbstractLabelledWidget::clearLabel()
{
  m_label->clear();
}

// CustomLabelledWidget::CustomLabelledWidget(const QString& text, QWidget*
// parent)
//  : AbstractLabelledWidget(text, parent)
//{}

// void
// CustomLabelledWidget::setWidget(QWidget* widget)
//{
//  m_widget->deleteLater();
//  m_widget = widget;
//  CustomLabelledWidget::initGui();
//}

// void
// CustomLabelledWidget::initGui(const QString& )
//{}
