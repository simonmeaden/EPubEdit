#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <QSharedPointer>

#include "epubcontainer.h"
#include "quazip.h"
#include "quazipfile.h"

class EPubDocument : public QObject
{
  Q_OBJECT
public:
  explicit EPubDocument(QObject* parent = nullptr);

  bool loadDocument(const QString& filename);

  QString mimetype();
  bool isMimetypeValid();

  QString filename() const;

signals:
  void updateMetadata();

protected:
private:
  QString m_filename;
  QString m_mimetype;

  static const QString MIMETYPE_FILE;
  static const QByteArray MIMETYPE;
  static const QString CONTAINER_FILE;
  //  static const QString TOC_TITLE;
  //  static const QString LIST_START;
  //  static const QString LIST_END;
  //  static const QString LIST_ITEM;
  //  static const QString LIST_BUILD_ITEM;
  //  static const QString LIST_FILEPOS;
  //  static const QString HTML_DOCTYPE;
  //  static const QString XML_HEADER;
  //  static const QString HTML_XMLNS;

  Container m_container;

  bool parseMimetype(QStringList files, QuaZip* archive);
  bool parseContainer(QStringList files, QuaZip* archive);
};
typedef QSharedPointer<EPubDocument> Document;

#endif // EPUBDOCUMENT_H
