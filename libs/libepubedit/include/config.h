#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QSize>

class BCP47Languages;

class Config : public QObject
{
  Q_OBJECT
public:
  enum SaveVersion
  {
    EPUB_3_0 = 30,
    EPUB_3_1 = 31,
    EPUB_3_2 = 32,
    EPUB_3_3 = 33,
    EPUB_3_4 = 34,
  };
  Config(QObject* parent = nullptr);
  ~Config();

  QString configDir() const;
  void setConfigDir(const QString& value);
  //  QString configFile() const;

  int statusTimeout() const;
  void setStatusTimeout(int value);

  SaveVersion saveVersion();
  void setSaveVersion(SaveVersion type);
  void setSaveVersion(const QString versionStr);
  static QStringList versions();
  static SaveVersion version(const QString& versionStr);
  QString versionToString();

  //! Returns the path to the directory in which files will be stored.
  QString libraryPath() const;
  //! Sets the directory in which files will be stored.
  //!
  //! By default this will be the standard place for the operating system.
  //!
  void setLibraryPath(const QString& filepath);

  bool save();
  void load();

  static const int StatusTimeout = 20;

  BCP47Languages* languages() const;

  QString optionsFile() const;
  void setOptionsFile(const QString& newOptionsFile);

  QString authorsFilename() const;
  void setAuthorsFilename(const QString& newAuthorsFile);

  const QString& homeDirectory() const;
  void setHomeDirectory(const QString& newHome_directory);

  const QString& libraryDirectory() const;
  void setLibraryDirectory(const QString& newLibraryDirectory);

  const QString& libraryFilename() const;
  void setLibraryFilename(const QString& newLibraryFilename);

  const QString& configFilename() const;

  const QString& seriesFilename() const;
  void setSeriesFilename(const QString& newSeriesFilename);

  const QList<int>& hSplitterSizes() const;
  void setHSplitterSizes(const QList<int>& newSplitterSizes);

  const QList<int>& vSplitterSizes() const;
  void setVSplitterSizes(const QList<int>& newSplitterSizes);

  const QList<int> &iSplitterSizes() const;
  void setISplitterSizes(const QList<int> &newSplitterSizes);

  const QSize &size() const;
  void setSize(const QSize &newSize);

signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

private:
  BCP47Languages* m_languages;
  QString m_configDirectory;
  QString m_homeDirectory;
  QString m_libraryDirectory;
  QString m_libraryFilename;
  QString m_configFilename;
  QString m_optionsFilename;
  QString m_authorsFilename;
  QString m_seriesFilename;
  QString m_dic_directory;
  QString m_bdic_directory;

  SaveVersion m_saveVersion;
  int m_statusTimeout; // timeout in seconds
  bool m_modified;

  QSize m_size;
  QList<int> m_hSplitterSizes, m_vSplitterSizes, m_iSplitterSizes;

  static const QString DEFAULT_LIBRARY_DIRECTORY_NAME;
  static QStringList VERSION_STRINGS;
  static const QString STATUS_TIMEOUT;
  static const QString SAVE_VERSION;
  static const QString LIBRARY_PATH;
  static const QString HSPLITTER_SIZES;
  static const QString VSPLITTER_SIZES;
  static const QString ISPLITTER_SIZES;

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);
  //  void setupConfigFile(const QString& filename = QString());
};

#endif // CONFIG_H
