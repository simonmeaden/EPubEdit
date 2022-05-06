#ifndef SHAREDAUTHORDATA_H
#define SHAREDAUTHORDATA_H

#include <QSharedData>
#include <QString>
#include <QPixmap>

class SharedAuthorData : public QSharedData
{
public:
  SharedAuthorData();
  SharedAuthorData(const SharedAuthorData& other);
  ~SharedAuthorData();

  bool m_modified;
  quint64 m_uid;
  QString m_display_name;
  QString m_forename;
  QString m_middlenames;
  QString m_surname;
  QString m_file_as;
  bool m_surname_last;
  QStringList m_word_list;

  QString m_website;
  QString m_wikipedia;
  QList<quint64> m_books;
  QPixmap m_pixmap;
};

#endif // SHAREDAUTHORDATA_H
