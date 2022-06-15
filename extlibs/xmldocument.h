#ifndef QTINYXML2H
#define QTINYXML2H

#include <QObject>

class XMLDocument;

class QTinyXmlDocument : public QObject
{
  Q_OBJECT
public:
  explicit QTinyXmlDocument(QObject *parent = nullptr);

signals:
  XMLDocument* m_document;
};

#endif // QTINYXML2H
