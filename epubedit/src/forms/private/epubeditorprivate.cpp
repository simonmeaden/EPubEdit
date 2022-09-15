#include "epubeditorprivate.h"

#include <QDebug>

#include "dockheader.h"
#include "docklistitem.h"
#include "document/authors.h"
#include "document/bookpointers.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "forms/codeedit.h"
#include "forms/epubedit.h"
#include "forms/epubeditor.h"
#include "listbuttonwidget.h"
#include "widgetitem.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"

#include <signalappender.h>

EPubEditorPrivate::EPubEditorPrivate(PConfig config, EPubEditor* parent)
  : AbstractEPubEditorPrivate(config, parent)
{
  Q_Q(EPubEditor);

  auto signalAppender = new SignalAppender();
  signalAppender->setFormat(
    "[%{file}] [%{type:-7}] <%{Function}> Line:%{line} %{message}");
  signalAppender->setDetailsLevel(Logger::Debug);
  cuteLogger->registerAppender(signalAppender);
//  q->connect(signalAppender,
//          &SignalAppender::writeMessage,
//          q,
//          &EPubEditor::sendLogMessage);

  auto img = QImage(":/icons/RemoveSplitLeft");
  auto header = q->header();
  auto widget = header->addIconButton(
    End, QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Remove Split"));
  q->connect(widget, &WidgetItem::widgetClicked, q, &EPubEditor::removeSplit);

  // menu button
  img = QImage(":/icons/Splitscreen");
  widget = header->addIconListButton(
    End, QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Split"));
  auto menuWidget = qobject_cast<ListButtonWidget*>(widget);
  q->connect(menuWidget, &ListButtonWidget::itemClicked, q, &EPubEditor::split);

  img = QImage(":/icons/SplitAboveAndBelow");
  menuWidget->addItem(
    QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Split"));

  img = QImage(":/icons/SplitSideBySide");
  menuWidget->addItem(
    QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Split Side by Side"));

  menuWidget->addItem(q_ptr->tr("Open in New Window"));

  img = QImage(":/icons/YellowArrowLeft");
  widget = header->addIconButton(
    Start, QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Go Back"));
  q->connect(widget, &WidgetItem::widgetClicked, q, &EPubEditor::goBack);

  img = QImage(":/icons/YellowArrowRight");
  widget = header->addIconButton(
    Start, QIcon(QPixmap::fromImage(img)), img.size(), q->tr("Go Forward"));
  q->connect(widget, &WidgetItem::widgetClicked, q, &EPubEditor::goForward);

  img = QImage(":/icons/Drag");
  widget =
    header->addIconButton(Start,
                          QIcon(QPixmap::fromImage(img)),
                          img.size(),
                          q->tr("Drag to drag documents between splits"));
  q->connect(widget, &WidgetItem::widgetClicked, q, &EPubEditor::dragFile);

  auto listWidget = header->addListWidget(Start);
  m_listWidget = qobject_cast<ListWidget*>(listWidget);
  m_listWidget->addItem(q->tr("<no document>"));
  q->connect(
    m_listWidget, &ListWidget::itemClicked, q, &EPubEditor::fileListClicked);

  auto typeWidget = header->addListWidget(
    Start, q->tr("Change editor type (Text/Code)"));
  m_typeWidget = qobject_cast<ListWidget*>(typeWidget);
  q->connect(
    m_typeWidget, &ListWidget::itemClicked, q, &EPubEditor::typeChanged);
  m_typeWidget->addItem("Text");
  m_typeWidget->addItem("Code");

  auto e = new EPubEdit(m_config, q_ptr);
  m_widget = e;
  q->connect(e, &EPubEdit::mouseClicked, q, &EPubEditor::widgetWasClicked);
  q->connect(e, &EPubEdit::lostFocus, q, &EPubEditor::hasLostFocus);
  q->connect(e, &EPubEdit::gotFocus, q, &EPubEditor::hasGotFocus);
  auto count = m_layout->count();
  LOG_DEBUG() << "EPubEditorPrivate::EPubEditorPrivate m_widget " << m_widget << " count before change = " << count << "\non m_layout " << m_layout;
  q->setWidget(e);
  count = m_layout->count();
  LOG_DEBUG() << "EPubEditorPrivate::EPubEditorPrivate m_widget " << m_widget << " count after change = " << count;
}

QTextCursor
EPubEditorPrivate::currentCursor()
{
  auto e = qobject_cast<EPubEdit*>(m_widget);
  if (e)
    return e->textCursor();
  else {
    auto c = qobject_cast<CodeEdit*>(m_widget);
    if (c) {
      return c->textCursor();
    }
  }
  return QTextCursor();
}

QSize
EPubEditorPrivate::minimumSize() const
{
  return QSize(200, 200);
}

//void EPubEditorPrivate::loadHref(const QString &href)
//{

//}

void
EPubEditorPrivate::setDocument(PDocument document)
{
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
    qDebug() << q_ptr->tr("Failed to open %1").arg(filename);
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
      auto textDocument = dynamic_cast<IEPubEditor*>(m_widget)->document();
      textDocument->addResource(
        QTextDocument::ImageResource, QUrl(href), QVariant(image));
    }
  }
  delete archive;
}

void
EPubEditorPrivate::removeSplit()
{
  // TODO remove split
  //  LOG_DEBUG() << "Remove split";
}

void
EPubEditorPrivate::goForward()
{
  // TODO go forward
  //  LOG_DEBUG() << QString("Go forward
  //  %1").arg(quint64(this)).toLatin1().data();
}

void
EPubEditorPrivate::goBack()
{
  // TODO go back
  //  LOG_DEBUG() << QString("Go back %1").arg(quint64(this)).toLatin1().data();
}

void
EPubEditorPrivate::dragFile()
{
  // TODO drag file
  //  LOG_DEBUG() << "Drag file";
}

QPoint
EPubEditorPrivate::adjustWidgetPositionInsideWidget(QPoint originalPos,
                                                    QSize requiredSize)
{
  auto parentRect = q_ptr->rect();
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
EPubEditorPrivate::menuClicked(int index, const QString& /*text*/)
{
  Q_Q(EPubEditor);
  switch (index) {
    case 0: {
      emit q->splitWidget(Qt::Vertical, createAndCloneEditor());
      break;
    }
    case 1: {
      emit q->splitWidget(Qt::Horizontal, createAndCloneEditor());
      break;
    }
    case 2: {
      emit q->splitToWindow(createAndCloneEditor());
      break;
    }
  }
}

EPubEditor*
EPubEditorPrivate::createAndCloneEditor()
{
  auto editor =
    new EPubEditor(m_config, qobject_cast<QWidget*>(q_ptr->parent()));
  clone(editor);
  return editor;
}

void
EPubEditorPrivate::verticalSplit()
{
  Q_Q(EPubEditor);
  emit q->splitWidget(Qt::Vertical, createAndCloneEditor());
}

void
EPubEditorPrivate::horizontalSplit()
{
  Q_Q(EPubEditor);
  emit q->splitWidget(Qt::Horizontal, createAndCloneEditor());
}

void
EPubEditorPrivate::toWindow()
{
  Q_Q(EPubEditor);
  emit q->splitToWindow(createAndCloneEditor());
}

void
EPubEditorPrivate::fileListClicked(int index, const QString& href)
{
  // TODO handle file list
  loadHref(href);
}

void
EPubEditorPrivate::setHtml(const QString& html)
{
  dynamic_cast<IEPubEditor*>(m_widget)->setText(html);
}

void
EPubEditorPrivate::loadConfig()
{
  m_options->load(m_config->optionsFile());
  m_seriesDB->load(m_config->seriesFilename());
  m_authorsDB->load(m_config->authorsFilename());
  m_libraryDB->load(m_config->libraryFilename());
}

AbstractDockWidget*
EPubEditorPrivate::clone(AbstractDockWidget* editor)
{
//  Q_Q(EPubEditor);
  auto e = dynamic_cast<EPubEditor*>(editor);

  if (e) {
    e->m_widget = m_widget;
    e->m_options = m_options;
    e->m_seriesDB = m_seriesDB;
    e->m_authorsDB = m_authorsDB;
    e->m_libraryDB = m_libraryDB;
    e->setDocument(m_document);
    e->loadHref(m_href);
  }
  return editor;
}

void
EPubEditorPrivate::saveConfig()
{
  m_config->save();
  m_options->save(m_config->optionsFile());
  m_seriesDB->save(m_config->seriesFilename());
  m_authorsDB->save(m_config->authorsFilename());
  m_libraryDB->save(m_config->libraryFilename());
}
