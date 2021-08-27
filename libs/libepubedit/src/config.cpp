#include "config.h"

#include "languages.h"
#include "paths.h"

const QString Config::STATUS_TIMEOUT = "status_timeout";
const QString Config::SAVE_VERSION = "save_version";
const QString Config::LIBRARY_PATH = "library_path";

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , m_configFile(QFile(Paths::join(m_configDir.path(), "epubedit.yaml")))
  , m_saveType(EPUB_3_2)
  , m_libraryDir(QDir(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      "library")))
  , m_statusTimeout(StatusTimeout)
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
  if (value >= 0)
    m_statusTimeout = value;
}

void
Config::setSaveVersion(Config::SaveType type)
{
  m_saveType = type;
}

Config::SaveType
Config::saveVersion()
{
  return SaveType(m_saveType);
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

void
Config::save(const QString& filename)
{
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
      << YAML::Key << SAVE_VERSION << YAML::Value << int(m_saveType)
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
  }
}

void
Config::load(const QString& filename)
{
  setupConfigFile(filename);

  if (m_configFile.exists()) {
    auto config = YAML::LoadFile(m_configFile);

    if (config[SAVE_VERSION]) {
      auto node = config[SAVE_VERSION];
      m_saveType = SaveType(node.as<int>());
    }

    if (config[LIBRARY_PATH]) {
      auto node = config[LIBRARY_PATH];
      m_libraryDir = config[LIBRARY_PATH].as<QString>();
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
