#include "utilities/stringutil.h"

namespace StringUtil {

QString
removeWhitespace(const QString& text)
{
  QString t;
  // removes all whitespace except single interior spaces
  auto simplified = text.simplified();
  for (auto &c : simplified) {
    // ignore all single space characters.
    if (c == ' ')
      continue;
    t += c;
  }
  return t;
}

} // end of namespace Strings
