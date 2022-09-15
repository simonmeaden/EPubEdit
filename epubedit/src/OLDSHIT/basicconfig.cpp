#include "basicconfig.h"
#include "language/languages.h"
#include "utilities/paths.h"

const QString BasicConfig::STATUS_TIMEOUT = "status_timeout";

BasicConfig::BasicConfig(QObject* parent)
  : QObject{ parent }
  , m_configDirectory(
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
  , m_homeDirectory(
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation))
  , m_statusTimeout(StatusTimeout)
{
  m_languages = new BCP47Languages();
  connect(m_languages,
          &BCP47Languages::sendMessage,
          this,
          &BasicConfig::receiveStatusMessage);
  connect(m_languages,
          &BCP47Languages::parsingError,
          this,
          &BasicConfig::sendLogMessage);
  QDir dir;
  dir.mkpath(m_configDirectory);
  m_languages->readFromLocalFile(
    Paths::join(m_configDirectory, "languages.yaml"));
}

BasicConfig::~BasicConfig() {}

void
BasicConfig::setConfigDirectory(const QString& ConfigDirectory)
{
  m_configDirectory = ConfigDirectory;
}

const QString&
BasicConfig::homeDirectory() const
{
  return m_homeDirectory;
}

void
BasicConfig::setHomeDirectory(const QString& homeDirectory)
{
  m_homeDirectory = homeDirectory;
}

BCP47Languages*
BasicConfig::languages() const
{
  return m_languages;
}

void
BasicConfig::saveLanguageFile()
{
  QDir dir;
  dir.mkpath(m_configDirectory);
  m_languages->saveToLocalFile(dir.filePath("languages.yaml"));
  emit tr("Language files load completed.");
}

void
BasicConfig::receiveStatusMessage(const QString& message)
{
  emit sendStatusMessage(message, m_statusTimeout);
}

int
BasicConfig::statusTimeout() const
{
  return m_statusTimeout;
}

void
BasicConfig::setStatusTimeout(int value)
{
  if (value >= 0) {
    m_statusTimeout = value;
    m_modified = true;
  }
}
