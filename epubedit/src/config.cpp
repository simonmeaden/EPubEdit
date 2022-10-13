#include "config.h"

#include "document/authors.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "language/languages.h"
#include "utilities/paths.h"

#include "document/bookpointers.h"

//====================================================================
//=== StringList
//====================================================================
void
LimitedStringList::append(const QString& str)
{
  appendIfNotIn(str);
}

void
LimitedStringList::appendIfNotIn(const QString& str)
{
  if (!contains(str)) {
    if (count() >= m_sizeLimit) {
      removeLast();
    }
    append(str);
  }
}

int
LimitedStringList::sizeLimit() const
{
  return m_sizeLimit;
}

void
LimitedStringList::setSizeLimit(int sizeLimit)
{
  m_sizeLimit = sizeLimit;
}

//====================================================================
//=== Config
//====================================================================

// default directory name for library.
const QString Config::DEFAULT_LIBRARY_DIRECTORY_NAME = "library";

QVector<QString> Config::VERSION_STRINGS = { "3.0.1",
                                             "3.1",
                                             "3.2",
                                             "3.3",
                                             "3.4" };

const QString Config::SAVE_VERSION = "save_version";
const QString Config::LIBRARY_PATH = "library_path";
const QString Config::WINDOW_GEOMETRY = "window_geometry";
const QString Config::MAINSPLITTER_SIZES = "main_splitter_sizes";
const QString Config::CENTRALSPLITTER_SIZES = "central_splitter_sizes";
const QString Config::EDITOR_SPLITTER_SIZES = "editor_splitter_sizes";
const QString Config::LEFT_SIDEBAR_VISIBLE = "show_left_sidebar";
const QString Config::RIGHT_SIDEBAR_VISIBLE = "show_right_sidebar";
const QString Config::INFO_VISIBLE = "show_info_view";
const QString Config::POSSIBLE_FILE_TYPES = "file_types";
const QString Config::RECENT_FILES = "recent_files";
const QString Config::CURRENT_STATUS = "current_status";
const QString Config::KEYMAP = "key_map";
const QString Config::STATUS_TIMEOUT = "status_timeout";

Config::Config(QObject* parent)
  : QObject(parent)
  , m_libraryDirectory(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      DEFAULT_LIBRARY_DIRECTORY_NAME))
  , m_libraryFilename(Paths::join(m_configDirectory, "library.yaml"))
  , m_configFilename(Paths::join(m_configDirectory, "epubedit.yaml"))
  , m_optionsFilename(Paths::join(m_configDirectory, "options.yaml"))
  , m_authorsFilename(Paths::join(m_configDirectory, "authors.yaml"))
  , m_seriesFilename(Paths::join(m_configDirectory, "series.yaml"))
  , m_saveVersion(EPUB_3_2)
{
  setDefaultkeyValuesIfNotSet();

  if (!m_fileTypes.contains("*.epub"))
    m_fileTypes << "*.epub";
}

Config::Config(BCP47Languages* languages, QObject* parent)
  : QObject(parent)
  , m_languages(languages)
  , m_libraryDirectory(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      DEFAULT_LIBRARY_DIRECTORY_NAME))
  , m_libraryFilename(Paths::join(m_configDirectory, "library.yaml"))
  , m_configFilename(Paths::join(m_configDirectory, "epubedit.yaml"))
  , m_optionsFilename(Paths::join(m_configDirectory, "options.yaml"))
  , m_authorsFilename(Paths::join(m_configDirectory, "authors.yaml"))
  , m_seriesFilename(Paths::join(m_configDirectory, "series.yaml"))
  , m_saveVersion(EPUB_3_2)
{
}

Config::~Config() {}

const QString&
Config::homeDirectory() const
{
  return m_homeDirectory;
}

void
Config::setHomeDirectory(const QString& homeDirectory)
{
  m_homeDirectory = homeDirectory;
}

BCP47Languages*
Config::languages() const
{
  return m_languages;
}

void
Config::setLanguages(BCP47Languages* Languages)
{
  m_languages = Languages;
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

Config::SaveVersion
Config::saveVersion()
{
  return SaveVersion(m_saveVersion);
}

void
Config::saveLanguageFile()
{
  QDir dir;
  dir.mkpath(m_configDirectory);
  m_languages->saveToLocalFile(dir.filePath("languages.yaml"));
  emit tr("Language files load completed.");
}

void
Config::receiveStatusMessage(const QString& message)
{
  emit sendStatusMessage(message, m_statusTimeout);
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
    m_saveVersion = version;
    m_modified = true;
  }
}

