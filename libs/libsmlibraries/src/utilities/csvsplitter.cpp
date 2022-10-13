#include "utilities/csvsplitter.h"
#include "utilities/characters.h"

namespace CSVSplitter {

QStringList split(const QString data, const QChar delimiter) {
  bool quote_detected = false;
  bool quote_completed = false;
  //    bool delim_detected = false;
  QStringList list;
  QString s;
  QChar c;

  for (int i = 0; i < data.length(); i++) {
    c = data[i];
    //        int v = c.unicode();

    if (c == Characters::QUOTATION) {
      if (quote_detected) {
        quote_detected = false;
        quote_completed = true;

      } else {
        quote_detected = true;
      }

    } else if (c == Characters::SINGLEQUOTE) {
      // A single quote character inside a double quoted section
      // needs to be escaped in CPP.
      //            if (quote_detected && !quote_completed) {
      s += Characters::BACKSLASH;
      s += c;
      //            }

    } else if (c == delimiter) {
      //            delim_detected = true;

      if (quote_detected && !quote_completed) {
        // comma within quotes, just treat as a character.
        // just add char to s.
        s += c;

      } else if (!quote_detected && quote_completed) {
        list += s;
        s.clear();
        quote_detected = false;
        quote_completed = false;
        //                delim_detected = false;

      } else {
        if (quote_completed) {
          // ignore characters until delimiter
          // TODO more work in here ???
        } else {
          list += s;
          s.clear();
          //                    delim_detected = false;
        }
      }

    } else if (c == '\r' || c == '\n') {
      // Final section between last delimiter and end of line
      list += s;
      return list;

    } else {
      //            if (quote_completed)
      s += c;
    }
  }

  // Should only reach here if the last line has no newline characters at the end.
  if (quote_detected) {
    list += s;
  }

  return list;
}

QStringList splitOnComma(const QString data) {
  return split(data, Characters::COMMA);
}

QStringList splitOnColon(const QString data) {
  return split(data, Characters::COLON);
}

QStringList splitOnSpace(const QString data) {
  return split(data, Characters::SPACE);
}

QStringList splitOnSemicolon(const QString data) {
  return split(data, Characters::SEMICOLON);
}

} // end of namespace CSVSplitter
