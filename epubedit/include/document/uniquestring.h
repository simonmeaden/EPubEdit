#ifndef UNIQUESTRING_H
#define UNIQUESTRING_H

#include <QList>
#include <QString>

/*!
 * \class UniqueString uniquestring.h
 * \brief The UniqueString class stroes a QString if, and only if it is
 *        not stored in it's internal list.
 *
 * Use the isValid() method to check whether a certain string was created.
 * iSEmpty() checks for an empty string and will return a bool value whether
 * the UniqueString is valid or not.
 */
class UniqueString
{
public:
  UniqueString();
  UniqueString(const QString& other);
  UniqueString(const UniqueString &other);
  ~UniqueString();

   QString toString() const;
   bool isEmpty() const;
   static bool exists(const QString& value);
   static bool exists(const UniqueString& value);

   /*!
    * \brief returns true if the UniqueString is valid.
    */
   bool isValid() const;
   bool isPossible(const QString& data);

  void operator=(const QString& rhs);

  bool operator==(const QString& rhs);
  bool operator==(const char* rhs) const;
  bool operator==(const UniqueString& rhs);
  friend bool operator==(const QString&, const UniqueString&);
  friend bool operator==(const UniqueString&, const QString&);
  friend bool operator==(const UniqueString&, const UniqueString&);

  bool operator<(const UniqueString& rhs);
  friend bool operator<(const UniqueString&, const UniqueString&);
  bool operator<(const QString& rhs);
  bool operator<(const char* rhs);


private:
  static QStringList m_uniqueList;
  QString m_data;
  bool m_valid;
};

//typedef QMap<UniqueString, QString> UniqueStringMap;

#endif // UNIQUESTRING_H
