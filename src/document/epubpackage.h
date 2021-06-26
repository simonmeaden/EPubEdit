#ifndef EPUBPACKAGE_H
#define EPUBPACKAGE_H

#include <QDomDocument>
#include <QList>
#include <QObject>
#include <QSharedPointer>

#include "epubmetadata.h"

class QuaZip;

class EPubPackage : public QObject
{
  Q_OBJECT
public:
  explicit EPubPackage(const QString& full_path,
                       const QString& media_type,
                       QObject* parent = nullptr);

  Metadata addMetadata(const QString& xmlns)
  {
    Metadata metadata(xmlns, this);
    return metadata;
  }
  bool parse(QuaZip* archive);

signals:

protected:
private:
  QString m_fullPath, m_mediaType;
  QString m_xmlns, m_xmlLang, m_uniqueIdentifier, m_prefix;
  int m_version;
  //  Metadata m_metadata;
  //  Metadata parseMetadata(const QDomNode& node, int version = 30);
};
typedef QSharedPointer<EPubPackage> Package;
typedef QList<Package> PackageList;

#endif // EPUBPACKAGE_H
