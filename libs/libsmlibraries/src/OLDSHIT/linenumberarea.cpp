#include "linenumberarea.h"

LineNumberArea::LineNumberArea(QWidget* editor)
  : QWidget(editor)
{
//  codeEditor = editor;
}

QSize
LineNumberArea::sizeHint() const
{
  return QSize(/*codeEditor->lineNumberAreaWidth()*/10, 0);
}

void
LineNumberArea::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  painter.fillRect(event->rect(), Qt::lightGray);

  QTextBlock block = m_textDocument->firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = int(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + int(blockBoundingRect(block).height());

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0,
                       top,
                       lineNumberArea->width(),
                       fontMetrics().height(),
                       Qt::AlignRight,
                       number);
    }

    block = block.next();
    top = bottom;
    bottom = top + int(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

QTextDocument *LineNumberArea::textDocument() const
{
  return m_textDocument;
}

void LineNumberArea::setTextDocument(QTextDocument *newTextDocument)
{
  m_textDocument = newTextDocument;
}
