#include "languages.h"
#include "qyamlcpp.h"

//====================================================================
//=== BCP47Language
//====================================================================
BCP47Language::BCP47Language()
  : m_macrolanguage(false)
  , m_collection(false)
  , m_deprecated(false)
{}

void
BCP47Language::setSubtag(const QString& tag)
{
  m_subtag = tag;
}

QString
BCP47Language::subtag() const
{
  return m_subtag;
}

void
BCP47Language::addDescription(const QString& desc)
{
  if (m_descriptions.isEmpty()) {
    m_description = desc;
  }
  m_descriptions.append(desc);
}

QString
BCP47Language::description() const
{
  return m_description;
}

QStringList
BCP47Language::descriptions() const
{
  return m_descriptions;
}

void
BCP47Language::setDateAdded(const QDate& date)
{
  m_added = date;
}

QDate
BCP47Language::dateAdded() const
{
  return m_added;
}

void
BCP47Language::setSuppressScript(const QString& lang)
{
  m_suppressScriptLang = lang;
}

QString
BCP47Language::suppressScriptLang() const
{
  return m_suppressScriptLang;
}

void
BCP47Language::setMacrolanguageName(const QString& macrolang)
{
  m_macrolanguageName = macrolang;
}

QString
BCP47Language::macrolanguageName() const
{
  return m_macrolanguageName;
}

void
BCP47Language::setCollection(bool collection)
{
  m_collection = collection;
}

bool
BCP47Language::isCollection() const
{
  return m_collection;
}

void
BCP47Language::setMacrolanguage(bool isMacrolanguage)
{
  m_macrolanguage = isMacrolanguage;
}

bool
BCP47Language::isMacrolanguage() const
{
  return m_macrolanguage;
}

QString
BCP47Language::comments() const
{
  return m_comments;
}

void
BCP47Language::setComments(const QString& comments)
{
  m_comments = comments;
}

void
BCP47Language::addComment(const QString& extra)
{
  m_comments.append("\n");
  m_comments.append(extra);
}

QString
BCP47Language::preferredValue() const
{
  return m_preferredValue;
}

void
BCP47Language::setPreferredValue(const QString& preferredValue)
{
  m_preferredValue = preferredValue;
}

bool
BCP47Language::isDeprecated() const
{
  return m_deprecated;
}

void
BCP47Language::setDeprecated(bool deprecated)
{
  m_deprecated = deprecated;
}

QStringList
BCP47Language::prefix() const
{
  return m_prefix;
}

void
BCP47Language::addPrefix(const QString& prefix)
{
  m_prefix.append(prefix);
}

QString
BCP47Language::tag() const
{
  return m_tag;
}

void
BCP47Language::setTag(const QString& tag)
{
  m_tag = tag;
}

BCP47Language::Type
BCP47Language::type() const
{
  return m_type;
}

void
BCP47Language::setType(const Type& type)
{
  m_type = type;
}

QString
BCP47Language::typeString()
{
  switch (m_type) {
    case LANGUAGE:
      return "language";
    case EXTLANG:
      return "extlang";
    case SCRIPT:
      return "script";
    case REGION:
      return "region";
    case VARIANT:
      return "variant";
    case GRANDFATHERED:
      return "grandfathered";
  }
  return QString();
}

//====================================================================
//=== BCP47Languages
//====================================================================
BCP47Languages::BCP47Languages(QObject* parent)
  : QObject(parent)
{}

