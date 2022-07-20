#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QList>
#include <QMap>
#include <QSize>
#include <QStandardPaths>
#include <QString>
#include <QStringList>

class BCP47Languages;

class LimitedStringList : public QList<QString>
{
public:
  void append(const QString& str);
  void appendIfNotIn(const QString& str);

  int sizeLimit() const;
  void setSizeLimit(int sizeLimit);

private:
  int m_sizeLimit = 10;
};

enum FileDataType
{
  Code,
  Html,
};

struct FileData
{
  QString filename;
};

class Config : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool rightSidebarVisible READ isRightSidebarVisible WRITE
               setRightSidebarVisible)
  Q_PROPERTY(bool leftSidebarVisible READ isLeftSidebarVisible WRITE
               setLeftSidebarVisible)

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

  const QList<int>& mainSplitterSizes() const;
  void setMainSplitterSizes(const QList<int>& newSplitterSizes);

  const QList<int>& centralSplitterSizes() const;
  void setCentralSplitterSizes(const QList<int>& sizes);

  const QList<QList<int>>& editorSplitterSizes() const;
  void setEditorSplitterSizes(const QList<QList<int>>& sizes);

  const QSize& size() const;
  void setSize(const QSize& newSize);

  bool isLeftSidebarVisible() const;
  void setLeftSidebarVisible(bool newLeftSidebarVisible);

  bool isRightSidebarVisible() const;
  void setRightSidebarVisible(bool newLeftSidebarVisible);

  bool infoIsVisible() const;
  void setInfoIsVisible(bool newInfoIsVisible);

  const QStringList& zipFileList() const;
  void setZipFileList(const QStringList& newZipFileList);

  const QString& currentFilename() const;
  void setCurrentFilename(const QString& newCurrentFilename);

  //! Returns those ebook formats that can be loaded.
  //!
  //! By default only epub files are permitted.
  const QStringList& fileTypes() const;
  //! Allows the user to add extra file types to load.
  //!
  //! By default only epub files are permitted.
  void addFileType(const QString& type);

  constexpr inline QMap<QString, FileData*>& fileData() { return m_fileData; }
  constexpr inline QStringList& recentFiles() { return m_recentFiles; }

  static const int StatusTimeout = 20;

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
  QStringList m_fileTypes;

  SaveVersion m_saveVersion;
  int m_statusTimeout; // timeout in seconds
  bool m_modified;

  QSize m_size;
  QList<int> m_mainSplitterSizes, m_centralSplitterSizes;
  QList<QList<int>> m_editorSplitterSizes;
  bool m_leftSidebarVisible = true;
  bool m_rightSidebarVisible = true;
  bool m_infoIsVisible = true;

  QStringList m_zipFileList;
  QString m_currentFilename;

  QStringList m_recentFiles;
  QMap<QString, FileData*> m_fileData;

  static const QString DEFAULT_LIBRARY_DIRECTORY_NAME;
  static QStringList VERSION_STRINGS;
  static const QString STATUS_TIMEOUT;
  static const QString SAVE_VERSION;
  static const QString LIBRARY_PATH;
  static const QString MAINSPLITTER_SIZES;
  static const QString CENTRALSPLITTER_SIZES;
  static const QString RIGHT_SIDEBAR_VISIBLE;
  static const QString LEFT_SIDEBAR_VISIBLE;
  static const QString INFO_VISIBLE;
  static const QString EDITOR_SPLITTER_SIZES;
  static const QString POSSIBLE_FILE_TYPES;
  static const QString RECENT_FILES;
  static const QString CURRENT_STATUS;

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);
};

#endif // CONFIG_H
