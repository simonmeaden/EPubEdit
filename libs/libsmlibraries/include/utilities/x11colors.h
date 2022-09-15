/*
 Copyright 2020 Simon Meaden

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/
#ifndef X11COLORS_H
#define X11COLORS_H

#include <QColor>
#include <QMap>
#include <QString>
#include <QtMath>

// clang-format off
/*!
   \ingroup utilities
   \file x11colors.h
   \brief An extension of the  namespace which supplies extra functions
          to the standard Svg namespace and adds an extra X11 namespace that matches
          the Svg color set with an set that matches the X11 rgb.txt colors.
   \since 5.7.0
   \license{The MIT License}
   \copyright © 2020 - 2021 Simon Meaden. All rights reserved.

   The Svg color set evolved from the X11 rgb.txt  color set, with many of the names
   being the same. However some of the names generate a similar, but different color.
   The X11 set is also larger and offers a more extended set of color name options.

   Similarly to QColorConstants::Svg:color_name you can use QColorConstants::X11::color_name.

   The extended functions, namely name(QColor) and color(QString) which exist as
   QColorConstants::Svg::name(), QColorConstants::Svg::color().QColorConstants::X11::name()
   and QColorConstants::X11::color() variants return a QColor object or a name string
   respectively.

   There are also two helper functions in the QColorConstants namespace. svgOrX11Name(QColor)
   which returns a QString and svgOrX11Color(QString) which returns a QColor. Both of
   these functions test against the Svg set first, returning Svg values by preference
   before testing against the X11 set.

   \note I admit that QColorConstants is not the best place for these functions, but I
   am unsure exactly where the best place is.

   X11 Colors
   ==========


*/
// clang-format on

