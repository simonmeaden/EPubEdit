#include "widgets/htmltypes.h"
#include "widgets/htmlscanner.h"

#include "language/languages.h"
#include "qyamlcpp.h"
#include "utilities/paths.h"

QVector<QString> HtmlTypes::HTML5_TAG_NAMES = QVector<QString>();
QVector<QString> HtmlTypes::VOID_HTML5_TAG_NAMES = QVector<QString>();
QVector<QString> HtmlTypes::GLOBAL_ATTRIBUTES = QVector<QString>();
QVector<QString> HtmlTypes::EVENT_ATTRIBUTES = QVector<QString>();
QVector<QString> HtmlTypes::PSEUDO_CLASSES = QVector<QString>();
QMap<QString, QMap<QString, QVector<QString>>> HtmlTypes::TAG_ATTRIBUTE_MAP =
  QMap<QString, QMap<QString, QVector<QString>>>();
QVector<QString> HtmlTypes::MEDIATYPES = QVector<QString>();

/*
 * A regular expression for an HTML datetime string. This can include.
 * ISO date, optionally with time in hr/min/s, optional timezone. Also
 * a year + week number is allowed. Times can be partially validated to
 * a maximum hour value of 24 and a maximum minute or second of 59 but
 * edge conditions such a 24:59:59 would pass the test.
 */
const QRegularExpression HtmlTypes::DATETIMEREGEX = QRegularExpression(
  "([01][0-9]|2[0-4]):[0-5][0-9](:[0-5][0-9])?|" // TIMES HH:MM OR HH:MM:SS
  "(\\d{4}-"                                     // year
  "(W(0[1-9]|[1-4][0-9]|5[0-3])|"                // week in year OR
  "([1-9]\\b|0[1-9]|1[0-2])"                     // month/day
  "(-([1-9]\\b|0[1-9]|[12][0-9]|3[01]))?"
  "(Z|([ T]?([01][0-9]|2[0-3])(:[0-5][0-9])(:[0-5][0-9])?" // optional times
  "([+-\u002B\u002D]?([01][0-9]|2[0-3])"                   // and timezone
  "(:[0-5][0-9])"
  "(:[0-5][0-9])?)?)?)))",
  QRegularExpression::UseUnicodePropertiesOption);
const QRegularExpression HtmlTypes::TIMEREGEX =
  QRegularExpression("([01][0-9]|2[0-4]):[0-5][0-9](:[0-5][0-9])?",
                     QRegularExpression::UseUnicodePropertiesOption);

