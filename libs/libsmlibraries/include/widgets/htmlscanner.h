#ifndef HTMLSCANNER_H
#define HTMLSCANNER_H

#include <QObject>
#include <QRegularExpression>
#include <QStringView>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>
#include <QVector>

// enum HtmlTypes
//{
//   NOTYPE,
//   NOVALUES,
//   TEXT,
//   COORDINATES,
//   FILENAME,
//   URL,
//   LANGUAGE_CODE,
//   MEDIA_QUERY,
//   MEDIA_TYPE,
//   AUTOPLAY,
//   CONTROLS,
//   LOOP,
//   MUTED,
//   FORM_ID,
// };

/*!
 * \enum  IsInType
 *
 * A value returned by the Tag::isIn method.
 */
enum IsInType
{
  NotIn,              //!< Not in the tag
  IsInTag,            //!< Is in the tag
  IsInAttributeName,  //!< Is in the attribute name
  IsInAttrubuteValue, //!< Is in the attribute value
  IsInName,           //!< is in the tag name
  IsInDocTypeData,    //!< Is in the DocType data
  IsInText,           //!<  Is in the text block
  IsInComment,        //!< Is in the comment
};

/*!
 * \enum TagType
 *
 * Defines the type of the Tag
 */
enum TagType
{
  NoType = -1,  //!< No tag type
  TagCharacter, //!< Special characters.
  TagText,      //!< text block
  //  TagWhitespace, //!< A special case of TagText that only contains
  //  whitespace.
  TagComment, //!< <!== ==>
  TagHtml,    //!< standard html tag
  TagXml,     //!< <?xml
  TagDoctype, //!< <!DOCTYPE
  TagCloser,  //!< </html>
};

/*!
 * \internal
 * \enum TagState
 *
 * Used internally whilst parsing the html file
 */
enum TagState
{
  PreTag = 0,
  OpenTag,
  DoubleQuotes, // inside double quote
  SingleQuotes, // inside single quotes
  Text,
  Comment,
  NoState,
  CloseTag,
};

/*!
 * \internal
 * \enum ExpectedState
 *
 * Used internally whilst parsing the html file
 */
enum ExpectedState
{
  NoExpectation = 0, // Don't have any expectations
  DocType = 0x1,

  DoubleQuote = 0x10,    // expect a double quote
  SingleQuote = 0x20,    // expect a single quote
  EndDoubleQuote = 0x40, // expect to end a double quote
  EndSingleQuote = 0x80, // expect to end a single quote

  Name = 0x1000,  // a tag name
  Attr = 0x2000,  // a tag attribute
  Value = 0x4000, // an attribute value

  TextBlock = 0x10000,

  EndComment = 0x100000,
};
Q_DECLARE_FLAGS(ExpectedStates, ExpectedState)

/*!
 * \enum ScannerError
 *
 * Flags used to indicate faults in the parsed html code.
 */
enum ScannerError
{
  NoError = 0,

  TagNameError = 0x1,

  MissingEndTag = 0x100,   //!< missing end tag e.g. <html> but no </html>
  MissingStartTag = 0x200, //!< missing start tag e.g. </html> but no <html>
  MisOrderedTags = 0x400,  //!< tag order wrong e.g. <b><i>text</b></i>

  MismatchedQuotes = 0x1000,

  BadAttributeName = 0x10000,
  BadAttributeValue = 0x20000,

  //  xml declaration errors
  XmlTagNameCaseError = 0x100000,       //!< xml MUST be lower case
  XmlTagNameError = 0x200000,           //!< xml MUST be lower case
  XmlTagNamePositionError = 0x400000,   //!, 'xml' not directly after ?
  XmlTagStartCharError = 0x800000,      //!< no ? after <
  XmlTagStartPositionError = 0x1000000, //!< ? not directly after <
  XmlAttributeError = 0x2000000,
  XmlTagEndError = 0x4000000,
  XmlTagEndPositionError = 0x8000000,
  XmlTagWrongVersion = 0x10000000,

};
Q_DECLARE_FLAGS(ScannerErrors, ScannerError)

/*!
 * \enum ScannerWarning
 *
 * Flags used to indicate items that are technically correct
 * but not preferred in the parsed html code.
 */
enum ScannerWarning
{
  NoWarning = 0,

  XmlTagStartWarning =
    0x1, //!< xml tag SHOULD start at the first character in the document
};
Q_DECLARE_FLAGS(ScannerWarnings, ScannerWarning)