namespace QColorConstants {
namespace Svg {
namespace {

const QMap<QString, QColor>
initaliseMap()
{
  QMap<QString, QColor> map;
  map.insert("aliceblue", QColorConstants::Svg::aliceblue);
  map.insert("antiquewhite", QColorConstants::Svg::antiquewhite);
  map.insert("aqua", QColorConstants::Svg::aqua);
  map.insert("aquamarine", QColorConstants::Svg::aquamarine);
  map.insert("azure", QColorConstants::Svg::azure);
  map.insert("beige", QColorConstants::Svg::beige);
  map.insert("bisque", QColorConstants::Svg::bisque);
  map.insert("black", QColorConstants::Svg::black);
  map.insert("blanchedalmond", QColorConstants::Svg::blanchedalmond);
  map.insert("blue", QColorConstants::Svg::blue);
  map.insert("blueviolet", QColorConstants::Svg::blueviolet);
  map.insert("brown", QColorConstants::Svg::brown);
  map.insert("burlywood", QColorConstants::Svg::burlywood);
  map.insert("cadetblue", QColorConstants::Svg::cadetblue);
  map.insert("chartreuse", QColorConstants::Svg::chartreuse);
  map.insert("chocolate", QColorConstants::Svg::chocolate);
  map.insert("coral", QColorConstants::Svg::coral);
  map.insert("cornflowerblue", QColorConstants::Svg::cornflowerblue);
  map.insert("cornsilk", QColorConstants::Svg::cornsilk);
  map.insert("crimson", QColorConstants::Svg::crimson);
  map.insert("cyan", QColorConstants::Svg::cyan);
  map.insert("darkblue", QColorConstants::Svg::darkblue);
  map.insert("darkcyan", QColorConstants::Svg::darkcyan);
  map.insert("darkgoldenrod", QColorConstants::Svg::darkgoldenrod);
  map.insert("darkgray", QColorConstants::Svg::darkgray);
  map.insert("darkgreen", QColorConstants::Svg::darkgreen);
  map.insert("darkgrey", QColorConstants::Svg::darkgrey);
  map.insert("darkkhaki", QColorConstants::Svg::darkkhaki);
  map.insert("darkmagenta", QColorConstants::Svg::darkmagenta);
  map.insert("darkolivegreen", QColorConstants::Svg::darkolivegreen);
  map.insert("darkorange", QColorConstants::Svg::darkorange);
  map.insert("darkorchid", QColorConstants::Svg::darkorchid);
  map.insert("darkred", QColorConstants::Svg::darkred);
  map.insert("darksalmon", QColorConstants::Svg::darksalmon);
  map.insert("darkseagreen", QColorConstants::Svg::darkseagreen);
  map.insert("darkslateblue", QColorConstants::Svg::darkslateblue);
  map.insert("darkslategray", QColorConstants::Svg::darkslategray);
  map.insert("darkslategrey", QColorConstants::Svg::darkslategrey);
  map.insert("darkturquoise", QColorConstants::Svg::darkturquoise);
  map.insert("darkviolet", QColorConstants::Svg::darkviolet);
  map.insert("deeppink", QColorConstants::Svg::deeppink);
  map.insert("deepskyblue", QColorConstants::Svg::deepskyblue);
  map.insert("dimgray", QColorConstants::Svg::dimgray);
  map.insert("dimgrey", QColorConstants::Svg::dimgrey);
  map.insert("dodgerblue", QColorConstants::Svg::dodgerblue);
  map.insert("firebrick", QColorConstants::Svg::firebrick);
  map.insert("floralwhite", QColorConstants::Svg::floralwhite);
  map.insert("forestgreen", QColorConstants::Svg::forestgreen);
  map.insert("fuchsia", QColorConstants::Svg::fuchsia);
  map.insert("gainsboro", QColorConstants::Svg::gainsboro);
  map.insert("ghostwhite", QColorConstants::Svg::ghostwhite);
  map.insert("gold", QColorConstants::Svg::gold);
  map.insert("goldenrod", QColorConstants::Svg::goldenrod);
  map.insert("gray", QColorConstants::Svg::gray);
  map.insert("grey", QColorConstants::Svg::grey);
  map.insert("green", QColorConstants::Svg::green);
  map.insert("greenyellow", QColorConstants::Svg::greenyellow);
  map.insert("honeydew", QColorConstants::Svg::honeydew);
  map.insert("hotpink", QColorConstants::Svg::hotpink);
  map.insert("indianred", QColorConstants::Svg::indianred);
  map.insert("indigo", QColorConstants::Svg::indigo);
  map.insert("ivory", QColorConstants::Svg::ivory);
  map.insert("khaki", QColorConstants::Svg::khaki);
  map.insert("lavender", QColorConstants::Svg::lavender);
  map.insert("lavenderblush", QColorConstants::Svg::lavenderblush);
  map.insert("lawngreen", QColorConstants::Svg::lawngreen);
  map.insert("lemonchiffon", QColorConstants::Svg::lemonchiffon);
  map.insert("lightblue", QColorConstants::Svg::lightblue);
  map.insert("lightcoral", QColorConstants::Svg::lightcoral);
  map.insert("lightcyan", QColorConstants::Svg::lightcyan);
  map.insert("lightgoldenrodyellow",
             QColorConstants::Svg::lightgoldenrodyellow);
  map.insert("lightgray", QColorConstants::Svg::lightgray);
  map.insert("lightgreen", QColorConstants::Svg::lightgreen);
  map.insert("lightgrey", QColorConstants::Svg::lightgrey);
  map.insert("lightpink", QColorConstants::Svg::lightpink);
  map.insert("lightsalmon", QColorConstants::Svg::lightsalmon);
  map.insert("lightseagreen", QColorConstants::Svg::lightseagreen);
  map.insert("lightskyblue", QColorConstants::Svg::lightskyblue);
  map.insert("lightslategray", QColorConstants::Svg::lightslategray);
  map.insert("lightslategrey", QColorConstants::Svg::lightslategrey);
  map.insert("lightsteelblue", QColorConstants::Svg::lightsteelblue);
  map.insert("lightyellow", QColorConstants::Svg::lightyellow);
  map.insert("lime", QColorConstants::Svg::lime);
  map.insert("limegreen", QColorConstants::Svg::limegreen);
  map.insert("linen", QColorConstants::Svg::linen);
  map.insert("magenta", QColorConstants::Svg::magenta);
  map.insert("maroon", QColorConstants::Svg::maroon);
  map.insert("mediumaquamarine", QColorConstants::Svg::mediumaquamarine);
  map.insert("mediumblue", QColorConstants::Svg::mediumblue);
  map.insert("mediumorchid", QColorConstants::Svg::mediumorchid);
  map.insert("mediumpurple", QColorConstants::Svg::mediumpurple);
  map.insert("mediumseagreen", QColorConstants::Svg::mediumseagreen);
  map.insert("mediumslateblue", QColorConstants::Svg::mediumslateblue);
  map.insert("mediumspringgreen", QColorConstants::Svg::mediumspringgreen);
  map.insert("mediumturquoise", QColorConstants::Svg::mediumturquoise);
  map.insert("mediumvioletred", QColorConstants::Svg::mediumvioletred);
  map.insert("midnightblue", QColorConstants::Svg::midnightblue);
  map.insert("mintcream", QColorConstants::Svg::mintcream);
  map.insert("mistyrose", QColorConstants::Svg::mistyrose);
  map.insert("moccasin", QColorConstants::Svg::moccasin);
  map.insert("navajowhite", QColorConstants::Svg::navajowhite);
  map.insert("navy", QColorConstants::Svg::navy);
  map.insert("oldlace", QColorConstants::Svg::oldlace);
  map.insert("olive", QColorConstants::Svg::olive);
  map.insert("olivedrab", QColorConstants::Svg::olivedrab);
  map.insert("orange", QColorConstants::Svg::orange);
  map.insert("orangered", QColorConstants::Svg::orangered);
  map.insert("orchid", QColorConstants::Svg::orchid);
  map.insert("palegoldenrod", QColorConstants::Svg::palegoldenrod);
  map.insert("palegreen", QColorConstants::Svg::palegreen);
  map.insert("paleturquoise", QColorConstants::Svg::paleturquoise);
  map.insert("palevioletred", QColorConstants::Svg::palevioletred);
  map.insert("papayawhip", QColorConstants::Svg::papayawhip);
  map.insert("peachpuff", QColorConstants::Svg::peachpuff);
  map.insert("peru", QColorConstants::Svg::peru);
  map.insert("pink", QColorConstants::Svg::pink);
  map.insert("plum", QColorConstants::Svg::plum);
  map.insert("powderblue", QColorConstants::Svg::powderblue);
  map.insert("purple", QColorConstants::Svg::purple);
  map.insert("red", QColorConstants::Svg::red);
  map.insert("rosybrown", QColorConstants::Svg::rosybrown);
  map.insert("royalblue", QColorConstants::Svg::royalblue);
  map.insert("saddlebrown", QColorConstants::Svg::saddlebrown);
  map.insert("salmon", QColorConstants::Svg::salmon);
  map.insert("sandybrown", QColorConstants::Svg::sandybrown);
  map.insert("seagreen", QColorConstants::Svg::seagreen);
  map.insert("seashell", QColorConstants::Svg::seashell);
  map.insert("sienna", QColorConstants::Svg::sienna);
  map.insert("silver", QColorConstants::Svg::silver);
  map.insert("skyblue", QColorConstants::Svg::skyblue);
  map.insert("slateblue", QColorConstants::Svg::slateblue);
  map.insert("slategray", QColorConstants::Svg::slategray);
  map.insert("slategrey", QColorConstants::Svg::slategrey);
  map.insert("snow", QColorConstants::Svg::snow);
  map.insert("springgreen", QColorConstants::Svg::springgreen);
  map.insert("steelblue", QColorConstants::Svg::steelblue);
  map.insert("tan", QColorConstants::Svg::tan);
  map.insert("teal", QColorConstants::Svg::teal);
  map.insert("thistle", QColorConstants::Svg::thistle);
  map.insert("tomato", QColorConstants::Svg::tomato);
  map.insert("turquoise", QColorConstants::Svg::turquoise);
  map.insert("violet", QColorConstants::Svg::violet);
  map.insert("wheat", QColorConstants::Svg::wheat);
  map.insert("white", QColorConstants::Svg::white);
  map.insert("whitesmoke", QColorConstants::Svg::whitesmoke);
  map.insert("yellow", QColorConstants::Svg::yellow);
  map.insert("yellowgreen", QColorConstants::Svg::yellowgreen);
  return map;
} // initaliseAll()
} // end of anonymous namespace
static const QMap<QString, QColor> COLORMAP = initaliseMap();

//! Returns the color associated with the name string, if any name exists,
//! otherwise an invalid QColor is returned.
//! \sa name(const QColor&)
static QColor
color(const QString& name)
{
  return COLORMAP.value(name);
}

//! Returns the first name string associated with this color, if any match,
//! otherwise an empty string is returned.
//! \sa color(const QString&)
static QString
name(const QColor& color)
{
  auto colorKeys = COLORMAP.keys();
  for (auto &k : colorKeys) {
    auto c = COLORMAP.value(k);
    if (c == color) {
      return k;
    }
  }
  return QString();
}

} // namespace Svg

namespace X11 {
// clang-format off
#if defined(Q_COMPILER_CONSTEXPR) & defined(Q_COMPILER_UNIFORM_INIT)
constexpr QColor snow                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 250 * 0x101, 250 * 0x101};
constexpr QColor ghostwhite            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 248 * 0x101, 248 * 0x101, 255 * 0x101};
constexpr QColor whitesmoke            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 245 * 0x101, 245 * 0x101, 255 * 0x101};
constexpr QColor gainsboro             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 220 * 0x101, 220 * 0x101, 220 * 0x101};
constexpr QColor navajowhite           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 222 * 0x101, 173 * 0x101};
constexpr QColor moccasin              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 228 * 0x101, 181 * 0x101};
constexpr QColor cornsilk              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 248 * 0x101, 220 * 0x101};
constexpr QColor ivory                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 255 * 0x101, 240 * 0x101};
constexpr QColor lemonchiffon          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 250 * 0x101, 205 * 0x101};
constexpr QColor seashell              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 245 * 0x101, 238 * 0x101};
constexpr QColor honeydew              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 255 * 0x101, 240 * 0x101};
constexpr QColor mintcream             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 245 * 0x101, 255 * 0x101, 250 * 0x101};
constexpr QColor azure                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor aliceblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 248 * 0x101, 255 * 0x101};
constexpr QColor lavender              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 230 * 0x101, 230 * 0x101, 250 * 0x101};
constexpr QColor lavenderblush         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 240 * 0x101, 245 * 0x101};
constexpr QColor mistyrose             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 228 * 0x101, 225 * 0x101};
constexpr QColor white                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor black                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor darkslategray         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 47 * 0x101, 79 * 0x101, 79 * 0x101};
constexpr QColor dimgray               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 105 * 0x101, 105 * 0x101, 105 * 0x101};
constexpr QColor slategray             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 112 * 0x101, 128 * 0x101, 144 * 0x101};
constexpr QColor lightslategray        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 119 * 0x101, 136 * 0x101, 153 * 0x101};
constexpr QColor gray                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 190 * 0x101, 190 * 0x101, 190 * 0x101};
constexpr QColor lightgray             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 211 * 0x101, 211 * 0x101, 211 * 0x101};
constexpr QColor midnightblue          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 25 * 0x101, 25 * 0x101, 112 * 0x101};
constexpr QColor navy                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 128 * 0x101};
constexpr QColor cornflowerblue        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 100 * 0x101, 149 * 0x101, 237 * 0x101};
constexpr QColor darkslateblue         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 72 * 0x101, 61 * 0x101, 139 * 0x101};
constexpr QColor slateblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 106 * 0x101, 90 * 0x101, 205 * 0x101};
constexpr QColor mediumslateblue       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 123 * 0x101, 104 * 0x101, 238 * 0x101};
constexpr QColor lightslateblue        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 132 * 0x101, 112 * 0x101, 255 * 0x101};
constexpr QColor mediumblue            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 205 * 0x101};
constexpr QColor royalblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 65 * 0x101, 105 * 0x101, 225 * 0x101};
constexpr QColor blue                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 255 * 0x101};
constexpr QColor dodgerblue            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 30 * 0x101, 144 * 0x101, 255 * 0x101};
constexpr QColor deepskyblue           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 191 * 0x101, 255 * 0x101};
constexpr QColor skyblue               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 135 * 0x101, 206 * 0x101, 235 * 0x101};
constexpr QColor lightskyblue          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 135 * 0x101, 206 * 0x101, 250 * 0x101};
constexpr QColor steelblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 70 * 0x101, 130 * 0x101, 180 * 0x101};
constexpr QColor lightsteelblue        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 176 * 0x101, 196 * 0x101, 222 * 0x101};
constexpr QColor lightblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 173 * 0x101, 216 * 0x101, 230 * 0x101};
constexpr QColor powderblue            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 176 * 0x101, 224 * 0x101, 230 * 0x101};
constexpr QColor paleturquoise         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 175 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor darkturquoise         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 206 * 0x101, 209 * 0x101};
constexpr QColor mediumturquoise       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 72 * 0x101, 209 * 0x101, 204 * 0x101};
constexpr QColor turquoise             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 64 * 0x101, 224 * 0x101, 208 * 0x101};
constexpr QColor cyan                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor lightcyan             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 224 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor cadetblue             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 95 * 0x101, 248 * 0x101, 160 * 0x101};
constexpr QColor mediumaquamarine      Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 102 * 0x101, 205 * 0x101, 170 * 0x101};
constexpr QColor aquamarine            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 127 * 0x101, 255 * 0x101, 212 * 0x101};
constexpr QColor darkgreen             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 100 * 0x101, 0 * 0x101};
constexpr QColor darkolivegreen        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 85 * 0x101, 107 * 0x101, 47 * 0x101};
constexpr QColor darkseagreen          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 143 * 0x101, 188 * 0x101, 143 * 0x101};
constexpr QColor seagreen              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 46 * 0x101, 139 * 0x101, 87 * 0x101};
constexpr QColor mediumseagreen        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 60 * 0x101, 179 * 0x101, 113 * 0x101};
constexpr QColor lightseagreen         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 32 * 0x101, 178 * 0x101, 170 * 0x101};
constexpr QColor palegreen             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 152 * 0x101, 251 * 0x101, 152 * 0x101};
constexpr QColor springgreen           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 255 * 0x101, 127 * 0x101};
constexpr QColor lawngreen             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 124 * 0x101, 252 * 0x101, 0 * 0x101};
constexpr QColor green                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 255 * 0x101, 0 * 0x101};
constexpr QColor chartreuse            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 127 * 0x101, 255 * 0x101, 0 * 0x101};
constexpr QColor mediumspringgreen     Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 250 * 0x101, 154 * 0x101};
constexpr QColor greenyellow           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 173 * 0x101, 255 * 0x101, 47 * 0x101};
constexpr QColor limegreen             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 50 * 0x101, 205 * 0x101, 50 * 0x101};
constexpr QColor yellowgreen           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 154 * 0x101, 205 * 0x101, 50 * 0x101};
constexpr QColor forestgreen           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 34 * 0x101, 139 * 0x101, 34 * 0x101};
constexpr QColor olivedrab             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 107 * 0x101, 142 * 0x101, 35 * 0x101};
constexpr QColor darkkhaki             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 189 * 0x101, 183 * 0x101, 107 * 0x101};
constexpr QColor khaki                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 230 * 0x101, 140 * 0x101};
constexpr QColor floralwhite           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 250 * 0x101, 240 * 0x101};
constexpr QColor antiquewhite          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 250 * 0x101, 235 * 0x101, 215 * 0x101};
constexpr QColor palegoldenrod         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 232 * 0x101, 170 * 0x101};
constexpr QColor lightgoldenrodyellow  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 250 * 0x101, 250 * 0x101, 210 * 0x101};
constexpr QColor lightyellow           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 255 * 0x101, 224 * 0x101};
constexpr QColor yellow                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 255 * 0x101, 0 * 0x101};
constexpr QColor gold                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 215 * 0x101, 0 * 0x101};
constexpr QColor lightgoldenrod        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 221 * 0x101, 130 * 0x101};
constexpr QColor goldenrod             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 218 * 0x101, 165 * 0x101, 32 * 0x101};
constexpr QColor darkgoldenrod         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 184 * 0x101, 134 * 0x101, 11 * 0x101};
constexpr QColor rosybrown             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 188 * 0x101, 143 * 0x101, 143 * 0x101};
constexpr QColor indianred             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 92 * 0x101, 92 * 0x101};
constexpr QColor saddlebrown           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 69 * 0x101, 19 * 0x101};
constexpr QColor sienna                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 160 * 0x101, 82 * 0x101, 45 * 0x101};
constexpr QColor peru                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 133 * 0x101, 63 * 0x101};
constexpr QColor burlywood             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 222 * 0x101, 184 * 0x101, 135 * 0x101};
constexpr QColor beige                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 245 * 0x101, 245 * 0x101, 220 * 0x101};
constexpr QColor wheat                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 245 * 0x101, 222 * 0x101, 179 * 0x101};
constexpr QColor sandybrown            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 244 * 0x101, 164 * 0x101, 96 * 0x101};
constexpr QColor tan                   Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 210 * 0x101, 180 * 0x101, 140 * 0x101};
constexpr QColor chocolate             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 210 * 0x101, 105 * 0x101, 30 * 0x101};
constexpr QColor firebrick             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 178 * 0x101, 34 * 0x101, 34 * 0x101};
constexpr QColor brown                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 165 * 0x101, 42 * 0x101, 42 * 0x101};
constexpr QColor darksalmon            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 233 * 0x101, 150 * 0x101, 122 * 0x101};
constexpr QColor salmon                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 250 * 0x101, 128 * 0x101, 114 * 0x101};
constexpr QColor lightsalmon           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 160 * 0x101, 122 * 0x101};
constexpr QColor orange                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 165 * 0x101, 0 * 0x101};
constexpr QColor darkorange            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 140 * 0x101, 0 * 0x101};
constexpr QColor coral                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 127 * 0x101, 80 * 0x101};
constexpr QColor lightcoral            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 128 * 0x101, 128 * 0x101};
constexpr QColor tomato                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 99 * 0x101, 71 * 0x101};
constexpr QColor orangered             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 69 * 0x101, 0 * 0x101};
constexpr QColor red                   Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor hotpink               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 105 * 0x101, 180 * 0x101};
constexpr QColor deeppink              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 20 * 0x101, 147 * 0x101};
constexpr QColor pink                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 192 * 0x101, 203 * 0x101};
constexpr QColor lightpink             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 182 * 0x101, 193 * 0x101};
constexpr QColor palevioletred         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 219 * 0x101, 112 * 0x101, 147 * 0x101};
constexpr QColor maroon                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 176 * 0x101, 48 * 0x101, 96 * 0x101};
constexpr QColor mediumvioletred       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 199 * 0x101, 21 * 0x101, 133 * 0x101};
constexpr QColor violetred             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 208 * 0x101, 32 * 0x101, 144 * 0x101};
constexpr QColor magenta               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 0 * 0x101, 255 * 0x101};
constexpr QColor violet                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 130 * 0x101, 238 * 0x101};
constexpr QColor plum                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 221 * 0x101, 160 * 0x101, 221 * 0x101};
constexpr QColor orchid                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 218 * 0x101, 112 * 0x101, 214 * 0x101};
constexpr QColor mediumorchid          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 186 * 0x101, 85 * 0x101, 211 * 0x101};
constexpr QColor darkorchid            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 153 * 0x101, 50 * 0x101, 204 * 0x101};
constexpr QColor darkviolet            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 148 * 0x101, 0 * 0x101, 211 * 0x101};
constexpr QColor blueviolet            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 138 * 0x101, 43 * 0x101, 226 * 0x101};
constexpr QColor purple                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 160 * 0x101, 32 * 0x101, 240 * 0x101};
constexpr QColor mediumpurple          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 147 * 0x101, 112 * 0x101, 219 * 0x101};
constexpr QColor thistle               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 216 * 0x101, 191 * 0x101, 216 * 0x101};
constexpr QColor snow2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 233 * 0x101, 233 * 0x101};
constexpr QColor snow3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 201 * 0x101, 201 * 0x101};
constexpr QColor snow4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 137 * 0x101, 137 * 0x101};
constexpr QColor seashell2             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 229 * 0x101, 222 * 0x101};
constexpr QColor seashell3             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 197 * 0x101, 191 * 0x101};
constexpr QColor seashell4             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 134 * 0x101, 130 * 0x101};
constexpr QColor AntiqueWhite1         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 239 * 0x101, 219 * 0x101};
constexpr QColor AntiqueWhite2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 223 * 0x101, 204 * 0x101};
constexpr QColor AntiqueWhite3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 192 * 0x101, 176 * 0x101};
constexpr QColor AntiqueWhite4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 131 * 0x101, 120 * 0x101};
constexpr QColor bisque                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 228 * 0x101, 196 * 0x101};
constexpr QColor bisque2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 213 * 0x101, 183 * 0x101};
constexpr QColor bisque3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 183 * 0x101, 158 * 0x101};
constexpr QColor bisque4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 125 * 0x101, 107 * 0x101};
constexpr QColor peachpuff             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 218 * 0x101, 185 * 0x101};
constexpr QColor PeachPuff2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 203 * 0x101, 173 * 0x101};
constexpr QColor PeachPuff3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 175 * 0x101, 149 * 0x101};
constexpr QColor PeachPuff4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 119 * 0x101, 101 * 0x101};
constexpr QColor NavajoWhite2          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 207 * 0x101, 161 * 0x101};
constexpr QColor NavajoWhite3          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 179 * 0x101, 139 * 0x101};
constexpr QColor NavajoWhite4          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 121 * 0x101, 94 * 0x101};
constexpr QColor LemonChiffon2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 233 * 0x101, 191 * 0x101};
constexpr QColor LemonChiffon3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 201 * 0x101, 165 * 0x101};
constexpr QColor LemonChiffon4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 137 * 0x101, 112 * 0x101};
constexpr QColor cornsilk2             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 232 * 0x101, 205 * 0x101};
constexpr QColor cornsilk3             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 200 * 0x101, 177 * 0x101};
constexpr QColor cornsilk4             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 136 * 0x101, 120 * 0x101};
constexpr QColor ivory2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 238 * 0x101, 224 * 0x101};
constexpr QColor ivory3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 205 * 0x101, 193 * 0x101};
constexpr QColor ivory4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 139 * 0x101, 131 * 0x101};
constexpr QColor honeydew2             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 224 * 0x101, 238 * 0x101, 224 * 0x101};
constexpr QColor honeydew3             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 193 * 0x101, 205 * 0x101, 193 * 0x101};
constexpr QColor honeydew4             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 131 * 0x101, 139 * 0x101, 131 * 0x101};
constexpr QColor LavenderBlush2        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 224 * 0x101, 229 * 0x101};
constexpr QColor LavenderBlush3        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 193 * 0x101, 197 * 0x101};
constexpr QColor LavenderBlush4        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 131 * 0x101, 134 * 0x101};
constexpr QColor MistyRose2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 213 * 0x101, 210 * 0x101};
constexpr QColor MistyRose3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 183 * 0x101, 181 * 0x101};
constexpr QColor MistyRose4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 125 * 0x101, 123 * 0x101};
constexpr QColor azure2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 224 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor azure3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 193 * 0x101, 205 * 0x101, 205 * 0x101};
constexpr QColor azure4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 131 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor SlateBlue2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 122 * 0x101, 103 * 0x101, 238 * 0x101};
constexpr QColor SlateBlue3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 105 * 0x101, 89 * 0x101, 205 * 0x101};
constexpr QColor SlateBlue4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 71 * 0x101, 60 * 0x101, 139 * 0x101};
constexpr QColor RoyalBlue1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 72 * 0x101, 118 * 0x101, 255 * 0x101};
constexpr QColor RoyalBlue2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 67 * 0x101, 110 * 0x101, 238 * 0x101};
constexpr QColor RoyalBlue3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 58 * 0x101, 95 * 0x101, 205 * 0x101};
constexpr QColor RoyalBlue4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 39 * 0x101, 64 * 0x101, 139 * 0x101};
constexpr QColor blue2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 238 * 0x101};
constexpr QColor blue4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 139 * 0x101};
constexpr QColor DodgerBlue2           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 28 * 0x101, 134 * 0x101, 238 * 0x101};
constexpr QColor DodgerBlue3           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 24 * 0x101, 116 * 0x101, 205 * 0x101};
constexpr QColor DodgerBlue4           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 16 * 0x101, 78 * 0x101, 139 * 0x101};
constexpr QColor SteelBlue1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 99 * 0x101, 184 * 0x101, 255 * 0x101};
constexpr QColor SteelBlue2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 92 * 0x101, 172 * 0x101, 238 * 0x101};
constexpr QColor SteelBlue3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 79 * 0x101, 148 * 0x101, 205 * 0x101};
constexpr QColor SteelBlue4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 54 * 0x101, 100 * 0x101, 139 * 0x101};
constexpr QColor DeepSkyBlue2          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 178 * 0x101, 238 * 0x101};
constexpr QColor DeepSkyBlue3          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 154 * 0x101, 205 * 0x101};
constexpr QColor DeepSkyBlue4          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 104 * 0x101, 139 * 0x101};
constexpr QColor SkyBlue2              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 126 * 0x101, 192 * 0x101, 238 * 0x101};
constexpr QColor SkyBlue3              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 108 * 0x101, 166 * 0x101, 205 * 0x101};
constexpr QColor SkyBlue4              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 74 * 0x101, 112 * 0x101, 139 * 0x101};
constexpr QColor LightSkyBlue1         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 176 * 0x101, 226 * 0x101, 255 * 0x101};
constexpr QColor LightSkyBlue2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 164 * 0x101, 211 * 0x101, 238 * 0x101};
constexpr QColor LightSkyBlue3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 141 * 0x101, 182 * 0x101, 205 * 0x101};
constexpr QColor LightSkyBlue4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 96 * 0x101, 123 * 0x101, 139 * 0x101};
constexpr QColor SlateGray1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 198 * 0x101, 226 * 0x101, 255 * 0x101};
constexpr QColor SlateGray2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 185 * 0x101, 211 * 0x101, 238 * 0x101};
constexpr QColor SlateGray3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 159 * 0x101, 182 * 0x101, 205 * 0x101};
constexpr QColor SlateGray4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 108 * 0x101, 123 * 0x101, 139 * 0x101};
constexpr QColor LightSteelBlue1       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 202 * 0x101, 225 * 0x101, 255 * 0x101};
constexpr QColor LightSteelBlue2       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 188 * 0x101, 210 * 0x101, 238 * 0x101};
constexpr QColor LightSteelBlue3       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 162 * 0x101, 181 * 0x101, 205 * 0x101};
constexpr QColor LightSteelBlue4       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 110 * 0x101, 123 * 0x101, 139 * 0x101};
constexpr QColor LightBlue1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 191 * 0x101, 239 * 0x101, 255 * 0x101};
constexpr QColor LightBlue2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 178 * 0x101, 223 * 0x101, 238 * 0x101};
constexpr QColor LightBlue3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 154 * 0x101, 192 * 0x101, 205 * 0x101};
constexpr QColor LightBlue4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 104 * 0x101, 131 * 0x101, 139 * 0x101};
constexpr QColor LightCyan2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 209 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor LightCyan3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 180 * 0x101, 205 * 0x101, 205 * 0x101};
constexpr QColor LightCyan4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 122 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor PaleTurquoise1        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 187 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor PaleTurquoise2        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 174 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor PaleTurquoise3        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 150 * 0x101, 205 * 0x101, 205 * 0x101};
constexpr QColor PaleTurquoise4        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 102 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor CadetBlue1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 152 * 0x101, 245 * 0x101, 255 * 0x101};
constexpr QColor CadetBlue2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 142 * 0x101, 229 * 0x101, 238 * 0x101};
constexpr QColor CadetBlue3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 122 * 0x101, 197 * 0x101, 205 * 0x101};
constexpr QColor CadetBlue4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 83 * 0x101, 134 * 0x101, 139 * 0x101};
constexpr QColor turquoise2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 229 * 0x101, 238 * 0x101};
constexpr QColor turquoise3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 197 * 0x101, 205 * 0x101};
constexpr QColor turquoise4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 134 * 0x101, 139 * 0x101};
constexpr QColor cyan2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor cyan3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 205 * 0x101, 205 * 0x101};
constexpr QColor cyan4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor DarkSlateGray1        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 151 * 0x101, 255 * 0x101, 255 * 0x101};
constexpr QColor DarkSlateGray2        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 141 * 0x101, 238 * 0x101, 238 * 0x101};
constexpr QColor DarkSlateGray3        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 121 * 0x101, 205 * 0x101, 205 * 0x101};
constexpr QColor DarkSlateGray4        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 82 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor aquamarine2           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 118 * 0x101, 238 * 0x101, 198 * 0x101};
constexpr QColor aquamarine4           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 69 * 0x101, 139 * 0x101, 116 * 0x101};
constexpr QColor DarkSeaGreen1         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 193 * 0x101, 255 * 0x101, 193 * 0x101};
constexpr QColor DarkSeaGreen2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 180 * 0x101, 238 * 0x101, 180 * 0x101};
constexpr QColor DarkSeaGreen3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 155 * 0x101, 205 * 0x101, 155 * 0x101};
constexpr QColor DarkSeaGreen4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 105 * 0x101, 139 * 0x101, 105 * 0x101};
constexpr QColor SeaGreen1             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 84 * 0x101, 255 * 0x101, 159 * 0x101};
constexpr QColor SeaGreen2             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 78 * 0x101, 238 * 0x101, 148 * 0x101};
constexpr QColor SeaGreen3             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 67 * 0x101, 205 * 0x101, 128 * 0x101};
constexpr QColor PaleGreen1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 154 * 0x101, 255 * 0x101, 154 * 0x101};
constexpr QColor PaleGreen2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 144 * 0x101, 238 * 0x101, 144 * 0x101};
constexpr QColor PaleGreen3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 124 * 0x101, 248 * 0x101, 205 * 0x101};
constexpr QColor PaleGreen4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 84 * 0x101, 139 * 0x101, 84 * 0x101};
constexpr QColor SpringGreen2          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 238 * 0x101, 118 * 0x101};
constexpr QColor SpringGreen3          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 205 * 0x101, 102 * 0x101};
constexpr QColor SpringGreen4          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 139 * 0x101, 69 * 0x101};
constexpr QColor green2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 238 * 0x101, 0 * 0x101};
constexpr QColor green3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 205 * 0x101, 0 * 0x101};
constexpr QColor green4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 139 * 0x101, 0 * 0x101};
constexpr QColor chartreuse2           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 118 * 0x101, 238 * 0x101, 0 * 0x101};
constexpr QColor chartreuse3           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 102 * 0x101, 205 * 0x101, 0 * 0x101};
constexpr QColor chartreuse4           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 69 * 0x101, 139 * 0x101, 0 * 0x101};
constexpr QColor OliveDrab1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 192 * 0x101, 255 * 0x101, 62 * 0x101};
constexpr QColor OliveDrab2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 179 * 0x101, 238 * 0x101, 58 * 0x101};
constexpr QColor OliveDrab4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 105 * 0x101, 139 * 0x101, 34 * 0x101};
constexpr QColor DarkOliveGreen1       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 202 * 0x101, 255 * 0x101, 112 * 0x101};
constexpr QColor DarkOliveGreen2       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 188 * 0x101, 238 * 0x101, 104 * 0x101};
constexpr QColor DarkOliveGreen3       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 162 * 0x101, 205 * 0x101, 90 * 0x101};
constexpr QColor DarkOliveGreen4       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 110 * 0x101, 139 * 0x101, 61 * 0x101};
constexpr QColor khaki2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 230 * 0x101, 133 * 0x101};
constexpr QColor khaki3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 198 * 0x101, 115 * 0x101};
constexpr QColor khaki4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 134 * 0x101, 78 * 0x101};
constexpr QColor LightGoldenrod1       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 236 * 0x101, 139 * 0x101};
constexpr QColor LightGoldenrod2       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 220 * 0x101, 130 * 0x101};
constexpr QColor LightGoldenrod3       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 190 * 0x101, 112 * 0x101};
constexpr QColor LightGoldenrod4       Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 129 * 0x101, 76 * 0x101};
constexpr QColor LightYellow2          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 238 * 0x101, 209 * 0x101};
constexpr QColor LightYellow3          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 205 * 0x101, 180 * 0x101};
constexpr QColor LightYellow4          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 139 * 0x101, 122 * 0x101};
constexpr QColor yellow2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 238 * 0x101, 0 * 0x101};
constexpr QColor yellow3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 205 * 0x101, 0 * 0x101};
constexpr QColor yellow4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 139 * 0x101, 0 * 0x101};
constexpr QColor gold2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 201 * 0x101, 0 * 0x101};
constexpr QColor gold3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 173 * 0x101, 0 * 0x101};
constexpr QColor gold4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 117 * 0x101, 0 * 0x101};
constexpr QColor goldenrod1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 193 * 0x101, 37 * 0x101};
constexpr QColor goldenrod2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 180 * 0x101, 34 * 0x101};
constexpr QColor goldenrod3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 155 * 0x101, 29 * 0x101};
constexpr QColor goldenrod4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 105 * 0x101, 20 * 0x101};
constexpr QColor DarkGoldenrod1        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 185 * 0x101, 15 * 0x101};
constexpr QColor DarkGoldenrod2        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 173 * 0x101, 14 * 0x101};
constexpr QColor DarkGoldenrod3        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 149 * 0x101, 12 * 0x101};
constexpr QColor DarkGoldenrod4        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 101 * 0x101, 8 * 0x101};
constexpr QColor RosyBrown1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 193 * 0x101, 193 * 0x101};
constexpr QColor RosyBrown2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 180 * 0x101, 180 * 0x101};
constexpr QColor RosyBrown3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 155 * 0x101, 155 * 0x101};
constexpr QColor RosyBrown4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 105 * 0x101, 105 * 0x101};
constexpr QColor IndianRed1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 106 * 0x101, 106 * 0x101};
constexpr QColor IndianRed2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 99 * 0x101, 99 * 0x101};
constexpr QColor IndianRed3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 85 * 0x101, 85 * 0x101};
constexpr QColor IndianRed4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 58 * 0x101, 58 * 0x101};
constexpr QColor sienna2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 121 * 0x101, 66 * 0x101};
constexpr QColor sienna3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 104 * 0x101, 57 * 0x101};
constexpr QColor sienna4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 71 * 0x101, 38 * 0x101};
constexpr QColor burlywood2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 197 * 0x101, 145 * 0x101};
constexpr QColor burlywood3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 170 * 0x101, 125 * 0x101};
constexpr QColor burlywood4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 115 * 0x101, 85 * 0x101};
constexpr QColor wheat2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 216 * 0x101, 174 * 0x101};
constexpr QColor wheat3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 186 * 0x101, 150 * 0x101};
constexpr QColor wheat4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 126 * 0x101, 102 * 0x101};
constexpr QColor tan2                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 154 * 0x101, 73 * 0x101};
constexpr QColor tan4                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 90 * 0x101, 43 * 0x101};
constexpr QColor chocolate2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 118 * 0x101, 33 * 0x101};
constexpr QColor chocolate3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 102 * 0x101, 29 * 0x101};
constexpr QColor firebrick2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 44 * 0x101, 44 * 0x101};
constexpr QColor firebrick3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 38 * 0x101, 38 * 0x101};
constexpr QColor firebrick4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 26 * 0x101, 26 * 0x101};
constexpr QColor brown2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 59 * 0x101, 59 * 0x101};
constexpr QColor brown3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 51 * 0x101, 51 * 0x101};
constexpr QColor brown4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 35 * 0x101, 35 * 0x101};
constexpr QColor salmon1               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 140 * 0x101, 105 * 0x101};
constexpr QColor salmon2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 130 * 0x101, 98 * 0x101};
constexpr QColor salmon3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 112 * 0x101, 84 * 0x101};
constexpr QColor salmon4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 76 * 0x101, 57 * 0x101};
constexpr QColor LightSalmon2          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 149 * 0x101, 114 * 0x101};
constexpr QColor LightSalmon3          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 129 * 0x101, 98 * 0x101};
constexpr QColor LightSalmon4          Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 87 * 0x101, 66 * 0x101};
constexpr QColor orange2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 154 * 0x101, 0 * 0x101};
constexpr QColor orange3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 133 * 0x101, 0 * 0x101};
constexpr QColor orange4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 90 * 0x101, 0 * 0x101};
constexpr QColor DarkOrange1           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 127 * 0x101, 0 * 0x101};
constexpr QColor DarkOrange2           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 118 * 0x101, 0 * 0x101};
constexpr QColor DarkOrange3           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 102 * 0x101, 0 * 0x101};
constexpr QColor DarkOrange4           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 69 * 0x101, 0 * 0x101};
constexpr QColor coral2                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 106 * 0x101, 80 * 0x101};
constexpr QColor coral3                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 91 * 0x101, 69 * 0x101};
constexpr QColor coral4                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 62 * 0x101, 47 * 0x101};
constexpr QColor tomato2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 92 * 0x101, 66 * 0x101};
constexpr QColor tomato3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 79 * 0x101, 57 * 0x101};
constexpr QColor tomato4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 54 * 0x101, 38 * 0x101};
constexpr QColor OrangeRed2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 64 * 0x101, 0 * 0x101};
constexpr QColor OrangeRed3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 55 * 0x101, 0 * 0x101};
constexpr QColor OrangeRed4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 37 * 0x101, 0 * 0x101};
constexpr QColor red2                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor red3                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor red4                  Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor DeepPink2             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 18 * 0x101, 137 * 0x101};
constexpr QColor DeepPink3             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 16 * 0x101, 118 * 0x101};
constexpr QColor DeepPink4             Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 10 * 0x101, 80 * 0x101};
constexpr QColor HotPink1              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 110 * 0x101, 180 * 0x101};
constexpr QColor HotPink2              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 106 * 0x101, 167 * 0x101};
constexpr QColor HotPink3              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 96 * 0x101, 144 * 0x101};
constexpr QColor HotPink4              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 58 * 0x101, 98 * 0x101};
constexpr QColor pink2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 169 * 0x101, 184 * 0x101};
constexpr QColor pink3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 145 * 0x101, 158 * 0x101};
constexpr QColor pink4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 99 * 0x101, 108 * 0x101};
constexpr QColor LightPink1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 174 * 0x101, 185 * 0x101};
constexpr QColor LightPink2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 162 * 0x101, 173 * 0x101};
constexpr QColor LightPink3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 140 * 0x101, 149 * 0x101};
constexpr QColor LightPink4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 95 * 0x101, 101 * 0x101};
constexpr QColor PaleVioletRed1        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 130 * 0x101, 171 * 0x101};
constexpr QColor PaleVioletRed2        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 121 * 0x101, 159 * 0x101};
constexpr QColor PaleVioletRed3        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 104 * 0x101, 137 * 0x101};
constexpr QColor PaleVioletRed4        Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 71 * 0x101, 93 * 0x101};
constexpr QColor maroon2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 48 * 0x101, 167 * 0x101};
constexpr QColor maroon3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 41 * 0x101, 144 * 0x101};
constexpr QColor maroon4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 28 * 0x101, 98 * 0x101};
constexpr QColor VioletRed1            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 255 * 0x101, 62 * 0x101, 150 * 0x101};
constexpr QColor VioletRed2            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 58 * 0x101, 140 * 0x101};
constexpr QColor VioletRed3            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 50 * 0x101, 120 * 0x101};
constexpr QColor VioletRed4            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 34 * 0x101, 82 * 0x101};
constexpr QColor magenta2              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 0 * 0x101, 238 * 0x101};
constexpr QColor magenta3              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 0 * 0x101, 205 * 0x101};
constexpr QColor magenta4              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 0 * 0x101, 139 * 0x101};
constexpr QColor orchid2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 122 * 0x101, 233 * 0x101};
constexpr QColor orchid3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 105 * 0x101, 201 * 0x101};
constexpr QColor orchid4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 71 * 0x101, 137 * 0x101};
constexpr QColor plum2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 174 * 0x101, 238 * 0x101};
constexpr QColor plum3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 150 * 0x101, 205 * 0x101};
constexpr QColor plum4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 102 * 0x101, 139 * 0x101};
constexpr QColor MediumOrchid1         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 224 * 0x101, 102 * 0x101, 255 * 0x101};
constexpr QColor MediumOrchid2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 209 * 0x101, 95 * 0x101, 238 * 0x101};
constexpr QColor MediumOrchid3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 180 * 0x101, 82 * 0x101, 205 * 0x101};
constexpr QColor MediumOrchid4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 122 * 0x101, 55 * 0x101, 139 * 0x101};
constexpr QColor DarkOrchid1           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 191 * 0x101, 62 * 0x101, 255 * 0x101};
constexpr QColor DarkOrchid2           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 178 * 0x101, 58 * 0x101, 238 * 0x101};
constexpr QColor DarkOrchid3           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 154 * 0x101, 50 * 0x101, 205 * 0x101};
constexpr QColor DarkOrchid4           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 104 * 0x101, 34 * 0x101, 139 * 0x101};
constexpr QColor purple2               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 145 * 0x101, 44 * 0x101, 238 * 0x101};
constexpr QColor purple3               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 125 * 0x101, 38 * 0x101, 205 * 0x101};
constexpr QColor purple4               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 85 * 0x101, 26 * 0x101, 139 * 0x101};
constexpr QColor MediumPurple1         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 171 * 0x101, 130 * 0x101, 255 * 0x101};
constexpr QColor MediumPurple2         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 159 * 0x101, 121 * 0x101, 238 * 0x101};
constexpr QColor MediumPurple3         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 137 * 0x101, 104 * 0x101, 205 * 0x101};
constexpr QColor MediumPurple4         Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 93 * 0x101, 71 * 0x101, 139 * 0x101};
constexpr QColor thistle2              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 238 * 0x101, 210 * 0x101, 238 * 0x101};
constexpr QColor thistle3              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 205 * 0x101, 181 * 0x101, 205 * 0x101};
constexpr QColor thistle4              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 123 * 0x101, 139 * 0x101};
constexpr QColor gray1                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 3 * 0x101, 3 * 0x101, 3 * 0x101};
constexpr QColor gray2                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 5 * 0x101, 5 * 0x101, 5 * 0x101};
constexpr QColor gray3                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 8 * 0x101, 8 * 0x101, 8 * 0x101};
constexpr QColor gray4                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 10 * 0x101, 10 * 0x101, 10 * 0x101};
constexpr QColor gray5                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 13 * 0x101, 13 * 0x101, 13 * 0x101};
constexpr QColor gray6                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 15 * 0x101, 15 * 0x101, 15 * 0x101};
constexpr QColor gray7                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 18 * 0x101, 18 * 0x101, 18 * 0x101};
constexpr QColor gray8                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 20 * 0x101, 20 * 0x101, 20 * 0x101};
constexpr QColor gray9                 Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 23 * 0x101, 23 * 0x101, 23 * 0x101};
constexpr QColor gray10                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 26 * 0x101, 26 * 0x101, 26 * 0x101};
constexpr QColor gray11                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 28 * 0x101, 28 * 0x101, 28 * 0x101};
constexpr QColor gray12                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 31 * 0x101, 31 * 0x101, 31 * 0x101};
constexpr QColor gray13                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 33 * 0x101, 33 * 0x101, 33 * 0x101};
constexpr QColor gray14                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 36 * 0x101, 36 * 0x101, 36 * 0x101};
constexpr QColor gray15                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 38 * 0x101, 38 * 0x101, 38 * 0x101};
constexpr QColor gray16                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 41 * 0x101, 41 * 0x101, 41 * 0x101};
constexpr QColor gray17                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 43 * 0x101, 43 * 0x101, 43 * 0x101};
constexpr QColor gray18                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 46 * 0x101, 46 * 0x101, 46 * 0x101};
constexpr QColor gray19                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 48 * 0x101, 48 * 0x101, 48 * 0x101};
constexpr QColor gray20                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 51 * 0x101, 51 * 0x101, 51 * 0x101};
constexpr QColor gray21                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 54 * 0x101, 54 * 0x101, 54 * 0x101};
constexpr QColor gray22                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 56 * 0x101, 56 * 0x101, 56 * 0x101};
constexpr QColor gray23                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 59 * 0x101, 59 * 0x101, 59 * 0x101};
constexpr QColor gray24                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 61 * 0x101, 61 * 0x101, 61 * 0x101};
constexpr QColor gray25                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 64 * 0x101, 64 * 0x101, 64 * 0x101};
constexpr QColor gray26                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 66 * 0x101, 66 * 0x101, 66 * 0x101};
constexpr QColor gray27                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 69 * 0x101, 69 * 0x101, 69 * 0x101};
constexpr QColor gray28                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 71 * 0x101, 71 * 0x101, 71 * 0x101};
constexpr QColor gray29                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 74 * 0x101, 74 * 0x101, 74 * 0x101};
constexpr QColor gray30                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 77 * 0x101, 77 * 0x101, 77 * 0x101};
constexpr QColor gray31                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 79 * 0x101, 79 * 0x101, 79 * 0x101};
constexpr QColor gray32                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 82 * 0x101, 82 * 0x101, 82 * 0x101};
constexpr QColor gray33                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 84 * 0x101, 84 * 0x101, 84 * 0x101};
constexpr QColor gray34                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 87 * 0x101, 87 * 0x101, 87 * 0x101};
constexpr QColor gray35                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 89 * 0x101, 89 * 0x101, 89 * 0x101};
constexpr QColor gray36                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 92 * 0x101, 92 * 0x101, 92 * 0x101};
constexpr QColor gray37                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 94 * 0x101, 94 * 0x101, 94 * 0x101};
constexpr QColor gray38                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 97 * 0x101, 97 * 0x101, 97 * 0x101};
constexpr QColor gray39                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 99 * 0x101, 99 * 0x101, 99 * 0x101};
constexpr QColor gray40                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 102 * 0x101, 102 * 0x101, 102 * 0x101};
constexpr QColor gray42                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 107 * 0x101, 107 * 0x101, 107 * 0x101};
constexpr QColor gray43                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 110 * 0x101, 110 * 0x101, 110 * 0x101};
constexpr QColor gray44                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 112 * 0x101, 112 * 0x101, 112 * 0x101};
constexpr QColor gray45                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 115 * 0x101, 115 * 0x101, 115 * 0x101};
constexpr QColor gray46                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 117 * 0x101, 117 * 0x101, 117 * 0x101};
constexpr QColor gray47                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 120 * 0x101, 120 * 0x101, 120 * 0x101};
constexpr QColor gray48                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 122 * 0x101, 122 * 0x101, 122 * 0x101};
constexpr QColor gray49                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 125 * 0x101, 125 * 0x101, 125 * 0x101};
constexpr QColor gray50                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 127 * 0x101, 127 * 0x101, 127 * 0x101};
constexpr QColor gray51                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 130 * 0x101, 130 * 0x101, 130 * 0x101};
constexpr QColor gray52                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 133 * 0x101, 133 * 0x101, 133 * 0x101};
constexpr QColor gray53                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 135 * 0x101, 135 * 0x101, 135 * 0x101};
constexpr QColor gray54                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 138 * 0x101, 138 * 0x101, 138 * 0x101};
constexpr QColor gray55                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 140 * 0x101, 140 * 0x101, 140 * 0x101};
constexpr QColor gray56                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 143 * 0x101, 143 * 0x101, 143 * 0x101};
constexpr QColor gray57                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 145 * 0x101, 145 * 0x101, 145 * 0x101};
constexpr QColor gray58                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 148 * 0x101, 148 * 0x101, 148 * 0x101};
constexpr QColor gray59                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 150 * 0x101, 150 * 0x101, 150 * 0x101};
constexpr QColor gray60                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 153 * 0x101, 153 * 0x101, 153 * 0x101};
constexpr QColor gray61                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 156 * 0x101, 156 * 0x101, 156 * 0x101};
constexpr QColor gray62                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 158 * 0x101, 158 * 0x101, 158 * 0x101};
constexpr QColor gray63                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 161 * 0x101, 161 * 0x101, 161 * 0x101};
constexpr QColor gray64                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 163 * 0x101, 163 * 0x101, 163 * 0x101};
constexpr QColor gray65                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 166 * 0x101, 166 * 0x101, 166 * 0x101};
constexpr QColor gray66                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 168 * 0x101, 168 * 0x101, 168 * 0x101};
constexpr QColor gray67                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 171 * 0x101, 171 * 0x101, 171 * 0x101};
constexpr QColor gray68                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 173 * 0x101, 173 * 0x101, 173 * 0x101};
constexpr QColor gray69                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 176 * 0x101, 176 * 0x101, 176 * 0x101};
constexpr QColor gray70                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 179 * 0x101, 179 * 0x101, 179 * 0x101};
constexpr QColor gray71                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 181 * 0x101, 181 * 0x101, 181 * 0x101};
constexpr QColor gray72                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 184 * 0x101, 184 * 0x101, 184 * 0x101};
constexpr QColor gray73                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 186 * 0x101, 186 * 0x101, 186 * 0x101};
constexpr QColor gray74                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 189 * 0x101, 189 * 0x101, 189 * 0x101};
constexpr QColor gray75                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 191 * 0x101, 191 * 0x101, 191 * 0x101};
constexpr QColor gray76                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 194 * 0x101, 194 * 0x101, 194 * 0x101};
constexpr QColor gray77                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 196 * 0x101, 196 * 0x101, 196 * 0x101};
constexpr QColor gray78                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 199 * 0x101, 199 * 0x101, 199 * 0x101};
constexpr QColor gray79                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 201 * 0x101, 201 * 0x101, 201 * 0x101};
constexpr QColor gray80                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 204 * 0x101, 204 * 0x101, 204 * 0x101};
constexpr QColor gray81                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 207 * 0x101, 207 * 0x101, 207 * 0x101};
constexpr QColor gray82                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 209 * 0x101, 209 * 0x101, 209 * 0x101};
constexpr QColor gray83                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 212 * 0x101, 212 * 0x101, 212 * 0x101};
constexpr QColor gray84                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 214 * 0x101, 214 * 0x101, 214 * 0x101};
constexpr QColor gray85                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 217 * 0x101, 217 * 0x101, 217 * 0x101};
constexpr QColor gray86                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 219 * 0x101, 219 * 0x101, 219 * 0x101};
constexpr QColor gray87                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 222 * 0x101, 222 * 0x101, 222 * 0x101};
constexpr QColor gray88                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 224 * 0x101, 224 * 0x101, 224 * 0x101};
constexpr QColor gray89                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 227 * 0x101, 227 * 0x101, 227 * 0x101};
constexpr QColor gray90                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 229 * 0x101, 229 * 0x101, 229 * 0x101};
constexpr QColor gray91                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 232 * 0x101, 232 * 0x101, 232 * 0x101};
constexpr QColor gray92                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 235 * 0x101, 235 * 0x101, 235 * 0x101};
constexpr QColor gray93                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 237 * 0x101, 237 * 0x101, 237 * 0x101};
constexpr QColor gray94                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 240 * 0x101, 240 * 0x101, 240 * 0x101};
constexpr QColor gray95                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 242 * 0x101, 242 * 0x101, 242 * 0x101};
constexpr QColor gray97                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 247 * 0x101, 247 * 0x101, 247 * 0x101};
constexpr QColor gray98                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 250 * 0x101, 250 * 0x101, 250 * 0x101};
constexpr QColor gray99                Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 252 * 0x101, 252 * 0x101, 252 * 0x101};
constexpr QColor darkgrey              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 169 * 0x101, 169 * 0x101, 169 * 0x101};
constexpr QColor darkblue              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 0 * 0x101, 139 * 0x101};
constexpr QColor darkcyan              Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 0 * 0x101, 139 * 0x101, 139 * 0x101};
constexpr QColor darkmagenta           Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 0 * 0x101, 139 * 0x101};
constexpr QColor darkred               Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 139 * 0x101, 0 * 0x101, 0 * 0x101};
constexpr QColor lightgreen            Q_DECL_UNUSED{ QColor::Rgb, 0xff * 0x101, 144 * 0x101, 238 * 0x101, 144 * 0x101};
// clang-format on

