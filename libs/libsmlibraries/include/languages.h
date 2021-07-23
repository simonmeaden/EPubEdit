#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QDate>
#include <QDir>
#include <QFile>
#include <QString>
#include <QUrl>

#include <QByteArray>
#include <QObject>

#include <QtDebug>

#include "filedownloader.h"

class BCP47Language
{
public:
  enum Type
  {
    LANGUAGE,
    EXTLANG,
    SCRIPT,
    REGION,
    VARIANT,
    GRANDFATHERED,
  };
  BCP47Language();
  virtual ~BCP47Language() = default;

  Type type() const;
  void setType(const Type& type);
  void setTypeFromString(const QString& typeStr) {}
  QString typeString();
  void setSubtag(const QString& tag);
  QString subtag() const;
  QString description() const;
  void addDescription(const QString& desc);
  void appendDescription(const QString& desc);
  QStringList descriptions() const;
  void setDateAdded(const QDate& date);
  QDate dateAdded() const;
  void setSuppressScript(const QString& lang);
  QString suppressScriptLang() const;
  void setMacrolanguageName(const QString& microlang);
  QString macrolanguageName() const;
  void setCollection(bool collection);
  bool isCollection() const;
  void setMacrolanguage(bool isMacrolanguage);
  bool isMacrolanguage() const;
  QString comments() const;
  void setComments(const QString& comments);
  void appendComment(const QString& extra);
  bool isDeprecated() const;
  void setDeprecated(bool deprecated);
  QString preferredValue() const;
  void setPreferredValue(const QString& preferredValue);
  QStringList prefix() const;
  void addPrefix(const QString& prefix);
  QString tag() const;
  void setTag(const QString& tag);

  //  void setCount(int count) { COUNT = count; }
  //  int count() { return COUNT; }
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

  //  int COUNT;
};

/*!
  \class BCP47Languages languages.h
  \brief A utility class to supply IAIN language tag names.

  For a more extensive handling see
  <https://www.w3.org/International/articles/language-tags/>.

  This class loads the language tag definitions from the [IAIN language tag
  registry]
  (https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry)
  and parses it into an easier to use-ish form. It will also save the data
  into a local YAML file for easier recovery.
 */
class BCP47Languages : public QObject
{
  enum State
  {
    UNKNOWN = 0,
    STARTED = 1,
    FINISHED = 2,
    DESCRIPTION = 4,
    COMMENT = 8,
  };

  Q_OBJECT
public:
  BCP47Languages(QObject* parent = nullptr);
  virtual ~BCP47Languages() = default;

  void addLanguage(const QString& name, QSharedPointer<BCP47Language> language);

  void rebuildFromRegistry();

  virtual void saveToLocalFile(QFile& file);
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

  //! Returns the entire map of Description to BCP47Language object.
  QMap<QString, QSharedPointer<BCP47Language>> languages();

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

  bool isType(const QString& type);

  QDate fileDate() const;

signals:
  void completed();

private:
  QMap<QString, QSharedPointer<BCP47Language>> m_languages;
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
  FileDownloader* m_downloader;

  const static QStringList m_types;

  void parseData();
};

#endif // LANGUAGES_H
