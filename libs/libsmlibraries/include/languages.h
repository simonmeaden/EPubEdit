#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QDate>
#include <QDir>
#include <QFile>
//#include <QFuture>
//#include <QFutureWatcher>
#include <QString>
#include <QThread>
#include <QUrl>
//#include <QtConcurrent/QtConcurrent>

#include <QByteArray>
#include <QObject>

#include <QtDebug>

#include "filedownloader.h"
#include "qyamlcpp.h"

void
loadYamlFile(QFile& file);

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
    LANGUAGE,      //!< Language tags
    EXTLANG,       //!< ExtLang tags
    SCRIPT,        //!< Script tags
    REGION,        //!< Regional tags
    VARIANT,       //!< Variant tags
    GRANDFATHERED, // Grandfathered tags
  };
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
  Type m_type = LANGUAGE;
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

  data is stored in a map of registry deascription against BCP47Language
  objects.

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

  Q_OBJECT
public:
  enum State
  {
    UNKNOWN,
    STARTED,
    FINISHED,
    DESCRIPTION,
    COMMENT,
  };

  BCP47Languages(QObject* parent = nullptr);
  virtual ~BCP47Languages() = default;

  //! Forces a rebuild of the language file fromn the registry.
  void rebuildFromRegistry();

  //! Reads the data from the local YAML file.
  //!
  //! This also reloads the registry in a background thread, checks if the file
  //! has been updated and updates the stored data and YAML file accordingly.
  virtual void readFromLocalFile(QFile& file);

  //! Returns the set of main language names as a list of QString.
  //!
  //! Main language types, defined by the BCP47Language::LANGUAGE type
  //!
  QStringList languageNames() const;

  //! Returns the tag for the supplied language name, with optional region name.
  //!
  //! For example if English are specified the **en** is returned,
  //! however if a region of *United States* is specified then **en-US** will
  //! be returned.
  QString languageTag(const QString& languageName, const QString& regionName);

  //! Returns the set of regional names as a list of QString.
  //!
  //! Region names, defined by the BCP47Language::REGION type are
  //! used to extend a language in a similar way to the EXTLANG types.
  //! However these types ONLY define the regional code and do not specify
  //! the primary language.
  //!
  //! Region names are used with a hyphon to specify a regional variation
  //! of a primary language for example Canadian French would be specified
  //! by fr-CA with fr being the primary language and CA specifying Canada
  //! as a region.
  QStringList regionNames() const;

  //! Returns the set of extended language names as a list of QStrings.
  //!
  //! These are defined using the xx_yyy format where
  //! xx is the main language defined by BCP47Language::prefix().at(0)
  //! and yyy is the sub-language defined by BCP47Language::preferredValue().
  //! For example 'Algerian Saharan Arabic' has a prefix of Arabic (ar) and
  //! a preferredValue() of (aao) and would be specified using 'ar_aao'.
  //!
  //! \note extlang languages have a single prefix, however other types can#
  //! have more than one, VARIANT for example which is why prefix() returns a
  //! QStringList rather than a single value.
  QStringList extlangNames() const;
  //! Return the tag value for the supplied extlang name .
  //!
  //! For example for the Gulf Arabic it would return **ar-afb** would be
  //! returned.
  //!
  //! If the name in invalid then an empty string will be returned.
  QString extLangTag(const QString& extlanName);

  //! Returns the BCP47Language data for the supplied name.
  //!
  //! \note This does not check the type of the language. It could be
  //! either a main language, a regional language or an extended language
  //! or something else.
  //!
  //! You will need to check the type using BCPLanguage::type() and
  //! handle it accordingly.
  QSharedPointer<BCP47Language> language(const QString& name);

  //! Returns the BCP47Language data for the script name.
  //!
  QStringList scriptNames() const;
  //! Return the tag value for the supplied language name and script
  //! name.
  //!
  //! For example for the Azerbaijani, written in Latin script it would return
  //! **az-latn**.
  //!
  //! If either of the names in invalid then an empty string will be returned.
  QString scriptTag(const QString& languageName, const QString& scriptName);

  //! Return the tag value for the supplied script name and optionally region
  //! name.
  //!
  //! For example for the Nadiza dialect of Slovenian it would return
  //! **sl-nedis**, however if the Italy region is specified then
  //! **sl-IT-nedis** would be returned.
  //!
  //! If either of the names in invalid then an empty string will be returned.
  QString variantTag(const QString& scriptName, const QString& region);

  //! The list of grandfathered tags can ba found using one of these
  //! names with the languages() method. These tags should generally not be used
  //! and are retained for historical reasons.
  QStringList grandfatheredNames() const;

  static bool isType(const QString& type);

  QDate fileDate() const;

  //! Returns the entire map of Description to BCP47Language objects.
  QMultiMap<QString, QSharedPointer<BCP47Language>> completeMap();
  //! Returns the map of Description to LANGUAGE types.
  QMap<QString, QSharedPointer<BCP47Language>> languageMap();
  //! Returns the map of Description to EXTLAN types.
  QMap<QString, QSharedPointer<BCP47Language>> extlanMap();
  //! Returns the map of Description to REGION types.
  QMap<QString, QSharedPointer<BCP47Language>> regionMap();
  //! Returns the map of Description to SCRIPT types.
  QMap<QString, QSharedPointer<BCP47Language>> scriptMap();
  //! Returns the map of Description to VARIANT types.
  QMap<QString, QSharedPointer<BCP47Language>> variantMap();

  void updateMaps();

  virtual void saveToLocalFile(QFile& file);

  //! Sets the registry name for the iain language registry.
  //!
  //! The regisrty url is set automatically. You should only need to enter
  //! a registry name if the registry is changed or if you need a non-standard
  //! registry.
  void setRegistry(const QString& registry);

signals:
  void fileLoaded(const QByteArray& array);
  void completed();
  void languagesReset();
  void parseCompleted(QMap<QString, QSharedPointer<BCP47Language>> map,
                      QDate fileDate);
  void sendMessage(const QString& message);
  void error(const QString& errorStr);
  void parsingError(const QString& errorStr);

private:
  QMultiMap<QString, QSharedPointer<BCP47Language>> m_languages;
  QMap<QString, QSharedPointer<BCP47Language>> m_language;
  QMap<QString, QSharedPointer<BCP47Language>> m_extlan;
  QMap<QString, QSharedPointer<BCP47Language>> m_region;
  QMap<QString, QSharedPointer<BCP47Language>> m_script;
  QMap<QString, QSharedPointer<BCP47Language>> m_variant;
  QStringList m_languageNames;
  QStringList m_extlangNames;
  QStringList m_regionNames;
  QStringList m_scriptNames;
  QStringList m_variantNames;
  QStringList m_grandfatheredNames;
  QDate m_fileDate;
  LanguageParser* worker;
  QFile m_languageFile;
  QString m_registryName;

  const static QStringList TAGTYPES;
  const static QString IAINREGISTRY;

  void addLanguage(const QString& name, QSharedPointer<BCP47Language> language);
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
};

#endif // LANGUAGES_H
