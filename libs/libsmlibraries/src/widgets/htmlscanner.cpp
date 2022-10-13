#include "widgets/htmlscanner.h"
//#include "widgets/html.h"
#include "utilities/characters.h"
#include "widgets/htmltypes.h"

//====================================================================
//=== HtmlScanner
//====================================================================
HtmlScanner::HtmlScanner(QTextDocument* parent)
  : QObject(parent)
  //  , COUT(stdout)
  , m_document(parent)
  , m_htmlTypes(new HtmlTypes(this))
{
}

HtmlScanner::~HtmlScanner()
{
  qDeleteAll(m_tagList);
  m_tagList.clear();
  qDeleteAll(m_matchedTags);
  m_matchedTags.clear();
}

void
HtmlScanner::load(const QString& text)
{
  m_tagList = scan(text);
  // TODO remove this and call it from the editor.
  checkTagOrder();
}

void
HtmlScanner::load(QTextStream& sin)
{
  QString text = sin.readAll();
  scan(text);
}

Tag*
HtmlScanner::rescanTag(const QString text, int startPoint, int count)
{
  auto list = scan(text, startPoint, count);
  if (list.isEmpty())
    return nullptr;
  return list.at(0);
}

void
HtmlScanner::addCharToString(QString& s,
                             QChar c,
                             int& sPos,
                             int i,
                             TagState state)
{
  if (s.isEmpty()) {
    if (state == PreTag) {
      sPos = 0;
    } else {
      sPos = i;
    }
  }
  s += c;
}

void
HtmlScanner::storeTagData(NameTag* tag, NameTag* secondTag, ScannerError error)
{
  auto match = new TagMatch();
  match->first = tag;
  match->second = secondTag;
  if (error != NoError) {
    if (secondTag)
      match->second->errors.setFlag(error);
    else
      match->first->errors.setFlag(error);
  }
  //  m_orderedTags.append(tag);
  m_matchedTags.append(match);
}

void
HtmlScanner::checkTagOrder()
{
  //  QVector<QString> names;
  auto matchList = QVector<Tag*>(m_tagList);
  if (!m_matchedTags.isEmpty()) {
    qDeleteAll(m_matchedTags);
    m_matchedTags.clear();
  }
  // empty tag list
  if (matchList.isEmpty())
    return;

  //  bool isXml = matchList.at(0)->type == TagXml ? true : false;

  Tag* tag;
  while (!matchList.isEmpty() && (tag = matchList.takeFirst())) {
    auto tagCount = 1;
    if (tag) {
      // specific tag types that are not matchable.
      if (tag->type == TagXml || tag->type == TagDoctype ||
          tag->type == TagComment) {
        continue;
      }

      auto nameTag = dynamic_cast<NameTag*>(tag);
      if (nameTag) {
        auto name = nameTag->name;

        // a void tag (in html 5 a void tag does not need to be closed).
        // however it does in xhtml
        if (m_htmlTypes->isValidVoidTag(name)) {
          continue;
        }
        // these tags are internally closed <source .../>
        auto htmlTag = dynamic_cast<ClosedTag*>(tag);
        if (htmlTag && htmlTag->isClosed()) {
          continue;
        }

        // Shouldn't meet a CloserTag here unless there is a match problem
        // They should have been hoovered out before otherwise.
        auto closer = dynamic_cast<CloserTag*>(tag);
        if (closer && closer->type == TagCloser) {
          storeTagData(closer, nullptr, MissingStartTag);
          continue;
        }

        for (auto i = 0; i < matchList.size(); i++) {
          auto nextNameTag = dynamic_cast<NameTag*>(matchList.at(i));
          if (nextNameTag) {
            if (m_htmlTypes->isValidVoidTag(nextNameTag->name)) {
              continue;
            }
            if (nextNameTag->name == name) {
              if (nextNameTag->type == TagCloser) {
                if (tagCount == 1) {
                  storeTagData(nameTag, nextNameTag);
                  matchList.removeAt(i);
                  tagCount = 0;
                  break;
                } else {
                  tagCount--;
                  continue;
                }
              } else {
                tagCount++;
                continue;
              }
            }
          }
        } // END for j
        if (tagCount > 0) {
          // no matching end tag.
          storeTagData(nameTag, nullptr, MissingEndTag);
        }
      } // END if(nameTag)
    }   // END if(tag)
  }     // END for i
}

