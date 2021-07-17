#include "config.h"

Config::Config()
  : configDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) +
              "/epubedit")
  , configFile("epubedit.yaml")
{}

Config::~Config() {}

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
