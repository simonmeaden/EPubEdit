#ifndef EMULTISPLITTER_H
#define EMULTISPLITTER_H

#include <multisplitter.h>

struct ESplitterData
{
  SplitterData* splitterData;
  QString filename;
  int cursorPos;
};

class EMultiSplitter : public MultiSplitter
{
public:
  EMultiSplitter(QWidget*parent);

private:
};

#endif // EMULTISPLITTER_H
