#include "forms/editorwidget.h"
#include "docker/docksplitter.h"
#include "forms/emultisplitter.h"
#include "forms/epubeditor.h"
#include "forms/infowidget.h"

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

#include "signalappender.h"
#include <FileAppender.h>
#include <Logger.h>

#include <QDebug>

//====================================================================
//=== CentralWidget
//====================================================================
EditorWidget::EditorWidget(PConfig config,
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

  auto signalAppender = new SignalAppender();
  signalAppender->setFormat(
    "[%{file}] [%{type:-7}] <%{Function}> Line:%{line} %{message}");
  signalAppender->setDetailsLevel(Logger::Debug);
  cuteLogger->registerAppender(signalAppender);
  connect(signalAppender,
          &SignalAppender::writeMessage,
          m_infoWidget,
          &InfoWidget::appendLog);
}

void
EditorWidget::initGui()
{
  auto layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  //  auto p = palette();
  //  p.setColor(QPalette::Window, QColor("red"));
  //  setPalette(p);

  m_mainSplitter = new DockSplitter(Qt::Vertical, this);
  connect(m_mainSplitter,
          &QSplitter::splitterMoved,
          this,
          &EditorWidget::splitterHasMoved);
  m_mainSplitter->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_mainSplitter);

  m_subSplitter = new QSplitter(this);
  connect(m_subSplitter,
          &QSplitter::splitterMoved,
          this,
          &EditorWidget::splitterHasMoved);
  m_subSplitter->setContentsMargins(0, 0, 0, 0);
  m_mainSplitter->addWidget(m_subSplitter);

  m_leftSidebar = new LeftSidebar(this);
  m_subSplitter->addWidget(m_leftSidebar);
  connect(this,
          &EditorWidget::metadataChanged,
          m_leftSidebar,
          &LeftSidebar::updateMetadataForm);
  connect(
    m_leftSidebar, &LeftSidebar::tocClicked, this, &EditorWidget::loadHref);

  m_editSplitter = new EMultiSplitter(m_config, this);
  m_subSplitter->addWidget(m_editSplitter);

  ////  auto editor = m_editSplitter->createEditor();

  m_rightSidebar = new RightSidebar(m_config, this);
  m_subSplitter->addWidget(m_rightSidebar);
  connect(this,
          &EditorWidget::metadataChanged,
          m_rightSidebar,
          &RightSidebar::updateMetadataForm);

  m_infoWidget = new InfoWidget(m_config, m_undoStack, this);
  m_mainSplitter->addWidget(m_infoWidget);
  connect(m_infoWidget,
          &InfoWidget::sizeChanged,
          m_mainSplitter,
          &DockSplitter::widgetSizeChanged);
  connect(
    this, &EditorWidget::widgetClicked, m_infoWidget, &InfoWidget::showWidget);
  connect(m_editSplitter,
          &EMultiSplitter::sendLogMessage,
          m_infoWidget,
          &InfoWidget::appendLog);

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
EditorWidget::editor()
{
  auto editor = qobject_cast<EPubEditor*>(m_editSplitter->currentWidget());
  if (editor)
    return editor;
  return nullptr;
}

QPlainTextEdit*
EditorWidget::logPage() const
{
  //  if (m_infoWidget)
  //    return m_infoWidget->logPage();
  return nullptr;
}

QUndoView*
EditorWidget::undoView()
{
  //  if (m_infoWidget)
  //    return m_infoWidget->undoView();
  return nullptr;
}

bool
EditorWidget::toggleLeftSidebar()
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
EditorWidget::toggleRightSidebar()
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
EditorWidget::loadDocument(const QString& filename)
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

    auto editor = qobject_cast<EPubEditor*>(m_editSplitter->currentWidget());
    if (editor)
      editor->setDocument(m_document);

    // find the first page
    auto manifest = m_document->manifest();
    auto pages = m_document->spine()->orderedItems;
    auto page = pages.first();
    auto manifestItem = manifest->itemsById.value(page);
    loadPage(manifestItem);
  }

  return m_loaded;
}

bool
EditorWidget::saveDocument(const QString& filename)
{
  if (!m_document.isNull()) {
    m_document->saveDocument(filename);
  }
  return false;
}

bool
EditorWidget::newDocument()
{
  auto current = m_editSplitter->currentEditor();
  auto visible = current->isVisible();
  auto eVisibleTo = current->isVisibleTo(m_editSplitter);
  auto cVisibleTo = m_editSplitter->isVisibleTo(this);
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

void
EditorWidget::splitterHasMoved()
{
  m_config->setMainSplitterSizes(m_mainSplitter->sizes());
  m_config->setCentralSplitterSizes(m_subSplitter->sizes());
}

void
EditorWidget::loadHref(const QString& href)
{
  auto widget = m_editSplitter->currentWidget();
  auto editor = qobject_cast<EPubEditor*>(widget);
  if (editor)
    editor->loadHref(href);
}

void
EditorWidget::loadPage(PManifestItem item)
{
  auto href = item->href;
  loadHref(href);
}

// void
// CentralWidget::setCurrentWidget(QWidget* editor)
//{
//   m_editSplitter->setCurrentWidget(editor);
// }

// QWidget*
// CentralWidget::createSplit(Qt::Orientation orientation, EPubEditor* editor)
//{
//   connect(
//     editor, &EPubEditor::gotFocus, m_editSplitter,
//     &MultiSplitter::setCurrentWidget);
//   connect(editor,
//           &EPubEditor::splitWidget,
//           this,
//           &CentralWidget::createSplit);
//   connect(editor,
//           &EPubEditor::splitToWindow,
//           m_editSplitter,
//           &EMultiSplitter::splitToWindow);
//   return m_editSplitter->createSplit(orientation, editor);
// }
