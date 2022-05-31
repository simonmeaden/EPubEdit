#include "forms/epubeditor.h"

#include <yaml-cpp/yaml.h>

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include "config.h"
#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/series.h"
#include "document/sharedbookdata.h"
#include "forms/epubcontents.h"
#include "forms/epubeditor.h"
#include "qyamlcpp.h"

EPubEditor::EPubEditor(QWidget* parent)
  : QTextEdit(parent)
{}

EPubEditor::EPubEditor(PConfig config, QWidget* parent)
  : QTextEdit(parent)
  , m_config(config)
  , m_options(POptions(new EBookOptions()))
  , m_libraryDB(PLibraryDB(new EBookLibraryDB(m_options)))
  , m_seriesDB(PSeriesDB(new EBookSeriesDB(m_options)))
  , m_authorsDB(PAuthorsDB(new EBookAuthorsDB()))

{
  setFrameStyle(QFrame::NoFrame);
  setContentsMargins(0, 0, 0, 0);

  loadConfig();
}

EPubEditor::EPubEditor(const EPubEditor& other)
  : QTextEdit(qobject_cast<QWidget*>(other.parent()))
{
  setEpubDocument(other.m_document);
}

EPubEditor::~EPubEditor() {}

void
EPubEditor::setEpubDocument(PDocument document)
{
  m_document = document;
  updateDocument();
}

PDocument
EPubEditor::epubDocument()
{
  return m_document;
}

void
EPubEditor::updateDocument()
{}

void
EPubEditor::loadFileIntoTextDocument(const QString& zipfile,
                                     const QString& imageName)
{
  auto fileName = JlCompress::extractFile(zipfile, imageName);
  QFile file(fileName);
  QTextStream in(&file);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    setHtml(text);
  }
}

bool
EPubEditor::loadDocument(const QString& filename)
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TODO drop changes/save document / cancel load maybe multi tabs?
  } else {
    QString href;
    m_document = PDocument(new EPubDocument(
      m_config, m_options, m_libraryDB, m_seriesDB, m_authorsDB, this));
    PBookData data = m_document->loadDocument(filename);
    if (data.isNull()) {
      // TODO error no book data.
    } else {

      auto manifest = m_document->manifest();
      auto spine = m_document->spine();
      m_pages.clear();
      m_pageOrder.clear();
      for (auto& itemName : spine.orderedItems) {
        auto spineItem = spine.items.value(itemName);
        if (spineItem->linear) {
          auto idref = spineItem->idref;
          auto manifestItem = manifest->itemsById.value(idref);
          href = manifestItem->href;
          auto mediatype = manifestItem->mediaType;
          Page page;
          page.mediaType = mediatype;
          page.path = href;
          m_pageOrder.append(idref);
          m_pages.insert(idref, page);
        }
      }

      // load images for book
      // these are stored into the text document.
      QStringList fileList;
      m_currentBookFilename = data->filename();
      auto imageFileKeys = manifest->images.keys();
      for (auto& name : imageFileKeys) {
        fileList.append(name.toString());
      }
      fileList = JlCompress::extractFiles(m_currentBookFilename, fileList);

      QMap<QString, QImage> images;
      for (auto& imageName : fileList) {
        QImageReader reader(imageName);
        auto image = reader.read();
        images.insert(imageName, image);
      }
      auto textDocument = document();
      auto imageKeys = images.keys();
      for (auto& imageName : imageKeys) {
        textDocument->addResource(QTextDocument::ImageResource,
                                  QUrl(imageName),
                                  QVariant(images.value(imageName)));
      }

      if (!m_pages.isEmpty()) {
        m_page = m_pages.value(m_pageOrder.at(1));
        href = m_page.path;

        loadFileIntoTextDocument(m_currentBookFilename, href);
      }

      bool showContentsList = false;
      if (!data.isNull()) {
        showContentsList = data->showContentsList();
      }

      if (showContentsList) {
        // TODO work out contents / TOC
      }
      m_loaded = true;
    }
  }

  return m_loaded;
}

bool
EPubEditor::saveDocument(const QString& filename)
{
  if (!m_document.isNull()) {
    m_document->saveDocument(filename);
  }
  return false;
}

bool
EPubEditor::newDocument()
{
  if (m_loaded) {
    //    QMessageBox::warning(this, )
    // TOD drop changes/save document / cancel load
  } else {
    m_document = PDocument(new EPubDocument(
      m_config, m_options, m_libraryDB, m_seriesDB, m_authorsDB, this));
    //    m_metadataForm->setDocument(m_document);
    m_loaded = true;
  }
  return m_loaded;
}

