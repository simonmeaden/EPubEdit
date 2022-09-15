#include "widgets/htmlhighlighter.h"
//#include "widgets/codeedit.h"
#include "utilities/x11colors.h"
#include "widgets/htmlscanner.h"

HtmlHighlighter::HtmlHighlighter(HtmlScanner* scanner, QPlainTextEdit* parent)
  : QSyntaxHighlighter(parent->document())
  , m_editor(parent)
  , m_scanner(scanner)
  , m_backgroundColor(QColorConstants::White)
  , m_matchColor(QColorConstants::X11::mediumblue)
  , m_nameColor(QColorConstants::X11::mediumblue)
  , m_attrColor(QColorConstants::X11::darkslategray)
  , m_valueColor(QColorConstants::X11::midnightblue)
  , m_sQuoteColor(QColorConstants::Svg::olive)
  , m_dQuoteColor(QColorConstants::X11::darkgreen)
  , m_lineBackgroundColor(QColorConstants::X11::azure)
  , m_commentColor(QColorConstants::X11::darkturquoise)
  , m_errorColor(QColorConstants::X11::orangered)
  , m_charForegroundColor(QColorConstants::White)
  , m_charBackgroundColor(QColorConstants::X11::lightgray)

{
  m_lineTextFormat.setForeground(Qt::black);
  m_lineTextFormat.setBackground(m_lineBackgroundColor);
  m_lineMatchFormat.setForeground(m_matchColor);
  m_lineMatchFormat.setBackground(m_lineBackgroundColor);
  m_lineNameFormat.setForeground(m_nameColor);
  m_lineNameFormat.setBackground(m_lineBackgroundColor);
  m_lineAttrFormat.setForeground(m_attrColor);
  m_lineAttrFormat.setBackground(m_lineBackgroundColor);
  m_lineValueFormat.setForeground(m_valueColor);
  m_lineValueFormat.setBackground(m_lineBackgroundColor);
  m_lineSQuoteFormat.setForeground(m_sQuoteColor);
  m_lineSQuoteFormat.setBackground(m_lineBackgroundColor);
  m_lineDQuoteFormat.setForeground(m_dQuoteColor);
  m_lineDQuoteFormat.setBackground(m_lineBackgroundColor);
  m_lineCommentFormat.setForeground(m_commentColor);
  m_lineCommentFormat.setBackground(m_lineBackgroundColor);
  m_lineErrorFormat.setForeground(m_errorColor);
  m_lineErrorFormat.setBackground(m_lineBackgroundColor);

  m_textFormat.setForeground(Qt::black);
  m_textFormat.setBackground(m_backgroundColor);
  m_matchFormat.setForeground(m_matchColor);
  m_matchFormat.setBackground(m_backgroundColor);
  m_nameFormat.setForeground(m_nameColor);
  m_nameFormat.setBackground(m_backgroundColor);
  m_attrFormat.setForeground(m_attrColor);
  m_attrFormat.setBackground(m_backgroundColor);
  m_valueFormat.setForeground(m_valueColor);
  m_valueFormat.setBackground(m_backgroundColor);
  m_sQuoteFormat.setForeground(m_sQuoteColor);
  m_sQuoteFormat.setBackground(m_backgroundColor);
  m_dQuoteFormat.setForeground(m_dQuoteColor);
  m_dQuoteFormat.setBackground(m_backgroundColor);
  m_commentFormat.setForeground(m_commentColor);
  m_commentFormat.setBackground(m_backgroundColor);
  m_errorFormat.setForeground(m_errorColor);
  m_lineErrorFormat.setBackground(m_backgroundColor);
  m_charFormat.setBackground(m_charForegroundColor);
  m_charFormat.setBackground(m_charBackgroundColor);
}

void
HtmlHighlighter::setDoubleQuoteToError(Attribute* att)
{
  if (att->errors.testFlag(ScannerError::MismatchedQuotes)) {
    m_lineDQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_lineDQuoteFormat.setUnderlineColor(QColor(Qt::red));
    m_dQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_dQuoteFormat.setUnderlineColor(QColor(Qt::red));
  }
}

void
HtmlHighlighter::clearDoubleQuotesFromError()
{
  m_lineDQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
  m_dQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
}

void
HtmlHighlighter::setSingleQuoteToError(Attribute* att)
{
  if (att->errors.testFlag(ScannerError::MismatchedQuotes)) {
    m_lineSQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_lineSQuoteFormat.setUnderlineColor(QColor(Qt::red));
    m_sQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_sQuoteFormat.setUnderlineColor(QColor(Qt::red));
  }
}

void
HtmlHighlighter::clearSingleQuotesFromError()
{
  m_lineSQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
  m_sQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
}

