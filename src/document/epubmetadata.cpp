#include "epubmetadata.h"

#include "quazip.h"
#include "quazipfile.h"

EPubMetadata::EPubMetadata(QObject* parent)
  : QObject(parent)
  , m_errors(Error)
{}

QString
EPubMetadata::xmlnsDc() const
{
  return m_xmlnsDc;
}

void
EPubMetadata::setXmlnsDc(const QString& xmlnsDc)
{
  m_xmlnsDc = xmlnsDc;
}

bool
EPubMetadata::parse(const QDomNode& node, int version)
{
  auto map = node.attributes();
  QDomNode att;
  if (!(att = map.namedItem("xmlns:dc")).isNull()) { // should be only value.
    setValue(EPubMetadata::XmlnsDc, att.nodeValue());
  }
  if (!(att = map.namedItem("xmlns:opf")).isNull()) { // should be only value.
    setValue(EPubMetadata::XmlnsOpf, att.nodeValue());
  }

  auto children = node.childNodes();
  for (int i = 0; i < children.size(); i++) {
    auto n = children.item(i);
    auto name = n.nodeName();
    if (name == "dc:title") {
      DCTITLE title(new DcTitle());
      auto titleMap = n.attributes();
      if (!(att = titleMap.namedItem("id")).isNull()) {
        setBaseValue(EPubMetadata::DcTitleId, att.nodeValue(), title);
      }
      if (!(att = titleMap.namedItem("dir")).isNull()) {
        setBaseValue(EPubMetadata::DcTitleDir, att.nodeValue(), title);
      }
      if (!(att = titleMap.namedItem("xml:lang")).isNull()) {
        setBaseValue(EPubMetadata::DcTitleXmlLang, att.nodeValue(), title);
      }
      if (version > 30) {
        if (!(att = titleMap.namedItem("opf:alt-rep")).isNull()) {
          setBaseValue(EPubMetadata::OpfAltRep, att.nodeValue(), title);
        }
        if (!(att = titleMap.namedItem("opf:alt-rep-lang")).isNull()) {
          setBaseValue(EPubMetadata::OpfAltRepLang, att.nodeValue(), title);
        }
        if (!(att = titleMap.namedItem("opf:file-as")).isNull()) {
          setBaseValue(EPubMetadata::OpfFileAs, att.nodeValue(), title);
        }
      }
      setBaseValue(EPubMetadata::Value, n.nodeValue(), title);
      setBase(EPubMetadata::Title, title);
    } else if (name == "dc:identifier") {
      DCUNIQUEIDENTIFIER identifier(new DcUniqueIdentifier());
      auto identifierMap = n.attributes();
      if (!(att = identifierMap.namedItem("id")).isNull()) {
        setBaseValue(EPubMetadata::DcIdentifierId, att.nodeValue(), identifier);
      }
      if (!(att = identifierMap.namedItem("opf:scheme")).isNull()) {
        setBaseValue(EPubMetadata::OpfScheme, att.nodeValue(), identifier);
      }
      setBaseValue(EPubMetadata::Value, n.nodeValue(), identifier);
      setBase(EPubMetadata::Identifier, identifier);
    } else if (name == "dc:language") {
      DCLANGUAGE lang(new DcLanguage());
      setBaseValue(EPubMetadata::Value, n.nodeValue(), lang);
      setBase(EPubMetadata::Language, lang);
    }
  }
  return true;
}

void
EPubMetadata::setFlag(EPubMetadata::State error)
{
  m_errors.setFlag(error, true);
}

void
EPubMetadata::clearFlag(EPubMetadata::State error)
{
  m_errors.setFlag(error, false);
}

bool
EPubMetadata::containsFlag(EPubMetadata::State error)
{
  return m_errors.testFlag(error);
}

void
EPubMetadata::setBase(EPubMetadata::Type type, BASE base)
{
  DCTITLE title = base.dynamicCast<DcTitle>();
  if (!title.isNull()) {
    auto count = m_base.count(Title);
    if (count > 0) // Must have AT LEAST one.
      m_base.insert(type, base);
    else { // only the first is the primary.
      m_base.insert(PrimaryTitle, title);
      clearFlag(NoDcTitle);
    }
    return;
  }

  DCUNIQUEIDENTIFIER id = base.dynamicCast<DcUniqueIdentifier>();
  if (!id.isNull()) {
    m_base.insert(Identifier, title);
    auto count = m_base.count(Identifier);
    if (count > 1) // Can only be one
      setFlag(TooManyIdentifiers);
    else // must be one.
      clearFlag(NoUniquIdentifier);

    return;
  }
}

void
EPubMetadata::setBaseValue(EPubMetadata::Type type,
                           const QString& value,
                           BASE base)
{
  DCTITLE title = base.dynamicCast<DcTitle>();
  if (!title.isNull()) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    switch (type) {
      case Type::DcTitleId:
        title->id = value;
        break;
      case DcTitleDir:
        title->dir = value;
        break;
      case DcTitleXmlLang:
        title->xmlLang = value;
        break;
      case OpfAltRep:
        title->altRep = value;
        break;
      case OpfAltRepLang:
        title->altRepLang = value;
        break;
      case OpfFileAs:
        title->fileAs = value;
        break;
    }
    return;
#pragma GCC diagnostic pop
  }

  DCUNIQUEIDENTIFIER id = base.dynamicCast<DcUniqueIdentifier>();
  if (!id.isNull()) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
    DCUNIQUEIDENTIFIER id = base.dynamicCast<DcUniqueIdentifier>();
    switch (type) {
      case Type::DcTitleId:
        id->id = value;
        break;
      case DcTitleDir:
        id->opfScheme = value;
        break;
    }
    return;
#pragma GCC diagnostic pop
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
  switch (type) {
    case Type::XmlnsDc:
      m_xmlnsDc = value;
      break;
    case Type::XmlnsOpf:
      m_xmlnsOpf = value;
      break;
  }
#pragma GCC diagnostic pop
}

void
EPubMetadata::setValue(EPubMetadata::Type type, const QString& value)
{
  switch (type) {
    case Type::XmlnsDc:
      m_xmlnsDc = value;
      break;
    case Type::XmlnsOpf:
      m_xmlnsOpf = value;
      break;
  }
}
