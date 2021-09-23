#ifndef SERIES_H
#define SERIES_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QCoreApplication>

#include "qyamlcpp.h"

class EBookOptions;

class EBookSeriesData
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  EBookSeriesData();
  ~EBookSeriesData();

  quint64 uid() const;
  void setUid(const quint64& uid);

  QString name() const;
  void setName(const QString& name);

  QStringList wordList() const;
  void setSeriesWords(const QStringList& wordList);

  static quint64 m_highest_uid;
  static quint64 nextUid() { return ++m_highest_uid; }

protected:
  quint64 m_uid;
  QString m_name;
  QStringList m_series_words;
};
typedef QSharedPointer<EBookSeriesData> SeriesData;
typedef QMap<quint64, SeriesData> SeriesMap;
typedef QMap<QString, SeriesData> SeriesByString;
typedef QStringList SeriesList;

class EBookSeriesDB : public QObject
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  explicit EBookSeriesDB(EBookOptions* options);
  EBookSeriesDB(const EBookSeriesDB& other);
  ~EBookSeriesDB();

  // yaml file stuff
  void setFilename(QString filename);
  bool save();
  bool load(QString filename);

  // series data stuff
  quint64 insertOrGetSeries(QString series);
  void insertSeries(SeriesData series_data);
  bool removeSeries(quint64 index);

  SeriesMap seriesMap() { return m_series_map; }
  SeriesList seriesList();
  SeriesData series(quint64 uid);
  SeriesData seriesByName(QString name);

protected:
  EBookOptions* m_options;
  QString m_filename;
  bool m_series_changed;
  SeriesMap m_series_map;
  SeriesByString m_series_by_name;
  SeriesList m_series_list;

  bool loadSeries();
  bool saveSeries();

  static quint64 m_highest_uid;
};
typedef QSharedPointer<EBookSeriesDB> SeriesDB;

#endif // SERIES_H
