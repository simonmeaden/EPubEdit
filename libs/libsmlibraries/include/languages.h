#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QDate>
#include <QDir>
#include <QFile>
//#include <QFuture>
//#include <QFutureWatcher>
#include <QString>
#include <QStringLiteral>
#include <QThread>
#include <QUrl>
//#include <QtConcurrent/QtConcurrent>

#include <QByteArray>
#include <QObject>

#include <QtDebug>

#include "filedownloader.h"
#include "qyamlcpp.h"
#include "unstatistical.h"

/*!
  \class BCP47Language languages.h
  \brief A class that specifies information about a single language tag,
  extended language tag, script tag, regional tag or grandfathered tag.

  There are several different types of tag.
  - LANGUAGE These are the primary language tags.
  - EXTLAN These are language extensions
  - SCRIPT These indicate the possible script for the language.
  - REGION These indicate the regional variation for the language.
  - VARIANT These indicate a variation on the primary language.
  - GRANDFATHERED These are generally older tag names which shouldn't
    really be used any more. They generally have a preferedValue() that
    shows the tag that you should now use. This can be recovered using
    preferedValue().
  - REDUNDANT These are generally older tag names which shouldn't
    really be used any more.

  For a more extensive handling see
  <https://www.w3.org/International/articles/language-tags/>.

  This class loads the language tag definitions from the
  [IAIN language tag registry]
  (https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry)
  and parses it into an easier to use-ish form. It will also save the data
  into a local YAML file for easier/faster recovery.
 */
class BCP47Language
{
public:
  enum Type
  {
    // below used both in tags and values flags
    BAD_TAG,
    LANGUAGE,      //!< Language tags or Values flag
    EXTLANG,       //!< ExtLang tags or Values flag
    SCRIPT,        //!< Script tags or Values flag
    REGION,        //!< Regional tags or Values flag
    VARIANT,       //!< Variant tags or Values flag
    GRANDFATHERED, //!< Grandfathered tags or Values flag
    REDUNDANT,     //!< Reduntant tag or Values flag
  };
  enum TagType
  {
    // primary language tags
    PRIMARY_LANGUAGE = 0x1, //!< Language tags or Values flag
    PRIVATE_LANGUAGE = 0x2, //!< 'x' or 'i' indicate a private language
    EXTENDED_AS_PRIMARY = 0x4,
    EXTENDED_LANGUAGE = 0x8,
    EXTLANG_MISMATCH = 0x10,   //!< Language and extended language don't match
    DUPLICATE_EXTENDED = 0x20, //!< extended followed by extended not allowed
    EXTENDED_FOLLOWS_SCRIPT =
      0x40, //!< script MUST follow extlang if it exists.
    EXTENDED_FOLLOWS_REGION =
      0x80, //!< region MUST follow extlang if it exists.
            //
    SCRIPT_LANGUAGE = 0x100,
    DUPLICATE_SCRIPT = 0x200, //!< DUPLICATE SCRIPT flag
    PRIVATE_SCRIPT = 0x400,   //!< A private script language.
    NO_SCRIPT = 0x800,        //!< No script section
                              //
    REGIONAL_LANGUAGE = 0x10000,
    PRIVATE_REGION = 0x20000,        //!< A private region.
    NO_REGION = 0x40000,             //!< No region section
    UN_STATISTICAL_REGION = 0x80000, //!< A UN statistical area code.
    DUPLICATE_REGION = 0x100000,     //!< A UN statistical area code.

    BAD_PRIMARY_LANGUAGE = 0x1000000, //!< Bad tag, only used in TagTypes flag

  };
  Q_DECLARE_FLAGS(TagTypes, TagType)

  BCP47Language();
  virtual ~BCP47Language() = default;

