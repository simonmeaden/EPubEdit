#include "forms/epubeditor.h"

#include <yaml-cpp/yaml.h>

#include <QMetaMethod>
#include <QMetaObject>

#include "docker/abstractdockitem.h"
#include "docker/buttonwidget.h"
#include "config.h"
#include "docker/dockheader.h"
#include "docker/docklistitem.h"
#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "document/sharedbookdata.h"
#include "widgets/codeedit.h"
#include "forms/epubcontents.h"
#include "widgets/epubedit.h"
#include "forms/epubeditor.h"
#include "widgets/imageedit.h"
#include "docker/headerwidget.h"
#include "docker/listbuttonwidget.h"
#include "docker/listwidget.h"
#include "qyamlcpp.h"
#include "docker/widgetitem.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include <signalappender.h>

EPubEditor::EPubEditor(PConfig config, QWidget* parent)
  : AbstractEPubEditor(config, parent)
  , m_options(POptions(new EBookOptions()))
  , m_libraryDB(PLibraryDB(new EBookLibraryDB(m_options)))
  , m_seriesDB(PSeriesDB(new EBookSeriesDB(m_options)))
  , m_authorsDB(PAuthorsDB(new EBookAuthorsDB()))
{
  loadConfig();

  auto f = font();
  f.setPointSize(9);
  setFont(f);

  auto signalAppender = new SignalAppender();
  signalAppender->setFormat(
    "[%{file}] [%{type:-7}] <%{Function}> Line:%{line} %{message}");
  signalAppender->setDetailsLevel(Logger::Debug);
  cuteLogger->registerAppender(signalAppender);
  connect(signalAppender,
          &SignalAppender::writeMessage,
          this,
          &EPubEditor::sendLogMessage);

  setupToolbar();

  // TODO change to user defined or from config
  setCurrentEditor(IEPubEditor::Code);
}

void EPubEditor::setupToolbar()
{
  auto img = QImage(":/icons/RemoveSplitLeft");
  auto header = dynamic_cast<DockHeader*>(this->header());
  auto widget = header->addIconButton(
    End, QIcon(QPixmap::fromImage(img)), img.size(), tr("Remove Split"));
  connect(widget, &WidgetItem::widgetClicked, this, &EPubEditor::removeSplit);

  // menu button
  img = QImage(":/icons/Splitscreen");
  widget = header->addIconListButton(
    End, QIcon(QPixmap::fromImage(img)), img.size(), tr("Split"));
  auto menuWidget = qobject_cast<ListButtonWidget*>(widget);
  connect(menuWidget, &ListButtonWidget::itemClicked, this, &EPubEditor::split);

  img = QImage(":/icons/SplitAboveAndBelow");
  menuWidget->addItem(QIcon(QPixmap::fromImage(img)), img.size(), tr("Split"));

  img = QImage(":/icons/SplitSideBySide");
  menuWidget->addItem(
    QIcon(QPixmap::fromImage(img)), img.size(), tr("Split Side by Side"));

  menuWidget->addItem(tr("Open in New Window"));

  img = QImage(":/icons/YellowArrowLeft");
  widget = header->addIconButton(
    Start, QIcon(QPixmap::fromImage(img)), img.size(), tr("Go Back"));
  connect(widget, &WidgetItem::widgetClicked, this, &EPubEditor::goBack);

  img = QImage(":/icons/YellowArrowRight");
  widget = header->addIconButton(
    Start, QIcon(QPixmap::fromImage(img)), img.size(), tr("Go Forward"));
  connect(widget, &WidgetItem::widgetClicked, this, &EPubEditor::goForward);

  img = QImage(":/icons/Drag");
  widget = header->addIconButton(Start,
                                 QIcon(QPixmap::fromImage(img)),
                                 img.size(),
                                 tr("Drag to drag documents between splits"));
  connect(widget, &WidgetItem::widgetClicked, this, &EPubEditor::dragFile);

  auto listWidget = header->addListWidget(Start);
  m_listWidget = qobject_cast<ListWidget*>(listWidget);
  m_listWidget->addItem(tr("<no document>"));
  connect(
    m_listWidget, &ListWidget::itemClicked, this, &EPubEditor::fileListClicked);

  auto typeWidget =
    header->addListWidget(Start, tr("Change editor type (Text/Code)"));
  m_typeWidget = qobject_cast<ListWidget*>(typeWidget);
  connect(
    m_typeWidget, &ListWidget::itemClicked, this, &EPubEditor::typeChanged);
  m_typeWidget->addItem("Text");
  m_typeWidget->addItem("Code");
}


