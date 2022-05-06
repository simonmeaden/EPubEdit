#include "mainwidget.h"

//====================================================================
//=== MainWidget
//====================================================================
MainWidget::MainWidget(PConfig config, QWidget* parent)
  : QWidget{ parent }
{
  setContentsMargins(0, 0, 0, 0);
  setAutoFillBackground(true);

  auto layout = new BorderLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  // add widgets to frame.
  m_toolbar = new ToolbarWidget(this);
  layout->addWidget(m_toolbar, BorderLayout::West);

  auto index = m_toolbar->addWidget(WidgetWrapper::BUTTON,
                                    WidgetWrapper::TOP,
                                    QImage(":/icons/New"),
                                    tr("New"));
  m_toolbar->setTooltip(index, tr("Create a new epub file."));
  index = m_toolbar->addWidget(WidgetWrapper::BUTTON,
                               WidgetWrapper::TOP,
                               QImage(":/icons/Open"),
                               tr("Open"));
  m_toolbar->setTooltip(index, tr("Open an epub file."));
  index = m_toolbar->addWidget(WidgetWrapper::BUTTON,
                               WidgetWrapper::TOP,
                               QImage((":/icons/Save")),
                               tr("Save"));
  m_toolbar->setTooltip(index, tr("Save the epub file."));


  m_centreFrame = new CentralWidget(config, this);
  auto centreLayout = new QStackedLayout;
  m_centreFrame->setContentsMargins(0, 0, 0, 0);
  centreLayout->setContentsMargins(0, 0, 0, 0);
  m_centreFrame->setLayout(centreLayout);
  layout->addWidget(m_centreFrame, BorderLayout::Center);
}

EPubEdit*
MainWidget::editor()
{
  return m_centreFrame->editor();
}

QPlainTextEdit*
MainWidget::logPage() const
{
  return m_centreFrame->logPage();
}


//====================================================================
//=== CentralWidget
//====================================================================
CentralWidget::CentralWidget(PConfig config, QWidget* parent)
  : QWidget(parent)
  , m_mainWidget(qobject_cast<MainWidget*>(parent))
  , m_config(config)
{
  setContentsMargins(0, 0, 0, 0);
  setAutoFillBackground(true);

  auto layout = new BorderLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_footer = new FooterWidget(this);
  layout->addWidget(m_footer, BorderLayout::South);

  QImage img(":/icons/hideshowcontents.png");
//  int index =
    m_footer->addWidget(WidgetWrapper::BUTTON, WidgetWrapper::LEFT, img);

  m_mainSplitter = new QSplitter(Qt::Vertical, this);
  m_mainSplitter->setContentsMargins(0, 0, 0, 0);
  connect(m_mainSplitter,
          &QSplitter::splitterMoved,
          this,
          &CentralWidget::splitterHasMoved);
  layout->addWidget(m_mainSplitter, BorderLayout::Center);
  m_mainSplitter->setSizes(config->vSplitterSizes());

  m_editor = new EPubEdit(config, this);
  m_mainSplitter->addWidget(m_editor);

  m_infoSplitter = new QSplitter(this);
  m_infoSplitter->setContentsMargins(0, 0, 0, 0);
  connect(m_infoSplitter,
          &QSplitter::splitterMoved,
          this,
          &CentralWidget::splitterHasMoved);
  m_infoSplitter->setSizes(config->iSplitterSizes());

  m_logPage = new QPlainTextEdit(this);
  m_logPage->setReadOnly(true);
  m_infoSplitter->addWidget(m_logPage);

  auto undoPage = m_editor->undoView();
  m_infoSplitter->addWidget(undoPage);

  //  m_infoWidget = new InformationWidget(config, this);
  m_mainSplitter->addWidget(m_infoSplitter);
}

EPubEdit*
CentralWidget::editor()
{
  return m_editor;
}

QPlainTextEdit*
CentralWidget::logPage() const
{
  return m_logPage;
}

void
CentralWidget::splitterHasMoved()
{
  m_config->setVSplitterSizes(m_mainSplitter->sizes());
  m_config->setISplitterSizes(m_infoSplitter->sizes());
  m_config->save();
}

