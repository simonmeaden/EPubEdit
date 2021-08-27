#include "forms/epubedit.h"

#include "quazip.h"
#include "quazipfile.h"

#include "qyamlcpp.h"
#include <yaml-cpp/yaml.h>

#include "config.h"
#include "document/epubdocument.h"
#include "forms/epubeditor.h"
#include "forms/metadataform.h"
//#include "util/csvsplitter.h"

EPubEdit::EPubEdit(Config* config, QWidget* parent)
  : QWidget(parent)
  , m_undoStack(new QUndoStack(this))
  , m_config(config)
  , m_loaded(false)
{
  m_undoView = new QUndoView(m_undoStack);
  m_undoView->setWindowTitle(tr("Metadata Undo List"));

  initGui();
}

EPubEdit::~EPubEdit()
{
  saveConfig();
}

bool
EPubEdit::loadDocument(const QString& filename)
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TODO drop changes/save document / cancel load maybe multi tabs?
  } else {
    m_document = QSharedPointer<EPubDocument>(new EPubDocument(m_config, this));
    m_document->loadDocument(filename);
    m_metadataForm->setDocument(m_document);
    connect(m_metadataForm,
            &MetadataForm::sendStatusMessage,
            this,
            &EPubEdit::sendStatusMessage);
    m_editor->setDocument(m_document);
    m_loaded = true;
  }

  return m_loaded;
}

bool
EPubEdit::saveDocument(const QString& filename)
{
  if (!m_document.isNull()) {
    m_document->saveDocument(filename);
  }
  return false;
}

bool
EPubEdit::newDocument()
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TOD drop changes/save document / cancel load
  } else {
    m_document = QSharedPointer<EPubDocument>(new EPubDocument(m_config, this));
    m_metadataForm->setDocument(m_document);
    m_loaded = true;
  }
  return m_loaded;
}

bool
EPubEdit::isLoaded() const
{
  return m_loaded;
}

QUndoView*
EPubEdit::undoView()
{
  m_undoView = new QUndoView(m_undoStack);
  m_undoView->setWindowTitle(tr("Metadata Undo List"));
  return m_undoView;
}

QAction*
EPubEdit::undoAction()
{
  return m_undoStack->createUndoAction(this, tr("&Undo"));
}

QAction*
EPubEdit::redoAction()
{
  return m_undoStack->createRedoAction(this, tr("&Redo"));
}

void
EPubEdit::setConfig(Config* config)
{
  m_config = config;
  saveConfig();
}

void
EPubEdit::updateMetadataForm()
{
  //  m_metadataForm->setTitles(m_metadata->orderedTitles());
  //  m_metadataForm->setAuthors(m_metadata->creatorList());
}

void
EPubEdit::metadataHasChanged(MetadataForm::Modifications modifications)
{
  if (modifications.testFlag(MetadataForm::TITLES_CHANGED)) {
    // TODO save titles.
  }
  if (modifications.testFlag(MetadataForm::AUTHORS_CHANGED)) {
    // TODO save authors.
  }
}

void
EPubEdit::initGui()
{
  QGridLayout* layout = new QGridLayout;
  setLayout(layout);

  m_tabs = new QTabWidget(this);
  layout->addWidget(m_tabs, 0, 0);

  auto f = new QFrame(this);
  auto fLayout = new QGridLayout;
  f->setLayout(fLayout);

  m_editor = new EPubEditor(m_config, this);
  m_tabs->addTab(m_editor, tr("EPUB Editor"));

  m_metadataForm = new MetadataForm(m_undoStack, this);
  m_metadataForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_metadataForm,
          &MetadataForm::dataHasChanged,
          this,
          &EPubEdit::metadataHasChanged);
  fLayout->addWidget(m_metadataForm, 0, 0);

  //  fLayout->addWidget(m_undoView, 0, 1);

  m_tabs->addTab(f, tr("Metadata"));

  m_tabs->setCurrentIndex(1);
}

void
EPubEdit::loadConfig(const QString& filename)
{
  m_config->load(filename);
}

void
EPubEdit::saveConfig(const QString& filename)
{
  m_config->save(filename);
}
