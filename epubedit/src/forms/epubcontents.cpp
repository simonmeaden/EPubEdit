#include "epubcontents.h"
//#include "basewidget.h"
#include "dockwidget.h"

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

//  auto layout = new DockLayout();
//  layout->setSpacing(0);
//  layout->setContentsMargins(0, 0, 0, 0);
//  setLayout(layout);

//  auto f = new QFrame(this);
//  layout->setCentralWidget(f);
}


