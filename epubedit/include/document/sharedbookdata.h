#ifndef SHAREDBOOKDATA_H
#define SHAREDBOOKDATA_H

#include <QDateTime>
#include <QSharedData>
#include <QString>

class SharedBookData : public QSharedData
{
public:
  SharedBookData();
  SharedBookData(const SharedBookData& other);
  ~SharedBookData();

  quint64 m_uid;
  QString m_filename;
  QString m_title;
  QStringList m_creators;
  QString m_series;
  quint16 m_seriesIndex;
  QString m_currentSpineId;
  int m_currentSpineLineNo;
  bool m_modified;
  QDateTime m_dateLastRead;
  int m_readPosition;
  bool m_showContentsList;
  // webpage. stored here during editing (when loaded) NOT saved.
  QString page;
};

#endif // SHAREDBOOKDATA_H
