/*
   Copyright 2020 Simon Meaden

   Permission is hereby granted, free of charge, to any person obtaining a copy of this
   software and associated documentation files (the "Software"), to deal in the Software
   without restriction, including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software, and to permit
                                                                         persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or
   substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
    PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "qyamlcpp/collection.h"

//template<class T>
//YAML::Node YAML::convert<QList<T>>::encode(const QList<T>& rhs)
//{
//   Node node(NodeType::Sequence);

//   std::list<T> slist = rhs.toStdList();
//   node = slist;

//   return node;
//}

//template<class T>
//bool YAML::convert<QList<T>>::decode(const YAML::Node& node, QList<T>& rhs)
//{
//   if (!node.IsSequence()) {
//      return false;
//   }

//   std::list<T> slist = node.as<std::list<T>>();
//   rhs = QList<T>::fromStdList(slist);

//   return true;
//}

//template<class K, class V>
//YAML::Node YAML::convert<QMap<K, V>>::encode(const QMap<K, V>& rhs)
//{
//   Node node(NodeType::Map);

//   std::map<K, V> smap = rhs.toStdMap();
//   node = smap;

//   return node;
//}

//template<class K, class V>
//bool YAML::convert<QMap<K, V>>::decode(const YAML::Node& node, QMap<K, V>& rhs)
//{
//   if (!node.IsMap()) {
//      return false;
//   }

//   std::map<K, V> smap = node.as<std::map<K, V>>();
//   rhs = QMap<K, V>(smap);

//   return true;
//}

//template<class T>
//YAML::Node YAML::convert<QVector<T>>::encode(const QVector<T>& rhs)
//{
//   Node node(NodeType::Sequence);

//   std::vector<T> svector = rhs.toStdVector();
//   node = svector;

//   return node;
//}

//template<class T>
//bool YAML::convert<QVector<T>>::decode(const YAML::Node& node, QVector<T>& rhs)
//{
//   if (!node.IsSequence()) {
//      return false;
//   }

//   std::vector<T> svector = node.as<std::vector<T>>();
//   rhs = QVector<T>::fromStdVector(svector);

//   return true;
//}

//template<class T>
//YAML::Node YAML::convert<QSet<T>>::encode(const QSet<T>& rhs)
//{
//   Node node(NodeType::Sequence);

//   std::list<T> slist = rhs.toList();
//   node = slist;

//   return node;
//}

//template<class T>
//bool YAML::convert<QSet<T>>::decode(const YAML::Node& node, QSet<T>& rhs)
//{
//   if (!node.IsSequence()) {
//      return false;
//   }

//   std::list<T> slist = node.as<std::list<T>>();
//   rhs = QSet<T>::fromList(QList<T>::fromStdList(slist));

//   return true;
//}
