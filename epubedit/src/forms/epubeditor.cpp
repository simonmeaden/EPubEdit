#include "forms/epubeditor.h"

#include <yaml-cpp/yaml.h>

#include <QMetaMethod>
#include <QMetaObject>

#include "abstractdockitem.h"
#include "buttonwidget.h"
#include "config.h"
#include "docklistitem.h"
#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/library.h"
#include "document/options.h"
#include "document/series.h"
#include "document/sharedbookdata.h"
#include "forms/epubcontents.h"
#include "forms/epubedit.h"
#include "forms/epubeditor.h"
#include "headerwidget.h"
#include "listbuttonwidget.h"
#include "listwidget.h"
#include "private/epubeditorprivate.h"
#include "qyamlcpp.h"
#include "widgetitem.h"

#include <signalappender.h>

// EPubEditor::EPubEditor(QWidget *parent)
//   : AbstractEPubEditor(parent)
//{
// }

EPubEditor::EPubEditor(PConfig config, QWidget* parent)
  : AbstractEPubEditor(config, parent)
  , d_ptr(new EPubEditorPrivate(config, this))
{
  Q_D(EPubEditor);
  d->m_config = config;
  d->m_options = POptions(new EBookOptions());
  d->m_libraryDB = PLibraryDB(new EBookLibraryDB(d->m_options));
  d->m_seriesDB = PSeriesDB(new EBookSeriesDB(d->m_options));
  d->m_authorsDB = PAuthorsDB(new EBookAuthorsDB());

  loadConfig();

  auto f = font();
  f.setPointSize(8);
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
}

EPubEditor::EPubEditor(EPubEditorPrivate& d)
  : d_ptr(&d)
{
}

EPubEditor::~EPubEditor() {}

void
EPubEditor::loadHref(const QString& href)
{
  Q_D(EPubEditor);
  d->loadHref(href);
}

QSize
EPubEditor::minimumSize()
{
  Q_D(EPubEditor);
  return d->minimumSize();
}

void
EPubEditor::setDocument(PDocument document)
{
  Q_D(EPubEditor);
  d->setDocument(document);
}

PDocument
EPubEditor::document()
{
  Q_D(EPubEditor);
  return d->m_document;
}

void
EPubEditor::removeSplit(QPoint)
{
  Q_D(EPubEditor);
  // TODO remove split
  LOG_DEBUG() << "Remove split";

  d->removeSplit();
}

void
EPubEditor::goForward()
{
  Q_D(EPubEditor);
  // TODO go forward
  LOG_DEBUG() << QString("Go forward %1").arg(quint64(this)).toLatin1().data();

  d->goForward();
}

void
EPubEditor::goBack()
{
  Q_D(EPubEditor);
  // TODO go back
  LOG_DEBUG() << QString("Go back %1").arg(quint64(this)).toLatin1().data();

  d->goBack();
}

void
EPubEditor::dragFile()
{
  Q_D(EPubEditor);
  // TODO drag file
  LOG_DEBUG() << "Drag file";

  d->dragFile();
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
EPubEditor::menuClicked(int index, const QString& text)
{
  Q_D(EPubEditor);
  d->menuClicked(index, text);
}

void
EPubEditor::verticalSplit()
{
  Q_D(EPubEditor);
  d->verticalSplit();
}

void
EPubEditor::horizontalSplit()
{
  Q_D(EPubEditor);
  d->horizontalSplit();
}

void
EPubEditor::toWindow()
{
  Q_D(EPubEditor);
  d->toWindow();
}

void
EPubEditor::fileListClicked(int index, const QString& href)
{
  Q_D(EPubEditor);
  // TODO handle file list
  d->loadHref(href);
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
  // TODO change between editor types.

}

bool
EPubEditor::isLoaded()
{
  Q_D(EPubEditor);
  return d->m_loaded;
}

void
EPubEditor::setHtml(const QString& html)
{
  Q_D(EPubEditor);
  d->setHtml(html);
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
  Q_D(EPubEditor);
  d->loadConfig();
}

EPubEditor*
EPubEditor::clone()
{
  Q_D(EPubEditor);
  auto editor = new EPubEditor(d->m_config, qobject_cast<QWidget*>(parent()));
  d_ptr->clone(editor);
  HeaderWidget::clone(editor);
  d->clone(editor);
  setWidget(editor);
  return editor;
}

void
EPubEditor::saveConfig()
{
  Q_D(EPubEditor);
  d->saveConfig();
}
