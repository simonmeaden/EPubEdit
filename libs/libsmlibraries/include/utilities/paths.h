#ifndef PATHS_H
#define PATHS_H

#include <QDir>
#include <QString>

namespace Paths {

QString
join(const QString& v);

template<typename... Args>
QString
join(const QString& first, Args... args)
{
  return QDir(first).filePath(join(args...));
}

}

#endif // PATHS_H
