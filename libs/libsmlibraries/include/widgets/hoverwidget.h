#ifndef HOVERWIDGET_H
#define HOVERWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QColor>
#include <QFrame>
#include <QHBoxLayout>
#include <QMargins>
#include <QPaintEvent>
#include <QPainter>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QThread>
#include <QTimer>
#include <QToolButton>

#include "utilities/x11colors.h"

/*!
   \file hoverwidget.h "widgets/hoverwidget.h"
   \class HoverWidget hoverwidget.h
   \brief Displays a hover dialog in LNPlainTextEdit windows where hover text
          is defined.

  The display holds two sections which are managed seperately. A title block
  and a text block.

   \since 5.7.0
   \license{The MIT License}
   \copyright © 2022 Simon Meaden. All rights reserved.
   \see LNPlainTextEdit
*/
class HoverWidget : public QFrame
{
  Q_OBJECT
  enum Type
  {
    Title,
    Text,
  };

  class HoverHighlighter : public QSyntaxHighlighter
  {
  public:
    HoverHighlighter(QTextDocument* document);
    void highlightBlock(const QString&text);

    QMap<int, Type> lines;
    QColor m_backgroundColor;
    QColor m_foregroundColor;
    QTextCharFormat m_titleFormat;
    QTextCharFormat m_textFormat;
  };

public:
  //! Default constructor for HoverWidget
  explicit HoverWidget(QWidget* parent = nullptr);
  //! Constructor for HoverWidget
  explicit HoverWidget(const QString& Title,
                       const QString& Text,
                       QWidget* parent = nullptr);

  //! Sets the title text
  void setTitle(const QString& Title);
  //! Sets the text block
  void setText(const QString& Text);

  //! Displays the dialog
  void show(int timeout = 0);

  //! Allows the user to define margins for the dialog
  void setMargins(const QMargins& Margins);

  //! Defines a spacer in pixels between the title block and the text block
  void setSpacer(int Spacer);

  //! Returns true if no text was siupplied.
  bool isEmpty();

signals:
  //! Signal sent when the dialog has finished displaying.
  void finished();

protected:
  //! \reimplements{QFrame::event}
  bool event(QEvent* event) override;


private:
  int m_width = 0;
  int m_height = 0;
  QString m_text;
  QString m_title;
  QColor m_background;
  HoverHighlighter* m_highlighter;

  QMargins m_margins;
  int m_spacer = 5;

  bool m_pressed = false;
  QPixmap m_copyIcon;
  QPixmap m_copyPressedIcon;
  QRect m_btnRect;
  QRect m_iconRect;
  QRect m_textRect;
  QRect m_titleRect;
  QPlainTextEdit* m_editor;
  QToolButton* m_copyBtn;

  void hoverLeave(QHoverEvent* event);
  void buttonClicked(bool);
  void timedout();
  void buildGui();
  void addData();
};

#endif // HOVERWIDGET_H
