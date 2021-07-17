#include "forms/epubeditor.h"

#include "document/epubdocument.h"

EPubEditor::EPubEditor(QWidget* parent)
  : QWidget(parent)
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
