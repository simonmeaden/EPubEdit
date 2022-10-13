/*
  Copyright 2013-2020 Simon Meaden

  Permission is hereby granted, free of charge, to any person obtaining a copy
                                                of this software and associated
  documentation files (the "Software"), to deal in the Software without
  restriction, including without limitation the rights to use, copy, modify,
  merge, publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @author: Simon Meaden

*/

#ifndef QYAMLCPP_COLLECTION_H
#define QYAMLCPP_COLLECTION_H

#include <QFont>
#include <QList>
#include <QMap>
#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>

#include <list>
#include <map>
#include <vector>

#include "node.h"
#include "yaml-cpp/yaml.h"

namespace YAML {

////! Conversion YAML::convert template class for QVector<T>
// template<class T>
// struct convert<QVector<T>>
//{
//  //! encode a QVector<T> into a YAML Node.
//  static Node encode(const QVector<T>& rhs)
//  {
//    Node node(NodeType::Sequence);

//    std::vector<T> svector = rhs.toStdVector();
//    node = svector;

//    return node;
//  }

//  //! decodes a YAML node back into a QVector<T>
//  static bool decode(const Node& node, QVector<T>& rhs)
//  {
//    if (!node.IsSequence()) {
//      return false;
//    }

//    std::vector<T> svector = node.as<std::vector<T>>();
//    rhs = QVector<T>::fromStdVector(svector);

//    return true;
//  }
//};

//! Conversion YAML::convert template class for QList<T>
template<class T>
struct convert<QList<T>>
{
  //! encode a QList<T> into a YAML Node.
  static Node encode(const QList<T>& rhs)
  {
    Node node(NodeType::Sequence);

    //    std::list<T> slist = rhs.toStdList(); // toStdList() is deprecated
    std::list<T> slist = std::list<T>(rhs.begin(), rhs.end());
    node = slist;

    return node;
  }
  //! decodes a YAML node back into a QList<T>
  static bool decode(const Node& node, QList<T>& rhs)
  {
    if (!node.IsSequence()) {
      return false;
    }

    std::list<T> slist = node.as<std::list<T>>();
    //    rhs = QList<T>::fromStdList(slist); // fromStdList() is deprecated
    rhs = QList<T>(slist.begin(), slist.end());

    return true;
  }
};

////! Conversion YAML::convert template class for QMap<K, V>
// template<class K, class V>
// struct convert<QMap<K, V>>
//{
//   //! encode a QMap<K, V> into a YAML Node.
//   static Node encode(const QMap<K, V>& rhs)
//   {
//     Node node(NodeType::Map);

//    std::map<K, V> smap = rhs.toStdMap();
//    node = smap;

//    return node;
//  }

//  //! decodes a YAML node back into a QMap<K, V>
//  static bool decode(const Node& node, QMap<K, V>& rhs)
//  {
//    if (!node.IsMap()) {
//      return false;
//    }

//    std::map<K, V> smap = node.as<std::map<K, V>>();
//    rhs = QMap<K, V>(smap);

//    return true;
//  }
//};

template<typename Key, typename Value>
struct convert<QMap<Key, Value>>
{
  //! encode a QMap<Key, Value> into a YAML Node.
  static Node encode(const QMap<Key, Value>& rhs)
  {
    Node node(NodeType::Map);
    auto it = rhs.constBegin();
    while (it != rhs.constEnd()) {
      node.force_insert(it.key(), it.value());
      ++it;
    }
    return node;
  }

  //! decodes a YAML node back into a QMap<Key, Value>
  static bool decode(const Node& node, QMap<Key, Value>& rhs)
  {
    if (!node.IsMap())
      return false;

    rhs.clear();
    const_iterator it = node.begin();
    while (it != node.end()) {
      rhs[it->first.as<Key>()] = it->second.as<Value>();
      ++it;
    }
    return true;
  }
};

////! Conversion YAML::convert template class for QList<T>
// template<class T>
// inline void
// operator>>(const Node& node, QVector<T>& q)
//{
//  std::vector<T> svector;
//  svector = node.as<std::vector<T>>();
//  q = QVector<T>::fromStdVector(svector);
//}

// template <>
//! Conversion YAML::convert template class for QSet<T>
template<class T>
struct convert<QSet<T>>
{
  //! encode a QSet<T> into a YAML Node.
  static Node encode(const QSet<T>& rhs)
  {
    Node node(NodeType::Sequence);

    std::list<T> slist = rhs.toList();
    node = slist;

    return node;
  }

  //! decodes a YAML node back into a QSet<T>
  static bool decode(const Node& node, QSet<T>& rhs)
  {
    if (!node.IsSequence()) {
      return false;
    }

    std::list<T> slist = node.as<std::list<T>>();
    rhs = QSet<T>::fromList(QList<T>::fromStdList(slist));

    return true;
  }
};

// template<class T>
// inline void
// operator>>(const Node& node, QSet<T>& q)
//{
//  std::list<T> slist = node.as<std::list<T>>();
//  q = QSet<T>::fromList(QList<T>::fromStdList(slist));
//}

} // end of namespace YAML

#endif // QYAMLCPP_COLLECTION_H