void
BCP47Languages::saveToLocalFile(QFile& file)
{
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    QString value;
    emitter << YAML::Comment(
      tr("BCP47 is a conversion of the IANA Language Subtag Registry\n"
         "into a more user friendly form. For the original file see:\n"
         "https://www.iana.org/assignments/language-subtag-registry/"
         "language-subtag-registry.\n\n"));
    emitter << YAML::BeginSeq;
    for (auto language : m_languages) {
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "type" << YAML::Value << language->typeString();
      value = language->subtag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "subtag" << YAML::Value << value;
      }
      value = language->tag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "tag" << YAML::Value << value;
      }
      value = language->description();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "description" << YAML::Value << value;
      }
      value = language->dateAdded().toString(Qt::ISODate);
      if (!value.isEmpty()) {
        emitter << YAML::Key << "added" << YAML::Value << value;
      }
      value = language->suppressScriptLang();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "suppress-script" << YAML::Value << value;
      }
      value = language->macrolanguageName();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "macrolanguage" << YAML::Value << value;
      }
      value = language->preferredValue();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "preferred-value" << YAML::Value << value;
      }
      auto prefixes = language->prefix();
      if (!prefixes.isEmpty()) {
        emitter << YAML::Key << "prefix" << YAML::Value;
        emitter << YAML::BeginSeq;
        emitter << YAML::Flow << prefixes;
        emitter << YAML::EndSeq;
      }
      if (language->isMacrolanguage()) {
        emitter << YAML::Key << "scope" << YAML::Value << "macrolanguage";
      }
      if (language->isCollection()) {
        emitter << YAML::Key << "scope" << YAML::Value << "collection";
      }
      if (language->isDeprecated()) {
        emitter << YAML::Key << "scope" << YAML::Value << "deprecated";
      }
      emitter << YAML::EndMap;
    }
    emitter << YAML::EndSeq;

    QTextStream outStream(&file);
    outStream << emitter.c_str();
    file.close();
  }
}

void
BCP47Languages::readLocalFile(QFile& file)
{
  // TODO
}

QSharedPointer<BCP47Language>
BCP47Languages::language(const QString& name)
{
  return m_languages.value(name);
}

QStringList
BCP47Languages::scriptNames() const
{
  return m_scriptNames;
}

QString
BCP47Languages::scriptTag(const QString& languageName,
                          const QString& scriptName)
{
  auto tags = m_language.value(languageName);
  auto scriptTag = m_script.value(scriptName);
  if (!tags.isNull() || !scriptTag.isNull())
    return tags->prefix().at(0) + "-" + scriptTag->subtag() + "-" +
           tags->subtag();
  return QString();
}

QString
BCP47Languages::variantTag(const QString& scriptName, const QString& region)
{
  auto tags = m_variant.value(scriptName);
  if (region.isEmpty()) {
    if (!tags.isNull())
      return tags->prefix().at(0) + "-" + tags->subtag();
  } else {
    auto regTag = m_region.value(region);
    if (!tags.isNull() || !regTag.isNull())
      return tags->prefix().at(0) + "-" + regTag->subtag() + "-" +
             tags->subtag();
  }
  return QString();
}

QStringList
BCP47Languages::grandfatheredNames() const
{
  return m_grandfatheredNames;
}

QStringList
BCP47Languages::regionNames() const
{
  return m_regionNames;
}

QStringList
BCP47Languages::languageNames() const
{
  return m_languageNames;
}

