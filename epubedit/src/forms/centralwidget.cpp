#include "forms/centralwidget.h"
#include "forms/epubeditor.h"
#include "forms/infowidget.h"

//====================================================================
//=== CentralWidget
//====================================================================
CentralWidget::CentralWidget(PConfig config,
                             QUndoStack* undoStack,
                             QWidget* parent)
  : QWidget(parent)
  , m_config(config)
  , m_undoStack(undoStack)
{
  setContentsMargins(0, 0, 0, 0);

  initGui();
}

void
CentralWidget::initGui()
{
  auto p = palette();
  p.setColor(QPalette::Window, QColor("blue"));
  setPalette(p);

//  auto layout = new QHBoxLayout;
//  layout->setContentsMargins(0, 0, 0, 0);
//  setLayout(layout);

//  m_splitter = new QSplitter(this);
//  layout->addWidget(m_splitter);

//  m_leftSidebar = new QWidget(this);
//  m_leftSidebar->setPalette(p);
//  m_splitter->addWidget(m_leftSidebar);

//  auto docker = new DockWidget(this);
//  p.setColor(QPalette::Window, QColor("red"));
//  m_editSplitter = new MultiSplitter(docker, this);
//  m_editSplitter->setPalette(p);
//  m_splitter->addWidget(m_editSplitter);

//  p.setColor(QPalette::Window, QColor("lightgreen"));
//  m_rightSidebar = new QWidget(this);
//  m_rightSidebar->setPalette(p);
//  m_splitter->addWidget(m_rightSidebar);

  //  auto editor = new EPubEditor(config, this);

  //  auto splitter = new MultiSplitter(editor, this);
  //  layout->addWidget(splitter);

  //  m_splitter = new QSplitter(Qt::Vertical, this);
  //  m_splitter->setContentsMargins(0, 0, 0, 0);
  //  connect(m_splitter,
  //          &QSplitter::splitterMoved,
  //          this,
  //          &CentralWidget::splitterHasMoved);
  //  layout->addWidget(m_splitter);

  //  m_splitter->setSizes(config->vSplitterSizes());

  //  m_editor = new EPubEdit(config, this);
  //  m_splitter->addWidget(m_editor);

  //  //== Information frame ==//
  //  m_infoWidget = new InfoWidget(config, undoStack, this);
  //  m_splitter->addWidget(m_infoWidget);
}

EPubEditor*
CentralWidget::editor()
{
  if (m_editor)
    return m_editor;
  return nullptr;
}

QPlainTextEdit*
CentralWidget::logPage() const
{
  if (m_infoWidget)
    return m_infoWidget->logPage();
  return nullptr;
}

QUndoView*
CentralWidget::undoView()
{
  if (m_infoWidget)
    return m_infoWidget->undoView();
  return nullptr;
}

void
CentralWidget::splitterHasMoved()
{
  m_config->setVSplitterSizes(m_splitter->sizes());
}