/*!
 * \enum ScannerInfo
 *
 * Flags used to indicate items that are technically correct
 * but not preferred in the parsed html code.
 */
enum ScannerInfo
{
  NoInfo = 0,

  DocAttributeNotReqd =
    0x1, //!< DOCTYPE is recommended as just <!DOCTYPE html> for HTML 5
};
Q_DECLARE_FLAGS(ScannerInfos, ScannerInfo)

/*!
 * \struct Attribute
 * \brief Stores data about a parsed html attribute
 */
struct Attribute
{
  //! The tag name
  QString name;
  //! The start position of the tag
  QTextCursor nameStartCursor;
  //! The attrivute value
  QString value;
  //! The end position of the tag
  QTextCursor valueStartCursor;
  //! Attribute parsing errors.
  ScannerErrors errors = NoError;
  //! text to display when hovering
  QString hoverText;
  //! allows for gaps between name and assignment
  int assignPos = -1;

  /*!
   * \brief isIn method of the attribute.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos);

  /*!
   * \brief Returns the attribute name start position.
   */
  int nameStart();
  /*!
   * \brief Returns the attribute value start position.
   */
  int valueStart();
  /*!
   * \brief Tests whether the attribute has a value. Some html 5 tags
   * have no values.
   */
  bool hasValue();

  /*!
   * \brief Creates a string version of the attribute.
   */
  QString toString();
};

/*!
 * \struct Tag
 * \brief Base struct of all Tag  types
 */
struct Tag
{
  Tag() {}
  virtual ~Tag() {}

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  virtual IsInType isIn(int cursorPos);

  /*!
   * \brief Returns the tag start position.
   */
  int start();
  /*!
   * \brief Returns the tag end position.
   */
  int end();
  /*!
   * \brief Returns the tag length.
   */
  int length();

  /*!
   * \brief Creates a string version of the tag.
   *
   * This is an empty virtual method in Tag.
   */
  virtual QString toString() = 0;

  //! Copies values from the supplied tag into this tag.
  virtual void copyTagInto(Tag* tag);

  //! The Tag type.
  TagType type = NoType;
  //! Indicates that the tag has been selected.
  bool selected = false;
  //! The QTextCursor at the start position of the tag.
  QTextCursor startCursor;
  //! The QTextCursor at the end position of the tag.
  QTextCursor endCursor;
  //! text to display when hovering.
  QString hoverText;
};

/*!
 * \struct SpecialCharTag
 * \brief Handles special characters such as left newline character.
 */
struct SpecialCharTag : Tag
{
  SpecialCharTag(QChar c)
    : character(c)
  {
    type = TagCharacter;
  }

  QString toString() override { return QString(character); }

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;

  QChar character;
};

/*!
 * \struct TextTag htmlscanner.h
 * \brief A text block. Not created directly.
 */
struct TextTag : Tag
{
  TextTag()
    : Tag()
  {
    type = TagText;
  }

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;

  /*!
   * \brief Returns the length of the text string.
   *
   * Equivalent of calling text().length().
   */
  int textLength();

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos) override;

  //! Returns true if the string only contains whitespace characters.
  //!
  //! This includes  '\t', '\n', '\v', '\f' and '\r' characters as well as space
  //! characters.
  bool isWhitespace() { return text.trimmed().isEmpty(); }

  /*!
   * \brief the text string
   */
  QString text;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

/*!
 * \struct ErrorTag htmlscanner.h
 * \brief Stores errors. Not created directly.
 */
struct ErrorTag : Tag
{
  //! Stores tag parsing errors.
  ScannerErrors errors = NoError;
  ScannerWarnings warnings = NoWarning;
  //  bool matching = false;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

/*!
 * \struct NameTag htmlscanner.h
 * \brief Stores tag name. Not created directly.
 */
struct NameTag : ErrorTag
{
  NameTag()
    : ErrorTag()
  {
  }

  /*!
   * \brief Returns the tag name start position.
   */
  int nameStart();
  /*!
   * \brief Returns the length of the text.
   */
  int nameLength();

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos) override;

  //! The tag name.
  QString name;
  //! The QTextCursor at the start position of the tag name.
  QTextCursor nameStartCursor;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;

  //! Stores newlines inside tags.
  QList<int> newLines;
};

/*!
 * \struct ClosedTag htmlscanner.h
 * \brief Stores data for a closer tag for example \code </html> \endcode.
 */