const QColor GhostWhite = ghostwhite;
const QColor WhiteSmoke = whitesmoke;
const QColor Gainsboro = gainsboro;
const QColor NavajoWhite = navajowhite;
const QColor FloralWhite = floralwhite;
const QColor LemonChiffon = lemonchiffon;
const QColor MintCream = mintcream;
const QColor AliceBlue = aliceblue;
const QColor LavenderBlush = lavenderblush;
const QColor MistyRose = mistyrose;
const QColor DarkSlateGray = darkslategray;
const QColor darkslategrey = darkslategray;
const QColor DarkSlateGrey = darkslategray;
const QColor DimGray = dimgray;
const QColor dimgrey = dimgray;
const QColor DimGrey = dimgray;
const QColor SlateGray = slategray;
const QColor slategrey = slategray;
const QColor SlateGrey = slategray;
const QColor LightSlateGray = lightslategray;
const QColor lightslategrey = lightslategray;
const QColor LightSlateGrey = lightslategray;
const QColor grey = gray;
const QColor LightGrey = lightgray;
const QColor lightgrey = lightgray;
const QColor LightGray = lightgray;
const QColor MidnightBlue = midnightblue;
const QColor navyblue = navy;
const QColor NavyBlue = navy;
const QColor CornflowerBlue = cornflowerblue;
const QColor DarkSlateBlue = darkslateblue;
const QColor slateblu = slateblue;
const QColor SlateBlue = slateblue;
const QColor MediumSlateBlue = mediumslateblue;
const QColor LightSlateBlue = lightslateblue;
const QColor MediumBlue = mediumblue;
const QColor RoyalBlue = royalblue;
const QColor DodgerBlue = dodgerblue;
const QColor DeepSkyBlue = deepskyblue;
const QColor SkyBlue = skyblue;
const QColor LightSkyBlue = lightskyblue;
const QColor SteelBlue = steelblue;
const QColor LightSteelBlue = lightsteelblue;
const QColor LightBlue = lightblue;
const QColor PowderBlue = powderblue;
const QColor PaleTurquoise = paleturquoise;
const QColor DarkTurquoise = darkturquoise;
const QColor MediumTurquoise = mediumturquoise;
const QColor LightCyan = lightcyan;
const QColor CadetBlue = cadetblue;
const QColor MediumAquamarine = mediumaquamarine;
const QColor DarkGreen = darkgreen;
const QColor DarkOliveGreen = darkolivegreen;
const QColor DarkSeaGreen = darkseagreen;
const QColor SeaGreen = seagreen;
const QColor MediumSeaGreen = mediumseagreen;
const QColor LightSeaGreen = lightseagreen;
const QColor PaleGreen = palegreen;
const QColor SpringGreen = springgreen;
const QColor LawnGreen = lawngreen;
const QColor MediumSpringGreen = mediumspringgreen;
const QColor GreenYellow = greenyellow;
const QColor LimeGreen = limegreen;
const QColor YellowGreen = yellowgreen;
const QColor ForestGreen = forestgreen;
const QColor OliveDrab = olivedrab;
const QColor DarkKhaki = darkkhaki;
const QColor PaleGoldenrod = palegoldenrod;
const QColor LightGoldenrodYellow = lightgoldenrodyellow;
const QColor LightYellow = lightyellow;
const QColor LightGoldenrod = lightgoldenrod;
const QColor DarkGoldenrod = darkgoldenrod;
const QColor RosyBrown = rosybrown;
const QColor IndianRed = indianred;
const QColor SaddleBrown = saddlebrown;
const QColor SandyBrown = sandybrown;
const QColor DarkSalmon = darksalmon;
const QColor LightSalmon = lightsalmon;
const QColor DarkOrange = darkorange;
const QColor LightCoral = lightcoral;
const QColor OrangeRed = orangered;
const QColor HotPink = hotpink;
const QColor DeepPink = deeppink;
const QColor LightPink = lightpink;
const QColor PaleVioletRed = palevioletred;
const QColor MediumVioletRed = mediumvioletred;
const QColor VioletRed = violetred;
const QColor MediumOrchid = mediumorchid;
const QColor DarkOrchid = darkorchid;
const QColor DarkViolet = darkviolet;
const QColor BlueViolet = blueviolet;
const QColor MediumPurple = mediumpurple;
const QColor snow1 = snow;
const QColor seashell1 = seashell;
const QColor bisque1 = bisque;
const QColor NavajoWhite1 = navajowhite;
const QColor LemonChiffon1 = lemonchiffon;
const QColor cornsilk1 = cornsilk;
const QColor ivory1 = ivory;
const QColor honeydew1 = honeydew;
const QColor LavenderBlush1 = lavenderblush;
const QColor MistyRose1 = mistyrose;
const QColor azure1 = azure;
const QColor SlateBlue1 = slateblue;
const QColor blue1 = blue;
const QColor blue3 = mediumblue;
const QColor DodgerBlue1 = dodgerblue;
const QColor DeepSkyBlue1 = deepskyblue;
const QColor SkyBlue1 = skyblue;
const QColor LightCyan1 = lightcyan;
const QColor turquoise1 = turquoise;
const QColor cyan1 = cyan;
const QColor aquamarine1 = aquamarine;
const QColor aquamarine3 = mediumaquamarine;
const QColor SeaGreen4 = seagreen;
const QColor SpringGreen1 = springgreen;
const QColor green1 = green;
const QColor chartreuse1 = chartreuse;
const QColor OliveDrab3 = olivedrab;
const QColor khaki1 = khaki;
const QColor LightYellow1 = lightyellow;
const QColor yellow1 = yellow;
const QColor gold1 = gold;
const QColor sienna1 = sienna;
const QColor burlywood1 = burlywood;
const QColor wheat1 = wheat;
const QColor tan1 = tan;
const QColor tan3 = peru;
const QColor chocolate1 = chocolate;
const QColor chocolate4 = saddlebrown;
const QColor firebrick1 = firebrick;
const QColor brown1 = brown;
const QColor LightSalmon1 = lightsalmon;
const QColor orange1 = orange;
const QColor coral1 = coral;
const QColor tomato1 = tomato;
const QColor OrangeRed1 = orangered;
const QColor red1 = red;
const QColor DeepPink1 = deeppink;
const QColor pink1 = pink;
const QColor maroon1 = maroon;
const QColor magenta1 = magenta;
const QColor orchid1 = orchid;
const QColor plum1 = plum;
const QColor purple1 = purple;
const QColor thistle1 = thistle;
const QColor gray0 = black;
const QColor grey0 = black;
const QColor grey1 = gray1;
const QColor grey2 = gray2;
const QColor grey3 = gray3;
const QColor grey4 = gray4;
const QColor grey5 = gray5;
const QColor grey6 = gray6;
const QColor grey7 = gray7;
const QColor grey8 = gray8;
const QColor grey9 = gray9;
const QColor grey10 = gray10;
const QColor grey11 = gray11;
const QColor grey12 = gray12;
const QColor grey13 = gray13;
const QColor grey14 = gray14;
const QColor grey15 = gray15;
const QColor grey16 = gray16;
const QColor grey17 = gray17;
const QColor grey18 = gray18;
const QColor grey19 = gray19;
const QColor grey20 = gray20;
const QColor grey21 = gray21;
const QColor grey22 = gray22;
const QColor grey23 = gray23;
const QColor grey24 = gray24;
const QColor grey25 = gray25;
const QColor grey26 = gray26;
const QColor grey27 = gray27;
const QColor grey28 = gray28;
const QColor grey29 = gray29;
const QColor grey30 = gray30;
const QColor grey31 = gray31;
const QColor grey32 = gray32;
const QColor grey33 = gray33;
const QColor grey34 = gray34;
const QColor grey35 = gray35;
const QColor grey36 = gray36;
const QColor grey37 = gray37;
const QColor grey38 = gray38;
const QColor grey39 = gray39;
const QColor grey40 = gray40;
const QColor gray41 = dimgray;
const QColor grey41 = dimgray;
const QColor grey42 = gray42;
const QColor grey43 = gray43;
const QColor grey44 = gray44;
const QColor grey45 = gray45;
const QColor grey46 = gray46;
const QColor grey47 = gray47;
const QColor grey48 = gray48;
const QColor grey49 = gray49;
const QColor grey50 = gray50;
const QColor grey51 = gray51;
const QColor grey52 = gray52;
const QColor grey53 = gray53;
const QColor grey54 = gray54;
const QColor grey55 = gray55;
const QColor grey56 = gray56;
const QColor grey57 = gray57;
const QColor grey58 = gray58;
const QColor grey59 = gray59;
const QColor grey60 = gray60;
const QColor grey61 = gray61;
const QColor grey62 = gray62;
const QColor grey63 = gray63;
const QColor grey64 = gray64;
const QColor grey65 = gray65;
const QColor grey66 = gray66;
const QColor grey67 = gray67;
const QColor grey68 = gray68;
const QColor grey69 = gray69;
const QColor grey70 = gray70;
const QColor grey71 = gray71;
const QColor grey72 = gray72;
const QColor grey73 = gray73;
const QColor grey74 = gray74;
const QColor grey75 = gray75;
const QColor grey76 = gray76;
const QColor grey77 = gray77;
const QColor grey78 = gray78;
const QColor grey79 = gray79;
const QColor grey80 = gray80;
const QColor grey81 = gray81;
const QColor grey82 = gray82;
const QColor grey83 = gray83;
const QColor grey84 = gray84;
const QColor grey85 = gray85;
const QColor grey86 = gray86;
const QColor grey87 = gray87;
const QColor grey88 = gray88;
const QColor grey89 = gray89;
const QColor grey90 = gray90;
const QColor grey91 = gray91;
const QColor grey92 = gray92;
const QColor grey93 = gray93;
const QColor grey94 = gray94;
const QColor grey95 = gray95;
const QColor gray96 = whitesmoke;
const QColor grey96 = whitesmoke;
const QColor grey97 = gray97;
const QColor grey98 = gray98;
const QColor grey99 = gray99;
const QColor gray100 = white;
const QColor grey100 = white;
const QColor DarkGrey = darkgrey;
const QColor darkgray = darkgrey;
const QColor DarkGray = darkgrey;
const QColor DarkBlue = darkblue;
const QColor DarkCyan = darkcyan;
const QColor DarkMagenta = darkmagenta;
const QColor DarkRed = darkred;
const QColor LightGreen = lightgreen;
#endif // Q_COMPILER_CONSTEXPR && Q_COMPILER_UNIFORM_INIT

