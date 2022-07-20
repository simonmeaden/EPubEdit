#ifndef IEPUBDOCUMENT_H
#define IEPUBDOCUMENT_H

#include <QTextDocument>
#include <QAbstractScrollArea>

//#include "forms/abstractepubeditor.h"

class IEpubDocument
{
public:
  IEpubDocument(QAbstractScrollArea *editor);

  QTextDocument *document();

  QTextCursor currentCursor();

private:
  QAbstractScrollArea* i_editor;
};

#endif // IEPUBDOCUMENT_H
