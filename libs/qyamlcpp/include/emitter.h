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
#ifndef QYAMLCPP_EMITTER_H
#define QYAMLCPP_EMITTER_H

#include <QBuffer>
#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QString>
#include <QVector>

#include "collection.h"
#include "node.h"
#include <yaml-cpp/yaml.h>

namespace YAML {

//! Stream a QVector<T> into a YAML emitter.
template<class T>
inline Emitter&
operator<<(Emitter& emitter, const QVector<T> v)
{
  Node node;
  node = v;
  return emitter << node;
}

//! Stream a QMap<K, V> into a YAML emitter.
template<class K, class V>
inline Emitter&
operator<<(Emitter& emitter, const QMap<K, V> v)
{
  Node node;
  node = v;
  return emitter << node;
}

////! Stream a QVector<T> into a YAML emitter.
// template<class T>
// inline Emitter&
// operator<<(Emitter& emitter, const QVector<T> v)
//{
//  Node node;
//  node = v;
//  return emitter << node;
//}

//! Stream a QString into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, const QString v)
{
  return emitter.Write(v.toStdString());
}

//! Stream a QStringList into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QStringList v)
{
  std::list<std::string> list;

  emitter << YAML::BeginSeq;

  for (const QString &s : v) {
    emitter << s;
  }

  emitter << YAML::EndSeq;

  return emitter;
}

//! Stream a QByteArray into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QByteArray v)
{
  size_t size = size_t(v.size());
  const char* data = v.constData();
  return emitter << YAML::Binary(reinterpret_cast<const unsigned char*>(data),
                                 size_t(size));
}

//! Stream a QBuffer into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QBuffer v)
{
  QByteArray data = v.buffer();
  return emitter << data;
}

//! Stream a QPixmap into a YAML emitter.
//!
//! I don't really suggest as an image will generally take up a huge amount of
//! space in your YAML file. I would suggest saving the QPixmap into a file and
//! storing the file path in your YAML file.
inline Emitter&
operator<<(Emitter& emitter, QPixmap v)
{
  QByteArray array;
  QBuffer buffer(&array);
  buffer.open(QIODevice::WriteOnly);
  v.save(&buffer, "PNG");
  emitter << array;
  return emitter;
}

//! Stream a QColor into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QColor color)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "red";
  emitter << YAML::Value << color.red();
  emitter << YAML::Key << "green";
  emitter << YAML::Value << color.green();
  emitter << YAML::Key << "blue";
  emitter << YAML::Value << color.blue();
  emitter << YAML::Key << "alpha";
  emitter << YAML::Value << color.alpha();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QFont into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QFont font)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "family";
  emitter << YAML::Value << font.family().toStdString();
  emitter << YAML::Key << "bold";
  emitter << YAML::Value << font.bold();
  emitter << YAML::Key << "capitalization";
  emitter << YAML::Value << font.capitalization();
  emitter << YAML::Key << "fixedpitch";
  emitter << YAML::Value << font.fixedPitch();
  emitter << YAML::Key << "hinting preference";
  emitter << YAML::Value << font.hintingPreference();
  emitter << YAML::Key << "italic";
  emitter << YAML::Value << font.italic();
  emitter << YAML::Key << "kerning";
  emitter << YAML::Value << font.kerning();
  emitter << YAML::Key << "letter spacing";
  emitter << YAML::Value << font.letterSpacing();
  emitter << YAML::Key << "letter spacing type";
  emitter << YAML::Value << font.letterSpacingType();
  emitter << YAML::Key << "overline";
  emitter << YAML::Value << font.overline();
  // not recommended to use pixelSize()
  emitter << YAML::Key << "point size";
  emitter << YAML::Value << font.pointSize();
  emitter << YAML::Key << "stretch";
  emitter << YAML::Value << font.stretch();
  emitter << YAML::Key << "strikeout";
  emitter << YAML::Value << font.strikeOut();
  emitter << YAML::Key << "style";
  emitter << YAML::Value << font.style();
  emitter << YAML::Key << "style hint";
  emitter << YAML::Value << font.styleHint();
  emitter << YAML::Key << "style name";
  emitter << YAML::Value << font.styleName().toStdString();
  emitter << YAML::Key << "style strategy";
  emitter << YAML::Value << font.styleStrategy();
  emitter << YAML::Key << "underline";
  emitter << YAML::Value << font.underline();
  emitter << YAML::Key << "weight";
  emitter << YAML::Value << font.weight();
  emitter << YAML::Key << "word spacing";
  emitter << YAML::Value << font.wordSpacing();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QTransform into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QTransform v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "m11";
  emitter << YAML::Value << v.m11();
  emitter << YAML::Key << "m12";
  emitter << YAML::Value << v.m12();
  emitter << YAML::Key << "m13";
  emitter << YAML::Value << v.m13();
  emitter << YAML::Key << "m21";
  emitter << YAML::Value << v.m21();
  emitter << YAML::Key << "m22";
  emitter << YAML::Value << v.m22();
  emitter << YAML::Key << "m23";
  emitter << YAML::Value << v.m23();
  emitter << YAML::Key << "m31";
  emitter << YAML::Value << v.m31();
  emitter << YAML::Key << "m22";
  emitter << YAML::Value << v.m32();
  emitter << YAML::Key << "m33";
  emitter << YAML::Value << v.m33();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QPoint into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QPoint v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "x";
  emitter << YAML::Value << v.x();
  emitter << YAML::Key << "y";
  emitter << YAML::Value << v.y();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QPointF into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QPointF v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "x";
  emitter << YAML::Value << v.x();
  emitter << YAML::Key << "y";
  emitter << YAML::Value << v.y();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QRect into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QRect v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "left";
  emitter << YAML::Value << v.left();
  emitter << YAML::Key << "top";
  emitter << YAML::Value << v.top();
  emitter << YAML::Key << "width";
  emitter << YAML::Value << v.width();
  emitter << YAML::Key << "height";
  emitter << YAML::Value << v.height();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QRectF into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QRectF v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "left";
  emitter << YAML::Value << v.left();
  emitter << YAML::Key << "top";
  emitter << YAML::Value << v.top();
  emitter << YAML::Key << "width";
  emitter << YAML::Value << v.width();
  emitter << YAML::Key << "height";
  emitter << YAML::Value << v.height();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a Qsize into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QSize v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "width";
  emitter << YAML::Value << v.width();
  emitter << YAML::Key << "height";
  emitter << YAML::Value << v.height();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QSizeF into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QSizeF v)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "width";
  emitter << YAML::Value << v.width();
  emitter << YAML::Key << "height";
  emitter << YAML::Value << v.height();
  emitter << YAML::EndMap;
  return emitter;
}

//! Stream a QImage into a YAML emitter.
//!
//! I don't really suggest as an image will generally take up a huge amount of
//! space in your YAML file. I would suggest saving the QImage into a file and
//! storing the file path in your YAML file.
inline Emitter&
operator<<(Emitter& emitter, QImage image)
{
  QByteArray array;
  QBuffer buffer(&array);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "PNG");
  emitter << array;
  return emitter;
}

//! Stream a QBrush into a YAML emitter.
inline Emitter&
operator<<(Emitter& emitter, QBrush brush)
{
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "transform";
  emitter << YAML::Value << brush.transform();
  QImage image = brush.textureImage();
  if (!image.isNull()) {
    emitter << YAML::Key << "textureimage";
    emitter << YAML::Value << image;
  }
  QPixmap pixmap = brush.texture();
  if (!pixmap.isNull()) {
    emitter << YAML::Key << "texture";
    emitter << YAML::Value << pixmap;
  }
  emitter << YAML::Key << "style";
  emitter << YAML::Value << int(brush.style());
  emitter << YAML::Key << "color";
  emitter << YAML::Value << brush.color();
  emitter << YAML::EndMap;
  return emitter;
}

} // end namespace YAML

#endif // QYAMLCPP_EMITTER_H