namespace {

QMap<QString, QColor>
initaliseMap()
{
  QMap<QString, QColor> map;
  map.insert("snow", snow);
  map.insert("ghost white", ghostwhite);
  map.insert("GhostWhite", ghostwhite);
  map.insert("WhiteSmoke", whitesmoke);
  map.insert("white smoke", whitesmoke);
  map.insert("gainsboro", gainsboro);
  map.insert("Gainsboro", gainsboro);
  map.insert("navajo white", navajowhite);
  map.insert("NavajoWhite", navajowhite);
  map.insert("moccasin", moccasin);
  map.insert("cornsilk", cornsilk);
  map.insert("ivory", ivory);
  map.insert("lemon chiffon", lemonchiffon);
  map.insert("LemonChiffon", lemonchiffon);
  map.insert("seashell", seashell);
  map.insert("honeydew", honeydew);
  map.insert("mint cream", mintcream);
  map.insert("MintCream", mintcream);
  map.insert("azure", azure);
  map.insert("alice blue", aliceblue);
  map.insert("AliceBlue", aliceblue);
  map.insert("lavender", lavender);
  map.insert("lavender blush", lavenderblush);
  map.insert("LavenderBlush", lavenderblush);
  map.insert("misty rose", mistyrose);
  map.insert("MistyRose", mistyrose);
  map.insert("white", white);
  map.insert("black", black);
  map.insert("dark slate gray", darkslategray);
  map.insert("DarkSlateGray", darkslategray);
  map.insert("dark slate grey", darkslategray);
  map.insert("DarkSlateGrey", darkslategray);
  map.insert("dim gray", dimgray);
  map.insert("DimGray", dimgray);
  map.insert("dim grey", dimgray);
  map.insert("DimGrey", dimgray);
  map.insert("slate gray", slategray);
  map.insert("SlateGray", slategray);
  map.insert("slate grey", slategray);
  map.insert("SlateGrey", slategray);
  map.insert("light slate gray", lightslategray);
  map.insert("LightSlateGray", lightslategray);
  map.insert("light slate grey", lightslategray);
  map.insert("LightSlateGrey", lightslategray);
  map.insert("gray", gray);
  map.insert("grey", gray);
  map.insert("light grey", lightgray);
  map.insert("LightGrey", lightgray);
  map.insert("light gray", lightgray);
  map.insert("LightGray", lightgray);
  map.insert("midnight blue", midnightblue);
  map.insert("MidnightBlue", midnightblue);
  map.insert("navy", navy);
  map.insert("navy blue", navy);
  map.insert("NavyBlue", navy);
  map.insert("cornflower blue", cornflowerblue);
  map.insert("CornflowerBlue", cornflowerblue);
  map.insert("dark slate blue", darkslateblue);
  map.insert("DarkSlateBlue", darkslateblue);
  map.insert("slate blue", slateblue);
  map.insert("SlateBlue", slateblue);
  map.insert("medium slate blue", mediumslateblue);
  map.insert("MediumSlateBlue", mediumslateblue);
  map.insert("light slate blue", lightslateblue);
  map.insert("LightSlateBlue", lightslateblue);
  map.insert("medium blue", mediumblue);
  map.insert("MediumBlue", mediumblue);
  map.insert("royal blue", royalblue);
  map.insert("RoyalBlue", royalblue);
  map.insert("blue", blue);
  map.insert("dodger blue", dodgerblue);
  map.insert("DodgerBlue", dodgerblue);
  map.insert("deep sky blue", deepskyblue);
  map.insert("DeepSkyBlue", deepskyblue);
  map.insert("sky blue", skyblue);
  map.insert("SkyBlue", skyblue);
  map.insert("light sky blue", lightskyblue);
  map.insert("LightSkyBlue", lightskyblue);
  map.insert("steel blue", steelblue);
  map.insert("SteelBlue", steelblue);
  map.insert("light steel blue", lightsteelblue);
  map.insert("LightSteelBlue", lightsteelblue);
  map.insert("light blue", lightblue);
  map.insert("LightBlue", lightblue);
  map.insert("powder blue", powderblue);
  map.insert("PowderBlue", powderblue);
  map.insert("pale turquoise", paleturquoise);
  map.insert("PaleTurquoise", paleturquoise);
  map.insert("dark turquoise", darkturquoise);
  map.insert("DarkTurquoise", darkturquoise);
  map.insert("medium turquoise", mediumturquoise);
  map.insert("MediumTurquoise", mediumturquoise);
  map.insert("turquoise", turquoise);
  map.insert("cyan", cyan);
  map.insert("light cyan", lightcyan);
  map.insert("LightCyan", lightcyan);
  map.insert("cadet blue", cadetblue);
  map.insert("CadetBlue", cadetblue);
  map.insert("medium aquamarine", mediumaquamarine);
  map.insert("MediumAquamarine", mediumaquamarine);
  map.insert("aquamarine", aquamarine);
  map.insert("dark green", darkgreen);
  map.insert("DarkGreen", darkgreen);
  map.insert("dark olive green", darkolivegreen);
  map.insert("DarkOliveGreen", darkolivegreen);
  map.insert("dark sea green", darkseagreen);
  map.insert("DarkSeaGreen", darkseagreen);
  map.insert("sea green", seagreen);
  map.insert("SeaGreen", seagreen);
  map.insert("medium sea green", mediumseagreen);
  map.insert("MediumSeaGreen", mediumseagreen);
  map.insert("light sea green", lightseagreen);
  map.insert("LightSeaGreen", lightseagreen);
  map.insert("pale green", palegreen);
  map.insert("PaleGreen", palegreen);
  map.insert("spring green", springgreen);
  map.insert("SpringGreen", springgreen);
  map.insert("lawn green", lawngreen);
  map.insert("LawnGreen", lawngreen);
  map.insert("green", green);
  map.insert("chartreuse", chartreuse);
  map.insert("medium spring green", mediumspringgreen);
  map.insert("MediumSpringGreen", mediumspringgreen);
  map.insert("green yellow", greenyellow);
  map.insert("GreenYellow", greenyellow);
  map.insert("lime green", limegreen);
  map.insert("LimeGreen", limegreen);
  map.insert("yellow green", yellowgreen);
  map.insert("YellowGreen", yellowgreen);
  map.insert("forest green", forestgreen);
  map.insert("ForestGreen", forestgreen);
  map.insert("olive drab", olivedrab);
  map.insert("OliveDrab", olivedrab);
  map.insert("dark khaki", darkkhaki);
  map.insert("DarkKhaki", darkkhaki);
  map.insert("khaki", khaki);
  map.insert("pale goldenrod", palegoldenrod);
  map.insert("PaleGoldenrod", palegoldenrod);
  map.insert("light goldenrod yellow", lightgoldenrodyellow);
  map.insert("LightGoldenrodYellow", lightgoldenrodyellow);
  map.insert("light yellow", lightyellow);
  map.insert("LightYellow", lightyellow);
  map.insert("yellow", yellow);
  map.insert("gold", gold);
  map.insert("light goldenrod", lightgoldenrod);
  map.insert("LightGoldenrod", lightgoldenrod);
  map.insert("goldenrod", goldenrod);
  map.insert("dark goldenrod", darkgoldenrod);
  map.insert("DarkGoldenrod", darkgoldenrod);
  map.insert("rosy brown", rosybrown);
  map.insert("RosyBrown", rosybrown);
  map.insert("indian red", indianred);
  map.insert("IndianRed", indianred);
  map.insert("saddle brown", saddlebrown);
  map.insert("SaddleBrown", saddlebrown);
  map.insert("sienna", sienna);
  map.insert("peru", peru);
  map.insert("burlywood", burlywood);
  map.insert("beige", beige);
  map.insert("wheat", wheat);
  map.insert("sandy brown", sandybrown);
  map.insert("SandyBrown", sandybrown);
  map.insert("tan", tan);
  map.insert("chocolate", chocolate);
  map.insert("firebrick", firebrick);
  map.insert("brown", brown);
  map.insert("dark salmon", darksalmon);
  map.insert("DarkSalmon", darksalmon);
  map.insert("salmon", salmon);
  map.insert("light salmon", lightsalmon);
  map.insert("LightSalmon", lightsalmon);
  map.insert("orange", orange);
  map.insert("dark orange", darkorange);
  map.insert("DarkOrange", darkorange);
  map.insert("coral", coral);
  map.insert("light coral", lightcoral);
  map.insert("LightCoral", lightcoral);
  map.insert("tomato", tomato);
  map.insert("orange red", orangered);
  map.insert("OrangeRed", orangered);
  map.insert("red", red);
  map.insert("hot pink", hotpink);
  map.insert("HotPink", hotpink);
  map.insert("deep pink", deeppink);
  map.insert("DeepPink", deeppink);
  map.insert("pink", pink);
  map.insert("light pink", lightpink);
  map.insert("LightPink", lightpink);
  map.insert("pale violet red", palevioletred);
  map.insert("PaleVioletRed", palevioletred);
  map.insert("maroon", maroon);
  map.insert("medium violet red", mediumvioletred);
  map.insert("MediumVioletRed", mediumvioletred);
  map.insert("violet red", violetred);
  map.insert("VioletRed", violetred);
  map.insert("magenta", magenta);
  map.insert("violet", violet);
  map.insert("plum", plum);
  map.insert("orchid", orchid);
  map.insert("medium orchid", mediumorchid);
  map.insert("MediumOrchid", mediumorchid);
  map.insert("dark orchid", darkorchid);
  map.insert("DarkOrchid", darkorchid);
  map.insert("dark violet", darkviolet);
  map.insert("DarkViolet", darkviolet);
  map.insert("blue violet", blueviolet);
  map.insert("BlueViolet", blueviolet);
  map.insert("purple", purple);
  map.insert("medium purple", mediumpurple);
  map.insert("MediumPurple", mediumpurple);
  map.insert("thistle", thistle);
  map.insert("snow1", snow);
  map.insert("snow2", snow2);
  map.insert("snow3", snow3);
  map.insert("snow4", snow4);
  map.insert("seashell1", seashell);
  map.insert("seashell2", seashell2);
  map.insert("seashell3", seashell3);
  map.insert("seashell4", seashell4);
  map.insert("floral white", floralwhite);
  map.insert("FloralWhite", floralwhite);
  map.insert("antique white", antiquewhite);
  map.insert("AntiqueWhite1", AntiqueWhite1);
  map.insert("AntiqueWhite2", AntiqueWhite2);
  map.insert("AntiqueWhite3", AntiqueWhite3);
  map.insert("AntiqueWhite4", AntiqueWhite4);
  map.insert("bisque", bisque);
  map.insert("bisque1", bisque);
  map.insert("bisque2", bisque2);
  map.insert("bisque3", bisque3);
  map.insert("bisque4", bisque4);
  map.insert("peach puff", peachpuff);
  map.insert("PeachPuff1", peachpuff);
  map.insert("PeachPuff2", PeachPuff2);
  map.insert("PeachPuff3", PeachPuff3);
  map.insert("PeachPuff4", PeachPuff4);
  map.insert("NavajoWhite1", navajowhite);
  map.insert("NavajoWhite2", NavajoWhite2);
  map.insert("NavajoWhite3", NavajoWhite3);
  map.insert("NavajoWhite4", NavajoWhite4);
  map.insert("LemonChiffon1", lemonchiffon);
  map.insert("LemonChiffon2", LemonChiffon2);
  map.insert("LemonChiffon3", LemonChiffon3);
  map.insert("LemonChiffon4", LemonChiffon4);
  map.insert("cornsilk1", cornsilk);
  map.insert("cornsilk2", cornsilk2);
  map.insert("cornsilk3", cornsilk3);
  map.insert("cornsilk4", cornsilk4);
  map.insert("ivory1", ivory);
  map.insert("ivory2", ivory2);
  map.insert("ivory3", ivory3);
  map.insert("ivory4", ivory4);
  map.insert("honeydew1", honeydew);
  map.insert("honeydew2", honeydew2);
  map.insert("honeydew3", honeydew3);
  map.insert("honeydew4", honeydew4);
  map.insert("LavenderBlush1", lavenderblush);
  map.insert("LavenderBlush2", LavenderBlush2);
  map.insert("LavenderBlush3", LavenderBlush3);
  map.insert("LavenderBlush4", LavenderBlush4);
  map.insert("MistyRose1", mistyrose);
  map.insert("MistyRose2", MistyRose2);
  map.insert("MistyRose3", MistyRose3);
  map.insert("MistyRose4", MistyRose4);
  map.insert("azure1", azure);
  map.insert("azure2", azure2);
  map.insert("azure3", azure3);
  map.insert("azure4", azure4);
  map.insert("SlateBlue1", slateblue);
  map.insert("SlateBlue2", SlateBlue2);
  map.insert("SlateBlue3", SlateBlue3);
  map.insert("SlateBlue4", SlateBlue4);
  map.insert("RoyalBlue1", RoyalBlue1);
  map.insert("RoyalBlue2", RoyalBlue2);
  map.insert("RoyalBlue3", RoyalBlue3);
  map.insert("RoyalBlue4", RoyalBlue4);
  map.insert("blue1", blue);
  map.insert("blue2", blue2);
  map.insert("blue3", mediumblue);
  map.insert("blue4", blue4);
  map.insert("DodgerBlue1", dodgerblue);
  map.insert("DodgerBlue2", DodgerBlue2);
  map.insert("DodgerBlue3", DodgerBlue3);
  map.insert("DodgerBlue4", DodgerBlue4);
  map.insert("SteelBlue1", SteelBlue1);
  map.insert("SteelBlue2", SteelBlue2);
  map.insert("SteelBlue3", SteelBlue3);
  map.insert("SteelBlue4", SteelBlue4);
  map.insert("DeepSkyBlue1", deepskyblue);
  map.insert("DeepSkyBlue2", DeepSkyBlue2);
  map.insert("DeepSkyBlue3", DeepSkyBlue3);
  map.insert("DeepSkyBlue4", DeepSkyBlue4);
  map.insert("SkyBlue1", skyblue);
  map.insert("SkyBlue2", SkyBlue2);
  map.insert("SkyBlue3", SkyBlue3);
  map.insert("SkyBlue4", SkyBlue4);
  map.insert("LightSkyBlue1", LightSkyBlue1);
  map.insert("LightSkyBlue2", LightSkyBlue2);
  map.insert("LightSkyBlue3", LightSkyBlue3);
  map.insert("LightSkyBlue4", LightSkyBlue4);
  map.insert("SlateGray1", SlateGray1);
  map.insert("SlateGray2", SlateGray2);
  map.insert("SlateGray3", SlateGray3);
  map.insert("SlateGray4", SlateGray4);
  map.insert("LightSteelBlue1", LightSteelBlue1);
  map.insert("LightSteelBlue2", LightSteelBlue2);
  map.insert("LightSteelBlue3", LightSteelBlue3);
  map.insert("LightSteelBlue4", LightSteelBlue4);
  map.insert("LightBlue1", LightBlue1);
  map.insert("LightBlue2", LightBlue2);
  map.insert("LightBlue3", LightBlue3);
  map.insert("LightBlue4", LightBlue4);
  map.insert("LightCyan1", lightcyan);
  map.insert("LightCyan2", LightCyan2);
  map.insert("LightCyan3", LightCyan3);
  map.insert("LightCyan4", LightCyan4);
  map.insert("PaleTurquoise1", PaleTurquoise1);
  map.insert("PaleTurquoise2", PaleTurquoise2);
  map.insert("PaleTurquoise3", PaleTurquoise3);
  map.insert("PaleTurquoise4", PaleTurquoise4);
  map.insert("CadetBlue1", CadetBlue1);
  map.insert("CadetBlue2", CadetBlue2);
  map.insert("CadetBlue3", CadetBlue3);
  map.insert("CadetBlue4", CadetBlue4);
  map.insert("turquoise1", turquoise);
  map.insert("turquoise2", turquoise2);
  map.insert("turquoise3", turquoise3);
  map.insert("turquoise4", turquoise4);
  map.insert("cyan1", cyan);
  map.insert("cyan2", cyan2);
  map.insert("cyan3", cyan3);
  map.insert("cyan4", cyan4);
  map.insert("DarkSlateGray1", DarkSlateGray1);
  map.insert("DarkSlateGray2", DarkSlateGray2);
  map.insert("DarkSlateGray3", DarkSlateGray3);
  map.insert("DarkSlateGray4", DarkSlateGray4);
  map.insert("aquamarine1", aquamarine);
  map.insert("aquamarine2", aquamarine2);
  map.insert("aquamarine3", mediumaquamarine);
  map.insert("aquamarine4", aquamarine4);
  map.insert("DarkSeaGreen1", DarkSeaGreen1);
  map.insert("DarkSeaGreen2", DarkSeaGreen2);
  map.insert("DarkSeaGreen3", DarkSeaGreen3);
  map.insert("DarkSeaGreen4", DarkSeaGreen4);
  map.insert("SeaGreen1", SeaGreen1);
  map.insert("SeaGreen2", SeaGreen2);
  map.insert("SeaGreen3", SeaGreen3);
  map.insert("SeaGreen4", seagreen);
  map.insert("PaleGreen1", PaleGreen1);
  map.insert("PaleGreen2", PaleGreen2);
  map.insert("PaleGreen3", PaleGreen3);
  map.insert("PaleGreen4", PaleGreen4);
  map.insert("SpringGreen1", springgreen);
  map.insert("SpringGreen2", SpringGreen2);
  map.insert("SpringGreen3", SpringGreen3);
  map.insert("SpringGreen4", SpringGreen4);
  map.insert("green1", green);
  map.insert("green2", green2);
  map.insert("green3", green3);
  map.insert("green4", green4);
  map.insert("chartreuse1", chartreuse);
  map.insert("chartreuse2", chartreuse2);
  map.insert("chartreuse3", chartreuse3);
  map.insert("chartreuse4", chartreuse4);
  map.insert("OliveDrab1", OliveDrab1);
  map.insert("OliveDrab2", OliveDrab2);
  map.insert("OliveDrab3", yellowgreen);
  map.insert("OliveDrab4", OliveDrab4);
  map.insert("DarkOliveGreen1", DarkOliveGreen1);
  map.insert("DarkOliveGreen2", DarkOliveGreen2);
  map.insert("DarkOliveGreen3", DarkOliveGreen3);
  map.insert("DarkOliveGreen4", DarkOliveGreen4);
  map.insert("khaki1", khaki);
  map.insert("khaki2", khaki2);
  map.insert("khaki3", khaki3);
  map.insert("khaki4", khaki4);
  map.insert("LightGoldenrod1", LightGoldenrod1);
  map.insert("LightGoldenrod2", LightGoldenrod2);
  map.insert("LightGoldenrod3", LightGoldenrod3);
  map.insert("LightGoldenrod4", LightGoldenrod4);
  map.insert("LightYellow1", lightyellow);
  map.insert("LightYellow2", LightYellow2);
  map.insert("LightYellow3", LightYellow3);
  map.insert("LightYellow4", LightYellow4);
  map.insert("yellow1", yellow);
  map.insert("yellow2", yellow2);
  map.insert("yellow3", yellow3);
  map.insert("yellow4", yellow4);
  map.insert("gold1", gold);
  map.insert("gold2", gold2);
  map.insert("gold3", gold3);
  map.insert("gold4", gold4);
  map.insert("goldenrod1", goldenrod1);
  map.insert("goldenrod2", goldenrod2);
  map.insert("goldenrod3", goldenrod3);
  map.insert("goldenrod4", goldenrod4);
  map.insert("DarkGoldenrod1", DarkGoldenrod1);
  map.insert("DarkGoldenrod2", DarkGoldenrod2);
  map.insert("DarkGoldenrod3", DarkGoldenrod3);
  map.insert("DarkGoldenrod4", DarkGoldenrod4);
  map.insert("RosyBrown1", RosyBrown1);
  map.insert("RosyBrown2", RosyBrown2);
  map.insert("RosyBrown3", RosyBrown3);
  map.insert("RosyBrown4", RosyBrown4);
  map.insert("IndianRed1", IndianRed1);
  map.insert("IndianRed2", IndianRed2);
  map.insert("IndianRed3", IndianRed3);
  map.insert("IndianRed4", IndianRed4);
  map.insert("sienna1", sienna);
  map.insert("sienna2", sienna2);
  map.insert("sienna3", sienna3);
  map.insert("sienna4", sienna4);
  map.insert("burlywood1", burlywood);
  map.insert("burlywood2", burlywood2);
  map.insert("burlywood3", burlywood3);
  map.insert("burlywood4", burlywood4);
  map.insert("wheat1", wheat);
  map.insert("wheat2", wheat2);
  map.insert("wheat3", wheat3);
  map.insert("wheat4", wheat4);
  map.insert("tan1", tan);
  map.insert("tan2", tan2);
  map.insert("tan3", peru);
  map.insert("tan4", tan4);
  map.insert("chocolate1", chocolate);
  map.insert("chocolate2", chocolate2);
  map.insert("chocolate3", chocolate3);
  map.insert("chocolate4", saddlebrown);
  map.insert("firebrick1", firebrick);
  map.insert("firebrick2", firebrick2);
  map.insert("firebrick3", firebrick3);
  map.insert("firebrick4", firebrick4);
  map.insert("brown1", brown);
  map.insert("brown2", brown2);
  map.insert("brown3", brown3);
  map.insert("brown4", brown4);
  map.insert("salmon1", salmon);
  map.insert("salmon2", salmon2);
  map.insert("salmon3", salmon4);
  map.insert("salmon4", salmon4);
  map.insert("LightSalmon1", lightsalmon);
  map.insert("LightSalmon2", LightSalmon2);
  map.insert("LightSalmon3", LightSalmon3);
  map.insert("LightSalmon4", LightSalmon4);
  map.insert("orange1", orange);
  map.insert("orange2", orange2);
  map.insert("orange3", orange3);
  map.insert("orange4", orange4);
  map.insert("DarkOrange1", DarkOrange1);
  map.insert("DarkOrange2", DarkOrange2);
  map.insert("DarkOrange3", DarkOrange3);
  map.insert("DarkOrange4", DarkOrange4);
  map.insert("coral1", coral);
  map.insert("coral2", coral2);
  map.insert("coral3", coral3);
  map.insert("coral4", coral4);
  map.insert("tomato1", tomato);
  map.insert("tomato2", tomato2);
  map.insert("tomato3", tomato3);
  map.insert("tomato4", tomato4);
  map.insert("OrangeRed1", orangered);
  map.insert("OrangeRed2", OrangeRed2);
  map.insert("OrangeRed3", OrangeRed3);
  map.insert("OrangeRed4", OrangeRed4);
  map.insert("red1", red);
  map.insert("red2", red2);
  map.insert("red3", red3);
  map.insert("red4", red4);
  map.insert("DeepPink1", deeppink);
  map.insert("DeepPink2", DeepPink2);
  map.insert("DeepPink3", DeepPink3);
  map.insert("DeepPink4", DeepPink4);
  map.insert("HotPink1", HotPink1);
  map.insert("HotPink2", HotPink2);
  map.insert("HotPink3", HotPink3);
  map.insert("HotPink4", HotPink4);
  map.insert("pink1", pink);
  map.insert("pink2", pink2);
  map.insert("pink3", pink3);
  map.insert("pink4", pink4);
  map.insert("LightPink1", LightPink1);
  map.insert("LightPink2", LightPink2);
  map.insert("LightPink3", LightPink3);
  map.insert("LightPink4", LightPink4);
  map.insert("PaleVioletRed1", PaleVioletRed1);
  map.insert("PaleVioletRed2", PaleVioletRed2);
  map.insert("PaleVioletRed3", PaleVioletRed3);
  map.insert("PaleVioletRed4", PaleVioletRed4);
  map.insert("maroon1", maroon);
  map.insert("maroon2", maroon2);
  map.insert("maroon3", maroon3);
  map.insert("maroon4", maroon4);
  map.insert("VioletRed1", VioletRed1);
  map.insert("VioletRed2", VioletRed2);
  map.insert("VioletRed3", VioletRed3);
  map.insert("VioletRed4", VioletRed4);
  map.insert("magenta1", magenta);
  map.insert("magenta2", magenta2);
  map.insert("magenta3", magenta3);
  map.insert("magenta4", magenta4);
  map.insert("orchid1", orchid);
  map.insert("orchid2", orchid2);
  map.insert("orchid3", orchid3);
  map.insert("orchid4", orchid4);
  map.insert("plum1", plum);
  map.insert("plum2", plum2);
  map.insert("plum3", plum3);
  map.insert("plum4", plum4);
  map.insert("MediumOrchid1", MediumOrchid1);
  map.insert("MediumOrchid2", MediumOrchid2);
  map.insert("MediumOrchid3", MediumOrchid3);
  map.insert("MediumOrchid4", MediumOrchid4);
  map.insert("DarkOrchid1", DarkOrchid1);
  map.insert("DarkOrchid2", DarkOrchid2);
  map.insert("DarkOrchid3", DarkOrchid3);
  map.insert("DarkOrchid4", DarkOrchid4);
  map.insert("purple1", purple);
  map.insert("purple2", purple2);
  map.insert("purple3", purple3);
  map.insert("purple4", purple4);
  map.insert("MediumPurple1", MediumPurple1);
  map.insert("MediumPurple2", MediumPurple2);
  map.insert("MediumPurple3", MediumPurple3);
  map.insert("MediumPurple4", MediumPurple4);
  map.insert("thistle1", thistle);
  map.insert("thistle2", thistle2);
  map.insert("thistle3", thistle3);
  map.insert("thistle4", thistle4);
  map.insert("gray0", black);
  map.insert("grey0", black);
  map.insert("gray1", gray1);
  map.insert("grey1", gray1);
  map.insert("gray2", gray2);
  map.insert("grey2", gray2);
  map.insert("gray3", gray3);
  map.insert("grey3", gray3);
  map.insert("gray4", gray4);
  map.insert("grey4", gray4);
  map.insert("gray5", gray5);
  map.insert("grey5", gray5);
  map.insert("gray6", gray6);
  map.insert("grey6", gray6);
  map.insert("gray7", gray7);
  map.insert("grey7", gray7);
  map.insert("gray8", gray8);
  map.insert("grey8", gray8);
  map.insert("gray9", gray9);
  map.insert("grey9", gray9);
  map.insert("gray10", gray10);
  map.insert("grey10", gray10);
  map.insert("gray11", gray11);
  map.insert("grey11", gray11);
  map.insert("gray12", gray12);
  map.insert("grey12", gray12);
  map.insert("gray13", gray13);
  map.insert("grey13", gray13);
  map.insert("gray14", gray14);
  map.insert("grey14", gray14);
  map.insert("gray15", gray15);
  map.insert("grey15", gray15);
  map.insert("gray16", gray16);
  map.insert("grey16", gray16);
  map.insert("gray17", gray17);
  map.insert("grey17", gray17);
  map.insert("gray18", gray18);
  map.insert("grey18", gray18);
  map.insert("gray19", gray19);
  map.insert("grey19", gray19);
  map.insert("gray20", gray20);
  map.insert("grey20", gray20);
  map.insert("gray21", gray21);
  map.insert("grey21", gray21);
  map.insert("gray22", gray22);
  map.insert("grey22", gray22);
  map.insert("gray23", gray23);
  map.insert("grey23", gray23);
  map.insert("gray24", gray24);
  map.insert("grey24", gray24);
  map.insert("gray25", gray25);
  map.insert("grey25", gray25);
  map.insert("gray26", gray26);
  map.insert("grey26", gray26);
  map.insert("gray27", gray27);
  map.insert("grey27", gray27);
  map.insert("gray28", gray28);
  map.insert("grey28", gray28);
  map.insert("gray29", gray29);
  map.insert("grey29", gray29);
  map.insert("gray30", gray30);
  map.insert("grey30", gray30);
  map.insert("gray31", gray31);
  map.insert("grey31", gray31);
  map.insert("gray32", gray32);
  map.insert("grey32", gray32);
  map.insert("gray33", gray33);
  map.insert("grey33", gray33);
  map.insert("gray34", gray34);
  map.insert("grey34", gray34);
  map.insert("gray35", gray35);
  map.insert("grey35", gray35);
  map.insert("gray36", gray36);
  map.insert("grey36", gray36);
  map.insert("gray37", gray37);
  map.insert("grey37", gray37);
  map.insert("gray38", gray38);
  map.insert("grey38", gray38);
  map.insert("gray39", gray39);
  map.insert("grey39", gray39);
  map.insert("gray40", gray40);
  map.insert("grey40", gray40);
  map.insert("gray41", dimgray);
  map.insert("grey41", dimgray);
  map.insert("gray42", gray42);
  map.insert("grey42", gray42);
  map.insert("gray43", gray43);
  map.insert("grey43", gray43);
  map.insert("gray44", gray44);
  map.insert("grey44", gray44);
  map.insert("gray45", gray45);
  map.insert("grey45", gray45);
  map.insert("gray46", gray46);
  map.insert("grey46", gray46);
  map.insert("gray47", gray47);
  map.insert("grey47", gray47);
  map.insert("gray48", gray48);
  map.insert("grey48", gray48);
  map.insert("gray49", gray49);
  map.insert("grey49", gray49);
  map.insert("gray50", gray50);
  map.insert("grey50", gray50);
  map.insert("gray51", gray51);
  map.insert("grey51", gray51);
  map.insert("gray52", gray52);
  map.insert("grey52", gray52);
  map.insert("gray53", gray53);
  map.insert("grey53", gray53);
  map.insert("gray54", gray54);
  map.insert("grey54", gray54);
  map.insert("gray55", gray55);
  map.insert("grey55", gray55);
  map.insert("gray56", gray56);
  map.insert("grey56", gray56);
  map.insert("gray57", gray57);
  map.insert("grey57", gray57);
  map.insert("gray58", gray58);
  map.insert("grey58", gray58);
  map.insert("gray59", gray59);
  map.insert("grey59", gray59);
  map.insert("gray60", gray60);
  map.insert("grey60", gray60);
  map.insert("gray61", gray61);
  map.insert("grey61", gray61);
  map.insert("gray62", gray62);
  map.insert("grey62", gray62);
  map.insert("gray63", gray63);
  map.insert("grey63", gray63);
  map.insert("gray64", gray64);
  map.insert("grey64", gray64);
  map.insert("gray65", gray65);
  map.insert("grey65", gray65);
  map.insert("gray66", gray66);
  map.insert("grey66", gray66);
  map.insert("gray67", gray67);
  map.insert("grey67", gray67);
  map.insert("gray68", gray68);
  map.insert("grey68", gray69);
  map.insert("gray69", gray69);
  map.insert("grey69", gray69);
  map.insert("gray70", gray70);
  map.insert("grey70", gray70);
  map.insert("gray71", gray71);
  map.insert("grey71", gray71);
  map.insert("gray72", gray72);
  map.insert("grey72", gray72);
  map.insert("gray73", gray73);
  map.insert("grey73", gray73);
  map.insert("gray74", gray74);
  map.insert("grey74", gray74);
  map.insert("gray75", gray75);
  map.insert("grey75", gray75);
  map.insert("gray76", gray76);
  map.insert("grey76", gray76);
  map.insert("gray77", gray77);
  map.insert("grey77", gray77);
  map.insert("gray78", gray78);
  map.insert("grey78", gray78);
  map.insert("gray79", gray79);
  map.insert("grey79", gray79);
  map.insert("gray80", gray80);
  map.insert("grey80", gray80);
  map.insert("gray81", gray81);
  map.insert("grey81", gray81);
  map.insert("gray82", gray82);
  map.insert("grey82", gray82);
  map.insert("gray83", gray83);
  map.insert("grey83", gray83);
  map.insert("gray84", gray84);
  map.insert("grey84", gray84);
  map.insert("gray85", gray85);
  map.insert("grey85", gray85);
  map.insert("gray86", gray86);
  map.insert("grey86", gray86);
  map.insert("gray87", gray87);
  map.insert("grey87", gray87);
  map.insert("gray88", gray88);
  map.insert("grey88", gray88);
  map.insert("gray89", gray89);
  map.insert("grey89", gray89);
  map.insert("gray90", gray90);
  map.insert("grey90", gray90);
  map.insert("gray91", gray91);
  map.insert("grey91", gray91);
  map.insert("gray92", gray92);
  map.insert("grey92", gray92);
  map.insert("gray93", gray93);
  map.insert("grey93", gray93);
  map.insert("gray94", gray94);
  map.insert("grey94", gray94);
  map.insert("gray95", gray95);
  map.insert("grey95", gray95);
  map.insert("gray96", whitesmoke);
  map.insert("grey96", whitesmoke);
  map.insert("gray97", gray97);
  map.insert("grey97", gray97);
  map.insert("gray98", gray98);
  map.insert("grey98", gray98);
  map.insert("gray99", gray99);
  map.insert("grey99", gray99);
  map.insert("gray100", white);
  map.insert("grey100", white);
  map.insert("dark grey", darkgrey);
  map.insert("DarkGrey", darkgrey);
  map.insert("dark gray", darkgrey);
  map.insert("DarkGray", darkgrey);
  map.insert("dark blue", darkblue);
  map.insert("DarkBlue", darkblue);
  map.insert("dark cyan", darkcyan);
  map.insert("DarkCyan", darkcyan);
  map.insert("dark magenta", darkmagenta);
  map.insert("DarkMagenta", darkmagenta);
  map.insert("dark red", darkred);
  map.insert("DarkRed", darkred);
  map.insert("light green", lightgreen);
  map.insert("LightGreen", lightgreen);
  return map;
} // initaliseSingle()

QVector<QString>
initaliseSingle()
{
  QVector<QString> list;
  list << "snow"
       << "ghost white"
       << "antique white"
       << "peach puff"
       << "white smoke"
       << "gainsboro"
       << "navajo white"
       << "moccasin"
       << "cornsilk"
       << "ivory"
       << "lemon chiffon"
       << "seashell"
       << "honeydew"
       << "mint cream"
       << "azure"
       << "alice blue"
       << "lavender"
       << "lavender blush"
       << "misty rose"
       << "white"
       << "black"
       << "dark slate gray"
       << "dim gray"
       << "slate gray"
       << "light slate gray"
       << "gray"
       << "light gray"
       << "midnight blue"
       << "navy blue"
       << "cornflower blue"
       << "dark slate blue"
       << "slate blue"
       << "medium slate blue"
       << "light slate blue"
       << "medium blue"
       << "royal blue"
       << "blue"
       << "dodger blue"
       << "deep sky blue"
       << "sky blue"
       << "light sky blue"
       << "steel blue"
       << "light steel blue"
       << "light blue"
       << "powder blue"
       << "pale turquoise"
       << "dark turquoise"
       << "medium turquoise"
       << "turquoise"
       << "cyan"
       << "light cyan"
       << "cadet blue"
       << "medium aquamarine"
       << "aquamarine"
       << "dark green"
       << "dark olive green"
       << "dark sea green"
       << "sea green"
       << "medium sea green"
       << "light sea green"
       << "pale green"
       << "spring green"
       << "lawn green"
       << "green"
       << "chartreuse"
       << "medium spring green"
       << "green yellow"
       << "lime green"
       << "yellow green"
       << "forest green"
       << "olive drab"
       << "dark khaki"
       << "khaki"
       << "pale goldenrod"
       << "light goldenrod yellow"
       << "light yellow"
       << "yellow"
       << "gold"
       << "light goldenrod"
       << "goldenrod"
       << "dark goldenrod"
       << "rosy brown"
       << "indian red"
       << "saddle brown"
       << "sienna"
       << "peru"
       << "burlywood"
       << "beige"
       << "wheat"
       << "sandy brown"
       << "tan"
       << "chocolate"
       << "firebrick"
       << "brown"
       << "dark salmon"
       << "salmon"
       << "light salmon"
       << "orange"
       << "dark orange"
       << "coral"
       << "light coral"
       << "tomato"
       << "orange red"
       << "red"
       << "hot pink"
       << "deep pink"
       << "pink"
       << "light pink"
       << "pale violet red"
       << "maroon"
       << "medium violet red"
       << "violet red"
       << "magenta"
       << "violet"
       << "plum"
       << "orchid"
       << "medium orchid"
       << "dark orchid"
       << "dark violet"
       << "blue violet"
       << "purple"
       << "medium purple"
       << "thistle"
       << "snow1"
       << "snow2"
       << "snow3"
       << "snow4"
       << "seashell1"
       << "seashell2"
       << "seashell3"
       << "seashell4"
       << "floral white"
       << "FloralWhite"
       << "AntiqueWhite1"
       << "AntiqueWhite2"
       << "AntiqueWhite3"
       << "AntiqueWhite4"
       << "bisque"
       << "bisque1"
       << "bisque2"
       << "bisque3"
       << "bisque4"
       << "PeachPuff1"
       << "PeachPuff2"
       << "PeachPuff3"
       << "PeachPuff4"
       << "NavajoWhite1"
       << "NavajoWhite2"
       << "NavajoWhite3"
       << "NavajoWhite4"
       << "LemonChiffon1"
       << "LemonChiffon2"
       << "LemonChiffon3"
       << "LemonChiffon4"
       << "cornsilk1"
       << "cornsilk2"
       << "cornsilk3"
       << "cornsilk4"
       << "ivory1"
       << "ivory2"
       << "ivory3"
       << "ivory4"
       << "honeydew1"
       << "honeydew2"
       << "honeydew3"
       << "honeydew4"
       << "LavenderBlush1"
       << "LavenderBlush2"
       << "LavenderBlush3"
       << "LavenderBlush4"
       << "MistyRose1"
       << "MistyRose2"
       << "MistyRose3"
       << "MistyRose4"
       << "azure1"
       << "azure2"
       << "azure3"
       << "azure4"
       << "SlateBlue1"
       << "SlateBlue2"
       << "SlateBlue3"
       << "SlateBlue4"
       << "RoyalBlue1"
       << "RoyalBlue2"
       << "RoyalBlue3"
       << "RoyalBlue4"
       << "blue1"
       << "blue2"
       << "blue3"
       << "blue4"
       << "DodgerBlue1"
       << "DodgerBlue2"
       << "DodgerBlue3"
       << "DodgerBlue4"
       << "SteelBlue1"
       << "SteelBlue2"
       << "SteelBlue3"
       << "SteelBlue4"
       << "DeepSkyBlue1"
       << "DeepSkyBlue2"
       << "DeepSkyBlue3"
       << "DeepSkyBlue4"
       << "SkyBlue1"
       << "SkyBlue2"
       << "SkyBlue3"
       << "SkyBlue4"
       << "LightSkyBlue1"
       << "LightSkyBlue2"
       << "LightSkyBlue3"
       << "LightSkyBlue4"
       << "SlateGray1"
       << "SlateGray2"
       << "SlateGray3"
       << "SlateGray4"
       << "LightSteelBlue1"
       << "LightSteelBlue2"
       << "LightSteelBlue3"
       << "LightSteelBlue4"
       << "LightBlue1"
       << "LightBlue2"
       << "LightBlue3"
       << "LightBlue4"
       << "LightCyan1"
       << "LightCyan2"
       << "LightCyan3"
       << "LightCyan4"
       << "PaleTurquoise1"
       << "PaleTurquoise2"
       << "PaleTurquoise3"
       << "PaleTurquoise4"
       << "CadetBlue1"
       << "CadetBlue2"
       << "CadetBlue3"
       << "CadetBlue4"
       << "turquoise1"
       << "turquoise2"
       << "turquoise3"
       << "turquoise4"
       << "cyan1"
       << "cyan2"
       << "cyan3"
       << "cyan4"
       << "DarkSlateGray1"
       << "DarkSlateGray2"
       << "DarkSlateGray3"
       << "DarkSlateGray4"
       << "aquamarine1"
       << "aquamarine2"
       << "aquamarine3"
       << "aquamarine4"
       << "DarkSeaGreen1"
       << "DarkSeaGreen2"
       << "DarkSeaGreen3"
       << "DarkSeaGreen4"
       << "SeaGreen1"
       << "SeaGreen2"
       << "SeaGreen3"
       << "SeaGreen4"
       << "PaleGreen1"
       << "PaleGreen2"
       << "PaleGreen3"
       << "PaleGreen4"
       << "SpringGreen1"
       << "SpringGreen2"
       << "SpringGreen3"
       << "SpringGreen4"
       << "green1"
       << "green2"
       << "green3"
       << "green4"
       << "chartreuse1"
       << "chartreuse2"
       << "chartreuse3"
       << "chartreuse4"
       << "OliveDrab1"
       << "OliveDrab2"
       << "OliveDrab3"
       << "OliveDrab4"
       << "DarkOliveGreen1"
       << "DarkOliveGreen2"
       << "DarkOliveGreen3"
       << "DarkOliveGreen4"
       << "khaki1"
       << "khaki2"
       << "khaki3"
       << "khaki4"
       << "LightGoldenrod1"
       << "LightGoldenrod2"
       << "LightGoldenrod3"
       << "LightGoldenrod4"
       << "LightYellow1"
       << "LightYellow2"
       << "LightYellow3"
       << "LightYellow4"
       << "yellow1"
       << "yellow2"
       << "yellow3"
       << "yellow4"
       << "gold1"
       << "gold2"
       << "gold3"
       << "gold4"
       << "goldenrod1"
       << "goldenrod2"
       << "goldenrod3"
       << "goldenrod4"
       << "DarkGoldenrod1"
       << "DarkGoldenrod2"
       << "DarkGoldenrod3"
       << "DarkGoldenrod4"
       << "RosyBrown1"
       << "RosyBrown2"
       << "RosyBrown3"
       << "RosyBrown4"
       << "IndianRed1"
       << "IndianRed2"
       << "IndianRed3"
       << "IndianRed4"
       << "sienna1"
       << "sienna2"
       << "sienna3"
       << "sienna4"
       << "burlywood1"
       << "burlywood2"
       << "burlywood3"
       << "burlywood4"
       << "wheat1"
       << "wheat2"
       << "wheat3"
       << "wheat4"
       << "tan1"
       << "tan2"
       << "tan3"
       << "tan4"
       << "chocolate1"
       << "chocolate2"
       << "chocolate3"
       << "chocolate4"
       << "firebrick1"
       << "firebrick2"
       << "firebrick3"
       << "firebrick4"
       << "brown1"
       << "brown2"
       << "brown3"
       << "brown4"
       << "salmon1"
       << "salmon2"
       << "salmon3"
       << "salmon4"
       << "LightSalmon1"
       << "LightSalmon2"
       << "LightSalmon3"
       << "LightSalmon4"
       << "orange1"
       << "orange2"
       << "orange3"
       << "orange4"
       << "DarkOrange1"
       << "DarkOrange2"
       << "DarkOrange3"
       << "DarkOrange4"
       << "coral1"
       << "coral2"
       << "coral3"
       << "coral4"
       << "tomato1"
       << "tomato2"
       << "tomato3"
       << "tomato4"
       << "OrangeRed1"
       << "OrangeRed2"
       << "OrangeRed3"
       << "OrangeRed4"
       << "red1"
       << "red2"
       << "red3"
       << "red4"
       << "DeepPink1"
       << "DeepPink2"
       << "DeepPink3"
       << "DeepPink4"
       << "HotPink1"
       << "HotPink2"
       << "HotPink3"
       << "HotPink4"
       << "pink1"
       << "pink2"
       << "pink3"
       << "pink4"
       << "LightPink1"
       << "LightPink2"
       << "LightPink3"
       << "LightPink4"
       << "PaleVioletRed1"
       << "PaleVioletRed2"
       << "PaleVioletRed3"
       << "PaleVioletRed4"
       << "maroon1"
       << "maroon2"
       << "maroon3"
       << "maroon4"
       << "VioletRed1"
       << "VioletRed2"
       << "VioletRed3"
       << "VioletRed4"
       << "magenta1"
       << "magenta2"
       << "magenta3"
       << "magenta4"
       << "orchid1"
       << "orchid2"
       << "orchid3"
       << "orchid4"
       << "plum1"
       << "plum2"
       << "plum3"
       << "plum4"
       << "MediumOrchid1"
       << "MediumOrchid2"
       << "MediumOrchid3"
       << "MediumOrchid4"
       << "DarkOrchid1"
       << "DarkOrchid2"
       << "DarkOrchid3"
       << "DarkOrchid4"
       << "purple1"
       << "purple2"
       << "purple3"
       << "purple4"
       << "MediumPurple1"
       << "MediumPurple2"
       << "MediumPurple3"
       << "MediumPurple4"
       << "thistle1"
       << "thistle2"
       << "thistle3"
       << "thistle4"
       << "gray0"
       << "gray1"
       << "gray2"
       << "gray3"
       << "gray4"
       << "gray5"
       << "gray6"
       << "gray7"
       << "gray8"
       << "gray9"
       << "gray10"
       << "gray11"
       << "gray12"
       << "gray13"
       << "gray14"
       << "gray15"
       << "gray16"
       << "gray17"
       << "gray18"
       << "gray19"
       << "gray20"
       << "gray21"
       << "gray22"
       << "gray23"
       << "gray24"
       << "gray25"
       << "gray26"
       << "gray27"
       << "gray28"
       << "gray29"
       << "gray30"
       << "gray31"
       << "gray32"
       << "gray33"
       << "gray34"
       << "gray35"
       << "gray36"
       << "gray37"
       << "gray38"
       << "gray39"
       << "gray40"
       << "gray41"
       << "gray42"
       << "gray43"
       << "gray44"
       << "gray45"
       << "gray46"
       << "gray47"
       << "gray48"
       << "gray49"
       << "gray50"
       << "gray51"
       << "gray52"
       << "gray53"
       << "gray54"
       << "gray55"
       << "gray56"
       << "gray57"
       << "gray58"
       << "gray59"
       << "gray60"
       << "gray61"
       << "gray62"
       << "gray63"
       << "gray64"
       << "gray65"
       << "gray66"
       << "gray67"
       << "gray68"
       << "gray69"
       << "gray70"
       << "gray71"
       << "gray72"
       << "gray73"
       << "gray74"
       << "gray75"
       << "gray76"
       << "gray77"
       << "gray78"
       << "gray79"
       << "gray80"
       << "gray81"
       << "gray82"
       << "gray83"
       << "gray84"
       << "gray85"
       << "gray86"
       << "gray87"
       << "gray88"
       << "gray89"
       << "gray90"
       << "gray91"
       << "gray92"
       << "gray93"
       << "gray94"
       << "gray95"
       << "gray96"
       << "gray97"
       << "gray98"
       << "gray99"
       << "gray100"
       << "dark grey"
       << "DarkGrey"
       << "dark gray"
       << "DarkGray"
       << "dark blue"
       << "DarkBlue"
       << "dark cyan"
       << "DarkCyan"
       << "dark magenta"
       << "DarkMagenta"
       << "dark red"
       << "DarkRed"
       << "light green"
       << "LightGreen";
  return list;
} // initaliseSingle()

QVector<QString>
initaliseAll()
{
  QVector<QString> list;
  list << "snow"
       << "ghost white"
       << "antique white"
       << "GhostWhite"
       << "WhiteSmoke"
       << "white smoke"
       << "gainsboro"
       << "Gainsboro"
       << "navajo white"
       << "NavajoWhite"
       << "moccasin"
       << "cornsilk"
       << "ivory"
       << "lemon chiffon"
       << "LemonChiffon"
       << "seashell"
       << "honeydew"
       << "mint cream"
       << "MintCream"
       << "azure"
       << "alice blue"
       << "AliceBlue"
       << "lavender"
       << "lavender blush"
       << "LavenderBlush"
       << "misty rose"
       << "MistyRose"
       << "white"
       << "black"
       << "dark slate gray"
       << "DarkSlateGray"
       << "dark slate grey"
       << "DarkSlateGrey"
       << "dim gray"
       << "DimGray"
       << "dim grey"
       << "DimGrey"
       << "slate gray"
       << "SlateGray"
       << "slate grey"
       << "SlateGrey"
       << "light slate gray"
       << "LightSlateGray"
       << "light slate grey"
       << "LightSlateGrey"
       << "gray"
       << "grey"
       << "light grey"
       << "LightGrey"
       << "light gray"
       << "LightGray"
       << "midnight blue"
       << "MidnightBlue"
       << "navy"
       << "navy blue"
       << "NavyBlue"
       << "cornflower blue"
       << "CornflowerBlue"
       << "dark slate blue"
       << "DarkSlateBlue"
       << "slate blue"
       << "SlateBlue"
       << "medium slate blue"
       << "MediumSlateBlue"
       << "light slate blue"
       << "LightSlateBlue"
       << "medium blue"
       << "MediumBlue"
       << "royal blue"
       << "RoyalBlue"
       << "blue"
       << "dodger blue"
       << "DodgerBlue"
       << "deep sky blue"
       << "DeepSkyBlue"
       << "sky blue"
       << "SkyBlue"
       << "light sky blue"
       << "LightSkyBlue"
       << "steel blue"
       << "SteelBlue"
       << "light steel blue"
       << "LightSteelBlue"
       << "light blue"
       << "LightBlue"
       << "powder blue"
       << "PowderBlue"
       << "pale turquoise"
       << "PaleTurquoise"
       << "dark turquoise"
       << "DarkTurquoise"
       << "medium turquoise"
       << "MediumTurquoise"
       << "turquoise"
       << "cyan"
       << "light cyan"
       << "LightCyan"
       << "cadet blue"
       << "CadetBlue"
       << "medium aquamarine"
       << "MediumAquamarine"
       << "aquamarine"
       << "dark green"
       << "DarkGreen"
       << "dark olive green"
       << "DarkOliveGreen"
       << "dark sea green"
       << "DarkSeaGreen"
       << "sea green"
       << "SeaGreen"
       << "medium sea green"
       << "MediumSeaGreen"
       << "light sea green"
       << "LightSeaGreen"
       << "pale green"
       << "PaleGreen"
       << "spring green"
       << "SpringGreen"
       << "lawn green"
       << "LawnGreen"
       << "green"
       << "chartreuse"
       << "medium spring green"
       << "MediumSpringGreen"
       << "green yellow"
       << "GreenYellow"
       << "lime green"
       << "LimeGreen"
       << "yellow green"
       << "YellowGreen"
       << "forest green"
       << "ForestGreen"
       << "olive drab"
       << "OliveDrab"
       << "darkkhaki"
       << "DarkKhaki"
       << "khaki"
       << "pale goldenrod"
       << "PaleGoldenrod"
       << "light goldenrod yellow"
       << "LightGoldenrodYellow"
       << "light yellow"
       << "LightYellow"
       << "yellow"
       << "gold"
       << "light goldenrod"
       << "LightGoldenrod"
       << "goldenrod"
       << "dark goldenrod"
       << "DarkGoldenrod"
       << "rosy brown"
       << "RosyBrown"
       << "indian red"
       << "IndianRed"
       << "saddle brown"
       << "SaddleBrown"
       << "sienna"
       << "peru"
       << "burlywood"
       << "beige"
       << "wheat"
       << "sandy brown"
       << "SandyBrown"
       << "tan"
       << "chocolate"
       << "firebrick"
       << "brown"
       << "dark salmon"
       << "DarkSalmon"
       << "salmon"
       << "light salmon"
       << "LightSalmon"
       << "orange"
       << "dark orange"
       << "DarkOrange"
       << "coral"
       << "light coral"
       << "LightCoral"
       << "tomato"
       << "orange red"
       << "OrangeRed"
       << "red"
       << "hot pink"
       << "HotPink"
       << "deep pink"
       << "DeepPink"
       << "pink"
       << "light pink"
       << "LightPink"
       << "pale violet red"
       << "PaleVioletRed"
       << "maroon"
       << "medium violet red"
       << "MediumVioletRed"
       << "violet red"
       << "VioletRed"
       << "magenta"
       << "violet"
       << "plum"
       << "orchid"
       << "medium orchid"
       << "MediumOrchid"
       << "dark orchid"
       << "DarkOrchid"
       << "dark violet"
       << "DarkViolet"
       << "blue violet"
       << "BlueViolet"
       << "purple"
       << "medium purple"
       << "MediumPurple"
       << "thistle"
       << "snow1"
       << "snow2"
       << "snow3"
       << "snow4"
       << "seashell1"
       << "seashell2"
       << "seashell3"
       << "seashell4"
       << "AntiqueWhite1"
       << "AntiqueWhite2"
       << "AntiqueWhite3"
       << "AntiqueWhite4"
       << "bisque"
       << "bisque1"
       << "bisque2"
       << "bisque3"
       << "bisque4"
       << "PeachPuff1"
       << "PeachPuff2"
       << "PeachPuff3"
       << "PeachPuff4"
       << "NavajoWhite1"
       << "NavajoWhite2"
       << "NavajoWhite3"
       << "NavajoWhite4"
       << "LemonChiffon1"
       << "LemonChiffon2"
       << "LemonChiffon3"
       << "LemonChiffon4"
       << "cornsilk1"
       << "cornsilk2"
       << "cornsilk3"
       << "cornsilk4"
       << "ivory1"
       << "ivory2"
       << "ivory3"
       << "ivory4"
       << "honeydew1"
       << "honeydew2"
       << "honeydew3"
       << "honeydew4"
       << "LavenderBlush1"
       << "LavenderBlush2"
       << "LavenderBlush3"
       << "LavenderBlush4"
       << "MistyRose1"
       << "MistyRose2"
       << "MistyRose3"
       << "MistyRose4"
       << "azure1"
       << "azure2"
       << "azure3"
       << "azure4"
       << "SlateBlue1"
       << "SlateBlue2"
       << "SlateBlue3"
       << "SlateBlue4"
       << "RoyalBlue1"
       << "RoyalBlue2"
       << "RoyalBlue3"
       << "RoyalBlue4"
       << "blue1"
       << "blue2"
       << "blue3"
       << "blue4"
       << "DodgerBlue1"
       << "DodgerBlue2"
       << "DodgerBlue3"
       << "DodgerBlue4"
       << "SteelBlue1"
       << "SteelBlue2"
       << "SteelBlue3"
       << "SteelBlue4"
       << "DeepSkyBlue1"
       << "DeepSkyBlue2"
       << "DeepSkyBlue3"
       << "DeepSkyBlue4"
       << "SkyBlue1"
       << "SkyBlue2"
       << "SkyBlue3"
       << "SkyBlue4"
       << "LightSkyBlue1"
       << "LightSkyBlue2"
       << "LightSkyBlue3"
       << "LightSkyBlue4"
       << "SlateGray1"
       << "SlateGray2"
       << "SlateGray3"
       << "SlateGray4"
       << "LightSteelBlue1"
       << "LightSteelBlue2"
       << "LightSteelBlue3"
       << "LightSteelBlue4"
       << "LightBlue1"
       << "LightBlue2"
       << "LightBlue3"
       << "LightBlue4"
       << "LightCyan1"
       << "LightCyan2"
       << "LightCyan3"
       << "LightCyan4"
       << "PaleTurquoise1"
       << "PaleTurquoise2"
       << "PaleTurquoise3"
       << "PaleTurquoise4"
       << "CadetBlue1"
       << "CadetBlue2"
       << "CadetBlue3"
       << "CadetBlue4"
       << "turquoise1"
       << "turquoise2"
       << "turquoise3"
       << "turquoise4"
       << "cyan1"
       << "cyan2"
       << "cyan3"
       << "cyan4"
       << "DarkSlateGray1"
       << "DarkSlateGray2"
       << "DarkSlateGray3"
       << "DarkSlateGray4"
       << "aquamarine1"
       << "aquamarine2"
       << "aquamarine3"
       << "aquamarine4"
       << "DarkSeaGreen1"
       << "DarkSeaGreen2"
       << "DarkSeaGreen3"
       << "DarkSeaGreen4"
       << "SeaGreen1"
       << "SeaGreen2"
       << "SeaGreen3"
       << "SeaGreen4"
       << "PaleGreen1"
       << "PaleGreen2"
       << "PaleGreen3"
       << "PaleGreen4"
       << "SpringGreen1"
       << "SpringGreen2"
       << "SpringGreen3"
       << "SpringGreen4"
       << "green1"
       << "green2"
       << "green3"
       << "green4"
       << "chartreuse1"
       << "chartreuse2"
       << "chartreuse3"
       << "chartreuse4"
       << "OliveDrab1"
       << "OliveDrab2"
       << "OliveDrab3"
       << "OliveDrab4"
       << "DarkOliveGreen1"
       << "DarkOliveGreen2"
       << "DarkOliveGreen3"
       << "DarkOliveGreen4"
       << "khaki1"
       << "khaki2"
       << "khaki3"
       << "khaki4"
       << "LightGoldenrod1"
       << "LightGoldenrod2"
       << "LightGoldenrod3"
       << "LightGoldenrod4"
       << "LightYellow1"
       << "LightYellow2"
       << "LightYellow3"
       << "LightYellow4"
       << "yellow1"
       << "yellow2"
       << "yellow3"
       << "yellow4"
       << "gold1"
       << "gold2"
       << "gold3"
       << "gold4"
       << "goldenrod1"
       << "goldenrod2"
       << "goldenrod3"
       << "goldenrod4"
       << "DarkGoldenrod1"
       << "DarkGoldenrod2"
       << "DarkGoldenrod3"
       << "DarkGoldenrod4"
       << "RosyBrown1"
       << "RosyBrown2"
       << "RosyBrown3"
       << "RosyBrown4"
       << "IndianRed1"
       << "IndianRed2"
       << "IndianRed3"
       << "IndianRed4"
       << "sienna1"
       << "sienna2"
       << "sienna3"
       << "sienna4"
       << "burlywood1"
       << "burlywood2"
       << "burlywood3"
       << "burlywood4"
       << "wheat1"
       << "wheat2"
       << "wheat3"
       << "wheat4"
       << "tan1"
       << "tan2"
       << "tan3"
       << "tan4"
       << "chocolate1"
       << "chocolate2"
       << "chocolate3"
       << "chocolate4"
       << "firebrick1"
       << "firebrick2"
       << "firebrick3"
       << "firebrick4"
       << "brown1"
       << "brown2"
       << "brown3"
       << "brown4"
       << "salmon1"
       << "salmon2"
       << "salmon3"
       << "salmon4"
       << "LightSalmon1"
       << "LightSalmon2"
       << "LightSalmon3"
       << "LightSalmon4"
       << "orange1"
       << "orange2"
       << "orange3"
       << "orange4"
       << "DarkOrange1"
       << "DarkOrange2"
       << "DarkOrange3"
       << "DarkOrange4"
       << "coral1"
       << "coral2"
       << "coral3"
       << "coral4"
       << "tomato"
       << "tomato2"
       << "tomato3"
       << "tomato4"
       << "OrangeRed1"
       << "OrangeRed2"
       << "OrangeRed3"
       << "OrangeRed4"
       << "red1"
       << "red2"
       << "red3"
       << "red4"
       << "DeepPink1"
       << "DeepPink2"
       << "DeepPink3"
       << "DeepPink4"
       << "HotPink1"
       << "HotPink2"
       << "HotPink3"
       << "HotPink4"
       << "pink1"
       << "pink2"
       << "pink3"
       << "pink4"
       << "LightPink1"
       << "LightPink2"
       << "LightPink3"
       << "LightPink4"
       << "PaleVioletRed1"
       << "PaleVioletRed2"
       << "PaleVioletRed3"
       << "PaleVioletRed4"
       << "maroon1"
       << "maroon2"
       << "maroon3"
       << "maroon4"
       << "VioletRed1"
       << "VioletRed2"
       << "VioletRed3"
       << "VioletRed4"
       << "magenta1"
       << "magenta2"
       << "magenta3"
       << "magenta4"
       << "orchid1"
       << "orchid2"
       << "orchid3"
       << "orchid4"
       << "plum1"
       << "plum2"
       << "plum3"
       << "plum4"
       << "MediumOrchid1"
       << "MediumOrchid2"
       << "MediumOrchid3"
       << "MediumOrchid4"
       << "DarkOrchid1"
       << "DarkOrchid2"
       << "DarkOrchid3"
       << "DarkOrchid4"
       << "purple1"
       << "purple2"
       << "purple3"
       << "purple4"
       << "MediumPurple1"
       << "MediumPurple2"
       << "MediumPurple3"
       << "MediumPurple4"
       << "thistle1"
       << "thistle2"
       << "thistle3"
       << "thistle4"
       << "gray0"
       << "grey0"
       << "gray1"
       << "grey1"
       << "gray2"
       << "grey2"
       << "gray3"
       << "grey3"
       << "gray4"
       << "grey4"
       << "gray5"
       << "grey5"
       << "gray6"
       << "grey6"
       << "gray7"
       << "grey7"
       << "gray8"
       << "grey8"
       << "gray9"
       << "grey9"
       << "gray10"
       << "grey10"
       << "gray11"
       << "grey11"
       << "gray12"
       << "grey12"
       << "gray13"
       << "grey13"
       << "gray14"
       << "grey14"
       << "gray15"
       << "grey15"
       << "gray16"
       << "grey16"
       << "gray17"
       << "grey17"
       << "gray18"
       << "grey18"
       << "gray19"
       << "grey19"
       << "gray20"
       << "grey20"
       << "gray21"
       << "grey21"
       << "gray22"
       << "grey22"
       << "gray23"
       << "grey23"
       << "gray24"
       << "grey24"
       << "gray25"
       << "grey25"
       << "gray26"
       << "grey26"
       << "gray27"
       << "grey27"
       << "gray28"
       << "grey28"
       << "gray29"
       << "grey29"
       << "gray30"
       << "grey30"
       << "gray31"
       << "grey31"
       << "gray32"
       << "grey32"
       << "gray33"
       << "grey33"
       << "gray34"
       << "grey34"
       << "gray35"
       << "grey35"
       << "gray36"
       << "grey36"
       << "gray37"
       << "grey37"
       << "gray38"
       << "grey38"
       << "gray39"
       << "grey39"
       << "gray40"
       << "grey40"
       << "gray41"
       << "grey41"
       << "gray42"
       << "grey42"
       << "gray43"
       << "grey43"
       << "gray44"
       << "grey44"
       << "gray45"
       << "grey45"
       << "gray46"
       << "grey46"
       << "gray47"
       << "grey47"
       << "gray48"
       << "grey48"
       << "gray49"
       << "grey49"
       << "gray50"
       << "grey50"
       << "gray51"
       << "grey51"
       << "gray52"
       << "grey52"
       << "gray53"
       << "grey53"
       << "gray54"
       << "grey54"
       << "gray55"
       << "grey55"
       << "gray56"
       << "grey56"
       << "gray57"
       << "grey57"
       << "gray58"
       << "grey58"
       << "gray59"
       << "grey59"
       << "gray60"
       << "grey60"
       << "gray61"
       << "grey61"
       << "gray62"
       << "grey62"
       << "gray63"
       << "grey63"
       << "gray64"
       << "grey64"
       << "gray65"
       << "grey65"
       << "gray66"
       << "grey66"
       << "gray67"
       << "grey67"
       << "gray68"
       << "grey68"
       << "gray69"
       << "grey69"
       << "gray70"
       << "grey70"
       << "gray71"
       << "grey71"
       << "gray72"
       << "grey72"
       << "gray73"
       << "grey73"
       << "gray74"
       << "grey74"
       << "gray75"
       << "grey75"
       << "gray76"
       << "grey76"
       << "gray77"
       << "grey77"
       << "gray78"
       << "grey78"
       << "gray79"
       << "grey79"
       << "gray80"
       << "grey80"
       << "gray81"
       << "grey81"
       << "gray82"
       << "grey82"
       << "gray83"
       << "grey83"
       << "gray84"
       << "grey84"
       << "gray85"
       << "grey85"
       << "gray86"
       << "grey86"
       << "gray87"
       << "grey87"
       << "gray88"
       << "grey88"
       << "gray89"
       << "grey89"
       << "gray90"
       << "grey90"
       << "gray91"
       << "grey91"
       << "gray92"
       << "grey92"
       << "gray93"
       << "grey93"
       << "gray94"
       << "grey94"
       << "gray95"
       << "grey95"
       << "gray96"
       << "grey96"
       << "gray97"
       << "grey97"
       << "gray98"
       << "grey98"
       << "gray99"
       << "grey99"
       << "gray100"
       << "grey100"
       << "dark grey"
       << "DarkGrey"
       << "dark gray"
       << "DarkGray"
       << "dark blue"
       << "DarkBlue"
       << "dark cyan"
       << "DarkCyan"
       << "dark magenta"
       << "DarkMagenta"
       << "dark red"
       << "DarkRed"
       << "light green"
       << "LightGreen";
  return list;
} // initaliseAll()

static const QMap<QString, QColor> COLORMAP = initaliseMap();
static const QVector<QString> SINGLENAMES = initaliseSingle();
static const QVector<QString> ALLNAMES = initaliseAll();

} // end of anonymous namespace

