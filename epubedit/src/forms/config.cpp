#include "config.h"

#include "languages.h"

Config::Config(QObject* parent)
  : QObject(parent)
  , configDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
              "/epubedit")
  , configFile("epubedit.yaml")
{
  m_languages = new BCP47Languages(this);
  QDir dir;
  dir.mkpath(getConfigDir());
  auto file = QFile(dir.filePath(getConfigFile()));
  if (file.exists()) {
    m_languages->readLocalFile(file);
  } else {
    auto processLanguages = new QThread();
    connect(m_languages,
            &BCP47Languages::completed,
            processLanguages,
            &QThread::quit);
    connect(m_languages,
            &BCP47Languages::completed,
            processLanguages,
            &QThread::deleteLater);
    connect(
      m_languages, &BCP47Languages::completed, this, &Config::saveLanguageFile);
    connect(processLanguages,
            &QThread::started,
            m_languages,
            &BCP47Languages::rebuildFromRegistry);
    processLanguages->start();
    m_languages->moveToThread(processLanguages);
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
  return configDir;
}

void
Config::setConfigDir(const QString& value)
{
  configDir = value;
}

int
Config::getStatusTimeout() const
{
  return statusTimeout;
}

void
Config::setConfigFile(const QString& value)
{
  configFile = value;
}

void
Config::setStatusTimeout(int value)
{
  statusTimeout = value;
}

QString
Config::getConfigFile() const
{
  return configFile;
}