void
HtmlScanner::moveTagRight(Tag* tag)
{
  // TODO move tag right
}

void
HtmlScanner::moveTagLeft(Tag* tag)
{
  // TODO move tag left
}

void
HtmlScanner::moveTo(Tag*, int position)
{
  // TODO move selected tag to position.
}

const QVector<TagMatch*>&
HtmlScanner::matchedTags() const
{
  return m_matchedTags;
}

Tag*
HtmlScanner::matchedTag(Tag* tag)
{
  for (auto matches : m_matchedTags) {
    if (matches->first == tag) {
      return matches->second;
    }
    if (matches->second == tag) {
      return matches->first;
    }
  }
  return nullptr;
}

// QTextDocument *HtmlScanner::document() const
//{
//   return m_document;
// }

// void HtmlScanner::setDocument(QTextDocument *Document)
//{
//   m_document = Document;
// }

QTextCursor
HtmlScanner::createCursor(int position)
{
  auto cursor = QTextCursor(m_document);
  cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, position);
  return cursor;
}

XmlDeclaration*
HtmlScanner::createXmlDeclaration(QString& s,
                                  int& sPos,
                                  TagState& state,
                                  ExpectedStates& expected,
                                  bool& hasQuestion,
                                  int& questionPos,
                                  QChar& storedChar,
                                  QTextCursor startCursor)
{
  auto xmltag = new struct XmlDeclaration();
  xmltag->startChar = storedChar;
  xmltag->type = TagXml;
  xmltag->startCursor = startCursor;
  xmltag->name = s;
  xmltag->nameStartCursor = createCursor(sPos);

  // the xml declaration MUST start <?xml no spaces lower case
  // the xml declaration MUST be on the first line and '<' SHOULD
  // be the first character on the document
  // TODO might not need both hasQuestion and storedChar
  if (hasQuestion) {
    if (questionPos != xmltag->start() + 1)
      xmltag->errors.setFlag(XmlTagStartPositionError);
  } else {
    xmltag->errors.setFlag(XmlTagStartCharError);
    if (xmltag->start() != 0)
      xmltag->warnings.setFlag(XmlTagStartWarning);
  }

  if (s.toLower() != "xml")
    xmltag->errors.setFlag(XmlTagNameError);
  if (!(s.toLower() == s))
    xmltag->errors.setFlag(XmlTagNameCaseError);
  if (xmltag->nameStart() != xmltag->start() + 2)
    xmltag->errors.setFlag(XmlTagNamePositionError);

  sPos = -1;
  s.clear();
  state = NoState;
  expected = Attr;
  return xmltag;
}

void
HtmlScanner::checkXmlAttribute(Tag* tag,
                               Attribute* attribute,
                               QString& s,
                               int& sPos)
{
  auto xmltag = dynamic_cast<XmlDeclaration*>(tag);
  attribute->value = s;
  attribute->valueStartCursor = createCursor(sPos);
  if (attribute->name == "version") {
    if (!(attribute->value == "'1.0'" || attribute->value == "'1.1'")) {
      attribute->errors.setFlag(XmlTagWrongVersion);
    }
  }
  if (attribute->name == "standalone") {
    if (attribute->value == "yes") {
      attribute->errors.setFlag(BadAttributeValue);
    }
  }
  xmltag->attributes.append(attribute);
}

