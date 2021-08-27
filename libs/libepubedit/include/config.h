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

  QString configDir() const;
  void setConfigDir(const QString& value);
  QString configFile() const;
  void setConfigFile(const QString& filepath);

  int statusTimeout() const;
  void setStatusTimeout(int value);

  SaveType saveVersion();
  void setSaveVersion(SaveType type);

  //! Returns the path to the directory in which files will be stored.
  QString libraryPath() const;
  //! Sets the directory in which files will be stored.
  //!
  //! By default this will be the standard place for the operating system.
  //!
  void setLibraryPath(const QString& filepath);

  void save(const QString& filename = QString());
  void load(const QString& filename = QString());

  static const int StatusTimeout = 20;

signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QDir m_libraryDir;
  QFile m_configFile;
  SaveType m_saveType;
  int m_statusTimeout; // timeout in seconds

  static const QString STATUS_TIMEOUT;
  static const QString SAVE_VERSION;
  static const QString LIBRARY_PATH;

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);
  void setupConfigFile(const QString& filename = QString());
};

#endif // CONFIG_H
