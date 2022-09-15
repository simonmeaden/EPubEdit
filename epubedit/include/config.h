#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QFile>
#include <QMap>
#include <QRect>
#include <QStandardPaths>
#include <QString>
#include <QVector>

class BCP47Languages;

#include "utilities/keymap.h"
#include "widgets/iepubeditor.h"

class LimitedStringList : public QVector<QString>
{
public:
  void append(const QString& str);
  void appendIfNotIn(const QString& str);

  int sizeLimit() const;
  void setSizeLimit(int sizeLimit);

private:
  int m_sizeLimit = 10;
};

struct FileData
{
  QString href;
  IEPubEditor::Type editorType;
  int currentPosition;
};

class Config : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool rightSidebarVisible READ isRightSidebarVisible WRITE
               setRightSidebarVisible)
  Q_PROPERTY(bool leftSidebarVisible READ isLeftSidebarVisible WRITE
               setLeftSidebarVisible)
  Q_PROPERTY(QString libraryPath READ libraryPath WRITE setLibraryPath)

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
  Config(BCP47Languages* languages, QObject* parent = nullptr);
  ~Config();

  const QString& homeDirectory() const;
  void setHomeDirectory(const QString& homeDirectory);

  BCP47Languages* languages() const;
  void setLanguages(BCP47Languages *Languages);

  int statusTimeout() const;
  void setStatusTimeout(int value);

  QString configDir() const;
  void setConfigDir(const QString& value);

  SaveVersion saveVersion();
  void setSaveVersion(SaveVersion type);
  void setSaveVersion(const QString versionStr);
  static QVector<QString> versions();
  static SaveVersion version(const QString& versionStr);
  QString versionToString();

  //! Returns the path to the directory in which files will be stored.
  QString libraryPath() const;
  //! Sets the directory in which files will be stored.
  //!
  //! By default this will be the standard place for the operating system.
  //!
  void setLibraryPath(const QString& filepath);

  bool save(const QString& filename = QString());
  bool load(const QString& filename = QString());

  QString optionsFile() const;
  void setOptionsFile(const QString& optionsFile);

  QString authorsFilename() const;
  void setAuthorsFilename(const QString& authorsFile);

  const QString& libraryDirectory() const;
  void setLibraryDirectory(const QString& libraryDirectory);

  const QString& libraryFilename() const;
  void setLibraryFilename(const QString& libraryFilename);

  const QString& configFilename() const;

  const QString& seriesFilename() const;
  void setSeriesFilename(const QString& seriesFilename);

  const QVector<int>& mainSplitterSizes() const;
  void setMainSplitterSizes(const QVector<int>& splitterSizes);

  const QVector<int>& centralSplitterSizes() const;
  void setCentralSplitterSizes(const QVector<int>& sizes);

  const QVector<QVector<int>>& editorSplitterSizes() const;
  void setEditorSplitterSizes(const QVector<QVector<int>>& sizes);

  bool isLeftSidebarVisible() const;
  void setLeftSidebarVisible(bool leftSidebarVisible);

  bool isRightSidebarVisible() const;
  void setRightSidebarVisible(bool rightSidebarVisible);

  bool infoIsVisible() const;
  void setInfoIsVisible(bool infoIsVisible);

  const QVector<QString>& zipFileList() const;
  void setZipFileList(const QVector<QString>& zipFileList);

  const QString& currentFilename() const;
  void setCurrentFilename(const QString& currentFilename);

  //! Returns those ebook formats that can be loaded.
  //!
  //! By default only epub files are permitted.
  const QVector<QString>& fileTypes() const;
  //! Allows the user to add extra file types to load.
  //!
  //! By default only epub files are permitted.
  void addFileType(const QString& type);

  constexpr inline QMap<QString, FileData*>& fileData() { return m_fileData; }
  constexpr inline QVector<QString>& recentFiles() { return m_recentFiles; }

  const QByteArray& windowGeometry() const;
  void setWindowGeometry(const QByteArray& geometry);

  void setKeyMapping(KeyEventMapper mapping, KeyMapper value);
  KeyMapper keyMapping(KeyEventMapper mapping);
  bool hasKeyMapping(KeyEventMapper mapping);
  QMap<KeyEventMapper, KeyMapper> keyMap();

  static const int StatusTimeout = 20;


signals:
  void sendStatusMessage(const QString& message, int timeout);
  void sendLogMessage(const QString& message);

private:
  QString m_configDirectory;
  QString m_homeDirectory;
  BCP47Languages* m_languages;
  QString m_libraryDirectory;
  QString m_libraryFilename;
  QString m_configFilename;
  QString m_optionsFilename;
  QString m_authorsFilename;
  QString m_seriesFilename;
  QString m_dic_directory;
  QString m_bdic_directory;
  QVector<QString> m_fileTypes;

  SaveVersion m_saveVersion;

  QByteArray m_windowGeometry;
  QVector<int> m_mainSplitterSizes, m_centralSplitterSizes;
  QVector<QVector<int>> m_editorSplitterSizes;
  bool m_leftSidebarVisible = true;
  bool m_rightSidebarVisible = true;
  bool m_infoIsVisible = true;
  int m_statusTimeout; // timeout in seconds
  bool m_modified = false;

  QVector<QString> m_zipFileList;
  QString m_currentFilename;

  QVector<QString> m_recentFiles;
  QMap<QString, FileData*> m_fileData;

  QMap<KeyEventMapper, KeyMapper> m_keyMap;

  void saveLanguageFile();
  void receiveStatusMessage(const QString& message);


  static const QString DEFAULT_LIBRARY_DIRECTORY_NAME;
  static QVector<QString> VERSION_STRINGS;
  static const QString SAVE_VERSION;
  static const QString LIBRARY_PATH;
  static const QString WINDOW_GEOMETRY;
  static const QString MAINSPLITTER_SIZES;
  static const QString CENTRALSPLITTER_SIZES;
  static const QString RIGHT_SIDEBAR_VISIBLE;
  static const QString LEFT_SIDEBAR_VISIBLE;
  static const QString INFO_VISIBLE;
  static const QString EDITOR_SPLITTER_SIZES;
  static const QString POSSIBLE_FILE_TYPES;
  static const QString RECENT_FILES;
  static const QString CURRENT_STATUS;
  static const QString KEYMAP;

  static const QString STATUS_TIMEOUT;

  void setDefaultkeyValuesIfNotSet();
};

#endif // CONFIG_H
