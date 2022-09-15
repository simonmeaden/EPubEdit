#include "widgets/lntextedit.h"
#include "utilities/x11colors.h"

//class LineNumberArea : public QWidget
//{
//public:
//  LineNumberArea(LNTextEdit* editor)
//    : QWidget(editor)
//    , m_codeEditor(editor)
//  {
//  }

//  QSize sizeHint() const
//  {
//    return QSize(m_codeEditor->lineNumberAreaWidth(), 0);
//  }

//protected:
//  void paintEvent(QPaintEvent* event)
//  {
//    m_codeEditor->lineNumberAreaPaintEvent(event);
//  }

//private:
//  LNTextEdit* m_codeEditor;
//};

//====================================================================
//=== LNTextEdit
//====================================================================
LNTextEdit::LNTextEdit(QWidget* parent)
  : QTextEdit(parent)
//  , m_lineNumberArea(new LineNumberArea(this))
  , m_backColor(QColorConstants::X11::DarkGrey)
  , m_foreColor(QColorConstants::X11::LightGrey)
  , m_selForeColor(QColorConstants::X11::DarkSlateGrey)
{
//  connect(document(),
//          &QTextDocument::blockCountChanged,
//          this,
//          &LNTextEdit::updateLineNumberAreaWidth);
//  connect(verticalScrollBar(),
//          &QScrollBar::valueChanged,
//          this,
//          qOverload<int>(&LNTextEdit::updateLineNumberArea));
//  connect(this,
//          &LNTextEdit::textChanged,
//          this,
//          qOverload<>(&LNTextEdit::updateLineNumberArea));
//  connect(this,
//          &LNTextEdit::cursorPositionChanged,
//          this,
//          qOverload<>(&LNTextEdit::updateLineNumberArea));

//  updateLineNumberAreaWidth(0);
}

//int
//LNTextEdit::getFirstVisibleBlockId()
//{
//  // Detect the first block for which bounding rect - once translated
//  // in absolute coordinated - is contained by the editor's text area

//  // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
//  // exists for "QTextEdit"...

//  auto curs = QTextCursor(this->document());
//  curs.movePosition(QTextCursor::Start);
//  for (auto i = 0; i < document()->blockCount(); ++i) {
//    QTextBlock block = curs.block();

//    QRect r1 = viewport()->geometry();
//    QRect r2 = document()
//                 ->documentLayout()
//                 ->blockBoundingRect(block)
//                 .translated(viewport()->geometry().x(),
//                             viewport()->geometry().y() -
//                               (verticalScrollBar()->sliderPosition()))
//                 .toRect();

//    if (r1.contains(r2, true)) {
//      return i;
//    }

//    curs.movePosition(QTextCursor::NextBlock);
//  }

//  return 0;
//}

//void
//LNTextEdit::lineNumberAreaPaintEvent(QPaintEvent* event)
//{
//  verticalScrollBar()->setSliderPosition(verticalScrollBar()->sliderPosition());

//  QPainter painter(m_lineNumberArea);
//  painter.fillRect(event->rect(), m_backColor);
//  auto blockNumber = getFirstVisibleBlockId();

//  auto block = document()->findBlockByNumber(blockNumber);
//  auto prev_block =
//    (blockNumber > 0) ? document()->findBlockByNumber(blockNumber - 1) : block;
//  auto translate_y =
//    (blockNumber > 0) ? -verticalScrollBar()->sliderPosition() : 0;

//  auto top = this->viewport()->geometry().top();

//  // Adjust text position according to the previous "non entirely visible" block
//  // if applicable. Also takes in consideration the document's margin offset.
//  auto additionalMargin = 0;
//  if (blockNumber == 0)
//    // Simply adjust to document's margin
//    additionalMargin =
//      document()->documentMargin() - 1 - verticalScrollBar()->sliderPosition();
//  else
//    // Getting the height of the visible part of the previous "non entirely
//    // visible" block
//    additionalMargin = document()
//                         ->documentLayout()
//                         ->blockBoundingRect(prev_block)
//                         .translated(0, translate_y)
//                         .intersected(viewport()->geometry())
//                         .height();

//  // Shift the starting point
//  top += additionalMargin;

//  auto bottom =
//    top + document()->documentLayout()->blockBoundingRect(block).height();

