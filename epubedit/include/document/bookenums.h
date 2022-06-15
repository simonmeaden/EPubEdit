#ifndef BOOKENUMS_H
#define BOOKENUMS_H

#include <QObject>

enum DocumentType
{
  PLAINTEXT,
  HTML
};
Q_DECLARE_METATYPE(DocumentType)

enum EPubDocumentType
{
  UNSUPPORTED_TYPE,
  EPUB,
  MOBI,
  AZW,
  PDF,
};
Q_DECLARE_METATYPE(EPubDocumentType)

enum Direction
{
  LTR,
  RTL,
  DEFAULT,
};
Q_DECLARE_METATYPE(Direction)

enum PageSpread {
  LEFT,
  RIGHT,
  NONE,
};
Q_DECLARE_METATYPE(PageSpread)

#endif // BOOKENUMS_H
