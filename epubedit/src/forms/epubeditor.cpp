#include "forms/epubeditor.h"

#include <yaml-cpp/yaml.h>

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include "config.h"
#include "dockitem.h"
#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "document/sharedbookdata.h"
#include "forms/epubcontents.h"
#include "forms/epubeditor.h"
#include "headerwidget.h"
#include "qyamlcpp.h"
#include "widgetitem.h"

EPubEditor::EPubEditor(QWidget* parent)
  : HeaderWidget(parent)
{}

EPubEditor::EPubEditor(PConfig config, QWidget* parent)
  : HeaderWidget(parent)
  , m_config(config)
  , m_options(POptions(new EBookOptions()))
  , m_libraryDB(PLibraryDB(new EBookLibraryDB(m_options)))
  , m_seriesDB(PSeriesDB(new EBookSeriesDB(m_options)))
  , m_authorsDB(PAuthorsDB(new EBookAuthorsDB()))

{
  setContentsMargins(0, 0, 0, 0);

  auto img = QImage(":/icons/RemoveSplitLeft");
  auto widget =
    m_header->addIconButton(End, QIcon(QPixmap::fromImage(img)), img.size());
  connect(widget, &WidgetItem::widgetClicked, this, &EPubEditor::removeSplit);

  img = QImage(":/icons/Splitscreen");
  widget =
    m_header->addIconButton(End, QIcon(QPixmap::fromImage(img)), img.size());
  connect(
    widget, &WidgetItem::widgetClicked, this, &EPubEditor::splitMenuClicked);

  m_editor = new QTextEdit(this);
  setCentralWidget(m_editor);

  loadConfig();
}

EPubEditor::EPubEditor(const EPubEditor& other)
  : HeaderWidget(qobject_cast<QWidget*>(other.parent()))
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
EPubEditor::removeSplit()
{}

QPoint
EPubEditor::adjustWidgetPositionInsideWidget(QPoint originalPos,
                                             QSize requiredSize)
{
  auto parentRect = rect();
  QRect childRect(originalPos, requiredSize);
  int adjust = 0;
  if (childRect.right() > parentRect.right()) {
    adjust = parentRect.right() - childRect.right();
  } else if (childRect.left() < parentRect.left()) {
    adjust = childRect.left() - parentRect.left();
  }
  QPoint childPos(originalPos.x() + adjust, originalPos.y());
  return childPos;
}

void
EPubEditor::splitMenuClicked(QPoint pos)
{
  QMenu* menu = new QMenu(this);
  auto img = QImage(":/icons/SplitAboveAndBelow");
  auto action = new QAction(QIcon(QPixmap::fromImage(img)), tr("Split"));
  action->setData(1);
  menu->addAction(action);
  img = QImage(":/icons/SplitSideBySide");
  action =
    new QAction(QIcon(QPixmap::fromImage(img)), tr("Split Side" /* by Side"*/));
  action->setData(2);
  menu->addAction(action);
  action = new QAction(tr("Open in New Window"));
  action->setData(3);
  menu->addAction(action);
  connect(menu, &QMenu::triggered, this, &EPubEditor::menuClicked);

  auto menuPos = adjustWidgetPositionInsideWidget(pos, menu->sizeHint());
  menu->popup(mapToGlobal(menuPos));
}

void
EPubEditor::menuClicked(QAction* action)
{
  auto index = action->data().toInt();
  switch (index) {
    case 1:
      emit splitWidget(Qt::Horizontal);
      break;
    case 2:
      emit splitWidget(Qt::Vertical);
      break;
    case 3:
      emit splitToWindow();
      break;
  }
}

void
EPubEditor::loadFileIntoTextDocument(const QString& zipfile,
                                     const QString& imageName)
{
  auto fileName = JlCompress::extractFile(zipfile, imageName);
  QFile file(fileName);
  QTextStream out(&file);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    m_editor->setHtml(text);
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

void
EPubEditor::setHtml(const QString& html)
{
  m_editor->setHtml(html);
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