  //! Returns the tag type
  Type type() const;
  //! Sets the tag type
  void setType(const Type& type);
  //! Sets the tag type from a tag string value.
  void setTypeFromString(const QString& typeStr) {}
  //! Return a tag string from the tag type.
  QString typeString();
  //! Set the subtag value
  void setSubtag(const QString& tag);
  //! Returns the subtag name.
  QString subtag() const;
  //! Returns the primary (first) tag description.
  //!
  //! This is used to locate the tags from one of the tag maps.
  QString description() const;
  //! Adds a description string to the list
  void addDescription(const QString& desc);
  //! Appends further  characters to the last string in the list.
  void appendDescription(const QString& desc);
  //! Returns the full list of descriptions for the tag.
  QStringList descriptions() const;
  //! Sets the date added value
  void setDateAdded(const QDate& date);
  //! Returns the date added value.
  QDate dateAdded() const;
  //! Sets the suppress-script name for this tag if any.
  void setSuppressScript(const QString& lang);
  //! Returns the suppress-script name, or an empty QString if none.
  QString suppressScriptLang() const;
  //! Sets the macrolanguage name for this tag, if any
  void setMacrolanguageName(const QString& microlang);
  //! Returns the macrolanguage name or an empty string if none.
  QString macrolanguageName() const;
  //! Sets the collection flag if this is a collection tag, default false.
  void setCollection(bool collection);
  //! Returns the collection flag.
  bool isCollection() const;
  //! Sets the macrolanguage flag if this tag is for a macrolanguage, default
  //! false.
  void setMacrolanguage(bool isMacrolanguage);
  //! Returns the macrolanguage flag. True if it is a macrolanguage.
  bool isMacrolanguage() const;
  //! Returns any comment for this tag, default is an empty string.
  //!
  //! \note this could be a multiline comment.
  QString comments() const;
  //! Sets the value of the comment, if any.
  void setComments(const QString& comments);
  //! Appends further  characters to the comment string.
  void appendComment(const QString& extra);
  //! Returns true if the BCPLanguage object has a comment, otherwise returns
  //! false.
  bool hasComment();
  //! Returns true if this tag is deprecated.
  bool isDeprecated() const;
  //! Sets the value of the deprecated flag.
  void setDeprecated(bool deprecated);
  //! Returns the preferred value of this tag.
  QString preferredValue() const;
  //! Sets the preferred value of this tag, if any.
  void setPreferredValue(const QString& preferredValue);
  //! Returns the list of possible prefix values.
  QStringList prefix() const;
  // Adds a prefix to the list.
  void addPrefix(const QString& prefix);
  //! Returns the value of the grandfathered tag.
  //!
  //! This is only used for grandfathered tags. Use preferredValue() to
  //! recover the value that you should use for modern tags.
  QString tag() const;
  //! Sets the name value of the grandfathered tag.
  void setTag(const QString& tag);

  //! static method to create a tag type from the name string.
  static Type fromString(const QString& name);

private:
  Type m_type;
  QString m_subtag;
  QString m_tag;
  QStringList m_descriptions;
  QDate m_added;
  QString m_suppressScriptLang;
  QString m_macrolanguageName;
  QString m_comments;
  QString m_preferredValue;
  QStringList m_prefix;
  bool m_macrolanguage;
  bool m_collection;
  bool m_deprecated;
};

/*!
  \class LanguageParser languages.h
  \brief A internal utility class to parse IAIN language tag names file.

 */
class LanguageParser : public QObject
{
  Q_OBJECT
  Q_FLAGS(Errors)

public:
  enum Error
  {
    NO_ERROR = 0,
    BAD_FILE_DATE = 1,
    EMPTY_NAME = 2,
    EMPTY_VALUE = 4,
    UNKNOWN_TAG_TYPE = 8,
    //    BAD_TAG_NAME = 16,
  };
  Q_DECLARE_FLAGS(Errors, Error)

  LanguageParser(QObject* parent = nullptr);

  void parse();

  void setData(const QByteArray& data);

signals:
  void finished();
  void parseCompleted(QMultiMap<QString, QSharedPointer<BCP47Language>>,
                      QDate,
                      bool);
  void parsingErrors(QMultiMap<int, LanguageParser::Errors>);

private:
  //  QMultiMap<int, Errors> m_errors;
  int m_lineNumber;
  QString m_data;
  //  QMultiMap<QString, QSharedPointer<BCP47Language>> m_map;
  //  QDate m_fileDate;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(LanguageParser::Errors)

/*!
  \class BCP47Languages languages.h
  \brief A utility class to supply IAIN language tag names.

  The complete list of languages tags. These are stored in a set of
  QString=>BCP47Language maps. The full list of mixed tag types can
  be recovered using the completeMap() method, alternatively you can
  use languageMap(), extlanMap(), regionMap(), scriptMap() or variantMap()
  to recover the specialised maps.

  The data for the list is recovered directly from the IANA website and
  requires web access. The the list should not be manually created. The web
  connection is only required the first time, although if it is available
  the file-date is checked and if a newer file is available it will be
  updated accordingly. Once downloaded and parsed it is stored locally
  in a YAML file to enable faster access. Downloading and parsing are both
  done in seperate threads so as not to delay the main application.

  Data is stored in a map of registry description against BCP47Language
  objects.

   Region names, defined by the BCP47Language::REGION type are
   used to extend a language in a similar way to the EXTLANG types.
   However these types ONLY define the regional code and do not specify
   the primary language.

   Region names are used with a hyphon to specify a regional variation
   of a primary language for example Canadian French would be specified
   by fr-CA with fr being the primary language and CA specifying Canada
   as a region.

   Prefixes are defined using the xx_yyy format where
   xx is the main language defined by BCP47Language::prefix().at(0)
   and yyy is the sub-language defined by BCP47Language::preferredValue().
   For example 'Algerian Saharan Arabic' has a prefix of Arabic (ar) and
   a preferredValue() of (aao) and would be specified using 'ar_aao'.

   \note extlang languages have a single prefix, however other types can#
   have more than one, VARIANT for example which is why prefix() returns a
   QStringList rather than a single value.

  You can force a data rebuild by calling rebuildFromRegistry();

  A different registry file can be set with setRegistry()

  For a more extensive handling see
  <https://www.w3.org/International/articles/language-tags/>.

  This class loads the language tag definitions from the
  [IAIN language tag registry]
  (https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry)
  and parses it into an easier to use-ish form. It will also save the data
  into a local YAML file for easier/faster recovery.
 */
class BCP47Languages : public QObject
{
  enum State
  {
    UNKNOWN,
    STARTED,
    FINISHED,
    DESCRIPTION,
    COMMENT,
  };

