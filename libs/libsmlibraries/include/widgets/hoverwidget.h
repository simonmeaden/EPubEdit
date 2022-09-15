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
  explicit HoverWidget(QWidget* parent = nullptr);
  explicit HoverWidget(const QString& Title,
                       const QString& Text,
                       QWidget* parent = nullptr);

  void setTitle(const QString& Title);
  void setText(const QString& Text);

  void show(int timeout = 0);

  void setMargins(const QMargins& Margins);

  void setSpacer(int Spacer);

  bool isEmpty();

signals:
  void finished();

protected:
  bool event(QEvent* event) override;

  void hoverLeave(QHoverEvent* event);

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

  void buttonClicked(bool);
  void timedout();
  void buildGui();
  void addData();
};

#endif // HOVERWIDGET_H