EPubEditor::~EPubEditor() {}

void
EPubEditor::loadHref(const QString& href)
{
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    auto w = widget();
    if (w) {
      auto e = qobject_cast<EPubEdit*>(w);
      if (e) { // is a text editor
        e->setHtml(text);
        m_href = href;
      } else { // must be a code editor
        auto c = qobject_cast<CodeEdit*>(w);
        if (c) {
          c->setPlainText(text);
        }
      }
    }
  }
}

QSize
EPubEditor::minimumSize()
{
  return QSize(200, 200);
}

void
EPubEditor::setCurrentEditor(IEPubEditor::Type type)
{
  switch (type) {
    case IEPubEditor::Text: {
      if (!m_epubEdit) {
        m_epubEdit = new EPubEdit(m_config, this);
        m_epubEdit->loadHref(m_href);
        connect(m_epubEdit,
                &EPubEdit::mouseClicked,
                this,
                &EPubEditor::widgetWasClicked);
        connect(
          m_epubEdit, &EPubEdit::lostFocus, this, &EPubEditor::hasLostFocus);
        connect(
          m_epubEdit, &EPubEdit::gotFocus, this, &EPubEditor::hasGotFocus);
      }
      m_currentEditor = m_epubEdit;
      setWidget(m_epubEdit);
      break;
    }
    case IEPubEditor::Code: {
      if (!m_codeedit) {
        m_codeedit = new CodeEdit(m_config, this);
        m_codeedit->loadHref(m_href);
        connect(m_codeedit,
                &CodeEdit::mouseClicked,
                this,
                &EPubEditor::widgetWasClicked);
        connect(
          m_codeedit, &CodeEdit::lostFocus, this, &EPubEditor::hasLostFocus);
        connect(
          m_codeedit, &CodeEdit::gotFocus, this, &EPubEditor::hasGotFocus);
      }
      m_currentEditor = m_codeedit;
      setWidget(m_codeedit);
      break;
    }
    case IEPubEditor::Image: {
      if (!m_imgEdit) {
        auto m_imgEdit = new ImageEdit(m_config, this);
        connect(m_imgEdit,
                &ImageEdit::mouseClicked,
                this,
                &EPubEditor::widgetWasClicked);
        connect(
          m_imgEdit, &ImageEdit::lostFocus, this, &EPubEditor::hasLostFocus);
        connect(
          m_imgEdit, &ImageEdit::gotFocus, this, &EPubEditor::hasGotFocus);
      }
      m_currentEditor = m_imgEdit;
      setWidget(m_imgEdit);
      break;
    }
  }
}

void
EPubEditor::setDocument(PDocument document)
{
  auto w = widget();
  if (w) {
    m_document = document;
    auto manifest = m_document->manifest();
    auto images = manifest->images;
    //  auto svgImages = manifest->svgImages;
    // TODO handle svg files.
    m_docFileList.clear();
    auto spine = m_document->spine();
    for (auto& uniqueId : spine->orderedItems) {
      auto spineItem = spine->items.value(uniqueId);
      auto idref = spineItem->idref;
      auto manifestItem = manifest->itemsById.value(idref);
      auto href = manifestItem->href;
      m_docFileList.append(href);
    }
    if (m_listWidget) {
      m_listWidget->setItems(m_docFileList);
    }

    auto filename = m_config->currentFilename();
    auto archive = new QuaZip(filename);
    if (!archive->open(QuaZip::mdUnzip)) {
      qDebug() << tr("Failed to open %1").arg(filename);
    }

    // load images into the text document.
    for (auto& href : images) {
      archive->setCurrentFile(href);
      QuaZipFile imageFile(archive);
      imageFile.setZip(archive);

      if (imageFile.open(QIODevice::ReadOnly)) {
        auto data = imageFile.readAll();
        //      m_images.append(data);
        auto image = QImage::fromData(data);
        auto textDocument = dynamic_cast<IEPubEditor*>(w)->document();
        textDocument->addResource(
          QTextDocument::ImageResource, QUrl(href), QVariant(image));
      }
    }
    delete archive;
  }
}