bool
EPubEditor::isLoaded() const
{
  return m_loaded;
}

// QUndoView*
// EPubEditor::undoView()
//{
//   return m_undoView;
// }

// QAction*
// EPubEditor::undoAction()
//{
//   return m_undoStack->createUndoAction(this, tr("&Undo"));
// }

// QAction*
// EPubEditor::redoAction()
//{
//   return m_undoStack->createRedoAction(this, tr("&Redo"));
// }

void
EPubEditor::updateMetadataForm()
{
  //  m_metadataForm->setTitles(m_metadata->orderedTitles());
  //  m_metadataForm->setAuthors(m_metadata->creatorList());
}

void
EPubEditor::metadataHasChanged(MetadataForm::Modifications modifications)
{
  if (modifications.testFlag(MetadataForm::TITLES_CHANGED)) {
    // TODO save titles.
  }
  if (modifications.testFlag(MetadataForm::AUTHORS_CHANGED)) {
    // TODO save authors.
  }
}

void
EPubEditor::splitterHasMoved(int /*pos*/, int /*index*/)
{
  // TODO handle splitter
  //  m_config->setHSplitterSizes(m_splitter->sizes());
  //  m_config->save();
}

// void
// EPubEditor::toggleOpenClicked()
//{
//   if (m_contentsFrame->isVisible())
//     m_contentsFrame->setVisible(false);
//   else
//     m_contentsFrame->setVisible(true);
// }

// void
// EPubEditor::initGui()
//{
//   auto layout = new QGridLayout();
//   layout->setContentsMargins(0, 0, 0, 0);
//   setLayout(layout);

//  QPalette p;
//  p.setColor(QPalette::Window, QColor("red"));
//  setPalette(p);

//  m_header = new HeaderWidget(this);
//  layout->addWidget(m_header, BorderLayout::North);

//  QImage img(":/icons/hideH");
//  m_header->addIconButton(
//    WidgetPosition::Left, img, tr("Toggle visibility of first"));

//  img = QImage(":/icons/hideV");
//  m_header->addIconButton(
//    WidgetPosition::Left, img, tr("Toggle visibility of second"));

//  img = QImage(":/icons/hideV");
//  m_header->addIconButton(
//    WidgetPosition::Right, img, tr("Toggle visibility of second"));
//  img = QImage(":/icons/hideH");
//  m_header->addIconButton(
//    WidgetPosition::Right, img, tr("Toggle visibility of first"));

//  m_splitter = new QSplitter(this);
//  m_splitter->setContentsMargins(0, 0, 0, 0);
//  connect(
//    m_splitter, &QSplitter::splitterMoved, this,
//    &EPubEditor::splitterHasMoved);
//  layout->addWidget(m_splitter);

//  m_editor = new EPubEditor(m_config);
//  m_editor->setContentsMargins(0, 0, 0, 0);
//  m_splitter->addWidget(m_editor);

//  m_contentsFrame = new ContentsFrame(this);
//  m_contentsFrame->setToolTip("BLOODY CONTENTS FORM!!!");
//  m_splitter->addWidget(m_contentsFrame);

//  m_splitter->setSizes(m_config->hSplitterSizes());

//  m_editor = new EPubEditor(m_config);
//  m_editor->setContentsMargins(0, 0, 0, 0);
//  m_editor->hide();
//  m_splitter->addWidget(m_editor);

//  m_metadataForm = new MetadataForm(this);
//  m_metadataForm->setSizePolicy(QSizePolicy::Expanding,
//  QSizePolicy::Expanding); connect(m_metadataForm,
//          &MetadataForm::dataHasChanged,
//          this,
//          &EPubEditor::metadataHasChanged);
//  m_splitter->addWidget(m_metadataForm);
//}

void
EPubEditor::loadConfig()
{
  m_options->load(m_config->optionsFile());
  m_seriesDB->load(m_config->seriesFilename());
  m_authorsDB->load(m_config->authorsFilename());
  m_libraryDB->load(m_config->libraryFilename());
}

void
EPubEditor::saveConfig()
{
  m_config->save();
  m_options->save(m_config->optionsFile());
  m_seriesDB->save(m_config->seriesFilename());
  m_authorsDB->save(m_config->authorsFilename());
  m_libraryDB->save(m_config->libraryFilename());
}