void
HtmlHighlighter::highlightBlock(const QString& /*text*/)
{
  auto block = currentBlock();
  auto blockStart = block.position();
  auto blockText = block.text();
  auto blockEnd = blockStart + blockText.length();

  for (auto tag : m_scanner->tagList()) {

    // tag is completely outside block
    if (tag->end() < blockStart || tag->start() > blockEnd)
      continue;

    auto lineNumber = block.blockNumber();
    auto type = tag->type;

    switch (type) {
      case NoType:
        // Nothing actually done here.
        break;
      case TagText: {
        auto texttag = dynamic_cast<TextTag*>(tag);
        setFormat(texttag->start() - blockStart,
                  1,
                  (lineNumber == m_currentLineNumber ? m_lineTextFormat
                                                     : m_textFormat));
        break;
      }
      case TagCharacter: {
        auto chartag = dynamic_cast<SpecialCharTag*>(tag);
        setFormat(chartag->start() - blockStart, 1, m_charFormat);
        break;
      }
      case TagHtml: {
        auto htmltag = dynamic_cast<HtmlTag*>(tag);

        if (htmltag->errors.testFlag(MissingStartTag) ||
            htmltag->errors.testFlag(MissingEndTag)) {
          setFormat(htmltag->start() - blockStart,
                    htmltag->length(),
                    (lineNumber == m_currentLineNumber ? m_lineErrorFormat
                                                       : m_errorFormat));
        } else {
          setFormat(htmltag->start() - blockStart,
                    htmltag->length(),
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat));

          setFormat(htmltag->nameStart() - blockStart,
                    htmltag->length(),
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat));

          for (auto& att : htmltag->attributes) {
            setFormat(
              att->nameStartCursor.position() - blockStart,
              att->name.length(),
              (lineNumber == m_currentLineNumber
                 ? (att->errors.testFlag(BadAttributeName) ? m_lineErrorFormat
                                                           : m_lineAttrFormat)
                 : (att->errors.testFlag(BadAttributeName) ? m_errorFormat
                                                           : m_attrFormat)));
            if (att->hasValue()) {
              auto s = att->value;
              if (s.startsWith('"')) {
                setDoubleQuoteToError(att);
                setFormat(att->valueStartCursor.position() - blockStart,
                          s.length(),
                          (lineNumber == m_currentLineNumber
                             ? m_lineDQuoteFormat
                             : m_dQuoteFormat));
                clearDoubleQuotesFromError();
              } else if (s.startsWith('\'')) {
                setSingleQuoteToError(att);
                setFormat(att->valueStartCursor.position() - blockStart,
                          s.length(),
                          (lineNumber == m_currentLineNumber
                             ? m_lineSQuoteFormat
                             : m_sQuoteFormat));
                clearSingleQuotesFromError();
              } else {
                setFormat(att->valueStartCursor.position() - blockStart,
                          att->value.length(),
                          (lineNumber == m_currentLineNumber ? m_lineValueFormat
                                                             : m_valueFormat));
              }
            }
          }

          if (htmltag->isClosed())
            setFormat(htmltag->closerStart() - blockStart,
                      1,
                      (lineNumber == m_currentLineNumber
                         ? m_lineNameFormat
                         : m_nameFormat)); // / if needed
          setFormat(tag->end() - blockStart - 1,
                    1,
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat)); // >
        }

        break;
      }
      case TagXml: {
        auto xmltag = dynamic_cast<XmlDeclaration*>(tag);

        setFormat(xmltag->start() - blockStart,
                  xmltag->length(),
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat)); // <
        if (xmltag->hasStarter()) {
          setFormat(xmltag->start() - blockStart + 1,
                    1,
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat)); // ?
        }

        setFormat(xmltag->nameStart() - blockStart,
                  xmltag->nameLength(),
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat)); // <

        for (auto& att : xmltag->attributes) {
          setFormat(att->nameStartCursor.position() - blockStart,
                    att->name.length(),
                    (lineNumber == m_currentLineNumber ? m_lineAttrFormat
                                                       : m_attrFormat));
          if (att->hasValue()) {
            auto s = att->value;
            if (s.startsWith('"')) {
              setDoubleQuoteToError(att);
              setFormat(att->valueStartCursor.position() - blockStart,
                        s.length(),
                        (lineNumber == m_currentLineNumber ? m_lineDQuoteFormat
                                                           : m_dQuoteFormat));
              clearDoubleQuotesFromError();
            } else if (s.startsWith('\'')) {
              setSingleQuoteToError(att);
              setFormat(att->valueStartCursor.position() - blockStart,
                        s.length(),
                        (lineNumber == m_currentLineNumber ? m_lineSQuoteFormat
                                                           : m_sQuoteFormat));
              clearSingleQuotesFromError();
            } else {
              setFormat(att->valueStartCursor.position() - blockStart,
                        att->value.length(),
                        (lineNumber == m_currentLineNumber ? m_lineValueFormat
                                                           : m_valueFormat));
            }
          }
        }

        if (xmltag->hasEnder()) {
          setFormat(xmltag->end() - blockStart - 2,
                    1,
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat)); // ?
        }

        setFormat(tag->end() - blockStart - 1,
                  1,
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat));
        break;
      }
      case TagCloser: {
        auto closer = dynamic_cast<CloserTag*>(tag);

        if (closer->errors.testFlag(MissingStartTag) ||
            closer->errors.testFlag(MissingEndTag)) {
          setFormat(closer->start() - blockStart,
                    closer->length(),
                    (lineNumber == m_currentLineNumber ? m_lineErrorFormat
                                                       : m_errorFormat));
        } else {
          setFormat(closer->start() - blockStart,
                    closer->length(),
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat));

          setFormat(closer->nameStart() - blockStart,
                    closer->nameLength(),
                    (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                       : m_nameFormat)); // name
        }

        break;
      }
      case TagComment: {
        auto comment = dynamic_cast<CommentTag*>(tag);
        setFormat(tag->start() - blockStart,
                  comment->length(),
                  (lineNumber == m_currentLineNumber ? m_lineCommentFormat
                                                     : m_commentFormat));
        break;
      }
      case TagDoctype: {

        auto doctag = dynamic_cast<DocTypeTag*>(tag);
        setFormat(doctag->start() - blockStart,
                  doctag->length(),
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat)); // <

        setFormat(doctag->nameStart() - blockStart,
                  doctag->nameLength(),
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat)); // <

        for (auto i = 0; i < doctag->data.size(); i++) {
          auto data = doctag->dataAt(i);
          auto position = doctag->dataPosition(i);
          if (data.startsWith('"')) {
            setFormat(position - blockStart,
                      data.length(),
                      (lineNumber == m_currentLineNumber ? m_lineDQuoteFormat
                                                         : m_dQuoteFormat));
          } else if (data.startsWith('\'')) {
            setFormat(position - blockStart,
                      data.length(),
                      (lineNumber == m_currentLineNumber ? m_lineSQuoteFormat
                                                         : m_sQuoteFormat));
          } else {
            setFormat(position - blockStart,
                      data.length(),
                      (lineNumber == m_currentLineNumber ? m_lineAttrFormat
                                                         : m_attrFormat));
          }
        }

        setFormat(tag->end() - blockStart - 1,
                  1,
                  (lineNumber == m_currentLineNumber ? m_lineNameFormat
                                                     : m_nameFormat)); // >
        break;
      }
    }
  }
}