struct ClosedTag : NameTag
{
  //! The Tag is closed.
  //!
  //! This flag will not require a matching closure tag. Examples would be
  //! <link .. /> or <meta .. /> which have attributes but no text so can be
  //! closed directly.
  //! \note Most of these need not be explicitely closed in HTML 5 files but
  //!       do in XHTML files.
  //!
  bool isClosed();

  /*!
   * \brief Returns the closer character position.
   */
  int closerStart();

  //! The QTextCursor at the start position of the tag closer character.
  QTextCursor closerStartCursor;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

//! Stores two tags. Used when matching two tags.
struct TagMatch
{
  NameTag* first;  //!< First member in a tag match
  NameTag* second; //!< Second member in a tag match or nullptr if no match
};

/*!
 * \struct CommentTag htmlscanner.h
 * \brief Used to store html comments
 */
struct CommentTag : ErrorTag
{
  CommentTag();

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;

  /*!
   * \brief Returns the text start position.
   */
  int textStart();

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos) override;

  //! The tag text.
  QString text;
  //! The QTextCursor at the start position of the tag text.
  QTextCursor textStartCursor;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

/*!
 * \struct CloserTag htmlscanner.h
 * \brief Stores closer tags. These are tags that start with  a backslash.
 */
struct CloserTag : NameTag
{
  CloserTag();

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;
};

/*!
 * \struct DocTypeTag htmlscanner.h
 * \brief Stores DOCTYPE tags.
 */
struct DocTypeTag : ClosedTag
{
  DocTypeTag();

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;

  /*!
   * \brief Returns the data string at index.
   */
  QString dataAt(int index);
  /*!
   * \brief Returns the data position within the tag at index.
   */
  int dataPosition(int index);

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos) override;

  //! The doctype data string.
  QVector<QString> data;
  //! The QTextCursor at the start position of each doctype data string.
  QVector<QTextCursor> dataPosCursors;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

/*!
 * \struct HtmlTag htmlscanner.h
 * \brief Stores tags. This is the most common type.
 */
struct HtmlTag : ClosedTag
{
  HtmlTag();
  ~HtmlTag();

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;

  /*!
   * \brief isIn method of all Tag types.
   *
   * Returns the result of a test whether the cursor is
   * within the tag. Returns one of the IsInType values.
   */
  IsInType isIn(int cursorPos) override;

  /*!
   * \brief tests whther the HtmlTag has in attributeIndex value set.
   */
  bool hasAttributeIndex() { return (attributeIndex >= 0); }

  //! The index of the attribute that has been detected by the IsIn method.
  int attributeIndex = -1;
  //! List of attrubutes.
  QVector<Attribute*> attributes;

  //! Copies values from the supplied tag into this tag.
  void copyTagInto(Tag* tag) override;
};

/*!
 * \struct XmlTag htmlscanner.h
 * \brief Stores the xml tag at start of the file.
 */
struct XmlDeclaration : HtmlTag
{
  XmlDeclaration()
    : HtmlTag()
  {
    type = TagXml;
  }

  /*!
   * \brief Creates a string version of the tag.
   *
   * Override to create a string version of derived tags.
   */
  QString toString() override;

  /*!
   * \brief tests whether the XmlTag has a starter character set.
   */
  bool hasStarter() { return (!startChar.isNull()); }
  /*!
   * \brief tests whether the XmlTag has an ender character set.
   */
  bool hasEnder() { return (!endChar.isNull()); }

  //! start character (normally ?)
  QChar startChar;
  //! end character (normally ?)
  QChar endChar;
};

class HtmlTypes;
class BCP47Languages;

/*!
 * \ingroup widgets
 * \class HtmlScanner htmlscanner.h "includes/widgets/htmlscanner.h"
 * \brief Scanner/Parser for html code.
 *
 * Walks throughan html page, detecting and marking html tags and detecting
 * incorrect code, such as misspelled tag and attribute names.
 *
 * \sa HtmlHighlighter
 */

class HtmlScanner : public QObject
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor for HtmlScanner.
   */
  explicit HtmlScanner(QTextDocument* parent = nullptr);
  ~HtmlScanner();

  /*!
   * \brief Scans an entire string of HTML 5 text.
   */
  void load(const QString& text);

  /*!
   * \brief Scans an entire stream of HTML 5 text.
   */
  void load(QTextStream& sin);

  /*!
   * \brief Scans part of an HTML 5 page starting at start for count characters.
   */
  Tag* rescanTag(const QString text, int startPoint, int count);

