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
  enum SaveType
  {
    EPUB_3_0 = 30,
    EPUB_3_1 = 31,
    EPUB_3_2 = 32,
    EPUB_3_3 = 33,
  };
  Config(QObject* parent);
  ~Config();

  QString getConfigDir() const;
  void setConfigDir(const QString& value);
  QString getConfigFile() const;
  void setConfigFile(const QString& value);

  int statusTimeout() const;
  void setStatusTimeout(int value);
  SaveType saveVersion() { return SaveType(m_saveType); }
  void setSaveVersion(SaveType type) { m_saveType = type; }

signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

private:
  BCP47Languages* m_languages;
  QString m_configDir;
  QString m_configFile;
  SaveType m_saveType;

  int m_statusTimeout = 20; // timeout in seconds

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);
};

#endif // CONFIG_H
