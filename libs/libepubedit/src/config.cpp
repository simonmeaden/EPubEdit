#include "config.h"

#include "languages.h"
#include "paths.h"

const QString Config::STATUS_TIMEOUT = "status_timeout";
const QString Config::SAVE_VERSION = "save_version";
const QString Config::LIBRARY_PATH = "library_path";
QStringList Config::VERSION_STRINGS = { "3.0.1", "3.1", "3.2", "3.3", "3.4" };

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , m_libraryDir(QDir(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      "library")))
  , m_configFile(QFile(Paths::join(m_configDir.path(), "epubedit.yaml")))
  , m_saveVersion(EPUB_3_2)
  , m_statusTimeout(StatusTimeout)
  , m_modified(false)
{
  load();
  m_languages = new BCP47Languages();
  connect(m_languages,
          &BCP47Languages::sendMessage,
          this,
          &Config::receiveStatusMessage);
  connect(
    m_languages, &BCP47Languages::parsingError, this, &Config::sendLogMessage);
  m_configDir.mkpath(m_configDir.path());
  auto file = QFile(m_configDir.filePath("languages.yaml"));
  m_languages->readFromLocalFile(file);
}

Config::~Config() {}

void
Config::saveLanguageFile()
{
  QDir dir;
  dir.mkpath(configDir());
  auto file = QFile(dir.filePath("languages.yaml"));
  m_languages->saveToLocalFile(file);
  emit tr("Language files load completed.");
}

void
Config::receiveStatusMessage(const QString& message)
{
  emit sendStatusMessage(message, m_statusTimeout);
}

QString
Config::configDir() const
{
  return m_configDir.path();
}

void
Config::setConfigDir(const QString& value)
{
  m_configDir = QDir(value);
}

int
Config::statusTimeout() const
{
  return m_statusTimeout;
}

void
Config::setConfigFile(const QString& filepath)
{
  m_configFile.setFileName(filepath);
}

void
Config::setStatusTimeout(int value)
{
  if (value >= 0) {
    m_statusTimeout = value;
    m_modified = true;
  }
}

Config::SaveVersion
Config::saveVersion()
{
  return SaveVersion(m_saveVersion);
}

void
Config::setSaveVersion(Config::SaveVersion type)
{
  if (m_saveVersion != type) {
    m_saveVersion = type;
    m_modified = true;
  }
}

void
Config::setSaveVersion(const QString versionStr)
{
  auto version = Config::version(versionStr);
  if (version != m_saveVersion) {
    m_statusTimeout = version;
    m_modified = true;
  }
}

QStringList
Config::versions()
{
  QStringList list;
  list << "3.0.1"
       << "3.1"
       << "3.2"
       << "3.3"
       << "3.4";
  return list;
}

Config::SaveVersion
Config::version(const QString& versionStr)
{
  if (versionStr == VERSION_STRINGS.at(0))
    return EPUB_3_0;
  else if (versionStr == VERSION_STRINGS.at(1))
    return EPUB_3_1;
  else if (versionStr == VERSION_STRINGS.at(2))
    return EPUB_3_2;
  else if (versionStr == VERSION_STRINGS.at(3))
    return EPUB_3_3;
  else if (versionStr == VERSION_STRINGS.at(4))
    return EPUB_3_4;
  return EPUB_3_2;
}

QString
Config::versionToString()
{
  switch (m_saveVersion) {
    case EPUB_3_0:
      return VERSION_STRINGS.at(0);
    case EPUB_3_1:
      return VERSION_STRINGS.at(1);
    case EPUB_3_2:
    default:
      return VERSION_STRINGS.at(2);
    case EPUB_3_3:
      return VERSION_STRINGS.at(3);
    case EPUB_3_4:
      return VERSION_STRINGS.at(4);
  }
}

QString
Config::libraryPath() const
{
  return m_libraryDir.path();
}

void
Config::setLibraryPath(const QString& filepath)
{
  m_libraryDir = QDir(filepath);
}

void
Config::setupConfigFile(const QString& filename)
{
  if (!filename.isEmpty()) {
    m_configFile.setFileName(filename);
  }
}

bool
Config::save(const QString& filename)
{
  if (m_modified) {
    setupConfigFile(filename);

    if (m_configFile.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;

      emitter << YAML::Comment(tr(
        "EPubEditor Configuration file.\n\n"
        "Care should be taken editing this file manually\n"
        "as the wrong key-value pair could cause problems.\n"
        "The best way is to use the in application configuration editor.\n\n"));

      emitter << YAML::BeginMap;
      emitter << YAML::Key << STATUS_TIMEOUT << YAML::Value << m_statusTimeout
              << YAML::Comment(
                   tr("Status display timeout in seconds. int value."));

      emitter << YAML::Key << LIBRARY_PATH << YAML::Value << m_libraryDir.path()
              << YAML::Comment(
                   tr("Files will be stored here unless otherwise specified."));

      emitter
        << YAML::Key << SAVE_VERSION << YAML::Value << int(m_saveVersion)
        << YAML::Comment(
             tr("Unless specified 3.2. Possible values 3.0, 3.1, 3.2 and 3.3"));
      emitter << YAML::EndMap;

      if (!emitter.good()) {
        qWarning() << tr("Configuration Emitter error: ")
                   << QString::fromStdString(emitter.GetLastError()) << "\n";
      }

      QTextStream outStream(&m_configFile);
      outStream << emitter.c_str();
      m_configFile.close();

      if (emitter.good()) {
        m_modified = false;
        return true;
      }
    }
  }
  return false;
}

void
Config::load(const QString& filename)
{
  setupConfigFile(filename);

  if (m_configFile.exists()) {
    auto config = YAML::LoadFile(m_configFile);

    if (config[SAVE_VERSION]) {
      auto node = config[SAVE_VERSION];
      m_saveVersion = SaveVersion(node.as<int>());
    }

    if (config[LIBRARY_PATH]) {
      auto node = config[LIBRARY_PATH];
      m_libraryDir = node.as<QString>();
    }

    if (config[STATUS_TIMEOUT]) {
      auto node = config[STATUS_TIMEOUT];
      m_statusTimeout = node.as<int>();
    }
  }
}

QString
Config::configFile() const
{
  return m_configFile.fileName();
}
