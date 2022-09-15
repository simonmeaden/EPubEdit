#ifndef EMULTISPLITTER_H
#define EMULTISPLITTER_H

#include "document/bookpointers.h"
#include "multisplitter/multisplitter.h"

class AbstractEPubEditor;
class EPubEditor;

class EMultiSplitter : public MultiSplitter
{
  Q_OBJECT
public:
  EMultiSplitter(PConfig config, QWidget* parent);

  QWidget* createSplit(Qt::Orientation orientation,
                       AbstractEPubEditor* editor = nullptr);

  EPubEditor *createEditor();
  EPubEditor* currentEditor();

  QList<AbstractEPubEditor *> widgets();

signals:
  void splitWidget(Qt::Orientation orientation, EPubEditor* widget = nullptr);
  void splitToWindow(QWidget* widget = nullptr);
  void sendLogMessage(const QString& message);

private:
  PConfig m_config;

  void createSplitterData(QSplitter *splitter);

};

#endif // EMULTISPLITTER_H
