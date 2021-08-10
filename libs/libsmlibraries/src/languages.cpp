#include "languages.h"
#include "qyamlcpp.h"

#include <string>

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
  m_descriptions.append(desc);
}

void
BCP47Language::appendDescription(const QString& desc)
{
  if (m_descriptions.length() == 0)
    m_descriptions.append(desc);
  else {
    int i = m_descriptions.size() - 1;
    QString description = m_descriptions.at(i);
    description += "\n";
    description += desc;
    m_descriptions.replace(i, description);
  }
}

QString
BCP47Language::description() const
{
  if (!m_descriptions.isEmpty())
    return m_descriptions.first();
  return QString();
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
BCP47Language::appendComment(const QString& extra)
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
BCP47Language::fromString(const QString& name)
{
  if (name.toLower() == "language")
    return LANGUAGE;
  else if (name.toLower() == "extlang")
    return EXTLANG;
  else if (name.toLower() == "script")
    return SCRIPT;
  else if (name.toLower() == "region")
    return REGION;
  else if (name.toLower() == "variant")
    return VARIANT;
  else if (name.toLower() == "grandfathered")
    return GRANDFATHERED;
  return LANGUAGE;
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
const QStringList BCP47Languages::TAGTYPES = QStringList() << "type"
                                                           << "tag"
                                                           << "subtag"
                                                           << "description"
                                                           << "added"
                                                           << "suppress-script"
                                                           << "prefix"
                                                           << "macrolanguage"
                                                           << "deprecated"
                                                           << "preferred-value"
                                                           << "scope"
                                                           << "comments";
const QString BCP47Languages::IAINREGISTRY =
  "https://www.iana.org/assignments/language-subtag-registry/"
  "language-subtag-registry";

BCP47Languages::BCP47Languages(QObject* parent)
  : QObject(parent)
  , m_registryName(IAINREGISTRY)
{}

void
BCP47Languages::saveToLocalFile(QFile& file)
{
  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    QString value;
    emitter << YAML::Comment(tr(
      "BCP47 is a conversion of the IANA Language Subtag Registry\n"
      "into a more user friendly form. For the original file see:\n"
      "https://www.iana.org/assignments/language-subtag-registry/"
      "language-subtag-registry.\n\n"
      "This file was generated automatically from the IANA Language Subtag\n"
      "Registry!\n"
      "You should not modify this file by hand as it is VERY easy\n"
      "make a mistake. You should delete the file and regenerate using\n"
      "the BCP47Languages::rebuildFromRegistry() method if the file becomes\n"
      "corrupted or outdated.\n\n"));
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "file-date" << YAML::Value
            << fileDate().toString(Qt::ISODate);
    emitter << YAML::Key << "languages" << YAML::Value;
    emitter << YAML::BeginSeq;
    for (auto& language : m_languages) {
      emitter << YAML::BeginMap;
      //      emitter << YAML::Key << "COUNT" << YAML::Value <<
      //      language->count();
      emitter << YAML::Key << "type" << YAML::Value << language->typeString();
      value = language->subtag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "subtag" << YAML::Value << value;
      }
      value = language->tag();
      if (!value.isEmpty()) {
        emitter << YAML::Key << "tag" << YAML::Value << value;
      }
      auto descriptions = language->descriptions();
      if (!descriptions.isEmpty()) {
        emitter << YAML::Key << "description" << YAML::Value;
        emitter << YAML::BeginSeq;
        for (auto description : descriptions) {
          emitter << description.toStdString().c_str();
        }
        emitter << YAML::EndSeq;
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
        for (auto prefix : prefixes) {
          emitter << prefix.toStdString().c_str();
        }
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
    emitter << YAML::EndMap;

    QTextStream outStream(&file);
    outStream << emitter.c_str();
    file.close();
  }
}

void
BCP47Languages::loadYamlFile(QFile& file)
{
  auto yaml = YAML::LoadFile(file);
  if (yaml["file-date"]) {
    auto node = yaml["file-date"];
    m_fileDate = QDate::fromString(node.as<QString>(), Qt::ISODate);
  }
  if (yaml["languages"]) {
    auto languagesNode = yaml["languages"];
    if (languagesNode && languagesNode.IsSequence()) {
      for (auto languageNode : languagesNode) {
        if (languageNode && languageNode.IsMap()) {
          auto language = QSharedPointer<BCP47Language>(new BCP47Language());
          if (languageNode["type"]) {
            auto value = languageNode["type"].as<QString>();
            language->setType(BCP47Language::fromString(value));
          }
          if (languageNode["subtag"]) {
            auto value = languageNode["subtag"].as<QString>();
            language->setSubtag(value);
          }
          if (languageNode["tag"]) {
            auto value = languageNode["tag"].as<QString>();
            language->setTag(value);
          }
          if (languageNode["added"]) {
            auto value = languageNode["added"].as<QString>();
            language->setDateAdded(QDate::fromString(value, Qt::ISODate));
          }
          if (languageNode["suppress-script"]) {
            auto value = languageNode["suppress-script"].as<QString>();
            language->setSuppressScript(value);
          }
          if (languageNode["macrolanguage"]) {
            auto value = languageNode["macrolanguage"].as<QString>();
            language->setMacrolanguageName(value);
          }
          if (languageNode["preferred-value"]) {
            auto value = languageNode["preferred-value"].as<QString>();
            language->setPreferredValue(value);
          }
          if (languageNode["scope"]) {
            auto value = languageNode["scope"].as<QString>();
            if (value == "deprecated")
              language->setDeprecated(true);
            else if (value == "collection")
              language->setCollection(true);
            else if (value == "macrolanguage")
              language->setMacrolanguage(true);
          }
          auto descriptions = languageNode["description"];
          if (descriptions && descriptions.IsSequence()) {
            for (auto description : descriptions) {
              language->addDescription(description.as<QString>());
            }
          }
          auto prefixes = languageNode["prefix"];
          if (prefixes && prefixes.IsSequence()) {
            for (auto prefix : prefixes) {
              language->addPrefix(prefix.as<QString>());
            }
          }
          addLanguage(language->description(), language);
        }
      }
    }
  }
}

void
BCP47Languages::updateMaps()
{
  m_language.clear();
  m_languageNames.clear();
  m_extlan.clear();
  m_extlangNames.clear();
  m_region.clear();
  m_regionNames.clear();
  m_script.clear();
  m_scriptNames.clear();
  m_variant.clear();
  m_variantNames.clear();
  m_grandfatheredNames.clear();
  for (auto& language : m_languages) {
    switch (language->type()) {
      case BCP47Language::LANGUAGE:
        m_languageNames.append(language->description());
        m_language.insert(language->description(), language);
        break;
      case BCP47Language::EXTLANG:
        m_extlangNames.append(language->description());
        m_extlan.insert(language->description(), language);
        break;
      case BCP47Language::REGION:
        m_regionNames.append(language->description());
        m_region.insert(language->description(), language);
        break;
      case BCP47Language::SCRIPT:
        m_scriptNames.append(language->description());
        m_script.insert(language->description(), language);
        break;
      case BCP47Language::VARIANT:
        m_variantNames.append(language->description());
        m_variant.insert(language->description(), language);
        break;
      case BCP47Language::GRANDFATHERED:
        m_grandfatheredNames.append(language->description());
        break;
    }
  }
}

void
BCP47Languages::checkLocalFileForNewer(
  QMultiMap<QString, QSharedPointer<BCP47Language>> map,
  QDate fileDate)
{
  if (m_fileDate < fileDate) {
    m_languages = map;
    updateMaps();
    emit languagesReset();
    saveToLocalFile(m_languageFile);
  }
}

void
BCP47Languages::reloadData()
{
  //  auto data = m_thread->languagesData();

  //  m_languages = data;
  //  updateMaps();
  //  emit languagesReset();
  //  saveToLocalFile(m_languageFile);

  //  m_thread->stop();
  //  m_thread->deleteLater();
  //  m_thread = nullptr;
}

void
BCP47Languages::readFromLocalFile(QFile& file)
{
  m_languageFile.setFileName(file.fileName());
  if (file.exists()) {
    loadYamlFile(file);
    emit completed();
  }

  // reloadss the language data from the registry in the background,
  // and checks the file date. If the local language file doesn't exist
  // or it is outdated then the file will be replaced with new data.
  rebuildFromRegistry();
}

void
BCP47Languages::rebuildFromRegistry()
{
  QThread* thread = new QThread;
  auto worker = new FileDownloader();
  worker->setDownloadUrl(m_registryName);
  worker->moveToThread(thread);
  connect(worker, &FileDownloader::error, this, &BCP47Languages::errorReceived);
  connect(
    worker, &FileDownloader::dataDownloaded, this, &BCP47Languages::parseData);
  connect(thread, &QThread::started, worker, &FileDownloader::download);
  connect(worker, &FileDownloader::finished, thread, &QThread::quit);
  connect(
    worker, &FileDownloader::finished, worker, &FileDownloader::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
}

void
BCP47Languages::setRegistry(const QString& registry)
{
  m_registryName = registry;
}

void
BCP47Languages::errorReceived(const QString& errorStr)
{
  emit error(errorStr);
}

void
BCP47Languages::parsingErrorsReceived(
  QMultiMap<int, LanguageParser::Errors> errors)
{
  // TODO parse errors and send message
  QString message;
  if (!errors.isEmpty()) {
    for (auto& line : errors.keys()) {
      auto error = errors.value(line);
      if (error.testFlag(LanguageParser::NO_ERROR)) {
        // This should never happen, but just in case
        message.append(tr("No Errors"));
      }
      if (error.testFlag(LanguageParser::BAD_FILE_DATE)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        message.append(
          tr("The file date \"%1\"does not exist. Possibly corrupt file!"));
      }
      if (error.testFlag(LanguageParser::EMPTY_NAME)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        if (error.testFlag(LanguageParser::EMPTY_VALUE)) {
          message.append(tr("Bad data line. Has \":\" but no name or value!"));
        } else {
          message.append(tr("Bad data line. Has \":\" but missing value!"));
        }
      }
      if (error.testFlag(LanguageParser::EMPTY_VALUE)) {
        if (error.testFlag(LanguageParser::EMPTY_NAME)) {
          // already handles in EMPTY_NAME test
          continue;
        }
        message.append(tr("Bad data line. Has \":\" but missing name!"));
      }
      if (error.testFlag(LanguageParser::UNKNOWN_TAG_TYPE)) {
        if (message.isEmpty()) {
          message.append(",");
        }
        message.append(tr("The tag type is not a valid type!"));
      }
      message.prepend(tr("Line %1: ").arg(line));
      message.append("\n");
    }
    emit parsingError(message);
  }
}

void
BCP47Languages::parseData(const QByteArray& data)
{
  QThread* thread = new QThread;
  worker = new LanguageParser();
  worker->setData(data);
  worker->moveToThread(thread);
  connect(worker,
          &LanguageParser::parsingErrors,
          this,
          &BCP47Languages::parsingErrorsReceived);
  connect(worker,
          &LanguageParser::parseCompleted,
          this,
          &BCP47Languages::iainFileParsed);
  connect(thread, &QThread::started, worker, &LanguageParser::parse);
  connect(worker, &LanguageParser::finished, thread, &QThread::quit);
  connect(
    worker, &LanguageParser::finished, worker, &LanguageParser::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  thread->start();
}

void
BCP47Languages::iainFileParsed(
  QMultiMap<QString, QSharedPointer<BCP47Language>> languages,
  QDate fileDate,
  bool noErrors)
{
  if (m_fileDate < fileDate) {
    if (noErrors) {
      m_fileDate = fileDate;
      m_languages = languages;
      saveToLocalFile(m_languageFile);
      emit sendMessage(
        tr("Language file updated %1").arg(m_fileDate.toString(Qt::ISODate)));
    } else {
      emit error(tr("The registry file had errors!"));
    }
  }
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

bool
BCP47Languages::isType(const QString& type)
{
  if (TAGTYPES.contains(type, Qt::CaseInsensitive)) {
    return true;
  }
  return false;
}

QDate
BCP47Languages::fileDate() const
{
  return m_fileDate;
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

QMultiMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::completeMap()
{
  return m_languages;
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::languageMap()
{
  return m_language;
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::extlanMap()
{
  return m_extlan;
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::regionMap()
{
  return m_region;
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::scriptMap()
{
  return m_script;
}

QMap<QString, QSharedPointer<BCP47Language>>
BCP47Languages::variantMap()
{
  return m_variant;
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
}

//====================================================================
//=== LanguageParser
//====================================================================
LanguageParser::LanguageParser(QObject* parent)
  : QObject(parent)
  , m_lineNumber(0)
{}

void
LanguageParser::parse()
{
  QString line;
  auto state = BCP47Languages::UNKNOWN;
  bool dateFound = false;
  QSharedPointer<BCP47Language> language;
  int count = 0;
  QMultiMap<QString, QSharedPointer<BCP47Language>> languageMap;
  QMultiMap<int, Errors> errors;
  QDate fileDate;

  for (auto& c : m_data) {
    if (c == '\n') {
      m_lineNumber++;
      if (!dateFound) {
        if (line.startsWith("file-date", Qt::CaseInsensitive)) {
          auto splits = line.split(":");
          if (splits.length() == 2) {
            // MUST be trimmed, otherwise pre spaces cause convertion failure.
            auto dateStr = splits.at(1).trimmed();
            auto d = QDate::fromString(dateStr, Qt::ISODate);
            fileDate = d;
            dateFound = true;
            line.clear();
          }
        } else {
          errors.insert(m_lineNumber, BAD_FILE_DATE);
        }
      } else {
        QString name, value;
        if (line == "%%") { // SEPARATOR for sections
          if (state != BCP47Languages::STARTED) {
            state = BCP47Languages::STARTED;
            language = QSharedPointer<BCP47Language>(new BCP47Language());
          }
          line.clear();
        } else if (line.contains(":")) {
          auto splits = line.split(":", Qt::SkipEmptyParts);
          auto size = splits.size();
          bool nameIsType = false;
          // clear any old values.
          name.clear();
          value.clear();

          if (size == 0) {
            // should never happen
            errors.insert(m_lineNumber, EMPTY_NAME | EMPTY_VALUE);
            continue;
          } else if (size == 1) {
            value = line;
          } else if (size == 2) {
            name = splits.at(0).trimmed();
            nameIsType = BCP47Languages::isType(name);
            value = splits.at(1).trimmed();

            Errors e = NO_ERROR;
            if (!nameIsType) {
              e.setFlag(UNKNOWN_TAG_TYPE, true);
            }
            if (value.isEmpty()) {
              e.setFlag(EMPTY_VALUE, true);
            }
            if (!e)
              errors.insert(m_lineNumber, EMPTY_NAME);
          } else {
            // If there are more than 1 colon then probably a comment
            // or description so reassemble splits into one value
            name = splits.at(0).trimmed();
            nameIsType = BCP47Languages::isType(name);
            int i;
            if (nameIsType) {
              i = 1;
              value = name;
            } else {
              i = 2;
              value = splits.at(1);
            }
            for (; i < size; i++) {
              value += (":" + splits.at(i));
            }
            // remove excess start/end spaces
            value = value.trimmed();
            size = 2;
          }

          if (size == 2 && nameIsType) {
            auto name = splits.at(0).trimmed().toLower();
            auto lName = name.toLower();
            auto lValue = value.toLower();
            if (lName == "type") {
              if (lValue == "language")
                language->setType(BCP47Language::LANGUAGE);
              else if (lValue == "extlang")
                language->setType(BCP47Language::EXTLANG);
              else if (lValue == "script")
                language->setType(BCP47Language::SCRIPT);
              else if (lValue == "region")
                language->setType(BCP47Language::REGION);
              else if (lValue == "variant")
                language->setType(BCP47Language::VARIANT);
              else if (lValue == "grandfathered")
                language->setType(BCP47Language::GRANDFATHERED);
              else {
                errors.insert(m_lineNumber, UNKNOWN_TAG_TYPE);
              };
              line.clear();
            } else if (lName == "tag") {
              if (language)
                language->setTag(value);
              line.clear();
            } else if (lName == "subtag") {
              if (language)
                language->setSubtag(value);
              line.clear();
            } else if (lName == "description") {
              if (language) {
                language->addDescription(value);
                languageMap.insert(name, language);
                count++;
                state = BCP47Languages::DESCRIPTION;
              }
              line.clear();
            } else if (lName == "added") {
              if (language)
                language->setDateAdded(QDate::fromString(value, Qt::ISODate));
              line.clear();
            } else if (lName == "suppress-script") {
              if (language)
                language->setSuppressScript(value);
              line.clear();
            } else if (lName == "prefix") {
              if (language)
                language->addPrefix(value);
              line.clear();
            } else if (lName == "macrolanguage") {
              if (language)
                language->setMacrolanguageName(value);
              line.clear();
            } else if (lName == "deprecated") {
              if (language)
                language->setDeprecated(true);
              line.clear();
            } else if (lName == "preferred-value") {
              if (language)
                language->setPreferredValue(value);
              line.clear();
            } else if (lName == "scope") {
              if (value == "macrolanguge") {
                if (language)
                  language->setMacrolanguage(true);
              } else if (value == "collection") {
                if (language)
                  language->setCollection(true);
              }
              line.clear();
            } else if (lName == "comments") {
              if (language) {
                language->setComments(value);
                state = BCP47Languages::COMMENT;
              }
              line.clear();
            } else {
              errors.insert(m_lineNumber, UNKNOWN_TAG_TYPE);
            }
          } else {
            // extra line. Probably a comment or description that runs over.
            switch (state) {
              case BCP47Languages::DESCRIPTION:
                language->appendDescription(line);
                break;
              case BCP47Languages::COMMENT:
                language->appendComment(line);
                break;
              default:
                break;
            }
            line.clear();
          }
        } else {
          // extra line. Probably a comment or description that runs over.
          switch (state) {
            case BCP47Languages::DESCRIPTION:
              language->appendDescription(line);
              break;
            case BCP47Languages::COMMENT:
              language->appendComment(line);
              break;
            default:
              break;
          }
          line.clear();
        }
      }
    } else {
      line += c;
    }
  }
  emit parseCompleted(languageMap, fileDate, errors.isEmpty());
  if (!errors.isEmpty())
    emit parsingErrors(errors);
  emit finished();
}

void
LanguageParser::setData(const QByteArray& data)
{
  m_data = QString(data);
}