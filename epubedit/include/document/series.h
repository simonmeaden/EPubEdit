#ifndef SERIES_H
#define SERIES_H

#include <QCoreApplication>
#include <QFile>
#include <QObject>
#include <QTextStream>

#include <algorithm>

#include "document/bookpointers.h"

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

  static quint64 m_highestUid;
  static quint64 nextUid() { return ++m_highestUid; }
  static void setMaxUid(quint64 seriesUid);

protected:
  quint64 m_uid;
  QString m_name;
  QStringList m_seriesWords;
};

class EBookSeriesDB : public QObject
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  explicit EBookSeriesDB(POptions options, QObject* parent = nullptr);
  EBookSeriesDB(const EBookSeriesDB& other);
  ~EBookSeriesDB();

  // yaml file stuff
  //  void setFilename(QString filename);
  bool save(const QString& filename);
  bool load(const QString& filename);

  // series data stuff
  quint64 insertOrGetSeries(QString series);
  void insertSeries(PSeriesData series_data);
  bool removeSeries(quint64 index);

  SeriesMap seriesMap() { return m_seriesMap; }
  SeriesList seriesList();
  PSeriesData series(quint64 uid);
  PSeriesData seriesByName(QString name);

protected:
  POptions m_options;
  bool m_seriesChanged;
  SeriesMap m_seriesMap;
  SeriesByString m_seriesByName;
  SeriesList m_seriesList;

  bool loadSeries(const QString& filename);
  bool saveSeries(const QString& filename);

//  static quint64 m_highestUid;
//  static quint64 nextUid();
};

#endif // SERIES_H
