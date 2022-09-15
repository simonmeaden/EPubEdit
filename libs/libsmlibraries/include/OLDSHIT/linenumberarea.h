#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QTextDocument>
#include <QWidget>
#include <QPainter>

class LineNumberArea : public QWidget
{
public:
  LineNumberArea(QWidget* editor);

  QSize sizeHint() const override;

  QTextDocument *textDocument() const;
  void setTextDocument(QTextDocument *newTextDocument);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
//  QWidget* codeEditor;
  QTextDocument*m_textDocument;
};

#endif // LINENUMBERAREA_H
