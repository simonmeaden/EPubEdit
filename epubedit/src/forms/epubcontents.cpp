#include "epubcontents.h"
#include "headerwidget.h"

//====================================================================
//=== ContentsForm
//====================================================================
ContentsFrame::ContentsFrame(QWidget* parent)
  : QWidget(parent)
{
  initGui();
}

void
ContentsFrame::setDocument(PDocument document)
{}

void
ContentsFrame::closeBtnClicked()
{
  if (isVisible())
    setVisible(false);
}

void
ContentsFrame::initGui()
{
  setAutoFillBackground(true);
  setContentsMargins(0, 0, 0, 0);

  auto layout = new BorderLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_header = new HeaderWidget(this);
  layout->addWidget(m_header, BorderLayout::North);

  auto f = new QFrame(this);
  layout->addWidget(f, BorderLayout::Center);
}


