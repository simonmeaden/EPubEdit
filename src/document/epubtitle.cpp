#include "epubtitle.h"

EPubTitle::EPubTitle(QObject* parent)
  : QObject(parent)
{}

QString
EPubTitle::id()
{
  return m_id;
}

void
EPubTitle::setId(QString id)
{
  m_id = id;
}

QString
EPubTitle::title() const
{
  return m_title;
}

void
EPubTitle::setTitle(const QString& title)
{
  m_title = title;
}
