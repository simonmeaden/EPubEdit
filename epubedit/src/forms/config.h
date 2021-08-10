#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
//#include <QThread>

class BCP47Languages;

class Config : public QObject
{
  Q_OBJECT
public:
  Config(QObject* parent);
  ~Config();

  QString getConfigDir() const;
  void setConfigDir(const QString& value);
  QString getConfigFile() const;
  void setConfigFile(const QString& value);

  int getStatusTimeout() const;
  void setStatusTimeout(int value);

signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

private:
  BCP47Languages* m_languages;
  QString m_configDir;
  QString m_configFile;

  int m_statusTimeout = 20; // timeout in seconds

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);
};

#endif // CONFIG_H