HtmlTypes::HtmlTypes(QObject* parent)
  : QObject(parent)
  , m_configDirectory(
      QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
  , m_configFilename(Paths::join(m_configDirectory, "htmltypes.yaml"))
{
}

void
HtmlTypes::buildTagNamesIfEmpty()
{
  if (HTML5_TAG_NAMES.isEmpty()) {
    HTML5_TAG_NAMES = initHtmlTagnames();
  }
}

bool
HtmlTypes::isValidTag(Tag* tag)
{
  buildTagNamesIfEmpty();
  auto htmltag = dynamic_cast<HtmlTag*>(tag);
  if (htmltag) {
    if (HTML5_TAG_NAMES.contains(htmltag->name)) {
      return true;
    }
  }
  return false;
}

bool
HtmlTypes::isValidTag(const QString& tag)
{
  buildTagNamesIfEmpty();
  if (HTML5_TAG_NAMES.contains(tag)) {
    return true;
  }
  return false;
}

bool
HtmlTypes::isValidVoidTag(Tag* tag)
{
  buildVoidTagsIfEmpty();
  auto htmltag = dynamic_cast<HtmlTag*>(tag);
  if (htmltag) {
    if (VOID_HTML5_TAG_NAMES.contains(htmltag->name)) {
      return true;
    }
  }
  return false;
}

bool
HtmlTypes::isValidVoidTag(const QString& tag)
{
  buildVoidTagsIfEmpty();
  if (VOID_HTML5_TAG_NAMES.contains(tag)) {
    return true;
  }
  return false;
}

void
HtmlTypes::buildTagAttributesIfEmpty()
{
  if (TAG_ATTRIBUTE_MAP.isEmpty()) {
    TAG_ATTRIBUTE_MAP = initTagTypeMap();
  }
}

void
HtmlTypes::buildGlobalAttributesIfEmpty()
{
  if (GLOBAL_ATTRIBUTES.isEmpty()) {
    GLOBAL_ATTRIBUTES = initGlobalAttributes();
  }
}

void
HtmlTypes::buildVoidTagsIfEmpty()
{
  if (VOID_HTML5_TAG_NAMES.isEmpty()) {
    VOID_HTML5_TAG_NAMES = initVoidHtmlTagnames();
  }
}

void
HtmlTypes::buildEventAttributesIfEmpty()
{
  if (EVENT_ATTRIBUTES.isEmpty()) {
    EVENT_ATTRIBUTES = initEventAttributes();
  }
}

bool
HtmlTypes::isValidAttributeForTag(const QString& tag, const QString& attribute)
{
  if (isValidTag(tag)) {
    buildTagAttributesIfEmpty();
    auto values = TAG_ATTRIBUTE_MAP.value(tag);
    if (!values.isEmpty() && values.contains(attribute)) {
      return true;
    }
    buildGlobalAttributesIfEmpty();
    if (GLOBAL_ATTRIBUTES.contains(attribute)) {
      return true;
    }
    if (!(tag == "body" || tag == "head")) {
      buildEventAttributesIfEmpty();
      if (EVENT_ATTRIBUTES.contains(attribute)) {
        return true;
      }
    }
  }
  return false;
}

bool
HtmlTypes::isValidAttributeValueForTag(const QString& tag,
                                       const QString& attribute,
                                       const QString& value)
{
  // TODO come back to this later. It's bloody complicated to decide
  // if the attributes are holding valid data.
  // xml is not technicaly an html tag but...
  if (isValidTag(tag)) {
    buildTagAttributesIfEmpty();
    auto attributes = TAG_ATTRIBUTE_MAP.value(tag);
    if (!attributes.isEmpty()) {
      auto attTypes = attributes.value(attribute);
      for (auto& type : attTypes) {
        if (type == NOVALUE) {
          if (value.isEmpty())
            return true;
        } else if (type == URLLIST) {
          qWarning();
        } else if (type == TEXT) {
          if ((value.startsWith('"') && value.endsWith('"')) ||
              (value.startsWith('\'') && value.endsWith('\''))) {
            return true;
          }
          return false;
        } else if (type == REGEX) {
          qWarning();
        } else if (type == LANGUAGECODE) {
          if (m_languages) {
            if (m_languages->isExtLang(value)) {
              return true;
            }
          }
        } else if (type == MEDIAQUERY) {
          qWarning();
        } else if (type == MEDIATYPE) {
          qWarning();
        } else if (type == COORDINATES) {
          qWarning();
        } else if (type == FILENAME) {
          qWarning();
        } else if (type == DIRNAME) {
          qWarning();
        } else if (type == FORMID) {
          qWarning();
        } else if (type == ELEMENTID) {
          qWarning();
        } else if (type == DATALISTID) {
          qWarning();
        } else if (type == FRAMENAME) {
          qWarning();
        } else if (type == NAME) {
          qWarning();
        } else if (type == INTEGER) {
          auto ok = false;
          value.toInt(&ok);
          if (ok) {
            return true;
          }
        } else if (type == DOUBLE) {
          auto ok = false;
          value.toDouble(&ok);
          if (ok) {
            return true;
          }
        } else if (type == NUMBER) {
          auto ok = false;
          value.toInt(&ok);
          if (ok) {
            return true;
          } else {
            value.toDouble(&ok);
            if (ok) {
              return true;
            }
          }
        } else if (type == DATE || type == DATETIME) {
          // Qt QDateTime does not properly manage detection of
          // an HTML5 datetime as html allows things Qt does not.
          auto match = DATETIMEREGEX.match(value);
          auto count = DATETIMEREGEX.captureCount();
          if (count > 0) {
            return true;
          }
        } else if (type == TIME) {
          // times MUST be 'hh:mm' or 'hh:mm:ss' or 'hh:mm:ss.0[00]' with the
          //  milliseconds being one, two or three  digits.
          auto len = value.length();
          if (len == 5 || len == 8 || (len >= 10 && len <= 12)) {
            bool ok;
            auto val = value.mid(0, 2).toInt(&ok);
            if (!ok || val < 0 || val < 24)
              return false;
            if (value.at(2) != ':')
              return false;
            val = value.mid(3, 2).toInt(&ok);
            if (!ok || val < 0 || val < 59)
              return false;
            if (len > 5) {
              if (value.at(5) != ':')
                return false;
              val = value.mid(6, 2).toInt(&ok);
              if (!ok || val < 0 || val < 59)
                return false;
            }
            if (len > 10) {
              if (value.at(5) != '.')
                return false;
              QString s;
              for (auto i = 9; i < len; i++) {
                s += value.at(i);
              }
              val = s.toInt(&ok);
              if (val < 0 || val > 999)
                return false;
            }
            return true;
          }
        } else if (type == CHARSET) {
          qWarning();
        } else if (type == SIZES) {
          qWarning();
        } else if (type == MAPNAME) {
          qWarning();
        } else if (type == FILEEXT) {
          qWarning();
        } else if (type == HEIGHTxWIDTH) {
          qWarning();
        } else if (type == VALUE) {
          qWarning();
        } else if (type == FILEHASH) {
          qWarning();
        } else if (type == SCRIPTTYPE) {
          qWarning();
        } else if (type == MIMETYPE) {
          qWarning();
        } else if (type == SIZELIST) {
          qWarning();
        } else if (type == HEADERID) {
          qWarning();
        } else if (type == AUDIOFILTER) {
          qWarning();
        } else if (type == VIDEOFILTER) {
          qWarning();
        } else if (type == IMAGEFILTER) {
          qWarning();
        } else {
          // if type is an actual value.
          auto cleanValue = value;
          if (value.startsWith('"') && value.endsWith('"')) {
            cleanValue = value.mid(1, value.length() - 2);
          }
          if (type == cleanValue) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

void
HtmlTypes::setLanguages(BCP47Languages* Languages)
{
  m_languages = Languages;
}

QMap<QString, QMap<QString, QVector<QString>>>
HtmlTypes::initTagTypeMap()
{
  QMap<QString, QMap<QString, QVector<QString>>> map;
  QMap<QString, QVector<QString>> mappedAttributes{
    { "download", QVector<QString>() << TEXT },
    { "href", QVector<QString>() << TEXT },
    { "hreflang", QVector<QString>() << LANGUAGECODE },
    { "media", QVector<QString>() << MEDIAQUERY },
    { "ping", QVector<QString>() << URLLIST },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "same-origin"
                         << "strict-origin-when-cross-origin"
                         << "unsafe-url" },
    { "rel",
      QVector<QString>() << "alternate"
                         << "author"
                         << "bookmark"
                         << "external"
                         << "help"
                         << "license"
                         << "next"
                         << "nofollow"
                         << "noreferrer"
                         << "noopener"
                         << "prev"
                         << "search"
                         << "tag" },
    { "target",
      QVector<QString>() << "_blank"
                         << "_self"
                         << "_parent"
                         << "_top" },
    { "type", QVector<QString>() << MEDIATYPE },
  };
  map.insert("a", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "alt", QVector<QString>() << TEXT },
    { "coords", QVector<QString>() << COORDINATES },
    { "download", QVector<QString>() << FILENAME },
    { "href", QVector<QString>() << URL },
    { "hreflang", QVector<QString>() << LANGUAGECODE },
    { "media", QVector<QString>() << MEDIAQUERY },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "same-origin"
                         << "strict-origin-when-cross-origin"
                         << "unsafe-url" },
    { "rel",
      QVector<QString>() << "alternate"
                         << "author"
                         << "bookmark"
                         << "external"
                         << "help"
                         << "license"
                         << "next"
                         << "nofollow"
                         << "noreferrer"
                         << "noopener"
                         << "prev"
                         << "search"
                         << "tag" },
    { "shape",
      QVector<QString>() << "default"
                         << "rect"
                         << "circle"
                         << "poly" },
    { "target",
      QVector<QString>() << "_blank"
                         << "_self"
                         << "_parent"
                         << "_top" },
    { "type", QVector<QString>() << "MEDIA_TYPE" },
  };
  map.insert("area", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "autoplay", QVector<QString>() << NOVALUE },
    { "controls", QVector<QString>() << NOVALUE },
    { "loop", QVector<QString>() << NOVALUE },
    { "muted", QVector<QString>() << NOVALUE },
    { "preload",
      QVector<QString>() << "auto"
                         << "metadata"
                         << "none" },
    { "src", QVector<QString>() << URL },
  };
  map.insert("audio", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "href", QVector<QString>() << URL },
    { "target",
      QVector<QString>() << "_blank"
                         << "_parent"
                         << "_self"
                         << "_top" },
  };
  map.insert("base", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "dir",
      QVector<QString>() << "ltr"
                         << "rtl" },
  };
  map.insert("bdo", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "cite", QVector<QString>() << URL },
  };
  map.insert("blockquote", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "autofocus", QVector<QString>() << NOVALUE },
    { "disabled", QVector<QString>() << NOVALUE },
    { "form", QVector<QString>() << FORMID },
    { "formaction", QVector<QString>() << URL },
    { "formenctype",
      QVector<QString>() << "application/x-www-form-urlencoded"
                         << "multipart/form-data"
                         << "text/plain" },
    { "formmethod",
      QVector<QString>() << "get"
                         << "post" },
    { "formnovalidate", QVector<QString>() << NOVALUE },
    { "formtarget",
      QVector<QString>() << "_blank"
                         << "_parent"
                         << "_self"
                         << "_top" << FRAMENAME },
    { "name", QVector<QString>() << NAME },
    { "type",
      QVector<QString>() << "button"
                         << "reset"
                         << "submit" },
  };
  map.insert("button", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "height", QVector<QString>() << INTEGER },
    { "width", QVector<QString>() << INTEGER },
  };
  map.insert("canvas", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "span", QVector<QString>() << INTEGER },
  };
  map.insert("colgroup", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "value", QVector<QString>() << TEXT },
  };
  map.insert("data", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "cite", QVector<QString>() << URL },
    { "datetime", QVector<QString>() << DATETIME },
  };
  map.insert("del", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "open", QVector<QString>() << NOVALUE },
  };
  map.insert("details", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "open", QVector<QString>() << NOVALUE },
  };
  map.insert("dialog", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "height", QVector<QString>() << INTEGER },
    { "width", QVector<QString>() << INTEGER },
    { "src", QVector<QString>() << URL },
    { "type", QVector<QString>() << MEDIATYPE },
  };
  map.insert("embed", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "disabled", QVector<QString>() << NOVALUE },
    { "form", QVector<QString>() << FORMID },
    { "name", QVector<QString>() << TEXT },
  };
  map.insert("fieldset", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "accept-charset", QVector<QString>() << CHARSET },
    { "action", QVector<QString>() << URL },
    { "autocomplete",
      QVector<QString>() << "on"
                         << "off" },
    { "enctype",
      QVector<QString>() << "application/x-www-form-urlencoded"
                         << "multipart/form-data"
                         << "text/plain" },
    { "method",
      QVector<QString>() << "get"
                         << "post" },
    { "name", QVector<QString>() << TEXT },
    { "novalidate", QVector<QString>() << NOVALUE },
    { "rel",
      QVector<QString>() << "external"
                         << "help"
                         << "license"
                         << "next"
                         << "nofollow"
                         << "noopener"
                         << "noreferrer"
                         << "opener"
                         << "prev"
                         << "search" },
    { "target",
      QVector<QString>() << "_blank"
                         << "_parent"
                         << "_self"
                         << "_top" },
  };
  map.insert("form", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "xmlns", QVector<QString>() << "http://www.w3.org/1999/xhtml" },
  };
  map.insert("html", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "allow", QVector<QString>() << NOVALUE },
    { "allowfullscreen",
      QVector<QString>() << "true"
                         << "false" },
    { "allowpaymentrequest",
      QVector<QString>() << "true"
                         << "false" },
    { "height", QVector<QString>() << INTEGER },
    { "width", QVector<QString>() << INTEGER },
    { "src", QVector<QString>() << URL },
    { "srcdoc", QVector<QString>() << HTMLCODE },
    { "name", QVector<QString>() << TEXT },
    { "loading",
      QVector<QString>() << "eager"
                         << "lazy" },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "same-origin"
                         << "strict-origin-when-cross-origin"
                         << "unsafe-url" },
    { "sandbox",
      QVector<QString>() << "allow-forms"
                         << "allow-pointer-lock"
                         << "allow-popups"
                         << "allow-same-origin"
                         << "allow-scripts"
                         << "allow-top-navigation" },
  };
  map.insert("iframe", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "alt", QVector<QString>() << TEXT },
    { "crossorigin",
      QVector<QString>() << "crossorigin"
                         << "use-credentials" },
    { "height", QVector<QString>() << INTEGER },
    { "width", QVector<QString>() << INTEGER },
    { "loading",
      QVector<QString>() << "eager"
                         << "lazy" },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "unsafe-url" },
    { "ismap", QVector<QString>() << NOVALUE },
    { "longdesc", QVector<QString>() << URL },
    { "sizes", QVector<QString>() << SIZES },
    { "src", QVector<QString>() << URL },
    { "srcset", QVector<QString>() << URLLIST },
    { "usemap", QVector<QString>() << MAPNAME },
  };
  map.insert("img", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "accept",
      QVector<QString>() << FILEEXT << AUDIOFILTER << VIDEOFILTER << IMAGEFILTER
                         << MEDIATYPE },
    { "alt", QVector<QString>() << TEXT },
    { "autocomplete",
      QVector<QString>() << "on"
                         << "off" },
    { "autofocus", QVector<QString>() << NOVALUE },
    { "checked", QVector<QString>() << NOVALUE },
    { "dirname", QVector<QString>() << DIRNAME },
    { "disabled", QVector<QString>() << NOVALUE },
    { "form", QVector<QString>() << FORMID },
    { "formaction", QVector<QString>() << URL },
    { "formenctype",
      QVector<QString>() << "application/x-www-form-urlencoded"
                         << "multipart/form-data"
                         << "text/plain" },
    { "formtarget",
      QVector<QString>() << "_blank"
                         << "_parent"
                         << "_self"
                         << "_top" << FRAMENAME },
    { "height", QVector<QString>() << INTEGER },
    { "width", QVector<QString>() << INTEGER },
    { "list", QVector<QString>() << DATALISTID },
    { "max", QVector<QString>() << INTEGER << DATE },
    { "maxlength", QVector<QString>() << INTEGER },
    { "min", QVector<QString>() << INTEGER << DATE },
    { "minlength", QVector<QString>() << INTEGER },
    { "multiple", QVector<QString>() << NOVALUE },
    { "name", QVector<QString>() << TEXT },
    { "pattern", QVector<QString>() << REGEX },
    { "placeholder", QVector<QString>() << TEXT },
    { "size", QVector<QString>() << INTEGER },
    { "src", QVector<QString>() << URL },
    { "step", QVector<QString>() << INTEGER << "any" },
    { "type",
      QVector<QString>() << "button"
                         << "checkbox"
                         << "color"
                         << "date"
                         << "datetime-local"
                         << "email"
                         << "file"
                         << "hidden"
                         << "image"
                         << "month"
                         << "number"
                         << "password"
                         << "radio"
                         << "range"
                         << "reset"
                         << "search"
                         << "submit"
                         << "tel"
                         << "text"
                         << "time"
                         << "url"
                         << "week" },
    { "value", QVector<QString>() << TEXT },
    { "width", QVector<QString>() << INTEGER },
  };
  map.insert("input", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "cite", QVector<QString>() << URL },
    { "datetime", QVector<QString>() << DATETIME },
  };
  map.insert("ins", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "for", QVector<QString>() << ELEMENTID },
    { "form", QVector<QString>() << FORMID },
  };
  map.insert("label", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "value", QVector<QString>() << INTEGER },
  };
  map.insert("li", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "crossorigin",
      QVector<QString>() << "anonymous"
                         << "use-credentials" },
    { "href", QVector<QString>() << URL },
    { "hreflang", QVector<QString>() << LANGUAGECODE },
    { "media", QVector<QString>() << MEDIAQUERY },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "unsafe-url" },
    { "rel",
      QVector<QString>() << "alternate"
                         << "author"
                         << "dns-prefetch"
                         << "help"
                         << "icon"
                         << "license"
                         << "next"
                         << "pingback"
                         << "preconnect"
                         << "prefetch"
                         << "preload"
                         << "prerender"
                         << "prev"
                         << "search"
                         << "stylesheet" },
    { "sizes", QVector<QString>() << HEIGHTxWIDTH },
    { "title", QVector<QString>() << NOVALUE },
    { "type", QVector<QString>() << MEDIATYPE },
  };
  map.insert("link", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "name", QVector<QString>() << MAPNAME },
  };
  map.insert("map", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "name", QVector<QString>() << CHARSET },
    { "content", QVector<QString>() << TEXT },
    { "http-equiv",
      QVector<QString>() << "content-security-policy"
                         << "content-type"
                         << "default-style"
                         << "refresh" },
    { "name",
      QVector<QString>() << "application-name"
                         << "author"
                         << "description"
                         << "generator"
                         << "keywords"
                         << "viewport" },
  };
  map.insert("meta", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "form", QVector<QString>() << FORMID },
    { "high", QVector<QString>() << NUMBER },
    { "low", QVector<QString>() << NUMBER },
    { "max", QVector<QString>() << NUMBER },
    { "min", QVector<QString>() << NUMBER },
    { "optimum", QVector<QString>() << NUMBER },
    { "value", QVector<QString>() << NUMBER },
  };
  map.insert("meter", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "data", QVector<QString>() << URL },
    { "form", QVector<QString>() << FORMID },
    { "height", QVector<QString>() << INTEGER },
    { "name", QVector<QString>() << MEDIATYPE },
    { "type", QVector<QString>() << MAPNAME },
    { "typemustmatch",
      QVector<QString>() << "true"
                         << "false" },
    { "usemap", QVector<QString>() << MAPNAME },
    { "width", QVector<QString>() << INTEGER },
  };
  map.insert("object", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "name", QVector<QString>() << NAME },
    { "value", QVector<QString>() << VALUE },
  };
  map.insert("param", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "max", QVector<QString>() << NUMBER },
    { "value", QVector<QString>() << NUMBER },
  };
  map.insert("progress", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "cite", QVector<QString>() << URL },
  };
  map.insert("q", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "async", QVector<QString>() << NOVALUE },
    { "crossorigin",
      QVector<QString>() << "anonymous"
                         << "use-credentials" },
    { "defer", QVector<QString>() << NOVALUE },
    { "integrity", QVector<QString>() << FILEHASH },
    { "nomodule",
      QVector<QString>() << "true"
                         << "false" },
    { "referrerpolicy",
      QVector<QString>() << "no-referrer"
                         << "no-referrer-when-downgrade"
                         << "origin"
                         << "origin-when-cross-origin"
                         << "same-origin"
                         << "strict-origin"
                         << "strict-origin-when-cross-origin"
                         << "unsafe-url" },
    { "src", QVector<QString>() << URL },
    { "type", QVector<QString>() << SCRIPTTYPE },
  };
  map.insert("script", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "autofocus", QVector<QString>() << NOVALUE },
    { "disabled", QVector<QString>() << NOVALUE },
    { "form", QVector<QString>() << FORMID },
    { "multiple", QVector<QString>() << NOVALUE },
    { "name", QVector<QString>() << NAME },
    { "required", QVector<QString>() << NOVALUE },
    { "size", QVector<QString>() << INTEGER },
  };
  map.insert("select", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "media", QVector<QString>() << MEDIAQUERY },
    { "sizes", QVector<QString>() << SIZELIST },
    { "src", QVector<QString>() << URL },
    { "srcset", QVector<QString>() << URL },
    { "type", QVector<QString>() << MIMETYPE },
  };
  map.insert("source", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "media", QVector<QString>() << MEDIAQUERY },
    { "type", QVector<QString>() << "text/css" },
  };
  map.insert("style", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "colspan", QVector<QString>() << INTEGER },
    { "headers", QVector<QString>() << HEADERID },
    { "rowspan", QVector<QString>() << INTEGER },
  };
  map.insert("td", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "autofocus", QVector<QString>() << NOVALUE },
    { "cols", QVector<QString>() << INTEGER },
    { "dirname", QVector<QString>() << DIRNAME },
    { "disabled", QVector<QString>() << NOVALUE },
    { "form", QVector<QString>() << FORMID },
    { "maxlength", QVector<QString>() << INTEGER },
    { "name", QVector<QString>() << TEXT },
    { "placeholder", QVector<QString>() << TEXT },
    { "readonly", QVector<QString>() << NOVALUE },
    { "required", QVector<QString>() << NOVALUE },
    { "rows", QVector<QString>() << INTEGER },
    { "wrap",
      QVector<QString>() << "hard"
                         << "soft" },
  };
  map.insert("textarea", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "abbr", QVector<QString>() << TEXT },
    { "colspan", QVector<QString>() << INTEGER },
    { "headers", QVector<QString>() << HEADERID },
    { "rowspan", QVector<QString>() << INTEGER },
    { "scope",
      QVector<QString>() << "col"
                         << "colgroup"
                         << "row"
                         << "rowgroup" },
  };
  map.insert("th", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "datetime", QVector<QString>() << DATETIME },
  };
  map.insert("time", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "default", QVector<QString>() << NOVALUE },
    { "kind",
      QVector<QString>() << "captions"
                         << "chapters"
                         << "descriptions"
                         << "metadata"
                         << "subtitles" },
    { "label", QVector<QString>() << TEXT },
    { "src", QVector<QString>() << URL },
    { "srclang", QVector<QString>() << LANGUAGECODE },
  };
  map.insert("track", mappedAttributes);

  mappedAttributes = QMap<QString, QVector<QString>>{
    { "autoplay", QVector<QString>() << NOVALUE },
    { "controls", QVector<QString>() << NOVALUE },
    { "height", QVector<QString>() << INTEGER },
    { "loop", QVector<QString>() << NOVALUE },
    { "muted", QVector<QString>() << NOVALUE },
    { "poster", QVector<QString>() << URL },
    { "preload",
      QVector<QString>() << "auto"
                         << "metadata"
                         << "none" },
    { "src", QVector<QString>() << URL },
    { "width", QVector<QString>() << INTEGER },
  };
  map.insert("video", mappedAttributes);

  return map;
}

