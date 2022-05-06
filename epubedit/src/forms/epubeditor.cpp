#include "forms/epubeditor.h"

#include "config.h"
#include "document/epubdocument.h"

EPubEditor::EPubEditor(PConfig config, QWidget* parent)
  : QTextEdit(parent)
  , m_config(config)
{}

EPubEditor::~EPubEditor() {}

void
EPubEditor::setDocument(PDocument document)
{
  m_document = document;
  updateDocument();
}

void
EPubEditor::updateDocument()
{}
