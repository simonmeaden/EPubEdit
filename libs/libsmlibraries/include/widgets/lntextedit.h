#ifndef LNTEXTEDIT_H
#define LNTEXTEDIT_H

#include <QTextEdit>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTextCursor>
#include <QTextBlock>
#include <QScrollBar>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

/*!
 * \ingroup widgets
 * \class LNTextEdit lntextedit.h "includes/widgets/lntextedit.h"
 * \brief The LNTextEdit class an extension of QTextEdit that displays line numbers.
 */
class LNTextEdit : public QTextEdit
{
public:
  //! Constructor for LNTextEdit
  LNTextEdit(QWidget* parent = nullptr);

//  int getFirstVisibleBlockId();
//  void lineNumberAreaPaintEvent(QPaintEvent* event);
//  int lineNumberAreaWidth();

//  const QBrush &lineNumberBackColor() const;
//  void setLineNumberBackColor(const QBrush &backColor);
//  const QColor &lineNumberForeColor() const;
//  void setLineNumberForeColor(const QColor &foreColor);
////  const QBrush &selBackColor() const;
////  void setSelBackColor(const QBrush &selBackColor);
//  const QColor &selForeColor() const;
//  void setSelForeColor(const QColor &selForeColor);

//protected:
//  void resizeEvent(QResizeEvent* e);

private:
  QWidget *m_lineNumberArea;
  QBrush m_backColor;
  QColor m_foreColor;
//  QBrush m_selBackColor;
  QColor m_selForeColor;

//  void updateLineNumberAreaWidth(int);
//  void updateLineNumberArea(QRectF /*rect_f*/);
//  void updateLineNumberArea(int /*slider_pos*/);
//  void updateLineNumberArea();
};

#endif // LNTEXTEDIT_H
