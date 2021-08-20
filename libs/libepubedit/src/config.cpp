#include "config.h"

#include "languages.h"

const QString Config::STATUS_TIMEOUT = "status_timeout";
const QString Config::SAVE_VERSION = "save_version";
const QString Config::LIBRARY_PATH = "library_path";

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDir(
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) +
      "/epubedit")
  , m_configFile("epubedit.yaml")
  , m_saveType(EPUB_3_2)
  , m_libraryPath(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation))
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
  QDir dir(m_configDir);
  dir.mkpath(m_configDir);
  auto file = QFile(dir.filePath("languages.yaml"));
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
  return m_configDir;
}

void
Config::setConfigDir(const QString& value)
{
  m_configDir = value;
}

int
Config::statusTimeout() const
{
  return m_statusTimeout;
}

void
Config::setConfigFile(const QString& value)
{
  m_configFile = value;
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
  return m_libraryPath;
}

void
Config::setLibraryPath(const QString& filepath)
{
  m_libraryPath = filepath;
}

QFile*
Config::setupConfigFile(const QString& filename)
{
  QFile* file;
  if (filename.isEmpty()) {
    QDir dir;
    dir.mkpath(m_configDir);
    dir.setPath(m_configDir);
    file = new QFile(dir.filePath(m_configFile), this);
  } else {
    file = new QFile(filename, this);
  }
  return file;
}

void
Config::save(const QString& filename)
{
  QFile* file = setupConfigFile(filename);

  if (file->open((QFile::ReadWrite | QFile::Truncate))) {
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

    emitter << YAML::Key << LIBRARY_PATH << YAML::Value << m_libraryPath
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

    QTextStream outStream(file);
    outStream << emitter.c_str();
    file->close();
  }
}

void
Config::load(const QString& filename)
{
  QFile* file = setupConfigFile(filename);

  if (file->exists()) {
    auto config = YAML::LoadFile(*file);

    if (config[SAVE_VERSION]) {
      auto node = config[SAVE_VERSION];
      m_saveType = SaveType(node.as<int>());
    }

    if (config[LIBRARY_PATH]) {
      auto node = config[LIBRARY_PATH];
      m_libraryPath = config[LIBRARY_PATH].as<QString>();
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
  return m_configFile;
}
