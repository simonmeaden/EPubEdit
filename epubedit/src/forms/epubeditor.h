#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QTextEdit>

class EPubDocument;

#include "document/bookpointers.h"

class EPubEditor : public QTextEdit
{
  Q_OBJECT
public:
  explicit EPubEditor(PConfig config, QWidget* parent = nullptr);
  ~EPubEditor();

  void setDocument(PDocument document);

signals:

private:
  PConfig m_config;
  PDocument m_document;

  void updateDocument();
};

#endif // EPUBEDITOR_H
