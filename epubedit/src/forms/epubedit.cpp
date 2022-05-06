#include "forms/epubedit.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include "qyamlcpp.h"
#include <yaml-cpp/yaml.h>

#include "document/epubdocument.h"
#include "document/sharedbookdata.h"
#include "forms/epubcontents.h"
#include "forms/epubeditor.h"
#include "forms/metadataform.h"
#include "forms/footerwidget.h"

EPubEdit::EPubEdit(PConfig config, QWidget* parent)
  : QWidget(parent)
  , m_config(config)
  , m_options(POptions(new EBookOptions()))
  , m_libraryDB(PLibraryDB(new EBookLibraryDB(m_options)))
  , m_seriesDB(PSeriesDB(new EBookSeriesDB(m_options)))
  , m_authorsDB(PAuthorsDB(new EBookAuthorsDB()))
  , m_undoStack(new QUndoStack(this))
  , m_loaded(false)
{
  m_undoView = new QUndoView(m_undoStack);
  m_undoView->setWindowTitle(tr("Metadata Undo List"));

  loadConfig();
  initGui();
}

EPubEdit::~EPubEdit() {}

void
EPubEdit::loadFileIntoTextDocument(const QString& zipfile,
                                   const QString& imageName)
{
  auto fileName = JlCompress::extractFile(zipfile, imageName);
  QFile file(fileName);
  QTextStream in(&file);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    m_editor->setHtml(text);
  }
}

bool
EPubEdit::loadDocument(const QString& filename)
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
      auto textDocument = m_editor->document();
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

      m_metadataForm->setDocument(m_document);
      connect(m_metadataForm,
              &MetadataForm::sendStatusMessage,
              this,
              &EPubEdit::sendStatusMessage);
      m_editor->setDocument(m_document);
      m_loaded = true;
    }
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
    m_document = PDocument(new EPubDocument(
      m_config, m_options, m_libraryDB, m_seriesDB, m_authorsDB, this));
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
EPubEdit::splitterHasMoved(int /*pos*/, int /*index*/)
{
  m_config->setHSplitterSizes(m_splitter->sizes());
  m_config->save();
}

void
EPubEdit::toggleOpenClicked()
{
  if (m_contentsFrame->isVisible())
    m_contentsFrame->setVisible(false);
  else
    m_contentsFrame->setVisible(true);
}

void
EPubEdit::initGui()
{
  setContentsMargins(0, 0, 0, 0);

  auto layout = new BorderLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_splitter = new QSplitter(this);
  m_splitter->setContentsMargins(0,0,0,0);
  connect(
    m_splitter, &QSplitter::splitterMoved, this, &EPubEdit::splitterHasMoved);
  layout->addWidget(m_splitter, BorderLayout::Center);

  m_contentsFrame = new ContentsFrame(this);
  m_contentsFrame->setToolTip("BLOODY CONTENTS FORM!!!");
  m_splitter->addWidget(m_contentsFrame);

  m_splitter->setSizes(m_config->hSplitterSizes());

  m_editor = new EPubEditor(m_config);
  m_editor->setContentsMargins(0, 0, 0, 0);
  m_splitter->addWidget(m_editor);

  m_metadataForm = new MetadataForm(m_undoStack, this);
  m_metadataForm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_metadataForm,
          &MetadataForm::dataHasChanged,
          this,
          &EPubEdit::metadataHasChanged);
  m_splitter->addWidget(m_metadataForm);

  m_footer = new FooterWidget(this);
  layout->addWidget(m_footer, BorderLayout::South);
}

void
EPubEdit::loadConfig()
{
  m_options->load(m_config->optionsFile());
  m_seriesDB->load(m_config->seriesFilename());
  m_authorsDB->load(m_config->authorsFilename());
  m_libraryDB->load(m_config->libraryFilename());
}

void
EPubEdit::saveConfig()
{
  m_config->save();
  m_options->save(m_config->optionsFile());
  m_seriesDB->save(m_config->seriesFilename());
  m_authorsDB->save(m_config->authorsFilename());
  m_libraryDB->save(m_config->libraryFilename());
}

////====================================================================
////=== FooterWidget
////====================================================================

//EPubEdit::FooterWidget::FooterWidget(QWidget* parent)
//  : QWidget(parent)
//  , m_parent(static_cast<EPubEdit*>(parent))
//  , m_back(QColorConstants::X11::DimGrey)
//  , m_hoverBack(QColorConstants::X11::DarkGrey)
//  , m_hoverOverButton(false)
//{
//  setFixedHeight(HEIGHT);
//  setMouseTracking(true);
//  setAttribute(Qt::WA_Hover);
//  setContentsMargins(0, 0, 0, 0);
//}

//void
//EPubEdit::FooterWidget::paintEvent(QPaintEvent* event)
//{
//  QWidget::paintEvent(event);
//  QPainter painter(this);

//  painter.fillRect(m_frameRect, m_back);
//  if (!m_hoverOverButton) {
//    painter.fillRect(m_buttonRect, m_back);
//  } else {
//    painter.fillRect(m_buttonRect, m_hoverBack);
//  }

//  QImage img(":/icons/hideshowcontents.png");
//  painter.drawImage(m_buttonRect.x() + 3, 3, img);
//}

//void
//EPubEdit::FooterWidget::resizeEvent(QResizeEvent* event)
//{
//  auto r = rect();
//  m_buttonRect = QRect(0, 0, BUTTON_WIDTH, HEIGHT);
//  m_frameRect = QRect(BUTTON_WIDTH, 0, r.width() - BUTTON_WIDTH, HEIGHT);

//  QWidget::resizeEvent(event);
//}

//void
//EPubEdit::FooterWidget::hoverEnter(QHoverEvent* event)
//{
//  if (m_buttonRect.contains(event->pos())) {
//    m_hoverOverButton = true;
//  } else {
//    m_hoverOverButton = false;
//  }
//  repaint(m_buttonRect);
//}

//void
//EPubEdit::FooterWidget::hoverLeave(QHoverEvent* event)
//{
//  m_hoverOverButton = false;
//  repaint(m_buttonRect);
//}

//void
//EPubEdit::FooterWidget::hoverMove(QHoverEvent* event)
//{
//  if (m_buttonRect.contains(event->pos())) {
//    m_hoverOverButton = true;
//  } else {
//    m_hoverOverButton = false;
//  }
//  repaint(m_buttonRect);
//}

//void
//EPubEdit::FooterWidget::mousePressEvent(QMouseEvent* event)
//{
//  if (m_buttonRect.contains(event->pos())) {
//    emit m_parent->toggleOpenClicked();
//  }
//}

//void
//EPubEdit::FooterWidget::mouseReleaseEvent(QMouseEvent* event)
//{}

//bool
//EPubEdit::FooterWidget::event(QEvent* event)
//{
//  switch (event->type()) {
//    case QEvent::HoverEnter:
//      hoverEnter(static_cast<QHoverEvent*>(event));
//      return true;
//      break;
//    case QEvent::HoverLeave:
//      hoverLeave(static_cast<QHoverEvent*>(event));
//      return true;
//      break;
//    case QEvent::HoverMove:
//      hoverMove(static_cast<QHoverEvent*>(event));
//      return true;
//      break;
//    default:
//      break;
//  }
//  return QWidget::event(event);
//}