//! Returns the color associated with the name string, if any name exists,
//! otherwise an invalid QColor is returned.
//! \sa name(const QColor&)
static QColor
color(const QString& name)
{
  return COLORMAP.value(name);
}

//! Returns the first name string associated with this color, if any match,
//! otherwise an empty string is returned.
//! \sa color(const QString&)
static QString
name(const QColor& color)
{
  auto colorKeys = COLORMAP.keys();
  for (auto &k : colorKeys) {
    auto c = COLORMAP.value(k);
    if (c == color) {
      return k;
    }
  }
  return QString();
}

//! Returns a list of all the names, without repeats due to titled case names.
//! \note There are some repeated names, for instance 'maroon' and 'maroon1'
//! are in fact repeats, but were left in as they are part of a 1-4 list of
//! similar colours. \sa allNames()
static QVector<QString>
singleNames()
{
  return SINGLENAMES;
}

//! Returns a list of all the names, including repeats.
//! \sa singleNames()
static QVector<QString>
allNames()
{
  return ALLNAMES;
}

} // namespace X11

namespace {
static constexpr double Pr = 0.299;
static constexpr double Pg = 0.587;
static constexpr double Pb = 0.114;
}
//! Returns the Svg or X11 name for the color if one exists.
//! The Svg name is returned in preference to the X11 name if it exists.
static QString
svgOrX11Name(const QColor& color)
{
  auto name = QColorConstants::Svg::name(color);
  if (!name.isEmpty()) {
    return name;
  } else {
    name = QColorConstants::X11::name(color);
    if (!name.isEmpty()) {
      return name;
    }
  }
  return QString();
}