  Q_OBJECT
public:
  BCP47Languages(QObject* parent = nullptr);
  virtual ~BCP47Languages() = default;

  //! \brief Forces a rebuild of the language file fromn the registry.
  void rebuildFromRegistry();

  //! \brief Reads the data from the local YAML file.
  //!
  //! This also reloads the registry in a background thread, checks if the file
  //! has been updated and updates the stored data and YAML file accordingly.
  virtual void readFromLocalFile(QFile& file);

  //! \brief Returns the set of description strings.
  //!
  //! This is the entire list of descriptions for ALL BCP47Language types.
  //! Use these values to access the entire list of types.
  QStringList descriptions() const;

  //! \brief Returns a list of the BCP47Language data objects for the supplied
  //! description.
  //!
  //! \note This does not check the type of the language. It could be
  //! either a main language, a regional language or an extended language
  //! or something else.
  //!
  //! \note If you know for example that you are requiring a
  //! BCP47Language::LANGUAGE use the languageForDescription(QString) method,
  //! similarly for SCRIPT, VARIANT etc. Alternatively you will need to iterate
  //! through the list until you find the type you require.
  //!
  //! \note the GRANDFATHERED type has one special case where tyhere are two
  //! grandfathered objects with the same description.
  //!
  //! \sa languageForDescription(QString)
  //! \sa extlangForDescription(QString)
  //! \sa regionForDescription(QString)
  //! \sa variantForDescription(QString)
  //! \sa scriptForDescription(QString)
  //! \sa grandfatheredForDescription(QString)
  //! \sa redundantForDescription(QString)
  QList<QSharedPointer<BCP47Language>> fromDescription(
    const QString& description);

  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::LANGUAGE types.
  QSharedPointer<BCP47Language> languageFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::EXTLANG types.
  QSharedPointer<BCP47Language> extlangFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::VARIANT types.
  QSharedPointer<BCP47Language> variantFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::REGION types.
  QSharedPointer<BCP47Language> regionFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::SCRIPT types.
  QSharedPointer<BCP47Language> scriptFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::REDUNDANT types.
  QSharedPointer<BCP47Language> redundantFromDescription(
    const QString& description);
  //! \brief Returns the BCP47Language data for the supplied description for
  //! BCP47Language::GRANDFATHERED types.
  QSharedPointer<BCP47Language> grandfatheredFromDescription(
    const QString& description);

  //! \brief Returns the BCP47Language data for the supplied subtag for
  //! BCP47Language::LANGUAGE types.
  QSharedPointer<BCP47Language> languageFromSubtag(const QString& subtag);
  //! \brief Returns the BCP47Language data for the supplied subtag for
  //! BCP47Language::EXTLANG types.
  QSharedPointer<BCP47Language> extlangFromSubtag(
    const QString& subtag);
  //! \brief Returns the BCP47Language data for the supplied subtag for
  //! BCP47Language::VARIANT types.
  QSharedPointer<BCP47Language> variantFromSubtag(const QString& subtag);
  //! \brief Returns the BCP47Language data for the supplied subtag for
  //! BCP47Language::REGION types.
  QSharedPointer<BCP47Language> regionFromSubtag(
    const QString& subtag);
  //! \brief Returns the BCP47Language data for the supplied subtag for
  //! BCP47Language::SCRIPT types.
  QSharedPointer<BCP47Language> scriptFromSubtag(const QString& subtag);
  //! \brief Returns the BCP47Language data for the supplied tag for
  //! BCP47Language::REDUNDANT types.
  QSharedPointer<BCP47Language> redundantFromTag(
    const QString& tag);
  //! \brief Returns the BCP47Language data for the supplied tag for
  //! BCP47Language::GRANDFATHERED types.
  QSharedPointer<BCP47Language> grandfatheredFromTag(
    const QString& tag);
  //! \brief Returns a list of descriptions of all those BCP47Language::EXTLANG
  //! types that have the supplied subtag, or an empty list if none exist.
  QStringList extlangsWithPrefix(const QString subtag) {
    QStringList list;
    for (auto extlang : m_extlangBySubtag.values()) {
      if (extlang && extlang->prefix().contains(subtag)) {
        list << extlang->description();
      }
    }
    return list;
  }

