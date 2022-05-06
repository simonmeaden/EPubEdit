#ifndef AUTHORS_H
#define AUTHORS_H

#include <QObject>
#include <QPixmap>

#include "document/epubmetadata.h"
#include "document/sharedauthordata.h"
#include "qyamlcpp.h"

// class EBookOptions;

class EBookAuthorData
{
public:
  enum Comparison
  {
    NO_MATCH,
    SURNAME_MATCH,
    FORENAME_MATCH,
    FORE_AND_SURNAME_MATCH,
    ALL_MATCH,
    PARTIAL_MATCH,
  };

  EBookAuthorData();
  EBookAuthorData(const EBookAuthorData& other);
  ~EBookAuthorData();

  bool isValid();
  bool isEmpty();
  bool isModified();
  bool operator==(const EBookAuthorData& rhs)
  {
    if (d->m_uid == rhs.d->m_uid) {
      return true;
    }
    return false;
  }
  Comparison compare(QString m_forename,
                     QString m_middlenames,
                     QString m_surname);

  QString forename() const;
  void setForename(const QString& value);
  QString middlenames() const;
  void setMiddlenames(const QString& value);
  QString surname() const;
  void setSurname(const QString& surname);
  QString fileAs() const;
  void setFile_as(const QString& file_as);
  quint64 uid() const;
  void setUid(const quint64& uid);
  QString displayName() const;
  void setDisplayName(const QString& displayName);
  QString website() const;
  void setWebsite(const QString& website);
  QString wikipedia() const;
  void setWikipedia(const QString& wikipedia);
  QList<quint64> books() const;
  void setBooks(const QList<quint64>& books);
  QPixmap pixmap() const;
  void setPixmap(const QPixmap& pixmap);
  bool surnameLast() const;
  void setSurnameLast(bool surnameLast);
  QStringList wordList() const;
  void setWordList(const QStringList& wordList);

protected:
private:
  QSharedDataPointer<SharedAuthorData> d;
};

typedef QSharedPointer<EBookAuthorData> AuthorData;
// typedef QList<AuthorData> AuthorList;
typedef QMap<quint64, AuthorData> AuthorMap;
typedef QMultiMap<QString, AuthorData> AuthorByString;

Q_DECLARE_METATYPE(AuthorData);

class EBookAuthorsDB : public QObject
{
public:
  explicit EBookAuthorsDB(QObject* parent = nullptr);
  ~EBookAuthorsDB();

  //  void setFilename(const QString& filename);
  bool save(const QString& filename);
  bool load(const QString& filename);

  bool removeBook(quint64 index);

  AuthorData author(QString name);
  AuthorData author(quint64 uid);
  QList<AuthorData> authors();
  QList<AuthorData> authorsBySurname(QString surname);
  QList<AuthorData> authorsByForename(QString surname);
  AuthorData authorByFileAs(QString file_as);
  quint64 insertAuthor(AuthorData author);
  AuthorData addAuthor(QString displayName);
  void addAuthor(AuthorData author_data);
  QStringList compareAndDiscard(QStringList names);

  static quint64 nextUid() { return ++m_highestUid; }

signals:

public slots:

protected:
  QMap<quint64, AuthorData> m_authorData;
  AuthorByString m_authorByFileas;
  AuthorByString m_authorByDisplayname;
  AuthorByString m_authorBySurname;
  AuthorByString m_authorByForename;

  bool m_author_changed;

  bool loadAuthors(const QString& filename);
  bool saveAuthors(const QString& filename);

  static quint64 m_highestUid;
};

#endif // AUTHORS_H
