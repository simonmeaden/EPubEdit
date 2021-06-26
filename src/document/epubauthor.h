#ifndef EPUBAUTHOR_H
#define EPUBAUTHOR_H

#include <QObject>
#include <QSharedPointer>

class EPubAuthor : public QObject
{
  Q_OBJECT
public:
  explicit EPubAuthor(QObject* parent = nullptr);

signals:
};
typedef QSharedPointer<EPubAuthor> Author;
typedef QList<Author> AuthorList;

#endif // EPUBAUTHOR_H
