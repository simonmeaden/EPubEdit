#include "widgets/htmlhighlighter.h"
#include "widgets/htmlscanner.h"

HtmlHighlighter::HtmlHighlighter(HtmlScanner* scanner, LNPlainTextEdit* parent)
  : QSyntaxHighlighter(parent->document())
  , m_editor(parent)
  , m_scanner(scanner)
  , m_backgroundColor(QColorConstants::White)
  , m_matchColor(QColorConstants::X11::chartreuse)
  , m_matchBackgroundColor(QColorConstants::X11::grey50)
  , m_nameColor(QColorConstants::X11::mediumblue)
  , m_attrColor(QColorConstants::X11::darkslategray)
  , m_valueColor(QColorConstants::X11::midnightblue)
  , m_sQuoteColor(QColorConstants::Svg::olive)
  , m_dQuoteColor(QColorConstants::X11::darkgreen)
  , m_commentColor(QColorConstants::X11::darkturquoise)
  , m_errorColor(QColorConstants::X11::orangered)
  , m_charForegroundColor(QColorConstants::White)
  , m_charBackgroundColor(QColorConstants::X11::lightgray)
  , m_tagMatchUnderlineColor(QColorConstants::X11::darkgreen)
{
  m_textFormat.setForeground(Qt::black);
  m_textFormat.setBackground(m_backgroundColor);
  m_matchFormat.setForeground(m_matchColor);
  m_matchFormat.setBackground(m_matchBackgroundColor);
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
  m_charFormat.setBackground(m_charForegroundColor);
  m_charFormat.setBackground(m_charBackgroundColor);
  m_tagMatchFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
  m_tagMatchFormat.setUnderlineColor(QColorConstants::X11::darkgreen);
}

void
HtmlHighlighter::setDoubleQuoteToError(Attribute* att)
{
  if (att->errors.testFlag(ScannerError::MismatchedQuotes)) {
    m_dQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_dQuoteFormat.setUnderlineColor(QColor(Qt::red));
  }
}

void
HtmlHighlighter::clearDoubleQuotesFromError()
{
  m_dQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
}

void
HtmlHighlighter::setSingleQuoteToError(Attribute* att)
{
  if (att->errors.testFlag(ScannerError::MismatchedQuotes)) {
    m_sQuoteFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    m_sQuoteFormat.setUnderlineColor(QColor(Qt::red));
  }
}

void
HtmlHighlighter::clearSingleQuotesFromError()
{
  m_sQuoteFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
}

bool
HtmlHighlighter::formatComment(Tag* tag,
                               const QString& text,
                               int blockStart,
                               int blockEnd,
                               int blockLength,
                               int tagStart,
                               int tagEnd,
                               int tagOffset,
                               int tagLength)
{
  auto comment = dynamic_cast<CommentTag*>(tag);
  if (comment) {
    int start = 0, length = 0;
    if (blockStart <= tagStart && blockEnd >= tagEnd) {
      // block totally within tag
      start = tagOffset;
      length = tagLength;
    } else if (tagStart >= blockStart && blockEnd < tagEnd) {
      // multiline tag start
      start = tagOffset;
      length = blockEnd - tagStart;
    } else {
      // multi line comment
      if (text.isEmpty()) {
        return true;
      } else {
        start = 0;
        length = blockLength;
      }
    }
    setFormat(start, length, m_commentFormat);
  }
  return false;
}