QVector<QString>
HtmlTypes::initHtmlTagnames()
{
  QVector<QString> vec{
    "a",        "abbr",     "address",  "area",       "article",  "aside",
    "audio",    "b",        "base",     "bdi",        "bdo",      "blockquote",
    "body",     "br",       "button",   "canvas",     "caption",  "cite",
    "code",     "col",      "colgroup", "data",       "datalist", "dd",
    "del",      "details",  "dfn",      "dialog",     "div",      "dl",
    "em",       "embed",    "fieldset", "figcaption", "figure",   "footer",
    "form",     "h1",       "h2",       "h3",         "h4",       "h5",
    "h6",       "head",     "header",
    "hgroup", // deprecated in HTML 5
    "hr",       "html",     "i",        "iframe",     "img",      "input",
    "ins",      "kbd",      "label",    "legend",     "li",       "link",
    "main",     "map",      "mark",     "menu",       "meta",     "meter",
    "nav",      "noscript", "object",   "ol",         "optgroup", "option",
    "output",   "p",        "param",    "picture",    "pre",      "progress",
    "q",        "rb",       "rp",       "rt",         "rtc",      "ruby",
    "s",        "samp",     "script",   "section",    "select",   "slot",
    "small",    "source",   "span",     "strong",     "style",    "sub",
    "summary",  "sup",      "table",    "tbody",      "td",       "template",
    "textarea", "tfoot",    "th",       "thead",      "time",     "title",
    "tr",       "track",    "u",        "ul",         "var",      "video",
    "wbr"
  };
  return vec;
}

