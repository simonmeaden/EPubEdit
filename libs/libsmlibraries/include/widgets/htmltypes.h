#ifndef HTMLTYPES_H
#define HTMLTYPES_H

#include <QObject>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QString>
//#include <QRegularExpression>
//#include <QRegularExpressionMatch>

class Tag;
class BCP47Languages;

//class HtmlEntities {
//public:
//  static const QString AMPERSAND;
//};

//const QString HtmlEntities = "&amp;";

/*!
 * \ingroup widgets
 * \class HtmlTypes htmltypes.h "include/widgets/htmltypes.h"
 * \brief A utility class to test for correct html tag names and attribute names.
 *
 * Use isValidTag(Tag*) or isValidTag(const QString&) to test if the tag name is good.
 *
 * Use isValidVoidTag(Tag*) or isValidVoidTag(const QString&) to test if the tag name
 * is a void tag name good. Void tags are those that are complete in and of themselves
 * for example <meta ...>. d tags MUST be closed with ../> if the file is xhtml but CAN
 * be closed if the file is an html file.
 *
 * Use isValidAttributeForTag(const QString&, const QString&) to check that the
 * attribute is valid for the tag.
 *
 * Use isValidAttributeValueForTag(const QString&, const QString&, const QString&) to
 * check that both the attribute and that attribute value is valid for this tag.
 *
 * \note that the checking for attribute value is not exhaustive as many contain text
 *       strings that are difficult to check. Values that have fixed values such as
 *       \code<audio preload>\endcode which have three valid values (none|metadata|auto)
 *       are checked correctly but \code<audio src="">\endcode contains an URI string
 *       and is harder to check for correctness only checks that is is a string.
 *
 * \note The Tag* types are used by HtmlScanner and HtmHighlighter, if you are using it
 *       as a standalone just use the QString versions.
 * \sa HtmlScanner
 * \sa HtmlHighlighter
 */
class HtmlTypes : QObject
{
  Q_OBJECT
public:
  //! Constructor for HtmlTypes
  HtmlTypes(QObject *parent);

  //! The tag has a valid HTML 5 tag name. True if valid, otherwise false.
  bool isValidTag(Tag* tag);
  //! The tag is a valid HTML 5 tag name. True if valid, otherwise false.
  bool isValidTag(const QString& tag);
  //! The tag has a valid VOID HTML 5 tag name. True if valid, otherwise false.
  bool isValidVoidTag(Tag*tag);
  //! The tag is a valid VOID HTML 5 tag name. True if valid, otherwise false.
  bool isValidVoidTag(const QString &tag);
  //! The attribute is a valid for the tag name. True if valid, otherwise false.
  bool isValidAttributeForTag(const QString& tag, const QString& attribute);
  //! The attribute value is a valid for the tag name. True if valid, otherwise false.
  //!
  //! \note that the checking for attribute value is not exhaustive as many contain text
  //!       strings that are difficult to check. Values that have fixed values such as
  //!       \code<audio preload>\endcode which have three valid values (none|metadata|auto)
  //!       are checked correctly but \code<audio src="">\endcode contains an URI string
  //!       and is harder to check for correctness only checks that is is a string.
  bool isValidAttributeValueForTag(const QString& tag,
                                   const QString& attribute,
                                   const QString& value);

  /*!
   * \brief sets the languages object.
   *
   * \future{This will be used to check correctness of html lang attributes.}
   * \notimplemented
   * \sa BCP47Languages
   */
  void setLanguages(BCP47Languages *Languages);



private:
  QString m_configDirectory;
  QString m_configFilename;
  BCP47Languages *m_languages;

  static constexpr char NOVALUE[] = "NOVALUE";
  static constexpr char URL[] = "URL";
  static constexpr char URLLIST[] = "URLLIST";
  static constexpr char TEXT[] = "TEXT";
  static constexpr char REGEX[] = "REGEX";
  static constexpr char LANGUAGECODE[] = "LANGUAGECODE";
  static constexpr char MEDIAQUERY[] = "MEDIAQUERY";
  static constexpr char MEDIATYPE[] = "MEDIATYPE";
  static constexpr char COORDINATES[] = "COORDINATES";
  static constexpr char FILENAME[] = "FILENAME";
  static constexpr char DIRNAME[] = "DIRNAME";
  static constexpr char FORMID[] = "FORMID";
  static constexpr char ELEMENTID[] = "ELEMENTID";
  static constexpr char DATALISTID[] = "DATALISTID";
  static constexpr char FRAMENAME[] = "FRAMENAME";
  static constexpr char NAME[] = "NAME";
  static constexpr char INTEGER[] = "INTEGER";
  static constexpr char DOUBLE[] = "DOUBLE";
  static constexpr char NUMBER[] = "NUMBER";
  static constexpr char RANGE[] = "RANGE";
  static constexpr char DATE[] = "DATE";
  static constexpr char DATETIME[] = "DATETIME";
  static constexpr char TIME[] = "TIME";
  static constexpr char CHARSET[] = "CHARSET";
  static constexpr char HTMLCODE[] = "HTMLCODE";
  static constexpr char SIZES[] = "SIZES";
  static constexpr char MAPNAME[] = "MAPNAME";
  static constexpr char FILEEXT[] = "FILEEXT";
  static constexpr char HEIGHTxWIDTH[] = "HEIGHTxWIDTH";
  static constexpr char VALUE[] = "VALUE";
  static constexpr char FILEHASH[] = "FILEHASH";
  static constexpr char SCRIPTTYPE[] = "SCRIPTTYPE";
  static constexpr char MIMETYPE[] = "MIMETYPE";
  static constexpr char SIZELIST[] = "SIZELIST";
  static constexpr char HEADERID[] = "HEADERID";

  static constexpr char AUDIOFILTER[] = "audio/*";
  static constexpr char VIDEOFILTER[] = "video/*";
  static constexpr char IMAGEFILTER[] = "image/*";

  static const QRegularExpression DATETIMEREGEX;
  static const QRegularExpression TIMEREGEX;

  static QVector<QString> HTML5_TAG_NAMES;
  static QVector<QString> VOID_HTML5_TAG_NAMES;
  static QVector<QString> GLOBAL_ATTRIBUTES;
  static QVector<QString> EVENT_ATTRIBUTES;
  static QVector<QString> PSEUDO_CLASSES;
  static QMap<QString, QMap<QString, QVector<QString>>> TAG_ATTRIBUTE_MAP;
  static QVector<QString> MEDIATYPES;

  static QMap<QString, QMap<QString, QVector<QString>>> initTagTypeMap();
  static QVector<QString> initHtmlTagnames();
  static QVector<QString> initVoidHtmlTagnames();
  static QVector<QString> initPseudoClasses();
  static QVector<QString> initGlobalAttributes();
  static QVector<QString> initEventAttributes();
  static QVector<QString> initMediaTypes();
  void buildTagNamesIfEmpty();
  void buildVoidTagsIfEmpty();
  void buildTagAttributesIfEmpty();
  void buildGlobalAttributesIfEmpty();
  void buildEventAttributesIfEmpty();
};

#endif // HTMLTYPES_H
