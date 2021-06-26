#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QSharedPointer>
#include <QTabWidget>
#include <QTextEdit>

class QuaZip;
class MetadataForm;

#include "document/epubdocument.h"

class EPubEdit : public QTabWidget
{
public:
  EPubEdit(QWidget* parent = nullptr);

  bool loadEpubDocument(const QString& filename);

protected:
private:
  MetadataForm* m_metadataForm;
  Document m_document;

  void initGui();

  void updateMetadataForm();
  void metadataHasChanged();
};

#endif // EPUBEDIT_H