void
HtmlHighlighter::setTextColor(const QBrush& newTextColor)
{
  m_textColor = newTextColor;
  m_textFormat.setForeground(m_commentColor);
  m_lineTextFormat.setForeground(m_commentColor);
  rehighlight();
}

void
HtmlHighlighter::setErrorColor(const QColor& newError)
{
  m_errorColor = newError;
  m_errorFormat.setForeground(m_errorColor);
  m_lineErrorFormat.setForeground(m_errorColor);
  rehighlight();
}

void
HtmlHighlighter::setCommentColor(const QColor& comment)
{
  m_commentColor = comment;
  m_commentFormat.setForeground(m_commentColor);
  m_lineCommentFormat.setForeground(m_commentColor);
  rehighlight();
}

void
HtmlHighlighter::setDQuoteColor(const QColor& dQuoteColor)
{
  m_dQuoteColor = dQuoteColor;
  m_dQuoteFormat.setForeground(m_dQuoteColor);
  m_lineSQuoteFormat.setBackground(m_lineBackgroundColor);
  rehighlight();
}

void
HtmlHighlighter::setSQuoteColor(const QColor& sQuoteColor)
{
  m_sQuoteColor = sQuoteColor;
  m_sQuoteFormat.setForeground(m_sQuoteColor);
  m_lineSQuoteFormat.setForeground(m_sQuoteColor);
  rehighlight();
}

void
HtmlHighlighter::setLineBackground(const QColor& lineBackground)
{
  m_lineBackgroundColor = lineBackground;
  m_lineTextFormat.setBackground(m_lineBackgroundColor);
  m_lineMatchFormat.setBackground(m_lineBackgroundColor);
  m_lineNameFormat.setBackground(m_lineBackgroundColor);
  m_lineAttrFormat.setBackground(m_lineBackgroundColor);
  m_lineValueFormat.setBackground(m_lineBackgroundColor);
  m_lineSQuoteFormat.setBackground(m_lineBackgroundColor);
  m_lineDQuoteFormat.setBackground(m_lineBackgroundColor);
  m_lineCommentFormat.setBackground(m_lineBackgroundColor);
  rehighlight();
}

