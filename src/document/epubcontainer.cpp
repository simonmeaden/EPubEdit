#include "epubcontainer.h"

EPubContainer::EPubContainer(QObject* parent)
  : QObject(parent)
{}

void
EPubContainer::setData(const QString& version, const QString& xmlns)
{
  m_version = version;
  m_xmlns = xmlns;
}

Package
EPubContainer::addPackage(const QString& full_path, const QString& media_type)
{
  Package package(new EPubPackage(full_path, media_type, this));
  m_packages.append(package);
  return package;
}

QString
EPubContainer::version() const
{
  return m_version;
}

QString
EPubContainer::xmlns() const
{
  return m_xmlns;
}
