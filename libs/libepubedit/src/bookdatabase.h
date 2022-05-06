#ifndef BOOKDATABASE_H
#define BOOKDATABASE_H

#include <QObject>
#include <QDateTime>
#include <QObject>
#include <QSharedData>
#include <QString>

class EBookData : public QSharedData
{
public:
  EBookData()
    : m_id(-1)
  {}
  EBookData(const EBookData& other)
    : QSharedData(other)
    , m_id(other.m_id)
    , m_lastRead(other.m_lastRead)
    , m_filename(other.m_filename)
    , m_title(other.m_title)
    , m_position(other.m_position)
    , m_showContents(other.m_showContents)
  {}
  ~EBookData() {}

private:
  int m_id;
  QDateTime m_lastRead;
  QString m_filename;
  QString m_title;
  int m_position;
  bool m_showContents;
};

class BookDatabase : public QObject
{
  Q_OBJECT
public:
  explicit BookDatabase(QObject* parent = nullptr);

signals:

private:
};

#endif // BOOKDATABASE_H
