#ifndef EPUBMETADATA_H
#define EPUBMETADATA_H

#include <QDomNode>
#include <QList>
#include <QMultiMap>
#include <QObject>
#include <QSharedPointer>

class QuaZip;

struct Base
{
  QString value;
};
typedef QSharedPointer<Base> BASE;

struct IdBase
{
  QString id;
};

struct DcLanguage
  : public Base
  , public IdBase
{};
typedef QSharedPointer<DcLanguage> DCLANGUAGE;

struct DcUniqueIdentifier
  : public Base
  , public IdBase
{
  QString opfScheme;
};
typedef QSharedPointer<DcUniqueIdentifier> DCUNIQUEIDENTIFIER;

struct DcTitle
  : public Base
  , public IdBase
{
  QString xmlLang;
  QString dir;
  QString altRep;
  QString altRepLang;
  QString fileAs;
};
typedef QSharedPointer<DcTitle> DCTITLE;

class EPubMetadata : public QObject
{
  Q_OBJECT
public:
  enum State
  {
    NoDcTitle = 1,
    NoUniquIdentifier = 2,
    TooManyIdentifiers = 4,
    Error = NoDcTitle | NoUniquIdentifier,
  };
  Q_DECLARE_FLAGS(Errors, State);
  Q_FLAG(Errors)
  enum Type
  {
    PrimaryTitle,
    Title,
    Identifier,
    Language,

    XmlnsDc,
    XmlnsOpf,
    DcTitleId,
    DcTitleXmlLang,
    DcTitleDir,
    OpfAltRep,
    OpfAltRepLang,
    OpfFileAs,
    DcIdentifierId,
    OpfScheme,

    Value,
  };

  explicit EPubMetadata(QObject* parent = nullptr);

  QString xmlnsDc() const;
  void setXmlnsDc(const QString& xmlnsDc);

  bool parse(const QDomNode& node, int version);

  void setFlag(State error);
  void clearFlag(State error);
  bool containsFlag(State error);

  void setBase(Type type, BASE base);
  void setBaseValue(Type type, const QString& value, BASE base);
  void setValue(Type type, const QString& value);

signals:

protected:
private:
  QString m_xmlnsDc;
  QString m_xmlnsOpf;
  Errors m_errors;
  QMultiMap<Type, BASE> m_base;
  //  DcTitle m_title;
};
typedef QSharedPointer<EPubMetadata> Metadata;
Q_DECLARE_OPERATORS_FOR_FLAGS(EPubMetadata::Errors)

#endif // EPUBMETADATA_H
