#include "iepubdocument.h"
#include "forms/epubedit.h"
#include "forms/codeedit.h"

IEpubDocument::IEpubDocument(QAbstractScrollArea* editor)
  : i_editor(editor)
{
}

QTextDocument* IEpubDocument::document() {
  auto e = qobject_cast<EPubEdit*>(i_editor);
  if (e)
  return e->document();
  else {
    auto c = qobject_cast<CodeEdit*>(i_editor);
    if (c) {
      return c->document();
    }
  }
  return nullptr;
}

QTextCursor
IEpubDocument::currentCursor()
{
  auto e = qobject_cast<EPubEdit*>(i_editor);
  if (e)
  return e->textCursor();
  else {
    auto c = qobject_cast<CodeEdit*>(i_editor);
    if (c) {
      return c->textCursor();
    }
  }
  return QTextCursor();
}
