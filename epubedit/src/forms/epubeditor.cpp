#include "forms/epubeditor.h"

#include "config.h"
#include "document/epubdocument.h"

EPubEditor::EPubEditor(Config* config, QWidget* parent)
  : QWidget(parent)
  , m_config(config)
{}

EPubEditor::~EPubEditor() {}

void
EPubEditor::setDocument(QSharedPointer<EPubDocument> document)
{
  m_document = document;
  updateDocument();
}

void
EPubEditor::updateDocument()
{}