void
HtmlHighlighter::highlightBlock(const QString& text)
{

  auto block = currentBlock();
  auto blockStart = block.position();
  auto blockText = text;
  auto blockEnd = blockStart + blockText.length();
  auto blockLength = text.length();

  for (auto tag : m_scanner->tagList()) {
    auto tagStart = tag->start();
    auto tagEnd = tag->end();
    auto tagLength = tag->length();
    auto tagOffset = tagStart - blockStart;

    // tag is completely outside block
    if (tag->end() < blockStart || tag->start() >= blockEnd)
      continue;

    auto type = tag->type;

    switch (type) {
      case NoType:
        // Nothing actually done here.
        break;
      case TagText: {
        auto texttag = dynamic_cast<TextTag*>(tag);
        if (texttag){
          setFormat(tagStart - blockStart, text.length(), m_textFormat);
        }
        break;
      }
      case TagCharacter: {
        auto chartag = dynamic_cast<SpecialCharTag*>(tag);
        setFormat(chartag->start() - blockStart, 1, m_charFormat);
        break;
      }
      case TagHtml: {
        auto htmltag = dynamic_cast<HtmlTag*>(tag);

        // tag start '<'
        if (m_brktMatchStart > -1 && m_brktMatchEnd > -1) {
          // these have to be separate in case of newline between them
          if (m_brktMatchStart >= blockStart && m_brktMatchStart < blockEnd) {
            setFormat(m_brktMatchStart - blockStart, 1, m_matchFormat);
          }
        } else {
          setFormat(tag->start() - blockStart, 1, m_nameFormat);
        }

        if (htmltag->errors.testFlag(MissingStartTag) ||
            htmltag->errors.testFlag(MissingEndTag)) {
          setFormat(
            htmltag->start() - blockStart, htmltag->length(), m_errorFormat);
        } else {

          setFormat(htmltag->nameStart() - blockStart,
                    htmltag->nameLength(),
                    m_nameFormat);

          for (auto& att : htmltag->attributes) {
            setFormat(att->nameStartCursor.position() - blockStart,
                      att->name.length(),
                      (att->errors.testFlag(BadAttributeName) ? m_errorFormat
                                                              : m_attrFormat));
            if (att->hasValue()) {
              auto s = att->value;
              if (s.startsWith('"')) {
                setDoubleQuoteToError(att);
                setFormat(att->valueStartCursor.position() - blockStart,
                          s.length(),
                          m_dQuoteFormat);
                clearDoubleQuotesFromError();
              } else if (s.startsWith('\'')) {
                setSingleQuoteToError(att);
                setFormat(att->valueStartCursor.position() - blockStart,
                          s.length(),
                          m_sQuoteFormat);
                clearSingleQuotesFromError();
              } else {
                setFormat(att->valueStartCursor.position() - blockStart,
                          att->value.length(),
                          m_valueFormat);
              }
            }
          }

          if (htmltag->isClosed())
            setFormat(htmltag->closerStart() - blockStart,
                      1,
                      m_nameFormat); // / if needed

          if (m_brktMatchStart > -1 && m_brktMatchEnd > -1) {
            // these have to be separate in case of newline between them
            if (m_brktMatchEnd >= blockStart && m_brktMatchEnd < blockEnd) {
              setFormat(m_brktMatchEnd - blockStart, 1, m_matchFormat);
            }
          } else {
            setFormat(tag->end() - blockStart - 1, 1,
                      m_nameFormat); // >
          }
        }

        break;
      }
      case TagXml: {
        auto xmltag = dynamic_cast<XmlDeclaration*>(tag);

        setFormat(xmltag->start() - blockStart,
                  xmltag->length(),
                  m_nameFormat); // <
        if (xmltag->hasStarter()) {
          setFormat(xmltag->start() - blockStart + 1, 1,
                    m_nameFormat); // ?
        }

        setFormat(xmltag->nameStart() - blockStart,
                  xmltag->nameLength(),
                  m_nameFormat); // <

        for (auto& att : xmltag->attributes) {
          setFormat(att->nameStartCursor.position() - blockStart,
                    att->name.length(),
                    m_attrFormat);
          if (att->hasValue()) {
            auto s = att->value;
            if (s.startsWith('"')) {
              setDoubleQuoteToError(att);
              setFormat(att->valueStartCursor.position() - blockStart,
                        s.length(),
                        m_dQuoteFormat);
              clearDoubleQuotesFromError();
            } else if (s.startsWith('\'')) {
              setSingleQuoteToError(att);
              setFormat(att->valueStartCursor.position() - blockStart,
                        s.length(),
                        m_sQuoteFormat);
              clearSingleQuotesFromError();
            } else {
              setFormat(att->valueStartCursor.position() - blockStart,
                        att->value.length(),
                        m_valueFormat);
            }
          }
        }

        if (xmltag->hasEnder()) {
          setFormat(xmltag->end() - blockStart - 2, 1,
                    m_nameFormat); // ?
        }

        setFormat(tag->end() - blockStart - 1, 1, m_nameFormat);
        break;
      }
      case TagCloser: {
        auto closer = dynamic_cast<CloserTag*>(tag);

        if (closer->errors.testFlag(MissingStartTag) ||
            closer->errors.testFlag(MissingEndTag)) {
          setFormat(
            closer->start() - blockStart, closer->length(), m_errorFormat);
        } else {
          setFormat(
            closer->start() - blockStart, closer->length(), m_nameFormat);

          setFormat(closer->nameStart() - blockStart,
                    closer->nameLength(),
                    m_nameFormat); // name
        }

        break;
      }
      case TagComment: {
        if (formatComment(tag,
                          text,
                          blockStart,
                          blockEnd,
                          blockLength,
                          tagStart,
                          tagEnd,
                          tagOffset,
                          tagLength)) {
          continue;
        }
        break;
      }
      case TagDoctype: {

        auto doctag = dynamic_cast<DocTypeTag*>(tag);
        setFormat(doctag->start() - blockStart,
                  doctag->length(),
                  m_nameFormat); // <

        setFormat(doctag->nameStart() - blockStart,
                  doctag->nameLength(),
                  m_nameFormat); // <

        for (auto i = 0; i < doctag->data.size(); i++) {
          auto data = doctag->dataAt(i);
          auto position = doctag->dataPosition(i);
          if (data.startsWith('"')) {
            setFormat(position - blockStart, data.length(), m_dQuoteFormat);
          } else if (data.startsWith('\'')) {
            setFormat(position - blockStart, data.length(), m_sQuoteFormat);
          } else {
            setFormat(position - blockStart, data.length(), m_attrFormat);
          }
        }

        setFormat(tag->end() - blockStart - 1, 1,
                  m_nameFormat); // >
        break;
      }
    }
  }

  // matches "" '' <> () {} and []
  if (m_brktMatchStart > -1 && m_brktMatchEnd > -1) {
    // these have to be separate in case of newline between them
    if (m_brktMatchStart >= blockStart && m_brktMatchStart < blockEnd) {
      setFormat(m_brktMatchStart - blockStart, 1, m_matchFormat);
    }
    if (m_brktMatchEnd >= blockStart && m_brktMatchEnd < blockEnd) {
      setFormat(m_brktMatchEnd - blockStart, 1, m_matchFormat);
    }
  }

  if (m_startTagMatch && m_endTagMatch) {
    if (m_startTagMatchModified) {
      auto start = QTextCursor(m_startTagMatch->startCursor);
      auto end = m_startTagMatch->endCursor;
      while (start != end) {
        start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        auto newFormat = start.charFormat();
        QTextCharFormat oldFormat(newFormat);
        m_startTagFormats.append(oldFormat);
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setUnderlineColor(m_tagMatchUnderlineColor);
        start.setCharFormat(newFormat);
        start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
      }
      m_startTagMatchModified = false;
    } else {
      auto start = QTextCursor(m_startTagMatch->startCursor);
      auto end = m_startTagMatch->endCursor;
      while (start != end) {
        start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        auto newFormat = start.charFormat();
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setUnderlineColor(m_tagMatchUnderlineColor);
        start.setCharFormat(newFormat);
        start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
      }
    }

    if (m_endTagMatchModified) {
      auto start = QTextCursor(m_startTagMatch->startCursor);
      auto end = m_endTagMatch->endCursor;
      while (start != end) {
        start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        auto newFormat = start.charFormat();
        QTextCharFormat oldFormat(newFormat);
        m_endTagFormats.append(oldFormat);
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setUnderlineColor(m_tagMatchUnderlineColor);
        start.setCharFormat(newFormat);
        start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
      }
      m_endTagMatchModified = false;
    } else {
      auto start = QTextCursor(m_endTagMatch->startCursor);
      auto end = m_endTagMatch->endCursor;
      while (start != end) {
        start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        auto newFormat = start.charFormat();
        newFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        newFormat.setUnderlineColor(m_tagMatchUnderlineColor);
        start.setCharFormat(newFormat);
        start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
      }
    }
  }
}