void
HtmlScanner::checkHtmlAttribute(Tag* tag,
                                Attribute* attribute,
                                QString& s,
                                int& sPos)
{
  // both Xml and Html are the same at this point.
  auto htmltag = dynamic_cast<HtmlTag*>(tag);
  attribute->valueStartCursor = createCursor(sPos);
  attribute->value = s;
  if (!m_htmlTypes->isValidAttributeValueForTag(
        htmltag->name, attribute->name, attribute->value)) {
    attribute->errors.setFlag(BadAttributeValue);
  }
  if (s.startsWith(Characters::QUOTATION) ||
      s.startsWith(Characters::SINGLEQUOTE)) {
    bool mismatched = (s.last(1) != s.first(1));
    if (mismatched) {
      attribute->errors.setFlag(ScannerError::MismatchedQuotes);
      attribute->hoverText = tr("Mismatched quotes");
    }
  }
  htmltag->attributes.append(attribute);
  sPos = -1;
  s.clear();
  attribute = nullptr;
}

void
HtmlScanner::handleQuotedAttribute(Tag* tag,
                                   Attribute* attribute,
                                   QString& s,
                                   int& sPos)
{
  if (tag) {
    if (tag->type == TagXml) {
      checkXmlAttribute(tag, attribute, s, sPos);
    } else if (tag->type == TagHtml) {
      checkHtmlAttribute(tag, attribute, s, sPos);
    } else if (tag->type == TagDoctype) {
      auto doctag = dynamic_cast<DocTypeTag*>(tag);
      doctag->data.append(s);
      doctag->dataPosCursors.append(createCursor(sPos));
    }
  }
}

void
HtmlScanner::checkAttributeNameErrors(Tag* tag,
                                      Attribute* attribute,
                                      QString& s)
{
  if (tag->type == TagHtml) { // not xml or doctype
    auto htmltag = dynamic_cast<HtmlTag*>(tag);
    if (htmltag && !isValidAttributeForTag(htmltag->name, attribute->name)) {
      attribute->errors.setFlag(BadAttributeName);
    }
  } else if (tag->type == TagXml) {
    if (!(s == "version" || s == "encoding" || s == "standalone")) {
      // only these tags allowed in xml declarations
      dynamic_cast<XmlDeclaration*>(tag)->errors.setFlag(XmlAttributeError);
    }
  }
}

