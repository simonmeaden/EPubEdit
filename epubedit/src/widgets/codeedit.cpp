#include "widgets/codeedit.h"
#include "config.h"
#include "widgets/htmlhighlighter.h"
#include "widgets/htmlscanner.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"
#include "utilities/x11colors.h"

CodeEdit::CodeEdit(PConfig config, QWidget* parent)
  : LNPlainTextEdit(parent)
  , m_document(new QTextDocument(this))
  , m_config(config)
  , m_scanner(new HtmlScanner(
      LNPlainTextEdit::document())) // TODO document() in two places
  , m_highlighter(new HtmlHighlighter(m_scanner, this)) // TODO replace later
{
  setDocument(m_document);
  m_highlighter->setBackground(QColorConstants::White);
  m_highlighter->setMatchColor(QColorConstants::X11::mediumblue);
  m_highlighter->setNameColor(QColorConstants::X11::mediumblue);
  m_highlighter->setAttrColor(QColorConstants::X11::darkkhaki);
  m_highlighter->setValueColor(QColorConstants::X11::midnightblue);
  m_highlighter->setSQuoteColor(QColorConstants::Svg::olive);
  m_highlighter->setDQuoteColor(QColorConstants::X11::darkgreen);
  m_highlighter->setLineBackground(QColorConstants::X11::grey80);
  setLNAreaSelectedBackColor(QColorConstants::X11::grey80);
  m_highlighter->setCommentColor(QColorConstants::X11::darkturquoise);
  m_highlighter->setErrorColor(QColorConstants::X11::orangered);

  connect(this,
          &QPlainTextEdit::cursorPositionChanged,
          m_highlighter,
          &HtmlHighlighter::cursorPosHasChanged);

  setKeyMap(m_config->keyMap());
}

QTextCursor
CodeEdit::currentCursor()
{
  return QPlainTextEdit::textCursor();
}

void
CodeEdit::setCurrentCursor(const QTextCursor& cursor)
{
  QPlainTextEdit::setTextCursor(cursor);
}

const QString
CodeEdit::href() const
{
  return m_href;
}

void
CodeEdit::loadHref(const QString& href)
{
  m_href = href;
  // TODO replace later
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    setText(text);
  }
}

void
CodeEdit::setText(const QString& text)
{
  disconnect(LNPlainTextEdit::document(),
             &QTextDocument::contentsChange,
             this,
             &CodeEdit::textHasChanged);
  QPlainTextEdit::setPlainText(text);
  m_scanner->load(text);
  auto cursor = textCursor();
  auto tag = m_scanner->tagAt(cursor.position());
  m_highlighter->setCurrentTag(tag);
  m_highlighter->rehighlight();
  connect(LNPlainTextEdit::document(),
          &QTextDocument::contentsChange,
          this,
          &CodeEdit::textHasChanged);
}

void
CodeEdit::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus())
    emit gotFocus(this);

  QPlainTextEdit::focusInEvent(event);
}

void
CodeEdit::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus())
    emit lostFocus(this);

  QPlainTextEdit::focusOutEvent(event);
}

void
CodeEdit::mousePressEvent(QMouseEvent* event)
{
  auto cursor = textCursor();
  auto block = cursor.block();
  m_highlighter->rehighlightBlock(block);

  LNPlainTextEdit::mousePressEvent(event);
}

void CodeEdit::mouseMoveEvent(QMouseEvent *event)
{
  // TODO
}

void
CodeEdit::mouseReleaseEvent(QMouseEvent* event)
{
  setFocus();
  QPlainTextEdit::mouseReleaseEvent(event);
}

bool
CodeEdit::eventFilter(QObject* obj, QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnter(static_cast<QHoverEvent*>(event)->position().toPoint());
      return true;
    case QEvent::HoverLeave:
      hoverLeave();
      return true;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(event)->position().toPoint());
      return true;
    default:
      break;
  }

  return LNPlainTextEdit::eventFilter(obj, event);
}

// void
// CodeEdit::cursorPositionHasChanged()
//{
//   qWarning();
// }

HtmlScanner*
CodeEdit::scanner() const
{
  return m_scanner;
}

void
CodeEdit::rehighlight()
{
  m_scanner->load(toPlainText());
  m_highlighter->rehighlight();
}

void
CodeEdit::textHasChanged(int position, int charsRemoved, int charsAdded)
{
  if (position == 0 || (charsRemoved == 0 && charsAdded == 0))
    return;

  auto currentTag = m_highlighter->currentTag();
  if (currentTag) {
    auto start = currentTag->start();
    auto length = currentTag->length() + charsAdded - charsRemoved;
    auto text = toPlainText();
    if (currentTag->type == TagText) {
      auto texttag = dynamic_cast<TextTag*>(currentTag);
      auto currentTagText = text.mid(start, length);
      texttag->text = currentTagText;
    } else {
      auto modifiedTagText = text.mid(start, length);
      auto modifiedTag = m_scanner->rescanTag(modifiedTagText, 0, length);
      modifiedTag->startCursor = currentTag->startCursor;
      modifiedTag->endCursor = currentTag->endCursor;
      m_highlighter->setCurrentTag(modifiedTag);
      m_scanner->replaceTag(currentTag, modifiedTag);
      delete currentTag;
    }
    m_highlighter->rehighlight();
  }
}

void
CodeEdit::hoverCheckTags(QPoint pos, int cursorPosition)
{
  for (auto tag : m_scanner->tagList()) {
    auto result = tag->isIn(cursorPosition);
    if (result == NotIn) {
      continue;
    } else {
      if (result == IsInAttributeName || result == IsInAttrubuteValue) {
        auto htmltag = dynamic_cast<HtmlTag*>(tag);
        if (htmltag && htmltag->hasAttributeIndex()) {
          auto attribute = htmltag->attributes.at(htmltag->attributeIndex);
          if (!attribute->hoverText.isEmpty()) {
            m_hoverTag = tag;
            m_hoverAttribute = attribute;
            QToolTip::showText(pos, attribute->hoverText, this);
          } else {
            QToolTip::hideText();
          }
        }
      } else {
        if (!tag->hoverText.isEmpty()) {
          m_hoverTag = tag;
          m_hoverAttribute = nullptr;
          QToolTip::showText(pos, tag->hoverText, this);
        } else {
          QToolTip::hideText();
        }
      }
    }
  }
}

void
CodeEdit::hoverEnter(QPoint pos)
{
  // TODO hover
  //  auto cursor = cursorForPosition(pos);
  //  hoverCheckTags(pos, cursor.position());
}

void
CodeEdit::hoverLeave()
{
  // TODO hover
  //  m_hoverTag = nullptr;
  //  m_hoverAttribute = nullptr;
}

void
CodeEdit::hoverMove(QPoint pos)
{
  // TODO hover
  //  auto cursor = cursorForPosition(pos);
  //  auto position = cursor.position();
  //  if (m_hoverTag) {
  //    if (m_hoverTag->isIn(position) != NotIn) {
  //      QToolTip::showText(pos, m_hoverTag->hoverText, this);
  //    } else if (m_hoverAttribute) {
  //      if (m_hoverAttribute->isIn(position) != NotIn) {
  //        QToolTip::showText(pos, m_hoverAttribute->hoverText, this);
  //      }
  //    } else {
  //      hoverCheckTags(pos, cursor.position());
  //    }
  //  }
}