//! Returns the Svg or X11 color for the name string
//! the name is tested against the Svg colors first.
static QColor
svgOrX11Color(const QString& initialColor)
{
  auto color = QColorConstants::Svg::color(initialColor);
  if (color.isValid()) {
    return color;
  } else {
    color = QColorConstants::X11::color(initialColor);
    if (color.isValid()) {
      return color;
    }
  }
  return Qt::white;
}

//! Returns the perceived brightness of the color.
//!
//! This can be used to select a text color that shows up well with the
//! specified color.
//! Darker colors return lower values in a range between 0 and 1.0, with
//! black at 0 and white at 1.0.
//!
//! This is based on the alternative HSP color model suggested by Darel Rex
//! Finley. http://alienryderflex.com/hsp.html public domain by Darel
//! Rex Finley, 2006

static double
perceivedBrightness(const QColor& color)
{
  auto r = color.redF();
  auto g = color.greenF();
  auto b = color.blueF();
  return qSqrt(r * r * Pr + g * g * Pg + b * b * Pb);
}

//! Returns true if the color is a darker shade, otherwise return false.
//!
//! This can be used to select a text color that shows up well with the
//! specified color.
//!
//! This is based on the alternative HSP color model suggested by Darel Rex
//! Finley. http://alienryderflex.com/hsp.html public domain by Darel
//! Rex Finley, 2006
static bool
isDark(const QColor& color)
{
  auto p = perceivedBrightness(color);

  if (p < 0.5) {
    return true;
  }
  return false;
}

} // namespacce QColorConstants

#endif // X11COLORS_H