QVector<Tag*>
HtmlScanner::scan(const QString& text, int startPoint, int n)
{
  auto tagList = QVector<Tag*>();
  Tag* tag = nullptr;
  Attribute* attribute = nullptr;
  auto endPoint = (n < 0 ? text.length() : startPoint + n);

  QString s;

  TagState state = PreTag;
  ExpectedStates expected = NoExpectation;
  // These variables are primarily used to test the xml and doctype
  // declarations.
  bool hasExplanation = false;
  bool hasQuestion = false;
  int questionPos = -1;
  QChar storedChar;
  //
  auto enderPos = -1; // TODO maybe remove. attribute created
  // already?
  auto sPos = -1;
  QTextCursor startCursor;

  for (auto i = startPoint; i < endPoint; i++) {
    auto c = text.at(i);

    if (c == Characters::FORWARDSLASH) { // / character
      if (tag) {
        if (tag->type != TagDoctype) {
          if (state == SingleQuotes || state == DoubleQuotes) {
            addCharToString(s, c, sPos, i, state);
            continue;
          } else {
            state = NoState;
            expected = Attr;

            enderPos = i;
            continue;
          }
        } else {
          addCharToString(s, c, sPos, i, state);
          enderPos = i;
        }
      } else {
        enderPos = i;
        continue;
      }
    }

    if (c == Characters::NEWLINE_LEFT_SYMBOL || text == Characters::TAB) {
      tag = new SpecialCharTag(c);
      tag->startCursor = createCursor(i);
      tag->endCursor = createCursor(i + 1);
      tagList.append(tag);
      tag = nullptr;
      continue;
    }

    if (c == Characters::LT) {
      if (s.isEmpty()) {
        // NOT TRIMMED THAT DROPS WHITESPACE ONLY
        s.clear();
        sPos = -1;
        startCursor = createCursor(i);
      } else {
        auto texttag = new TextTag();
        texttag->text = s;
        //        texttag->textStartCursor = createCursor(sPos);
        texttag->startCursor = createCursor(sPos);
        texttag->endCursor = createCursor(i);
        tagList.append(texttag);
        tag = nullptr;
        enderPos = -1;
        sPos = -1;
        s.clear();
        startCursor = createCursor(i);
        state = CloseTag;
        expected = Name;
        hasQuestion = false;
        hasExplanation = false;
        continue;
      }
      state = OpenTag;
      expected = Name;
      //      startCursor = createCursor(start);
      continue;
    } else if (c == Characters::NEWLINE || c == Characters::SPACE) {
      if ((state == CloseTag) &&
          (expected == NoExpectation || expected == TextBlock)) {
        // probably text
        s += c;
        if (sPos == -1 && expected != TextBlock) {
          sPos = i;
          expected = TextBlock;
        }
      } else if (expected == Value) {
        attribute->value = s;
        attribute->valueStartCursor = createCursor(sPos);
        sPos = -1;
        s.clear();
        dynamic_cast<HtmlTag*>(tag)->attributes.append(attribute);
        attribute = nullptr;
      } else if (expected == Name) {
        if (s.isEmpty()) {
          // probably caused by a space directly after the opening <
          continue;
        } else if (s == "xml") {
          // caused by missing ? in <?xml is <xml
          tag = createXmlDeclaration(s,
                                     sPos,
                                     state,
                                     expected,
                                     hasQuestion,
                                     questionPos,
                                     storedChar,
                                     startCursor);
        } else {
          auto htmltag = new HtmlTag();
          tag = htmltag;
          htmltag->name = s;
          htmltag->nameStartCursor = createCursor(sPos);
          if (!m_htmlTypes->isValidTag(htmltag->name)) {
            htmltag->errors.setFlag(TagNameError);
          }
          sPos = -1;
          s.clear();
          state = NoState;
          expected = Attr;
        }
      } else if ((state == NoState || state == SingleQuotes ||
                  state == DoubleQuotes) &&
                 (expected == Attr) && tag->type == TagDoctype) {
        if (!s.isEmpty()) {
          if (state == SingleQuotes || state == DoubleQuotes) {
            addCharToString(s, c, sPos, i);
          } else {
            auto doctag = dynamic_cast<DocTypeTag*>(tag);
            if (doctag) {
              doctag->data.append(s);
              doctag->dataPosCursors.append(createCursor(sPos));
              if (c == Characters::NEWLINE)
                doctag->newLines.append(i);
              sPos = -1;
              s.clear();
            }
            continue;
          }
        } else {
          auto doctag = dynamic_cast<DocTypeTag*>(tag);
          if (doctag) {
            if (c == Characters::NEWLINE)
              doctag->newLines.append(i);
          }
        }
        // state & expected don't change.
      } else if (state == Comment || state == SingleQuotes ||
                 state == DoubleQuotes) {
        addCharToString(s, c, sPos, i, state);
      } else if (state == PreTag) {
        s += c;
      } else if (expected.testFlag(TextBlock)) {
        if (c == Characters::NEWLINE) {
          if (state == NoState) {
            // newlines after tag closure.
            s += c;
          } else {
            if (tag) {
              // newlines within tag
              auto nametag = dynamic_cast<NameTag*>(tag);
              if (nametag)
                nametag->newLines.append(i);
            }
          }
        }
      }
      continue;
    } else if (c == Characters::ASSIGNMENT) {
      if (state == NoState && expected.testFlag(Attr)) {
        expected = Value | SingleQuote | DoubleQuote;
        if (!attribute)
          attribute = new Attribute();
        attribute->name = s;
        attribute->nameStartCursor = createCursor(sPos);
        attribute->assignPos = i;
        checkAttributeNameErrors(tag, attribute, s);
        sPos = -1;
        s.clear();
      }
    } else if (c == Characters::QUESTIONMARK) { // Before/ after ?xml tag
      hasQuestion = true;
      questionPos = i;
      storedChar = c;
      if (expected == Attr) { // After <?xml ... ?> char
        dynamic_cast<struct XmlDeclaration*>(tag)->endChar = c;
      }
      continue;
    } else if (c == Characters::EXCLAMATIONMARK) { // Before ?xml tag
      hasExplanation = true;
      continue;
    } else if (c == Characters::GT) { // End of tag
      if (expected == Value) {
        attribute->value = s;
        attribute->valueStartCursor = createCursor(sPos);
        //          sPos = -1;
        s.clear();
        dynamic_cast<HtmlTag*>(tag)->attributes.append(attribute);
        attribute = nullptr;
      } else if (expected == Attr) {
        if (enderPos > -1) {
          if (tag && (tag->type == TagHtml || tag->type == TagXml)) {
            auto htmltag = dynamic_cast<HtmlTag*>(tag);
            htmltag->closerStartCursor = createCursor(enderPos);
          }
        } else {
          if (tag && tag->type == TagHtml) {
            auto htmltag = dynamic_cast<HtmlTag*>(tag);
            if (!s.isEmpty()) {
              attribute = new Attribute();
              attribute->name = s;
              attribute->nameStartCursor = createCursor(sPos);
              if (!m_htmlTypes->isValidAttributeForTag(htmltag->name,
                                                       attribute->name)) {
                attribute->errors.setFlag(BadAttributeName);
              }
              htmltag->attributes.append(attribute);
              attribute = nullptr;
            }
          }
        }
      } else if (expected == Name) {
        if (enderPos > -1) {
          auto closerTag = new CloserTag();
          tag = closerTag;
          closerTag->name = s;
          closerTag->nameStartCursor = createCursor(sPos);
          if (!m_htmlTypes->isValidTag(closerTag->name)) {
            closerTag->errors.setFlag(TagNameError);
          }
        } else {
          auto htmltag = new HtmlTag();
          tag = htmltag;
          if (enderPos > -1) {
            htmltag->closerStartCursor = createCursor(enderPos);
            s = s.mid(0, s.length() - 1);
          }
          htmltag->name = s;
          htmltag->nameStartCursor = createCursor(sPos);
          if (!m_htmlTypes->isValidTag(s)) {
            htmltag->errors.setFlag(TagNameError);
          }
        }
        //          sPos = -1;
        s.clear();
        expected = NoExpectation;
      } else if (expected == EndComment) {
        if (tag) {
          auto commentTag = dynamic_cast<CommentTag*>(tag);
          commentTag->text = s.mid(0, s.length() - 2);
          commentTag->textStartCursor = createCursor(i - s.length());
        }
      } else {
        if (tag) {
          if (tag && (tag->type == TagHtml || tag->type == TagXml) &&
              attribute) {
            auto htmltag = dynamic_cast<HtmlTag*>(tag);
            if (htmltag) {
              htmltag->attributes.append(attribute);
              attribute = nullptr;
            }
          }
        }
      }
      if (tag) {
        tag->startCursor = startCursor;
        tag->endCursor = createCursor(i + 1);
      }
      tagList.append(tag);
      tag = nullptr;
      enderPos = -1;
      sPos = -1;
      s.clear();
      state = CloseTag;
      expected = NoExpectation;
      hasQuestion = false;
      hasExplanation = false;
    } else if (c == Characters::SINGLEQUOTE ||
               c == Characters::QUOTATION) { // Single quote
      if (s.isEmpty()) {
        addCharToString(s, c, sPos, i, state);
        if (state == NoState) {
          if (c == Characters::SINGLEQUOTE)
            state = SingleQuotes;
          else if (c == Characters::QUOTATION)
            state = DoubleQuotes;
        }
      } else {
        addCharToString(s, c, sPos, i, state);
        handleQuotedAttribute(tag, attribute, s, sPos);
        attribute = nullptr;
        state = NoState;
        sPos = -1;
        s.clear();
        expected = Attr;
      }
    } else {
      addCharToString(s, c, sPos, i, state);
      if (expected == Name) { // Special tag types handled here
        if (hasExplanation && s == "DOCTYPE") {
          hasExplanation = false;
          auto doctag = new DocTypeTag();
          tag = doctag;
          doctag->type = TagDoctype;
          doctag->startCursor = startCursor;
          doctag->name = s;
          doctag->nameStartCursor = createCursor(sPos);
          state = NoState;
          expected = Attr;
          sPos = -1;
          s.clear();
        } else if (hasQuestion && s.toLower() == "xml") {
          tag = createXmlDeclaration(s,
                                     sPos,
                                     state,
                                     expected,
                                     hasQuestion,
                                     questionPos,
                                     storedChar,
                                     startCursor);
        } else if (hasExplanation && s == "--") { // start comment
          hasExplanation = false;
          auto commenttag = new CommentTag();
          tag = commenttag;
          tag->type = TagComment;
          s.clear();
          state = Comment;
          expected = EndComment;
        } else if (s.endsWith("--")) {
          expected = EndComment;
        }
      }
    }
  }
  if (!s.isEmpty()) {
    auto texttag = new TextTag();
    texttag->text = s;
    texttag->startCursor = createCursor(sPos);
    texttag->endCursor = createCursor(text.length());
    tagList.append(texttag);
    tag = nullptr;
  }

  return tagList;
}

