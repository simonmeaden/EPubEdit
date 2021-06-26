#ifndef EPUBCONTAINER_H
#define EPUBCONTAINER_H

#include <QObject>
#include <QSharedPointer>

#include "epubpackage.h"
#include "quazip.h"
#include "quazipfile.h"

class EPubContainer : public QObject
{
  Q_OBJECT
public:
  explicit EPubContainer(QObject* parent = nullptr);

  void setData(const QString& version, const QString& xmlns);
  Package addPackage(const QString& full_path, const QString& media_type);
  QString version() const;
  QString xmlns() const;

signals:

protected:
private:
  QString m_version;
  QString m_xmlns;
  PackageList m_packages;
};
typedef QSharedPointer<EPubContainer> Container;
typedef QList<Container> ContainerList;

#endif // EPUBCONTAINER_H
