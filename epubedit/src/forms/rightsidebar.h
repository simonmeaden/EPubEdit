#ifndef RIGHTSIDEBAR_H
#define RIGHTSIDEBAR_H

#include <QFrame>
#include <QHBoxLayout>
#include <QPlainTextEdit>

#include "document/bookpointers.h"

class MetadataForm;

class RightSidebar : public QFrame
{
  Q_OBJECT
public:
  explicit RightSidebar(PConfig config, QWidget *parent = nullptr);

  void updateMetadataForm(PMetadata metadata);

signals:

private:
  MetadataForm* m_metadataForm;


//  void metadataHasChanged(MetadataForm::Modifications modifications);

};

#endif // RIGHTSIDEBAR_H
