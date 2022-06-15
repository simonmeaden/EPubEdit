#include "forms/centralwidget.h"
#include "docksplitter.h"
#include "forms/epubeditor.h"
#include "forms/infowidget.h"
#include "forms/emultisplitter.h"

#include "leftsidebar.h"
#include "rightsidebar.h"

#include "common.h"
#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/metadata.h"
#include "document/options.h"
#include "document/series.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include <QDebug>

//====================================================================
//=== CentralWidget
//====================================================================
CentralWidget::CentralWidget(PConfig config,
                             QUndoStack* undoStack,
                             QWidget* parent)
  : QWidget(parent)
  , m_config(config)
  , m_undoStack(undoStack)
  , m_options(POptions(new EBookOptions()))
  , m_libraryDB(PLibraryDB(new EBookLibraryDB(m_options)))
  , m_seriesDB(PSeriesDB(new EBookSeriesDB(m_options)))
  , m_authorsDB(PAuthorsDB(new EBookAuthorsDB()))
{
  setContentsMargins(0, 0, 0, 0);
  initGui();
}

void
CentralWidget::initGui()
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_mainSplitter = new DockSplitter(Qt::Vertical, this);
  connect(m_mainSplitter,
          &QSplitter::splitterMoved,
          this,
          &CentralWidget::splitterHasMoved);
  m_mainSplitter->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_mainSplitter);

  m_subSplitter = new QSplitter(this);
  connect(m_subSplitter,
          &QSplitter::splitterMoved,
          this,
          &CentralWidget::splitterHasMoved);
  m_subSplitter->setContentsMargins(0, 0, 0, 0);
  m_mainSplitter->addWidget(m_subSplitter);

  m_leftSidebar = new LeftSidebar(this);
  m_subSplitter->addWidget(m_leftSidebar);
  connect(this,
          &CentralWidget::metadataChanged,
          m_leftSidebar,
          &LeftSidebar::updateMetadataForm);
  connect(
    m_leftSidebar, &LeftSidebar::tocClicked, this, &CentralWidget::tocClicked);

  m_editSplitter = new EMultiSplitter(this);
  m_subSplitter->addWidget(m_editSplitter);

  m_editor = new EPubEditor(m_config, this);
  m_editSplitter->addWidget(m_editor);
  connect(
    m_editor, &EPubEditor::splitWidget, m_editSplitter, &EMultiSplitter::createSplit);
  connect(
    m_editor, &EPubEditor::splitToWindow, m_editSplitter, &EMultiSplitter::splitToWindow);

  m_rightSidebar = new RightSidebar(m_config, this);
  m_subSplitter->addWidget(m_rightSidebar);
  connect(this,
          &CentralWidget::metadataChanged,
          m_rightSidebar,
          &RightSidebar::updateMetadataForm);

  m_infoWidget = new InfoWidget(m_config, m_undoStack, this);
  m_mainSplitter->addWidget(m_infoWidget);
  connect(m_infoWidget,
          &InfoWidget::sizeChanged,
          m_mainSplitter,
          &DockSplitter::widgetSizeChanged);
  connect(
    this, &CentralWidget::widgetClicked, m_infoWidget, &InfoWidget::showWidget);

  m_mainSplitter->setSizes(m_config->mainSplitterSizes());
  m_subSplitter->setSizes(m_config->centralSplitterSizes());

  if (m_config->isLeftSidebarVisible())
    m_leftSidebar->show();
  else
    m_leftSidebar->hide();
  if (m_config->isRightSidebarVisible())
    m_rightSidebar->show();
  else
    m_rightSidebar->hide();
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
  //  if (m_infoWidget)
  //    return m_infoWidget->logPage();
  return nullptr;
}

QUndoView*
CentralWidget::undoView()
{
  //  if (m_infoWidget)
  //    return m_infoWidget->undoView();
  return nullptr;
}

bool
CentralWidget::toggleLeftSidebar()
{
  if (m_leftSidebar->isVisible()) {
    m_leftSidebar->hide();
    m_config->setLeftSidebarVisible(false);
    return false;
  } else {
    m_leftSidebar->show();
    m_config->setLeftSidebarVisible(true);
    return true;
  }
}

bool
CentralWidget::toggleRightSidebar()
{
  if (m_rightSidebar->isVisible()) {
    m_rightSidebar->hide();
    m_config->setRightSidebarVisible(false);
    return false;
  } else {
    m_rightSidebar->show();
    m_config->setRightSidebarVisible(true);
    return true;
  }
}

bool
CentralWidget::loadDocument(const QString& filename)
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TODO drop changes/save document / cancel load maybe multi tabs?
  } else {
    m_document = PDocument(new EPubDocument(
      m_config, m_options, m_libraryDB, m_seriesDB, m_authorsDB, this));
    PBookData data = m_document->loadDocument(filename);
    if (data.isNull()) {
      // TODO error no book data.
    } else {
      emit metadataChanged(m_document->metadata());
    }
  }

  return m_loaded;
}

bool
CentralWidget::saveDocument(const QString& filename)
{
  if (!m_document.isNull()) {
    m_document->saveDocument(filename);
  }
  return false;
}

bool
CentralWidget::newDocument()
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TOD drop changes/save document / cancel load
  } else {
    m_document = PDocument(new EPubDocument(
      m_config, m_options, m_libraryDB, m_seriesDB, m_authorsDB, this));
    emit metadataChanged(m_document->metadata());
    m_loaded = true;
  }
  return m_loaded;
}

// void
// CentralWidget::loadFileIntoTextDocument(const QString& zipfile,
//                                         const QString& imageName)
//{
//   auto fileName = JlCompress::extractFile(zipfile, imageName);
//   QFile file(fileName);
//   QTextStream out(&file);
//   if (file.open(QIODevice::ReadOnly)) {
//     auto text = file.readAll();
//     m_editor->setHtml(text);
//   }
// }

void
CentralWidget::splitterHasMoved()
{
  m_config->setMainSplitterSizes(m_mainSplitter->sizes());
  m_config->setCentralSplitterSizes(m_subSplitter->sizes());
}

void
CentralWidget::tocClicked(const QString& href)
{
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  QTextStream out(&file);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    m_editor->setHtml(text);
  }
}
