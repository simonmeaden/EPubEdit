#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QThread>

class BCP47Languages;

class Config : public QObject
{
public:
  Config(QObject* parent);
  ~Config();

  QString getConfigDir() const;
  void setConfigDir(const QString& value);
  QString getConfigFile() const;
  void setConfigFile(const QString& value);

  int getStatusTimeout() const;
  void setStatusTimeout(int value);

private:
  BCP47Languages* m_languages;
  QString m_configDir;
  QString m_configFile;

  int statusTimeout = 20; // timeout in seconds

  void saveLanguageFile();
};

#endif // CONFIG_H