QVector<Tag*>
HtmlScanner::tagList()
{
  return m_tagList;
}

void
HtmlScanner::setTag(int index, Tag* tag)
{
  m_tagList.replace(index, tag);
}

void
HtmlScanner::insertTag(int index, Tag* tag)
{
  m_tagList.insert(index, tag);
}

void
HtmlScanner::insertAfter(Tag* tag, Tag* insertTag)
{
  if (m_tagList.contains(tag)) {
    auto index = indexOfTag(tag);
    m_tagList.insert(index + 1, insertTag);
  } else {

  }
}

void
HtmlScanner::replaceTag(Tag* firstTag, Tag* secondTag)
{
  int index = m_tagList.indexOf(firstTag);
  m_tagList.replace(index, secondTag);
}

int
HtmlScanner::indexOfTag(Tag* tag)
{
  return m_tagList.indexOf(tag);
}

// void
// HtmlScanner::swapTags(int firstIndex, int secondIndex)
//{
//   // TODO
// }

// void
// HtmlScanner::swapTags(Tag* firstTag, Tag* secondTag)
//{
//   // TODO
// }

Tag*
HtmlScanner::tagAt(int position)
{
  for (auto tag : m_tagList) {
    if (position >= tag->start() && position < tag->end()) {
      return tag;
    }
  }
  return nullptr;
}

