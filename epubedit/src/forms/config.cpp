#include "config.h"

#include "languages.h"

Config::Config(QObject* parent)
  : QObject(parent)
  , m_configDir(
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
      "/epubedit")
  , m_configFile("epubedit.yaml")
  , m_saveType(EPUB_3_2)
{
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
  dir.mkpath(getConfigDir());
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
  m_statusTimeout = value;
}

QString
Config::getConfigFile() const
{
  return m_configFile;
}
