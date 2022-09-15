#ifndef HTMLEDIT_H
#define HTMLEDIT_H

#include "document/bookpointers.h"
#include "widgets/iepubeditor.h"
#include "widgets/lnplaintextedit.h"

#include <QKeyEvent>
#include <QToolTip>

class HtmlHighlighter;
class HtmlScanner;
class Tag;
class Attribute;

class CodeEdit
  : public LNPlainTextEdit
  , public IEPubEditor
{
  Q_OBJECT
public:
  CodeEdit(PConfig config, QWidget* parent);

  // IEPubEditor interface
//  QTextDocument* document() override;
  QTextCursor currentCursor() override;
  void setCurrentCursor(const QTextCursor& cursor) override;
  const QString href() const override;
  void loadHref(const QString& href) override;
  void setText(const QString& text) override;

  HtmlScanner* scanner() const;

  void rehighlight();

signals:
  void lostFocus(QWidget*);
  void gotFocus(QWidget*);
  void mouseClicked(QPoint pos);

protected:
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);

private:
  QTextDocument* m_document = nullptr;
  QWidget* m_parent;
  PConfig m_config;
  QString m_href;
  HtmlScanner* m_scanner;
  HtmlHighlighter* m_highlighter;
  Tag* m_hoverTag = nullptr;
  Attribute* m_hoverAttribute = nullptr;

  //  void cursorPositionHasChanged();
  void textHasChanged(int position, int charsRemoved, int charsAdded);
  //  void textModified(bool modified);
  void hoverCheckTags(QPoint pos, int cursorPosition);
};

#endif // HTMLEDIT_H