QVector<QString>
Config::versions()
{
  QVector<QString> list;
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

bool
Config::save(const QString& /*filename*/)
{
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

    emitter << YAML::Key << WINDOW_GEOMETRY << YAML::Value << m_windowGeometry;

    emitter << YAML::Key << MAINSPLITTER_SIZES << YAML::Value
            << m_mainSplitterSizes;
    emitter << YAML::Key << CENTRALSPLITTER_SIZES << YAML::Value
            << m_centralSplitterSizes;
    emitter << YAML::Key << EDITOR_SPLITTER_SIZES << YAML::Value
            << m_editorSplitterSizes;

    emitter << YAML::Key << LEFT_SIDEBAR_VISIBLE << YAML::Value
            << m_leftSidebarVisible;
    emitter << YAML::Key << RIGHT_SIDEBAR_VISIBLE << YAML::Value
            << m_rightSidebarVisible;
    emitter << YAML::Key << INFO_VISIBLE << YAML::Value << m_infoIsVisible;
    emitter << YAML::Key << POSSIBLE_FILE_TYPES << YAML::Value << m_fileTypes;
    emitter << YAML::Key << RECENT_FILES << YAML::Value << m_recentFiles;
    //    emitter << YAML::Key << CURRENT_STATUS << YAML::Value << m_fileData;

    emitter << YAML::Key << KEYMAP << YAML::Value << m_keyMap;

    emitter << YAML::EndMap;

    if (!emitter.good()) {
      qWarning() << tr("Configuration Emitter error: ")
                 << QString::fromStdString(emitter.GetLastError()) << "\n";
    }

    QTextStream out(&file);
    out << emitter.c_str();
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
Config::setDefaultkeyValuesIfNotSet()
{
  if (!m_keyMap.value(PreviousBookmark).isValid()) {
    m_keyMap.insert(PreviousBookmark,
                    KeyMapper(Qt::Key_Comma, Qt::ControlModifier));
  }
  if (!m_keyMap.value(NextBookmark).isValid()) {
    m_keyMap.insert(NextBookmark, KeyMapper(Qt::Key_Stop, Qt::ControlModifier));
  }
  if (!m_keyMap.value(AddBookmark).isValid()) {
    m_keyMap.insert(AddBookmark, KeyMapper(Qt::Key_M, Qt::ControlModifier));
  }
  if (!m_keyMap.value(ToggleBookmark).isValid()) {
    m_keyMap.insert(
      ToggleBookmark,
      KeyMapper(Qt::Key_M, Qt::ControlModifier | Qt::AltModifier));
  }
  if (!m_keyMap.value(NextBookmark).isValid()) {
    m_keyMap.insert(
      NextBookmark,
      KeyMapper(Qt::Key_M, Qt::ControlModifier | Qt::ShiftModifier));
  }
}

bool
Config::load(const QString& /*filename*/)
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

    if (config[WINDOW_GEOMETRY]) {
      auto node = config[WINDOW_GEOMETRY];
      m_windowGeometry = node.as<QByteArray>();
    }

    if (config[MAINSPLITTER_SIZES]) {
      auto node = config[MAINSPLITTER_SIZES];
      m_mainSplitterSizes = node.as<QVector<int>>();
    }

    if (config[CENTRALSPLITTER_SIZES]) {
      auto node = config[CENTRALSPLITTER_SIZES];
      m_centralSplitterSizes = node.as<QVector<int>>();
    }

    if (config[EDITOR_SPLITTER_SIZES]) {
      auto node = config[EDITOR_SPLITTER_SIZES];
      m_editorSplitterSizes = node.as<QVector<QVector<int>>>();
    }

    if (config[LEFT_SIDEBAR_VISIBLE]) {
      auto node = config[LEFT_SIDEBAR_VISIBLE];
      m_leftSidebarVisible = node.as<bool>();
    }

    if (config[RIGHT_SIDEBAR_VISIBLE]) {
      auto node = config[RIGHT_SIDEBAR_VISIBLE];
      m_rightSidebarVisible = node.as<bool>();
    }

    if (config[INFO_VISIBLE]) {
      auto node = config[INFO_VISIBLE];
      m_infoIsVisible = node.as<bool>();
    }

    if (config[POSSIBLE_FILE_TYPES]) {
      auto node = config[POSSIBLE_FILE_TYPES];
      m_fileTypes = node.as<QVector<QString>>();
    }

    if (config[RECENT_FILES]) {
      auto node = config[RECENT_FILES];
      m_recentFiles = node.as<QVector<QString>>();
    }

    if (config[KEYMAP]) {
      auto node = config[KEYMAP];
      if (node.IsMap()) {
        m_keyMap = node.as<QMap<KeyEventMapper, KeyMapper>>();
      }
    }

    //    if (config[CURRENT_STATUS]) {
    //      auto node = config[CURRENT_STATUS];
    ////      m_recentFiles = node.as<QMap<QString, FileData*>>();
    //    }
    return true;
  }
  return false;
}