void
HtmlHighlighter::setValueColor(const QColor& valueColor)
{
  m_valueColor = valueColor;
  m_valueFormat.setForeground(m_valueColor);
  m_lineValueFormat.setForeground(m_valueColor);
  rehighlight();
}

void
HtmlHighlighter::setAttrColor(const QColor& attrColor)
{
  m_attrColor = attrColor;
  m_attrFormat.setForeground(m_attrColor);
  m_lineAttrFormat.setForeground(m_attrColor);
  rehighlight();
}

void
HtmlHighlighter::setNameColor(const QColor& nameColor)
{
  m_nameColor = nameColor;
  m_nameFormat.setForeground(m_nameColor);
  m_lineNameFormat.setForeground(m_nameColor);
  rehighlight();
}

void
HtmlHighlighter::setMatchColor(const QColor& matchColor)
{
  m_matchColor = matchColor;
  m_matchFormat.setForeground(m_matchColor);
  m_lineMatchFormat.setForeground(m_matchColor);
  rehighlight();
}

void
HtmlHighlighter::setBackground(const QColor& background)
{
  m_backgroundColor = background;
  m_textFormat.setBackground(m_backgroundColor);
  m_matchFormat.setBackground(m_backgroundColor);
  m_nameFormat.setBackground(m_backgroundColor);
  m_attrFormat.setBackground(m_backgroundColor);
  m_valueFormat.setBackground(m_backgroundColor);
  m_sQuoteFormat.setBackground(m_backgroundColor);
  m_dQuoteFormat.setBackground(m_backgroundColor);
  m_commentFormat.setBackground(m_backgroundColor);
  rehighlight();
}

void
HtmlHighlighter::setCharBackgroundColor(const QColor& CharBackgroundColor)
{
  m_charBackgroundColor = CharBackgroundColor;
  m_charFormat.setBackground(m_charBackgroundColor);
}

void
HtmlHighlighter::setCharForegroundColor(const QColor& CharForegroundColor)
{
  m_charForegroundColor = CharForegroundColor;
  m_charFormat.setForeground(m_charForegroundColor);
}

Tag*
HtmlHighlighter::currentTag() const
{
  return m_currentTag;
}

void
HtmlHighlighter::setCurrentTag(Tag* currentTag)
{
  m_currentTag = currentTag;
}

int
HtmlHighlighter::currentLineNumber() const
{
  return m_currentLineNumber;
}

void
HtmlHighlighter::cursorPosHasChanged()
{
  auto cursor = m_editor->textCursor();
  auto pos = cursor.position();
  auto ln = cursor.block().blockNumber();
  m_currentLineNumber = ln;

  if (m_currentTag)
    m_currentTag->selected = false;
  m_currentTag = nullptr;
  m_matchingTag = nullptr;

  auto list = m_scanner->tagList();
  if (list.isEmpty()) // not scanned yet
    return;

  Tag *prevTag = nullptr, *currentTag = nullptr, *matchingTag = nullptr;

  for (auto tag : list) {
    tag->selected = false;
    if (tag->isIn(pos) != NotIn) {
      currentTag = tag;
      break;
    }
    prevTag = tag;
  }

  auto cstart = -1, pend = -1;
  if (currentTag)
    cstart = currentTag->start();
  if (prevTag)
    pend = prevTag->end();
  if (cstart == pend && cstart == pos) {
    // at junction between tags
    if (prevTag && prevTag->type == TagText) {
      // at end of previous text so choose text
      m_prevTag = nullptr;
      m_currentTag = prevTag;
      if (currentTag)
        m_nextTag = currentTag;
    } else {
      // between two tags
      if (currentTag && currentTag->type == TagText) {
        // special case current is '\n'
        m_prevTag = nullptr;
        m_currentTag = currentTag;
        m_nextTag = nullptr;
      } else {
        m_prevTag = prevTag;
        m_currentTag = new TextTag(); // in case text added
        list.insert(list.indexOf(prevTag) + 1, m_currentTag);
        m_nextTag = nullptr;
      }
    }
  } else {
    // inside existing tag.
    m_prevTag = nullptr;
    m_currentTag = currentTag;
    m_nextTag = nullptr;
  }

  // search for matching tag/closure tag if any.
  auto closed = dynamic_cast<ClosedTag*>(currentTag);
  if (currentTag &&
      !(currentTag->type == TagText || (closed && closed->isClosed()))) {
    for (auto match : m_scanner->matchedTags()) {
      if (currentTag == match->first) {
        currentTag = match->first;
        matchingTag = match->second;
        break;
      } else if (currentTag == match->second) {
        currentTag = match->second;
        matchingTag = match->first;
        break;
      }
    }
  }

  m_matchingTag = matchingTag; // could be null
  m_currentIndex = list.indexOf(m_currentTag);

  rehighlight(); // needed to match tags.
}
