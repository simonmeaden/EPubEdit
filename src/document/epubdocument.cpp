#include "libepubedit/document/epubdocument.h"

//#include <qlogger/qlogger.h>
// using namespace qlogger;

#include "document/htmlparser.h"
#include "epubdocument_p.h"
#include "libepubedit/authors.h"
#include "libepubedit/document/epubcontainer.h"
#include "libepubedit/ebookcommon.h"
#include "libepubedit/iebookdocument.h"
#include "libepubedit/interface_global.h"
#include "libepubedit/library.h"

EPubDocument::EPubDocument(QObject* parent)
  : ITextDocument(parent)
  , d_ptr(new EPubDocumentPrivate(this))
{
  //  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(EPubDocumentPrivate* doc, QObject* parent)
  : ITextDocument(parent)
  , d_ptr(doc)
{
  //  setUndoRedoEnabled(false);
}

EPubDocument::EPubDocument(const EPubDocument& doc)
  : ITextDocument(doc.parent())
  , d_ptr(doc.d_ptr)
{}

EPubDocument::EPubDocument(EPubDocumentPrivate& d)
  : ITextDocument(d.q_ptr)
{}

QString
EPubDocument::buildTocFromData()
{
  Q_D(EPubDocument);
  return d->buildTocFromFiles();
}

bool
EPubDocument::isModified()
{
  Q_D(EPubDocument);
  return d->isModified();
}

EPubDocument::~EPubDocument() {}

bool
EPubDocument::loaded()
{
  Q_D(EPubDocument);
  return d->loaded();
}

void
EPubDocument::openDocument(const QString& path)
{
  Q_D(EPubDocument);
  d->openDocument(path);
}

void
EPubDocument::saveDocument(const QString& path)
{
  Q_D(EPubDocument);
  d->closeDocument(path);
}

EPubContents*
EPubDocument::cloneData()
{
  Q_D(EPubDocument);
  return d->cloneData();
}

void
EPubDocument::setClonedData(EPubContents* cloneData)
{
  Q_D(EPubDocument);
  d->setClonedData(cloneData);
}

QString
EPubDocument::filename()
{
  Q_D(EPubDocument);
  return d->filename();
}

void
EPubDocument::setFilename(const QString& filename)
{
  Q_D(EPubDocument);
  d->setFilename(filename);
}

QString
EPubDocument::tocAsString()
{
  Q_D(EPubDocument);
  return d->toc();
}

QString
EPubDocument::title()
{
  Q_D(EPubDocument);
  return d->title();
}

void
EPubDocument::setTitle(const QString& title)
{
  Q_D(EPubDocument);
  d->setTitle(title);
}

QString
EPubDocument::subject()
{
  Q_D(EPubDocument);
  return d->subject();
}

void
EPubDocument::setSubject(const QString& subject)
{
  Q_D(EPubDocument);
  d->setSubject(subject);
}

QString
EPubDocument::language()
{
  Q_D(EPubDocument);
  // TODO
  return QString();
}

void
EPubDocument::setLanguage(const QString& language)
{
  Q_D(EPubDocument);
  // TODO
}

QDateTime
EPubDocument::date()
{
  Q_D(EPubDocument);
  // TODO
  return QDateTime();
}

void
EPubDocument::setDate(const QDateTime& date)
{
  Q_D(EPubDocument);
  // TODO
}

QStringList
EPubDocument::creators()
{
  Q_D(EPubDocument);
  return d->creators();
}

QString
EPubDocument::creatorNames(const QStringList& authors)
{
  Q_D(EPubDocument);
  if (authors.isEmpty())
    return d->creatorNames(d->creators());
  else {
    return d->creatorNames(authors);
  }
}

QString
EPubDocument::publisher()
{
  Q_D(EPubDocument);
  return d->publisher();
}

void
EPubDocument::setPublisher(const QString& publisher)
{
  Q_D(EPubDocument);
  d->setPublisher(publisher);
}

Metadata
EPubDocument::metadata()
{
  Q_D(EPubDocument);
  return d->metadata();
}

// QMap<QString, QString>
ManifestItemMap
EPubDocument::pages()
{
  Q_D(EPubDocument);
  return d->pages();
}

QStringList
EPubDocument::spine()
{
  Q_D(EPubDocument);
  return d->spine();
}

QStringList
EPubDocument::cssKeys()
{
  Q_D(EPubDocument);
  return d->cssKeys();
}

CSSMap
EPubDocument::css()
{
  Q_D(EPubDocument);
  return d->cssMap();
}

QString
EPubDocument::css(QString key)
{
  Q_D(EPubDocument);
  return d->css(key);
}

QString
EPubDocument::javascript(QString key)
{
  Q_D(EPubDocument);
  return d->javascript(key);
}

ManifestItem
EPubDocument::itemByHref(QString href)
{
  Q_D(EPubDocument);
  return d->itemByHref(href);
}

IEBookInterface*
EPubDocument::plugin()
{
  Q_D(EPubDocument);
  return d->plugin();
}

void
EPubDocument::setPlugin(IEBookInterface* plugin)
{
  Q_D(EPubDocument);
  d->setPlugin(plugin);
}

QDate
EPubDocument::published()
{
  Q_D(EPubDocument);
  return d->published();
}

void
EPubDocument::setPublished(const QDate& published)
{
  Q_D(EPubDocument);
  return d->setPublished(published);
}
