#ifndef UNIQUESTRING_H
#define UNIQUESTRING_H

#include <QList>
#include <QString>

class UniqueString
{
public:
  UniqueString();
  UniqueString(const QString& other, bool unique);
  ~UniqueString();

  bool isUnique() const;
  void setUnique(bool unique);

  inline QString toString() const { return m_data; }
  inline bool isEmpty() const { return m_data.isEmpty(); }

  inline void operator=(const QString& rhs) { m_data = rhs; }

  inline bool operator==(const QString& rhs) { return (m_data == rhs); }
  inline bool operator==(const char* rhs) const { return (m_data == rhs); }
  inline bool operator==(const UniqueString& rhs)
  {
    return (m_data == rhs.m_data);
  }
  friend bool operator==(const QString&, const UniqueString&);
  friend bool operator==(const UniqueString&, const QString&);
  friend bool operator==(const UniqueString&, const UniqueString&);

  inline bool operator<(const UniqueString& rhs)
  {
    return (m_data < rhs.m_data);
  }
  friend bool operator<(const UniqueString&, const UniqueString&);
  inline bool operator<(const QString& rhs) { return (m_data < rhs); }
  inline bool operator<(const char* rhs) { return (m_data < rhs); }

private:
  QString m_data;
  bool m_unique = true;
};

class UniqueStringList
{
public:
  UniqueStringList();
  ~UniqueStringList();

  UniqueString append(const QString& value, int lineNumber);

  QStringList nonUniqueStringList();
  QStringList uniqueList() const;
  QStringList allValues() const;
  QList<int> lineNumbers(QString value);

  bool contains(const QString& value);
  bool contains(const UniqueString& value);

private:
  QStringList m_allValues;
  QList<int> m_lineNumbers;
  QStringList m_unique;
  QStringList m_nonUnique;
};

typedef QMap<UniqueString, QString> UniqueStringMap;

#endif // UNIQUESTRING_H
