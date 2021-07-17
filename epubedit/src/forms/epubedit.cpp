#include "forms/epubedit.h"

#include "quazip.h"
#include "quazipfile.h"

#include "qyamlcpp.h"
#include <yaml-cpp/yaml.h>

#include "document/epubdocument.h"
#include "forms/epubeditor.h"
#include "forms/metadataform.h"
//#include "util/csvsplitter.h"

const QString EPubEdit::STATUS_TIMEOUT = "status_timeout";

EPubEdit::EPubEdit(Config config, QWidget* parent)
  : QWidget(parent)
  , m_undoStack(new QUndoStack(this))
  , m_config(config)
  , m_loaded(false)
{
  m_undoView = new QUndoView(m_undoStack);
  m_undoView->setWindowTitle(tr("Metadata Undo List"));

  loadConfig();
  initGui();
}

EPubEdit::~EPubEdit() {}

bool
EPubEdit::loadDocument(const QString& filename)
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TOD drop changes/save document / cancel load
  } else {
    m_document = QSharedPointer<EPubDocument>(new EPubDocument());
    m_document->openDocument(filename);
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
EPubEdit::newDocument()
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TOD drop changes/save document / cancel load
  } else {
    m_document = QSharedPointer<EPubDocument>(new EPubDocument());
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
EPubEdit::getUndoView()
{
  m_undoView = new QUndoView(m_undoStack);
  m_undoView->setWindowTitle(tr("Metadata Undo List"));
  return m_undoView;
}

QAction*
EPubEdit::getUndoAction()
{
  return m_undoStack->createUndoAction(this, tr("&Undo"));
}

QAction*
EPubEdit::getRedoAction()
{
  return m_undoStack->createRedoAction(this, tr("&Redo"));
}

void
EPubEdit::setConfig(const Config& config)
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
EPubEdit::metadataHasChanged()
{
  //  if (m_metadataForm->titlesModified()) {
  //    auto titles = m_metadataForm->titles();
  //    m_metadata->setOrderedTitles(titles);
  //  }

  //  if (m_metadataForm->authorsModified()) {
  //    auto authors = m_metadataForm->authors();
  //    m_metadata->setCreatorList(authors);
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

  m_metadataForm = new MetadataForm(m_undoStack, this);
  m_metadataForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  fLayout->addWidget(m_metadataForm, 0, 0);

  fLayout->addWidget(m_undoView, 0, 1);

  m_tabs->addTab(f, tr("Metadata"));

  m_editor = new EPubEditor(this);
  m_tabs->addTab(m_editor, tr("EPUB Editor"));
}

void
EPubEdit::loadConfig(const QString& filename)
{
  QFile* file;
  if (filename.isEmpty()) {
    QDir dir;
    dir.mkpath(m_config.getConfigDir());
    file = new QFile(dir.filePath(m_config.getConfigFile()), this);
  } else {
    file = new QFile(filename, this);
  }

  if (file->exists()) {
    auto config = YAML::LoadFile(*file);
    if (config[STATUS_TIMEOUT]) {
      auto node = config[STATUS_TIMEOUT];
      m_config.setStatusTimeout(node.as<int>());
    }
  } else {
    m_config.setStatusTimeout(20);
  }
}

void
EPubEdit::saveConfig(const QString& filename)
{
  QFile* file;
  if (filename.isEmpty())
    file = new QFile(
      QDir(m_config.getConfigDir()).filePath(m_config.getConfigFile()), this);
  else
    file = new QFile(filename, this);

  if (file->open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;

    emitter << YAML::Comment(tr(
      "EPubEditor Configuration file.\n\n"
      "Care should be taken editing this file manually\n"
      "as the wrong key-value pair could cause problems.\n"
      "The best way is to use the in application configuration editor.\n\n"));
    emitter << YAML::BeginMap;
    if (m_config.getStatusTimeout() > 0) {
      emitter << YAML::Key << STATUS_TIMEOUT << YAML::Value
              << m_config.getStatusTimeout()
              << YAML::Comment(
                   tr("Status display timeout in seconds. int value."));
    }
    emitter << YAML::EndMap;

    if (!emitter.good()) {
      qWarning() << tr("Configuration Emitter error: ")
                 << QString::fromStdString(emitter.GetLastError()) << "\n";
    }

    QTextStream outStream(file);
    outStream << emitter.c_str();
    file->close();
  }
}
