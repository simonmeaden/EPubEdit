#ifndef BOOKDATABASE_H
#define BOOKDATABASE_H

#include <QObject>

class BookDatabase : public QObject
{
  Q_OBJECT
public:
  explicit BookDatabase(QObject *parent = nullptr);

signals:

};

#endif // BOOKDATABASE_H