//  // Draw the numbers (displaying the current line number in green)
//  while (block.isValid() && top <= event->rect().bottom()) {
//    if (block.isVisible() && bottom >= event->rect().top()) {
//      QString number = QString::number(blockNumber + 1);
//      //      painter.setPen(m_foreColor);
//      painter.setPen((textCursor().blockNumber() == blockNumber)
//                       ? m_selForeColor
//                       : m_foreColor);
//      painter.drawText(-5,
//                       top,
//                       m_lineNumberArea->width(),
//                       fontMetrics().height(),
//                       Qt::AlignRight,
//                       number);
//    }

//    block = block.next();
//    top = bottom;
//    bottom =
//      top +
//      (int)document()->documentLayout()->blockBoundingRect(block).height();
//    ++blockNumber;
//  }
//}

//int
//LNTextEdit::lineNumberAreaWidth()
//{
//  int digits = 1;
//  int max = qMax(1, this->document()->blockCount());
//  while (max >= 10) {
//    max /= 10;
//    ++digits;
//  }

//  int space = 13 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits);

//  return space;
//}

//const QColor&
//LNTextEdit::lineNumberForeColor() const
//{
//  return m_foreColor;
//}

//void
//LNTextEdit::setLineNumberForeColor(const QColor& foreColor)
//{
//  m_foreColor = foreColor;
//}

//const QBrush&
//LNTextEdit::lineNumberBackColor() const
//{
//  return m_backColor;
//}

//void
//LNTextEdit::setLineNumberBackColor(const QBrush& backColor)
//{
//  m_backColor = backColor;
//}

//void
//LNTextEdit::resizeEvent(QResizeEvent* e)
//{
//  QTextEdit::resizeEvent(e);

//  QRect cr = this->contentsRect();
//  m_lineNumberArea->setGeometry(
//    QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
//}

//const QColor&
//LNTextEdit::selForeColor() const
//{
//  return m_selForeColor;
//}

//void
//LNTextEdit::setSelForeColor(const QColor& selForeColor)
//{
//  m_selForeColor = selForeColor;
//}

//// const QBrush &LNTextEdit::selBackColor() const
////{
////   return m_selBackColor;
//// }

//// void LNTextEdit::setSelBackColor(const QBrush &selBackColor)
////{
////   m_selBackColor = selBackColor;
//// }

//void
//LNTextEdit::updateLineNumberAreaWidth(int /*blockCount*/)
//{
//  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
//}

//void
//LNTextEdit::updateLineNumberArea(QRectF)
//{
//  updateLineNumberArea();
//}

//void
//LNTextEdit::updateLineNumberArea(int)
//{
//  updateLineNumberArea();
//}

//void
//LNTextEdit::updateLineNumberArea()
//{
//  /*
//   * When the signal is emitted, the sliderPosition has been adjusted according
//   * to the action, but the value has not yet been propagated (meaning the
//   * valueChanged() signal was not yet emitted), and the visual display has not
//   * been updated. In slots connected to this signal you can thus safely adjust
//   * any action by calling setSliderPosition() yourself, based on both the
//   * action and the slider's value.
//   */
//  // Make sure the sliderPosition triggers one last time the valueChanged()
//  // signal with the actual value !!!!
//  this->verticalScrollBar()->setSliderPosition(
//    this->verticalScrollBar()->sliderPosition());

//  // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
//  // to grab the imformations from "sliderPosition()" and "contentsRect()".
//  // See the necessary connections used (Class constructor implementation part).

//  QRect rect = this->contentsRect();
//  m_lineNumberArea->update(
//    0, rect.y(), m_lineNumberArea->width(), rect.height());
//  updateLineNumberAreaWidth(0);
//  //----------
//  int dy = this->verticalScrollBar()->sliderPosition();
//  if (dy > -1) {
//    m_lineNumberArea->scroll(0, dy);
//  }

//  // Addjust slider to alway see the number of the currently being edited
//  // line...
//  int first_block_id = getFirstVisibleBlockId();
//  if (first_block_id == 0 ||
//      textCursor().block().blockNumber() == first_block_id - 1)
//    this->verticalScrollBar()->setSliderPosition(
//      dy - this->document()->documentMargin());

//  //    // Snap to first line (TODO...)
//  //    if (first_block_id > 0)
//  //    {
//  //        int slider_pos = this->verticalScrollBar()->sliderPosition();
//  //        int prev_block_height = (int)
//  //        this->document()->documentLayout()->blockBoundingRect(this->document()->findBlockByNumber(first_block_id-1)).height();
//  //        if (dy <= this->document()->documentMargin() + prev_block_height)
//  //            this->verticalScrollBar()->setSliderPosition(slider_pos -
//  //            (this->document()->documentMargin() + prev_block_height));
//  //    }
//}