QVector<QString>
HtmlTypes::initVoidHtmlTagnames()
{
  QVector<QString> vec{ "area",    "base",   "br",    "col",
                        "command", "embed",  "hr",    "img",
                        "input",   "keygen", "link",  "meta",
                        "param",   "source", "track", "wbr" };
  return vec;
}

QVector<QString>
HtmlTypes::initPseudoClasses()
{
  QVector<QString> vec{
    ":fullscreen",
    ":modal",
    ":picture-in-picture",
    ":autofill",
    ":enabled",
    ":disabled",
    ":disabled",
    ":read-only",
    ":read-write",
    ":placeholder-shown",
    ":placeholder-shown",
    ":blank",
    ":valid",
    ":invalid",
    ":in-range",
    ":out-of-range",
    ":required",
    ":optional",
    ":user-invalid",
    ":dir()",
    ":dir()",
    ":lang()",
    ":any-link",
    ":link",
    ":visited",
    ":local-link",
    ":target",
    ":target-within",
    ":scope",
    ":playing",
    ":paused",
    ":current",
    ":past",
    ":future",
    ":root",
    ":empty",
    ":nth-child",
    ":nth-last-child",
    ":first-child",
    ":last-child",
    ":only-child",
    ":nth-of-type",
    ":nth-last-of-type",
    ":first-of-type",
    ":last-of-type",
    ":only-of-type",
    ":hover",
    ":hover",
    ":hover",
    ":focus-visible",
    ":focus-within",
  };
  return vec;
}

