#include "leftsidebar.h"
#include "tocform.h"

//#include "common.h"

LeftSidebar::LeftSidebar(QWidget* parent)
  : QFrame{ parent }
{
  initGui();
}

void
LeftSidebar::initGui()
{
  auto layout = new QGridLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QStringList titles;
  titles << tr("Table of Contents") << tr("Secondary");

  auto typeBox = new QComboBox(this);
  typeBox->addItems(titles);
  layout->addWidget(typeBox, 0, 0);
  auto typeLayout = new QStackedLayout;
  typeLayout->setContentsMargins(0, 0, 0, 0);
  connect(typeBox,
          qOverload<int>(&QComboBox::currentIndexChanged),
          typeLayout,
          &QStackedLayout::setCurrentIndex);
  layout->addLayout(typeLayout, 1, 0);

  m_tocForm = new TocForm(this);
  typeLayout->addWidget(m_tocForm);
  typeLayout->addWidget(new QFrame(this));
  connect(m_tocForm, &TocForm::itemSelected, this, &LeftSidebar::tocClicked);
}

void
LeftSidebar::updateMetadataForm(PMetadata metadata)
{
  m_tocForm->setMetadata(metadata);
}
