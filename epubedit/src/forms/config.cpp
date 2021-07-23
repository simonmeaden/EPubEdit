#include "config.h"

#include "languages.h"

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDir(
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
      "/epubedit")
  , m_configFile("epubedit.yaml")
{
  m_languages = new BCP47Languages();
  QDir dir;
  dir.mkpath(m_configDir);
  auto file = QFile(dir.filePath("languages.yaml"));
  //  QFileInfo info(dir.filePath("languages.yaml"));
  //  info.absoluteFilePath();
  if (file.exists()) {
    m_languages->readFromLocalFile(file);
  } else {
    auto languagesThread = new QThread();
    connect(
      m_languages, &BCP47Languages::completed, languagesThread, &QThread::quit);
    connect(m_languages,
            &BCP47Languages::completed,
            languagesThread,
            &QThread::deleteLater);
    connect(
      m_languages, &BCP47Languages::completed, this, &Config::saveLanguageFile);
    connect(languagesThread,
            &QThread::started,
            m_languages,
            &BCP47Languages::rebuildFromRegistry);
    languagesThread->start();
    m_languages->moveToThread(languagesThread);
  }
}

Config::~Config() {}

void
Config::saveLanguageFile()
{
  QDir dir;
  dir.mkpath(getConfigDir());
  auto file = QFile(dir.filePath("languages.yaml"));
  m_languages->saveToLocalFile(file);
}

QString
Config::getConfigDir() const
{
  return m_configDir;
}

void
Config::setConfigDir(const QString& value)
{
  m_configDir = value;
}

int
Config::getStatusTimeout() const
{
  return statusTimeout;
}

void
Config::setConfigFile(const QString& value)
{
  m_configFile = value;
}

void
Config::setStatusTimeout(int value)
{
  statusTimeout = value;
}

QString
Config::getConfigFile() const
{
  return m_configFile;
}
