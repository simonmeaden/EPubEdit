#include "config.h"

#include "document/authors.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "languages.h"
#include "paths.h"

#include "document/bookpointers.h"

// default directory name for library.
const QString Config::DEFAULT_LIBRARY_DIRECTORY_NAME = "library";

const QString Config::STATUS_TIMEOUT = "status_timeout";
const QString Config::SAVE_VERSION = "save_version";
const QString Config::LIBRARY_PATH = "library_path";
const QString Config::HSPLITTER_SIZES = "hor_splitter_sizes";
const QString Config::VSPLITTER_SIZES = "vert_splitter_sizes";
const QString Config::ISPLITTER_SIZES = "info_splitter_sizes";
QStringList Config::VERSION_STRINGS = { "3.0.1", "3.1", "3.2", "3.3", "3.4" };

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDirectory(
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
  , m_homeDirectory(
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation))
  , m_libraryDirectory(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      DEFAULT_LIBRARY_DIRECTORY_NAME))
  , m_libraryFilename(Paths::join(m_configDirectory, "library.yaml"))
  , m_configFilename(Paths::join(m_configDirectory, "epubedit.yaml"))
  , m_optionsFilename(Paths::join(m_configDirectory, "options.yaml"))
  , m_authorsFilename(Paths::join(m_configDirectory, "authors.yaml"))
  , m_seriesFilename(Paths::join(m_configDirectory, "series.yaml"))
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
  QDir dir;
  dir.mkpath(m_configDirectory);
  m_languages->readFromLocalFile(
    Paths::join(m_configDirectory, "languages.yaml"));
}

Config::~Config() {}

void
Config::saveLanguageFile()
{
  QDir dir;
  dir.mkpath(configDir());
  m_languages->saveToLocalFile(dir.filePath("languages.yaml"));
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
  return m_configDirectory;
}

void
Config::setConfigDir(const QString& value)
{
  m_configDirectory = value;
}

int
Config::statusTimeout() const
{
  return m_statusTimeout;
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
  return m_libraryDirectory;
}

void
Config::setLibraryPath(const QString& filepath)
{
  m_libraryDirectory = filepath;
}

// void
// Config::setupConfigFile(const QString& filename)
//{
//   if (!filename.isEmpty()) {
//     m_configFile.setFileName(filename);
//   }
// }

bool
Config::save()
{
  //  if (m_modified) {
  QFile file(m_configFilename);

  if (file.open((QFile::ReadWrite | QFile::Truncate))) {
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

    emitter << YAML::Key << LIBRARY_PATH << YAML::Value << m_libraryDirectory
            << YAML::Comment(
                 tr("Files will be stored here unless otherwise specified."));

    emitter
      << YAML::Key << SAVE_VERSION << YAML::Value << int(m_saveVersion)
      << YAML::Comment(
           tr("Unless specified 3.2. Possible values 3.0, 3.1, 3.2 and 3.3"));

    emitter << YAML::Key << HSPLITTER_SIZES << YAML::Value << m_hSplitterSizes;
    emitter << YAML::Key << VSPLITTER_SIZES << YAML::Value << m_vSplitterSizes;
    emitter << YAML::Key << ISPLITTER_SIZES << YAML::Value << m_iSplitterSizes;

    emitter << YAML::EndMap;

    if (!emitter.good()) {
      qWarning() << tr("Configuration Emitter error: ")
                 << QString::fromStdString(emitter.GetLastError()) << "\n";
    }

    QTextStream outStream(&file);
    outStream << emitter.c_str();
    file.close();

    if (emitter.good()) {
      m_modified = false;
      return true;
    }
  }
  //  }
  return false;
}

void
Config::load()
{
  QFile file(m_configFilename);
  if (file.exists()) {
    auto config = YAML::LoadFile(m_configFilename);

    if (config[SAVE_VERSION]) {
      auto node = config[SAVE_VERSION];
      m_saveVersion = SaveVersion(node.as<int>());
    }

    if (config[LIBRARY_PATH]) {
      auto node = config[LIBRARY_PATH];
      m_libraryDirectory = node.as<QString>();
    }

    if (config[STATUS_TIMEOUT]) {
      auto node = config[STATUS_TIMEOUT];
      m_statusTimeout = node.as<int>();
    }

    if (config[HSPLITTER_SIZES]) {
      auto node = config[HSPLITTER_SIZES];
      m_hSplitterSizes = node.as<QList<int>>();
    }
    if (config[VSPLITTER_SIZES]) {
      auto node = config[VSPLITTER_SIZES];
      m_vSplitterSizes = node.as<QList<int>>();
    }
  }
}

BCP47Languages*
Config::languages() const
{
  return m_languages;
}

QString
Config::optionsFile() const
{
  return m_optionsFilename;
}

void
Config::setOptionsFile(const QString& newOptionsFile)
{
  m_optionsFilename = newOptionsFile;
}

QString
Config::authorsFilename() const
{
  return m_authorsFilename;
}

void
Config::setAuthorsFilename(const QString& newAuthorsFile)
{
  m_authorsFilename = newAuthorsFile;
}

const QString&
Config::homeDirectory() const
{
  return m_homeDirectory;
}

void
Config::setHomeDirectory(const QString& newHome_directory)
{
  m_homeDirectory = newHome_directory;
}

const QString&
Config::libraryDirectory() const
{
  return m_libraryDirectory;
}

void
Config::setLibraryDirectory(const QString& newLibraryDirectory)
{
  m_libraryDirectory = newLibraryDirectory;
}

const QString&
Config::libraryFilename() const
{
  return m_libraryFilename;
}

void
Config::setLibraryFilename(const QString& newLibraryFilename)
{
  m_libraryFilename = newLibraryFilename;
}

const QString&
Config::configFilename() const
{
  return m_configFilename;
}

const QString&
Config::seriesFilename() const
{
  return m_seriesFilename;
}

void
Config::setSeriesFilename(const QString& newSeriesFilename)
{
  m_seriesFilename = newSeriesFilename;
}

const QList<int> &Config::hSplitterSizes() const
{
  return m_hSplitterSizes;
}

void Config::setHSplitterSizes(const QList<int> &newSplitterSizes)
{
  m_hSplitterSizes = newSplitterSizes;
}

const QList<int> &Config::vSplitterSizes() const
{
  return m_vSplitterSizes;
}

void Config::setVSplitterSizes(const QList<int> &newSplitterSizes)
{
  m_vSplitterSizes = newSplitterSizes;
}

const QList<int> &Config::iSplitterSizes() const
{
  return m_iSplitterSizes;
}

void Config::setISplitterSizes(const QList<int> &newSplitterSizes)
{
  m_iSplitterSizes = newSplitterSizes;
}

const QSize &Config::size() const
{
  return m_size;
}

void Config::setSize(const QSize &newSize)
{
  m_size = newSize;
}