bool
HtmlScanner::isValidTag(Tag* tag)
{
  return m_htmlTypes->isValidTag(tag);
}

bool
HtmlScanner::isValidTagName(const QString& name)
{
  return m_htmlTypes->isValidTag(name);
}

bool
HtmlScanner::isValidAttributeForTag(const QString& name,
                                    const QString& attribute)
{
  return m_htmlTypes->isValidAttributeForTag(name, attribute);
}

bool
HtmlScanner::isValidAttributeValueForTag(const QString& name,
                                         const QString& attribute,
                                         const QString& value)
{
  return m_htmlTypes->isValidAttributeValueForTag(name, attribute, value);
}

void
HtmlScanner::setLanguages(BCP47Languages* languages)
{
  m_htmlTypes->setLanguages(languages);
}

//====================================================================
//=== Tag
//====================================================================
IsInType
Tag::isIn(int cursorPos)
{
  if (cursorPos >= start() && cursorPos < end()) {
    return IsInTag;
  }
  return NotIn;
}

int
Tag::start()
{
  return startCursor.position();
}

int
Tag::end()
{
  return endCursor.position();
}

int
Tag::length()
{
  return end() - start();
}

void
Tag::copyTagInto(Tag* tag)
{
  if (tag) {
    type = tag->type;
    selected = tag->selected;
    startCursor = tag->startCursor;
    endCursor = tag->endCursor;
    hoverText = tag->hoverText;
  }
}