QString
BCP47Languages::languageTag(const QString& languageName,
                            const QString& regionName)
{
  auto tag = m_language.value(languageName);
  if (regionName.isEmpty()) {
    if (!tag.isNull())
      return tag->subtag();
  } else {
    auto regTag = m_region.value(regionName);
    if (!tag.isNull() && !regTag.isNull()) {
      return tag->subtag() + "-" + regTag->subtag();
    }
  }
  return QString();
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::languages()
{
  return m_languages;
}

QStringList
BCP47Languages::extlangNames() const
{
  return m_extlangNames;
}

QString
BCP47Languages::extLangTag(const QString& extlanName)
{
  auto langTag = m_language.value(extlanName);
  return langTag->prefix().at(0) + "-" + langTag->preferredValue();
}

void
BCP47Languages::addLanguage(const QString& name,
                            QSharedPointer<BCP47Language> language)
{
  m_languages.insert(name, language);
  switch (language->type()) {
    case BCP47Language::LANGUAGE:
      m_languageNames.append(language->descriptions());
      m_language.insert(name, language);
      break;
    case BCP47Language::EXTLANG:
      m_extlangNames.append(language->descriptions());
      m_extlan.insert(name, language);
      break;
    case BCP47Language::REGION:
      m_regionNames.append(language->descriptions());
      m_region.insert(name, language);
      break;
    case BCP47Language::SCRIPT:
      m_scriptNames.append(language->descriptions());
      m_script.insert(name, language);
      break;
    case BCP47Language::VARIANT:
      m_variantNames.append(language->descriptions());
      m_variant.insert(name, language);
      break;
    case BCP47Language::GRANDFATHERED:
      m_grandfatheredNames.append(language->descriptions());
      break;
  }
}

void
BCP47Languages::rebuildFromRegistry()
{
  auto url = QUrl("https://www.iana.org/assignments/language-subtag-registry/"
                  "language-subtag-registry");
  m_downloader = new FileDownloader(url, this);
  connect(m_downloader,
          &FileDownloader::downloadComplete,
          this,
          &BCP47Languages::loadData);
}

void
BCP47Languages::loadData()
{
  if (!m_languages.isEmpty()) {
    m_languages.clear();
  }

  QString data(m_downloader->downloadedData());
  QString date;
  for (auto c : data) {
    if (c == '\n') {
      break;
    } else {
      date += c;
    }
  }

  if (date.toLower().startsWith("file-date")) {
    auto splits = date.split(":");
    if (splits.length() == 2) {
      // MUST be trimmed, otherwise pre spaces cause convertion failure.
      auto dateStr = splits.at(1).trimmed();
      auto d = QDate::fromString(dateStr, Qt::ISODate);
      m_fileDate = d;
    }
    data = data.mid(date.length() + 1);
  }

  QSharedPointer<BCP47Language> language;
  QString languageName;
  int count = 0;
  auto splits = data.split("%%", Qt::SkipEmptyParts);
  bool commented = false;
  for (auto section : splits) {
    auto subsections = section.split("\n", Qt::SkipEmptyParts);
    for (auto subsection : subsections) {
      // comments come at the end and can be multi-line
      if (commented) {
        language->addComment(subsection);
        continue;
      }
      auto parts = subsection.split(":", Qt::SkipEmptyParts);
      if (parts.size() == 2) {
        auto name = parts.at(0).trimmed().toLower();
        auto value = parts.at(1).trimmed();
        if (name == "type") {
          language = QSharedPointer<BCP47Language>(new BCP47Language());
          if (value == "language")
            language->setType(BCP47Language::LANGUAGE);
          else if (value == "extlang")
            language->setType(BCP47Language::EXTLANG);
          else if (value == "script")
            language->setType(BCP47Language::SCRIPT);
          else if (value == "region")
            language->setType(BCP47Language::REGION);
          else if (value == "variant")
            language->setType(BCP47Language::VARIANT);
          else if (value == "grandfathered")
            language->setType(BCP47Language::GRANDFATHERED);
          else
            qDebug() << tr("Unknown language type \"%1\".").arg(name);
        } else if (name == "tag") {
          if (language)
            language->setTag(value);
        } else if (name == "subtag") {
          if (language)
            language->setSubtag(value);
        } else if (name == "description") {
          if (language)
            language->addDescription(value);
        } else if (name == "added") {
          if (language)
            language->setDateAdded(QDate::fromString(value, Qt::ISODate));
        } else if (name == "suppress-script") {
          if (language)
            language->setSuppressScript(value);
        } else if (name == "prefix") {
          if (language)
            language->addPrefix(value);
        } else if (name == "macrolanguage") {
          if (language)
            language->setMacrolanguageName(value);
        } else if (name == "deprecated") {
          if (language)
            language->setDeprecated(true);
        } else if (name == "preferred-value") {
          if (language)
            language->setPreferredValue(value);
        } else if (name == "scope") {
          if (value == "macrolanguge") {
            if (language)
              language->setMacrolanguage(true);
          } else if (value == "collection") {
            if (language)
              language->setCollection(true);
          }
        } else if (name == "comments") {
          if (language) {
            language->setComments(value);
            commented = true;
          }
        } else {
          qDebug() << tr("Unknown language value \"%1\"").arg(name);
        }
      }
    }
    commented = false;
    addLanguage(language->description(), language);
    qDebug() << tr("Count : %1").arg(count++);
  }

  emit completed();
}
