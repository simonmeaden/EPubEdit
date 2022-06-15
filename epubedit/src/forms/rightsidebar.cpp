#include "rightsidebar.h"

//#include "common.h"
#include "forms/metadataform.h"
#include "document/metadata.h"
#include "config.h"

RightSidebar::RightSidebar(PConfig config, QWidget* parent)
  : QFrame{ parent }
{
  setContentsMargins(0, 0, 0, 0);

  auto layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_metadataForm = new MetadataForm(config, this);
  layout->addWidget(m_metadataForm);
}

void
RightSidebar::updateMetadataForm(PMetadata metadata)
{
  m_metadataForm->setMetadata(metadata);
}

// void
// RightSidebar::metadataHasChanged(MetadataForm::Modificationsow); modifications)
//{
//   if (modifications.testFlag(MetadataForm::TITLES_CHANGED)) {
//     // TODO save titles.
//   }
//   if (modifications.testFlag(MetadataForm::AUTHORS_CHANGED)) {
//     // TODO save authors.
//   }
// }
