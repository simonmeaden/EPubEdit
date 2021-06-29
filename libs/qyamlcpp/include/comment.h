/***************************************************************************
  Copyright (c) 2013-2019, Simon Meaden
***************************************************************************

  Simonsoft - QYaml-Cpp

  qyaml-cpp is a set of extensions to yaml-cpp to handle some common
  Qt 5 types such a QString, QPint and others.

  This code is licensed under the GPL Version 3.0
***************************************************************************/
#ifndef QYAMLCPP_COMMENT_H
#define QYAMLCPP_COMMENT_H

#include <QByteArray>
#include <QString>

#include <yaml-cpp/emittermanip.h>

namespace YAML {

/*!
    \brief YAML::Comment extension for QString
*/
inline _Comment Comment(const QString content)
{
  const std::string ssdstr = content.toStdString();
  return _Comment(ssdstr);
}

/*!
    \brief YAML::Comment extension for QByteArray
*/
inline _Comment Comment(const QByteArray content)
{
  const std::string ssdstr = content.toStdString();
  return _Comment(ssdstr);
}

} // end namespace YAML

#endif // QYAMLCPP_COMMENT_H