QString
Config::optionsFile() const
{
  return m_optionsFilename;
}

void
Config::setOptionsFile(const QString& optionsFile)
{
  m_optionsFilename = optionsFile;
}

QString
Config::authorsFilename() const
{
  return m_authorsFilename;
}

void
Config::setAuthorsFilename(const QString& authorsFile)
{
  m_authorsFilename = authorsFile;
}

const QString&
Config::libraryDirectory() const
{
  return m_libraryDirectory;
}

void
Config::setLibraryDirectory(const QString& libraryDirectory)
{
  m_libraryDirectory = libraryDirectory;
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
Config::setSeriesFilename(const QString& seriesFilename)
{
  m_seriesFilename = seriesFilename;
}

const QVector<int>&
Config::mainSplitterSizes() const
{
  return m_mainSplitterSizes;
}

void
Config::setMainSplitterSizes(const QVector<int>& splitterSizes)
{
  m_mainSplitterSizes = splitterSizes;
}

const QVector<int>&
Config::centralSplitterSizes() const
{
  return m_centralSplitterSizes;
}

void
Config::setCentralSplitterSizes(const QVector<int>& sizes)
{
  m_centralSplitterSizes = sizes;
}

const QVector<QVector<int>>&
Config::editorSplitterSizes() const
{
  return m_editorSplitterSizes;
}

void
Config::setEditorSplitterSizes(const QVector<QVector<int>>& sizes)
{
  m_editorSplitterSizes = sizes;
}

// const QSize&
// Config::size() const
//{
//   return m_windowRect.size();
// }

// void
// Config::setSize(const QSize& size)
//{
//   m_windowRect.setSize(size);
// }

const QByteArray&
Config::windowGeometry() const
{
  return m_windowGeometry;
}

void
Config::setWindowGeometry(const QByteArray& geometry)
{
  m_windowGeometry = geometry;
}

void
Config::setKeyMapping(KeyEventMapper mapping, KeyMapper value)
{
  m_keyMap.insert(mapping, value);
}

KeyMapper
Config::keyMapping(KeyEventMapper mapping)
{
  return m_keyMap.value(mapping);
}

bool
Config::hasKeyMapping(KeyEventMapper mapping)
{
  if (m_keyMap.contains(mapping))
    return true;
  return false;
}

QMap<KeyEventMapper, KeyMapper>
Config::keyMap()
{
  return m_keyMap;
}

bool Config::isModified() const
{
  return m_modified;
}

bool
Config::isLeftSidebarVisible() const
{
  return m_leftSidebarVisible;
}

void
Config::setLeftSidebarVisible(bool leftSidebarVisible)
{
  m_leftSidebarVisible = leftSidebarVisible;
}

bool
Config::isRightSidebarVisible() const
{
  return m_rightSidebarVisible;
}

void
Config::setRightSidebarVisible(bool leftSidebarVisible)
{
  m_rightSidebarVisible = leftSidebarVisible;
}

bool
Config::infoIsVisible() const
{
  return m_infoIsVisible;
}

void
Config::setInfoIsVisible(bool infoIsVisible)
{
  m_infoIsVisible = infoIsVisible;
}

const QVector<QString>&
Config::zipFileList() const
{
  return m_zipFileList;
}

void
Config::setZipFileList(const QVector<QString>& zipFileList)
{
  m_zipFileList = zipFileList;
}

const QString&
Config::currentFilename() const
{
  return m_currentFilename;
}

void
Config::setCurrentFilename(const QString& currentFilename)
{
  m_currentFilename = currentFilename;
}

const QVector<QString>&
Config::fileTypes() const
{
  return m_fileTypes;
}

void
Config::addFileType(const QString& type)
{
  m_fileTypes.append(type);
}
