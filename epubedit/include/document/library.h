#ifndef LIBRARY_H
#define LIBRARY_H

#include <QDateTime>
#include <QFile>
#include <QObject>
#include <QString>
#include <QTextStream>

#include "document/sharedbookdata.h"
#include "document/bookpointers.h"
#include "qyamlcpp.h"

class EBookOptions;
class SharedBookData;

class BookData
{
public:
  BookData();
  BookData(quint64 uid,
           const QString& filename,
           const QString& title,
           QStringList authors);
  BookData(quint64 uid,
           const QString& filename,
           const QString& title,
           QStringList authors,
           const QString& series,
           quint16 index);
  BookData(const BookData& other);

  quint64 uid();
  void setUId(quint64 uid);
  QString filename();
  void setFilename(const QString& filename);
  QString title();
  void setTitle(const QString& title);
  QStringList creators();
  void setCreators(QStringList authors);
  QString series();
  void setSeries(const QString& series);
  quint16 seriesIndex();
  //  void setSeriesIndex(QString index);
  void setSeriesIndex(quint16 index);
  QString currentSpineId() const;
  void setCurrentSpineId(QString id);
  quint16 currentSpineLineNo();
  void setCurrentSpineLineNo(quint8 lineNo);
  bool modified();
  void setModified(bool modified);
  QDateTime dateLastRead();
  void setDateLastRead(QDateTime dateLastRead);
  quint64 readPosition();
  void setReadPosition(quint64 readPosition);
  bool showContentsList();
  void setShowContentsList(bool showContentsList);

  // temporary storage of webpage??
  QString page();
  void setPage(const QString& page);

  bool isValid();

  static quint64 highestUid;
  static quint64 nextUid();

private:
  QSharedDataPointer<SharedBookData> d;
};

class EBookOptions;

class EBookLibraryDB : public QObject
{
public:
  explicit EBookLibraryDB(POptions options, QObject* parent=nullptr);
  ~EBookLibraryDB();

  // yaml file stuff
  bool save(const QString& filename);
  bool load(const QString& filename);

  // book stuff.
  quint64 insertOrUpdateBook(PBookData bookData);
  bool removeBook(quint64 index);

  PBookData bookByUid(quint64 uid);
  BookList bookByTitle(QString title);
  PBookData bookByFile(QString filename);

  QString currentBookId(QString filename);

  bool isModified();
  void setModified(bool modified);

  static const QString FILENAME;
  static const QString TITLE;
  static const QString CREATORS;
  static const QString SERIES_ID;
  static const QString SERIES_INDEX;
  static const QString CURRENT_SPINE_ID;
  static const QString SPINE_LINENO;
  static const QString LAST_READ;
  static const QString READ_POSITION;
  static const QString SHOW_CONTENTS;

private:
  POptions m_options;
  QString m_filename;
  BookByUInt m_bookData;
  BookByString m_bookByTitle;
  BookByString m_bookByFile;

  bool m_modified;

  bool loadLibrary(const QString& filename);
  bool saveLibrary(const QString& filename);
};

#endif // LIBRARY_H