//====================================================================
//=== Attribute
//====================================================================
IsInType
Attribute::isIn(int cursorPos)
{
  if (cursorPos >= nameStart() && cursorPos < nameStart() + name.length()) {
    return IsInAttributeName;
  } else if (cursorPos >= valueStart() &&
             cursorPos < valueStart() + value.length()) {
    return IsInAttrubuteValue;
  }
  return NotIn;
}

int
Attribute::nameStart()
{
  return nameStartCursor.position();
}

int
Attribute::valueStart()
{
  return valueStartCursor.position();
}

bool
Attribute::hasValue()
{
  if (value.isEmpty())
    return false;
  return true;
}

QString
Attribute::toString()
{
  QString s = name;
  if (!value.isEmpty()) {
    s += "=";
    s += value;
  }
  return s;
}

//====================================================================
//=== NameTag
//====================================================================
int
NameTag::nameStart()
{
  return nameStartCursor.position();
}

int
NameTag::nameLength()
{
  return name.length();
}

IsInType
NameTag::isIn(int cursorPos)
{
  if (Tag::isIn(cursorPos) == IsInTag) {
    if (cursorPos >= nameStart() && cursorPos < nameStart() + name.length()) {
      return IsInName;
    }
    return IsInTag;
  }
  return NotIn;
}

void
NameTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<NameTag*>(tag);
  if (htmltag) {
    ErrorTag::copyTagInto(tag);
    name = htmltag->name;
    nameStartCursor = htmltag->nameStartCursor;
  }
}

//====================================================================
//=== ClosedTag
//====================================================================
bool
ClosedTag::isClosed()
{
  return !closerStartCursor.isNull();
}

int
ClosedTag::closerStart()
{
  return closerStartCursor.position();
}

void
ClosedTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<ClosedTag*>(tag);
  if (htmltag) {
    NameTag::copyTagInto(tag);
    closerStartCursor = htmltag->closerStartCursor;
  }
}

//====================================================================
//=== CommentTag
//====================================================================
CommentTag::CommentTag()
  : ErrorTag()
{
  type = TagComment;
}

QString
CommentTag::toString()
{
  QString s = "<!-- ";
  s += text;
  s += " -->";
  return s;
}

int
CommentTag::textStart()
{
  return textStartCursor.position();
}

IsInType
CommentTag::isIn(int cursorPos)
{
  if (Tag::isIn(cursorPos) == IsInTag) {
    if (cursorPos >= textStart() && cursorPos < textStart() + text.length()) {
      return IsInComment;
    }
    return IsInTag;
  }
  return NotIn;
}

void
CommentTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<CommentTag*>(tag);
  if (htmltag) {
    ErrorTag::copyTagInto(tag);
    text = htmltag->text;
    textStartCursor = htmltag->textStartCursor;
  }
}

//====================================================================
//=== CloserTag
//====================================================================
CloserTag::CloserTag()
  : NameTag()
{
  type = TagCloser;
}

QString
CloserTag::toString()
{
  QString s = "</";
  for (auto i = start() + 2; i < end() - 1; i++) {
    if (i < s.length())
      continue;
    if (i == nameStart()) {
      s += name;
      continue;
    }
    if (newLines.contains(i)) {
      s += Characters::NEWLINE;
      continue;
    }
    s += Characters::SPACE; // shouldn't happen in this tag type.
  }
  s += ">";
  return s;
}

//====================================================================
//=== DocTypeTag
//====================================================================
DocTypeTag::DocTypeTag()
  : ClosedTag()
{
  type = TagDoctype;
}

QString
DocTypeTag::toString()
{
  QString s = "<!";
  for (auto i = start() + 2; i < end() - 1; i++) {
    if (i < s.length())
      continue;
    if (i == nameStart()) {
      s += name;
      continue;
    }
    if (newLines.contains(i)) {
      s += Characters::NEWLINE;
      continue;
    }
    for (auto& c : dataPosCursors) {
      if (i == c.position()) {
        s += data.at(dataPosCursors.indexOf(c));
        continue;
      }
    }
    s.append(Characters::SPACE);
  }
  s += " >";
  return s;
}

QString
DocTypeTag::dataAt(int index)
{
  return data.at(index);
}