Tag*
HtmlHighlighter::endTagMatch() const
{
  return m_endTagMatch;
}

void
HtmlHighlighter::setEndTagMatch(Tag* endTagMatch)
{
  if (m_endTagMatch && m_endTagMatch != endTagMatch) {
    auto start = QTextCursor(m_endTagMatch->startCursor);
    for (auto& format : m_endTagFormats) {
      start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
      start.setCharFormat(format);
      start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
      start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
    }
    m_endTagFormats.clear();
  }
  m_endTagMatch = endTagMatch;
  m_endTagMatchModified = true;
  rehighlight();
}

Tag*
HtmlHighlighter::startTagMatch() const
{
  return m_startTagMatch;
}

void
HtmlHighlighter::setStartTagMatch(Tag* startTagMatch)
{
  if (m_startTagMatch && m_startTagMatch != startTagMatch) {
    auto start = QTextCursor(m_startTagMatch->startCursor);
    for (auto& format : m_startTagFormats) {
      start.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
      start.setCharFormat(format);
      start.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
      start.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor);
    }
    m_startTagFormats.clear();
  }
  m_startTagMatch = startTagMatch;
  m_startTagMatchModified = true;
  rehighlight();
}

void
HtmlHighlighter::setBracketMatchEnd(int brktMatchEnd)
{
  m_brktMatchEnd = brktMatchEnd;
}