  /*!
   * \brief Returns the HTML page as a list of Tag pointers.
   *
   * An empty QVector<QString> unless scan has previously been called.
   */
  QVector<Tag*> tagList();

  /*!
   *  \brief Moves the Tag one tag right.
   *
   *  If the tag is the final tag then nothing is done.
   */
  void moveTagRight(Tag* tag);

  /*!
   *  \brief Moves the Tag one tag left.
   *
   *  If the tag is the first tag then nothing is done.
   */
  void moveTagLeft(Tag* tag);

  /*!
   * \brief Moves Tag to selected position.
   */
  void moveTo(Tag*, int position);

  /*!

   *  \brief Sets the Tag at index to the new Tag.
   */
  void setTag(int index, Tag* tag);

  /*!

   *  \brief Inserts the Tag at index.
   */
  void insertTag(int index, Tag* tag);

  /*!
   *  \brief Replaces the Tag at index to the new Tag.
   */
  void replaceTag(Tag* firstTag, Tag* secondTag);

//  /*!
//   *  \brief Swaps the two Tag's at index to the new Tag.
//   */
//  void swapTags(int firstIndex, int secondIndex);

//  /*!
//   *  \brief Swaps the two Tag's at index to the new Tag.
//   */
//  void swapTags(Tag* fisrtTag, Tag* secondTag);

  /*!
   * \brief Recovers the tag at index in the Tag list.
   */
  Tag* tagAt(int position);

  // TODO remove this. Only used during debugging
  //  QTextStream COUT;

  /*!
   * \brief Returns true if the Tag is a valid tag.
   */
  bool isValidTag(Tag* tag);

  /*!
   * \brief Returns true if the Tag name is a valid tag, otherwise returns
   * false.
   */
  bool isValidTagName(const QString& name);

  /*!
   * \brief Returns true if the attribute string is valid for the supplied tag
   * name, otherwise returns false.
   */
  bool isValidAttributeForTag(const QString& name, const QString& attribute);

  /*!
   * \brief Returns true if the attribute value string is valid for the supplied
   * tag name and attribute name, otherwise returns false.
   */
  bool isValidAttributeValueForTag(const QString& name,
                                   const QString& attribute,
                                   const QString& value);

  /*!
   * \brief Sets the language list to allow a language tag to be tested for
   * validity.
   *
   * If this is not set then language tag testing is not done.
   */
  void setLanguages(BCP47Languages* languages);

  /*!
   * \brief Returns the list of matching tags.
   *
   * When the HTML page is scanned it produces a list of matching Tag's. This
   * list will only match to the next closing tag of the same type,
   */
  const QVector<TagMatch*>& matchedTags() const;

  //  QTextDocument *document() const;
  //  void setDocument(QTextDocument *Document);

signals:

protected:
  Q_FLAG(ExpectedStates)

private:
  QTextDocument* m_document = nullptr;
  ScannerErrors errors;
  QVector<Tag*> m_tagList;
  QVector<TagMatch*> m_matchedTags;
  HtmlTypes* m_htmlTypes = nullptr;

  //  static QMap<QString, QMap<QString, QVector<QString>>> initTagTypeMap();

  QVector<Tag*> scan(const QString& text, int startPoint = 0, int n = -1);
  void addCharToString(QString& s,
                       QChar c,
                       int& sPos,
                       int i,
                       TagState state = NoState);
  void storeTagData(NameTag* tag,
                    NameTag* secondTag = nullptr,
                    ScannerError error = NoError);
  void checkTagOrder();
  QTextCursor createCursor(int position);
  XmlDeclaration* createXmlDeclaration(QString& s,
                                       int& sPos,
                                       TagState& state,
                                       ExpectedStates& expected,
                                       bool& hasQuestion,
                                       int& questionPos,
                                       QChar& storedChar,
                                       QTextCursor startCursor = QTextCursor());
  void checkXmlAttribute(Tag* tag,
                           Attribute* attribute,
                           QString& s,
                           int& sPos);
  void checkHtmlAttribute(Tag* tag,
                    Attribute* attribute,
                    QString& s,
                    int& sPos);
  void handleQuotedAttribute(Tag *tag,
                             Attribute* attribute,
                             QString& s,
                             int& sPos);
  void checkAttributeNameErrors(Tag *tag, Attribute *attribute, QString &s);
}; // end of HtmlScanner
Q_DECLARE_OPERATORS_FOR_FLAGS(ExpectedStates)

#endif // HTMLSCANNER_H