  //! \brief Returns the set of primary language descriptions as a list of
  //! QString.
  QStringList languageDescriptions() const;

  //! \brief Returns the set of primary language subtags as a list of
  //! QString.
  QStringList languageSubtags() const;

  //! \brief Returns the set of regional descriptions as a list of QString.
  QStringList regionDescriptions() const;

  //! \brief Returns the set of regional subtags as a list of QString.
  QStringList regionSubtags() const;

  //! \brief Returns the set of extended language descriptions as a list of
  //! QStrings.
  QStringList extlangDescriptions() const;

  //! \brief Returns the set of extended language subtags as a list of QStrings.
  QStringList extlangSubtags() const;

  //! \brief The list of script  descriptions as a list of QStrings.
  QStringList scriptDescriptions() const;

  //! \brief The list of script  subtags as a list of QStrings.
  QStringList scriptSubtags() const;

  //! \brief The list of variant  descriptions as a list of QStrings.
  QStringList variantDescriptions() const;

  //! \brief The list of variant  subtags as a list of QStrings.
  QStringList variantSubtags() const;

  //! \brief The list of grandfathered  descriptions as a list of QStrings.
  //!
  //! These tags should generally not be used and are retained for historical
  //! reasons.
  QStringList grandfatheredDescriptions() const;

  //! \brief The list of grandfathered  tags as a list of QStrings.
  //!
  //! These tags should generally not be used and are retained for historical
  //! reasons.
  QStringList grandfatheredTags() const;

  //! \brief The list of redundant  descriptions as a list of QStrings.
  //!
  //! These tags should generally not be used and are retained for historical
  //! reasons.
  QStringList redundantDescriptions() const;

  //! \brief The list of redundant  tags as a list of QStrings.
  //!
  //! These tags should generally not be used and are retained for historical
  //! reasons.
  QStringList redundantTags() const;

  //! \brief Returns the tag for the supplied language name, with optional
  //! region name.
  //!
  //! For example if English are specified the **en** is returned,
  //! however if a region of *United States* is specified then **en-US** will
  //! be returned.
  QString languageTag(const QString& languageName, const QString& regionName);

  //! \brief Return the tag value for the supplied extlang name .
  //!
  //! For example for the Gulf Arabic it would return **ar-afb** would be
  //! returned.
  //!
  //! If the name in invalid then an empty string will be returned.
  QString extLangTag(const QString& extlanName);

  //! \brief Return the tag value for the supplied language name and script
  //! name.
  //!
  //! For example for the Azerbaijani, written in Latin script it would return
  //! **az-latn**.
  //!
  //! If either of the names in invalid then an empty string will be returned.
  QString scriptTag(const QString& languageName, const QString& scriptName);

  //! \brief Return the tag value for the supplied script name and optionally
  //! region name.
  //!
  //! For example for the Nadiza dialect of Slovenian it would return
  //! **sl-nedis**, however if the Italy region is specified then
  //! **sl-IT-nedis** would be returned.
  //!
  //! If either of the names in invalid then an empty string will be returned.
  QString variantTag(const QString& scriptName, const QString& region);

  //! \brief static method which checks whether the supplied string is one of
  //! the valid tag names. Returns true if it is, otherwise returns false.
  //!
  //! The string comparison is case insensitive.
  static bool isType(const QString& type);

  //! Returns the BCP47Language::Type for the supplied subtag string or BAD_TAG
  //! if it is not a valid subtag of any type.
  //!
  //! \sa BCP47Language::Type
  BCP47Language::Type type(const QString& value);

  BCP47Language::TagTypes testTag(QString& value);

  //! Tests whether the subtag string is a valid primary language tag. Returns
  //! true if it is otherwise returns false.
  bool isPrimaryLanguage(const QString& subtag);