PDocument
EPubEditor::document()
{
  return m_document;
}

void
EPubEditor::removeSplit(QPoint)
{
  // TODO remove split
  LOG_DEBUG() << "Remove split";
}

void
EPubEditor::goForward()
{
  // TODO go forward
  LOG_DEBUG() << QString("Go forward %1").arg(quint64(this)).toLatin1().data();
}

void
EPubEditor::goBack()
{
  // TODO go back
  LOG_DEBUG() << QString("Go back %1").arg(quint64(this)).toLatin1().data();
}

void
EPubEditor::dragFile()
{
  // TODO drag file
  LOG_DEBUG() << "Drag file";
}

void
EPubEditor::hasGotFocus(QWidget* /*widget*/)
{
  emit gotFocus(this);
}

void
EPubEditor::hasLostFocus(QWidget* /*widget*/)
{
  emit lostFocus(this);
}

void
EPubEditor::menuClicked(int index, const QString& /*text*/)
{
  switch (index) {
    case 0: {
      emit splitWidget(Qt::Vertical, createAndCloneEditor());
      break;
    }
    case 1: {
      emit splitWidget(Qt::Horizontal, createAndCloneEditor());
      break;
    }
    case 2: {
      emit splitToWindow(createAndCloneEditor());
      break;
    }
  }
}

EPubEditor*
EPubEditor::createAndCloneEditor()
{
  return clone(m_currentEditor);
}

void
EPubEditor::split(int index, const QString& /*text*/)
{
  switch (index) {
    case 0:
      verticalSplit();
      break;
    case 1:
      horizontalSplit();
      break;
    case 2:
      toWindow();
      break;
    default:
      break;
  }
}

void
EPubEditor::verticalSplit()
{
  emit splitWidget(Qt::Vertical, createAndCloneEditor());
}

void
EPubEditor::horizontalSplit()
{
  emit splitWidget(Qt::Horizontal, createAndCloneEditor());
}

void
EPubEditor::toWindow()
{
  emit splitToWindow(createAndCloneEditor());
}

void
EPubEditor::fileListClicked(int index, const QString& href)
{
  // TODO handle file list
  loadHref(href);
  m_listWidget->setText(href);
  LOG_DEBUG() << QString("File list clicked [%1] %2 : %3")
                   .arg(index)
                   .arg(quint64(this))
                   .arg(href)
                   .toLatin1()
                   .data();
}

void
EPubEditor::typeChanged(int index, const QString& text)
{
  switch (index) {
    case 0: {
      if (m_currentEditor && m_currentEditor != m_epubEdit) // type unchanged.
        return;
      setCurrentEditor(IEPubEditor::Text);
      m_currentEditor->setCurrentCursor(m_epubEdit->currentCursor());
      m_typeWidget->setText(text);
      break;
    }
    case 1: {
      if (m_currentEditor && m_currentEditor == m_codeedit) // type unchanged.
        return;
      setCurrentEditor(IEPubEditor::Code);
      m_codeedit->setCurrentCursor(m_codeedit->currentCursor());
      m_typeWidget->setText(text);
      break;
    }
    default:
      return;
  }
}

bool
EPubEditor::isLoaded()
{
  return m_loaded;
}

// void
// EPubEditor::setHtml(const QString& html)
//{
//   dynamic_cast<IEPubEditor*>(widget())->setText(html);
// }

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

EPubEditor*
EPubEditor::clone(IEPubEditor* editor)
{
  //  auto editor = new EPubEditor(m_config, qobject_cast<QWidget*>(parent()));
  auto h = dynamic_cast<EPubEditor*>(editor);
  if (h) {
    HeaderWidget::clone(h);
    h->m_options = m_options;
    h->m_seriesDB = m_seriesDB;
    h->m_authorsDB = m_authorsDB;
    h->m_libraryDB = m_libraryDB;
    h->setDocument(m_document);
    h->loadHref(m_href);
  }
  setWidget(h);
  return h;
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