int
DocTypeTag::dataPosition(int index)
{
  if (index >= 0 && index < dataPosCursors.size())
    return dataPosCursors.at(index).position();
  return -1;
}

IsInType
DocTypeTag::isIn(int cursorPos)
{
  auto result = NameTag::isIn(cursorPos);
  if (result == IsInTag) {
    for (auto i = 0; i < data.size(); i++) {
      auto d = data.at(i);
      auto p = dataPosCursors.at(i).position();
      if (cursorPos >= p && cursorPos < p + d.length()) {
        return IsInDocTypeData;
      }
    }
    return IsInTag;
  } else if (result == IsInName) {
    return result;
  }
  return NotIn;
}

void
DocTypeTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<DocTypeTag*>(tag);
  if (htmltag) {
    ClosedTag::copyTagInto(tag);
    data = htmltag->data;
    dataPosCursors = htmltag->dataPosCursors;
  }
}

//====================================================================
//=== HtmlTag
//====================================================================
HtmlTag::HtmlTag()
  : ClosedTag()
{
  type = TagHtml;
}

HtmlTag::~HtmlTag()
{
  qDeleteAll(attributes);
  attributes.clear();
}

QString
HtmlTag::toString()
{
  QString s = "<";
  for (auto i = start() + 2; i < end() - 1; i++) {
    if (i < s.length())
      continue;
    if (i == nameStart()) {
      s += name;
      continue;
    }
    if (newLines.contains(i)) {
      s += Characters::NEWLINE;
      continue;
    }
    for (auto a : attributes) {
      if (i == a->nameStart()) {
        s += a->name;
        continue;
      }
      if (!a->value.isEmpty() && i == a->valueStart()) {
        s += a->value;
        continue;
      }
      if (a->assignPos >= 0 && i == a->assignPos)
        s += Characters::ASSIGNMENT;
    }
    s.append(Characters::SPACE);
  }
  s += ">";
  return s;
}

IsInType
HtmlTag::isIn(int cursorPos)
{
  auto result = NameTag::isIn(cursorPos);
  if (result == IsInTag) {
    for (auto i = 0; i < attributes.size(); i++) {
      auto d = attributes.at(i);
      auto attResult = d->isIn(cursorPos);
      if (attResult == IsInAttributeName || attResult == IsInAttrubuteValue) {
        attributeIndex = i;
        return result;
      }
    }
    attributeIndex = -1;
    return result;
  } else if (result == IsInName) {
    return result;
  }
  return NotIn;
}

void
HtmlTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<HtmlTag*>(tag);
  if (htmltag) {
    attributeIndex = htmltag->attributeIndex;
    attributes = htmltag->attributes;
  }
}

//====================================================================
//=== XmlTag
//====================================================================
QString
XmlDeclaration::toString()
{
  QString s = "<";
  s += startChar;
  s += name;
  for (auto& a : attributes) {
    s += " ";
    s += a->toString();
  }
  s += endChar;
  s += ">";
  return s;
}

//====================================================================
//=== TextTag
//====================================================================
QString
TextTag::toString()
{
  return text;
}

int
TextTag::textLength()
{
  return text.length();
}

IsInType
TextTag::isIn(int cursorPos)
{
  if (Tag::isIn(cursorPos) == IsInTag) {
    if (cursorPos >= start() && cursorPos < start() + text.length()) {
      return IsInText;
    }
    return IsInTag;
  }
  return NotIn;
}

void
TextTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<TextTag*>(tag);
  if (htmltag) {
    Tag::copyTagInto(tag);
    text = htmltag->text;
  }
}

void
ErrorTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<ErrorTag*>(tag);
  if (htmltag) {
    Tag::copyTagInto(tag);
    errors = htmltag->errors;
  }
}

void
SpecialCharTag::copyTagInto(Tag* tag)
{
  auto htmltag = dynamic_cast<SpecialCharTag*>(tag);
  if (htmltag) {
    Tag::copyTagInto(tag);
    character = htmltag->character;
  }
}