  //! Tests whether the subtag string is a valid extlang tag. Returns
  //! true if it is otherwise returns false.
  bool isExtLang(const QString& subtag);

  //! Tests whether the subtag string is a valid variant tag. Returns
  //! true if it is otherwise returns false.
  bool isVariant(const QString& subtag);

  //! Tests whether the subtag string is a valid region tag. Returns
  //! true if it is otherwise returns false.
  bool isRegion(const QString& subtag);

  //! Tests whether the subtag string is a valid script tag. Returns
  //! true if it is otherwise returns false.
  bool isScript(const QString& subtag);

  //! Tests whether the subtag string is a valid grandfathered tag. Returns
  //! true if it is otherwise returns false.
  bool isGrandfathered(const QString& subtag);

  //! Tests whether the subtag string is a valid redundant tag. Returns
  //! true if it is otherwise returns false.
  bool isRedundant(const QString& subtag);

  QDate fileDate() const;

  //! Returns the entire map of Description to BCP47Language objects.
  QMultiMap<QString, QSharedPointer<BCP47Language>> dataset();

  //! Saves the data to a local file.
  //!
  //! By default the data is saved to a YAML file. This method is virtual to
  //! allow users to create their own languages class that saves the data to a
  //! different file format.
  virtual void saveToLocalFile(QFile& file);

  //! Sets the registry name for the iain language registry.
  //!
  //! The registry url is set automatically. You should only need to enter
  //! a registry name if the registry is changed or if you need a non-standard
  //! registry.
  void setRegistry(const QString& registry);

signals:
  //! \brief Emitted when the local YAML data file has been fully loaded.
  void completed();
  //! \brief Emitted when a newer IAIN BCP47 file has been located and parsing
  //! has been completed.
  //!
  //! The original data and YAML file will be replaced with the newer data.
  void languagesReset();
  //! \brief Emitted when internal messages are sent out to the user.
  void sendMessage(const QString& message);
  //! \brief Emitted when general errors occur.
  void error(const QString& errorStr);
  //! \brief Emitted when parsing errors occur.
  void parsingError(const QString& errorStr);

private:
  QMultiMap<QString, QSharedPointer<BCP47Language>> m_datasetByDescription;
//  QMultiMap<QString, QSharedPointer<BCP47Language>> m_datasetBySubtag;
  QMap<QString, QSharedPointer<BCP47Language>> m_languageByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_languageBySubtag;
  QMap<QString, QSharedPointer<BCP47Language>> m_extlangByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_extlangBySubtag;
  QMap<QString, QSharedPointer<BCP47Language>> m_regionByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_regionBySubtag;
  QMap<QString, QSharedPointer<BCP47Language>> m_scriptByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_scriptBySubtag;
  QMap<QString, QSharedPointer<BCP47Language>> m_variantByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_variantBySubtag;
  // some grandfathered descriptions are NOT unique.
  QMultiMap<QString, QSharedPointer<BCP47Language>>
    m_grandfatheredByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_grandfatheredByTag;
  QMap<QString, QSharedPointer<BCP47Language>> m_redundantByDescription;
  QMap<QString, QSharedPointer<BCP47Language>> m_redundantByTag;

  QDate m_fileDate;
  LanguageParser* worker;
  QFile m_languageFile;
  QString m_registryName;
  UNStatisticalCodes* m_unStatistical;

  const static QStringList TAGTYPES;
  const static QString IAINREGISTRY;

  void addLanguage(const QString& description,
                   QSharedPointer<BCP47Language> language);
  void loadYamlFile(QFile& file);
  void checkLocalFileForNewer(
    QMultiMap<QString, QSharedPointer<BCP47Language>> map,
    QDate fileDate);
  void reloadData();
  void parseData(const QByteArray& data);
  void errorReceived(const QString& errorStr);
  void parsingErrorsReceived(QMultiMap<int, LanguageParser::Errors> errors);
  void iainFileParsed(QMultiMap<QString, QSharedPointer<BCP47Language>>,
                      QDate,
                      bool noErrors);
  void updateMaps();

  BCP47Language::TagTypes testPrimaryLanguage(const QString& value);
  void testExtendedlanguage(const QString& value,
                            BCP47Language::TagTypes& tagTypes);
  void testScript(const QString& value, BCP47Language::TagTypes& tagTypes);
  void testRegion(const QString& value, BCP47Language::TagTypes& tagTypes);

  friend class LanguageParser;
  QList<QSharedPointer<BCP47Language>> getUniqueLanguages();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BCP47Language::TagTypes)

#endif // LANGUAGES_H