void
HtmlHighlighter::clearBracketMatch()
{
  m_brktMatchStart = -1;
  m_brktMatchEnd = -1;
}

void
HtmlHighlighter::setBracketMatchStart(int brktMatchStart)
{
  m_brktMatchStart = brktMatchStart;
}

void
HtmlHighlighter::setTextColor(const QColor& textColor)
{
  m_textColor = textColor;
  m_textFormat.setForeground(textColor);
  rehighlight();
}

void
HtmlHighlighter::setErrorColor(const QColor& error)
{
  m_errorColor = error;
  m_errorFormat.setForeground(m_errorColor);
  rehighlight();
}

void
HtmlHighlighter::setCommentColor(const QColor& comment)
{
  m_commentColor = comment;
  m_commentFormat.setForeground(m_commentColor);
  rehighlight();
}

void
HtmlHighlighter::setDQuoteColor(const QColor& dQuoteColor)
{
  m_dQuoteColor = dQuoteColor;
  m_dQuoteFormat.setForeground(m_dQuoteColor);
  rehighlight();
}

void
HtmlHighlighter::setSQuoteColor(const QColor& sQuoteColor)
{
  m_sQuoteColor = sQuoteColor;
  m_sQuoteFormat.setForeground(m_sQuoteColor);
  rehighlight();
}

void
HtmlHighlighter::setValueColor(const QColor& valueColor)
{
  m_valueColor = valueColor;
  m_valueFormat.setForeground(m_valueColor);
  rehighlight();
}

void
HtmlHighlighter::setAttrColor(const QColor& attrColor)
{
  m_attrColor = attrColor;
  m_attrFormat.setForeground(m_attrColor);
  rehighlight();
}

void
HtmlHighlighter::setNameColor(const QColor& nameColor)
{
  m_nameColor = nameColor;
  m_nameFormat.setForeground(m_nameColor);
  rehighlight();
}

void
HtmlHighlighter::setMatchColor(const QColor& matchColor,
                               const QColor& matchBackgound)
{
  m_matchColor = matchColor;
  m_matchBackgroundColor = matchBackgound;
  m_matchFormat.setForeground(m_matchColor);
  m_matchFormat.setBackground(m_matchBackgroundColor);
  rehighlight();
}

void
HtmlHighlighter::setBackground(const QColor& background)
{
  m_backgroundColor = background;
  m_textFormat.setBackground(m_backgroundColor);
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

const QColor&
HtmlHighlighter::tagMatchUnderlineColor() const
{
  return m_tagMatchUnderlineColor;
}

void
HtmlHighlighter::setTagMatchUnderlineColor(const QColor& tagMatchUnderlineColor)
{
  m_tagMatchUnderlineColor = tagMatchUnderlineColor;
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
  if (cstart == pend || cstart == pos) {
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