QVector<QString>
HtmlTypes::initGlobalAttributes()
{
  QVector<QString> vec{
    "accesskey",       //
    "autocapitalize",  //
    "autofocus",       //
    "class",           //
    "contenteditable", //
    "contextmenu",     //
    "dir",             //
    "draggable",       //
    "enterkeyhint",    //
    "exportparts",     //
    "hidden",          //
    "id",              //
    "inputmode",       //
    "is",              //
    "itemid",          //
    "itemref",         //
    "itemscope",       //
    "itemtype",        //
    "lang",            //
    "xml:lang",        // only for XHTML
    "nonce",           //
    "part",            //
    "slot",            //
    "spellcheck",      //
    "style",           //
    "tabindex",        //
    "title",           //
    "translate",       //
  };
  return vec;
}

QVector<QString>
HtmlTypes::initEventAttributes()
{
  QVector<QString> vec{
    "onafterprint",
    "onbeforeprint",
    "onbeforeunload",
    "onerror",
    "onhashchange",
    "onload",
    "onmessage",
    "onoffline",
    "ononline",
    "onpagehide",
    "onpageshow",
    "onpopstate",
    "onresize",
    "onstorage",
    "onunload",
    "onblur",
    "onchange",
    "oncontextmenu",
    "onfocus",
    "oninput",
    "oninvalid",
    "onreset",
    "onsearch",
    "onselect",
    "onsubmit",
    "onkeydown",
    "onkeypress",
    "onkeyup",
    "onclick",
    "ondblclick",
    "onmousedown",
    "onmousemove",
    "onmouseout",
    "onmouseover",
    "onmouseup",
    "onmousewheel",
    "onwheel",
    "ondrag",
    "ondragend",
    "ondragenter",
    "ondragleave",
    "ondragover",
    "ondragstart",
    "ondrop",
    "onscroll",
    "oncopy",
    "oncut",
    "onpaste",
    "onabort",
    "oncanplay",
    "oncanplaythrough",
    "oncuechange",
    "ondurationchange",
    "onemptied",
    "onended",
    "onerror",
    "onloadeddata",
    "onloadedmetadata",
    "onloadstart",
    "onpause",
    "onplay",
    "onplaying",
    "onprogress",
    "onratechange",
    "onseeked",
    "onseeking",
    "onstalled",
    "onsuspend",
    "ontimeupdate",
    "onvolumechange",
    "ontoggle",
  };
  return vec;
}

QVector<QString>
HtmlTypes::initMediaTypes()
{
  return QVector<QString>();
}
