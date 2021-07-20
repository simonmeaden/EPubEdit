#ifndef LANGUAGES_H
#define LANGUAGES_H

#include <QDate>
#include <QString>

struct BCP47Language
{
  BCP47Language(const QString& tag,
           const QString& date,
           QStringList desc,
           const QString& other = QString());
  QString subtag;
  QStringList descriptions;
  QDate added;
  QString suppress_script_lang;
  QString scope;

  static QDate dateFromString(const QString& date);
};

class Languages
{
public:
  Languages();

private:
  QMap<QString, BCP47Language*> m_languages;
  QDate m_date;
};

#endif // LANGUAGES_H
