#ifndef QYAMLCPP__PARSE_H
#define QYAMLCPP__PARSE_H

#include <QFile>
#include <QString>
#include <QTextStream>

#include "qyamlcpp/node.h"

namespace YAML {

/*!
   Loads the input QString as a single YAML document.

   @throws {@link ParserException} if it is malformed.
*/
inline Node
Load(const QString& input)
{
  return Load(input.toStdString());
}

/*!
   Loads the input QByteArray as a single YAML document.

   @throws {@link ParserException} if it is malformed.
*/
inline Node
Load(const QByteArray& input)
{
  return Load(input.toStdString());
}

/*!
    Loads the filename as a YAML file.
*/
inline Node
LoadFile(const QString& filename)
{
  return LoadFile(filename.toStdString());
}

/*!
    \brief YAML::LoadFile extension for QString
*/
inline Node
LoadFile(QFile& file)
{
  if (!file.exists()) {
    return Node();
  }

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return Node();
  }

  QTextStream istream(&file);
  QString data = istream.readAll();
  return Load(data.toStdString());
}

}
#endif // QYAMLCPP__PARSE_H
