#include "mainwindow.h"
#include "config.h"
#include "document/epubmetadata.h"
#include "forms/configurationeditor.h"
#include "forms/epubedit.h"
//#include "languages.h"
#include "forms/languagetagbuilderdialog.h"

//#define VERSION_2
//#define VERSION_3_0
#define VERSION_3_1
//#define VERSION_3_2
//#define VERSION_3_3

#define TESTDIALOG

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_width(1200)
  , m_height(800)
{
  qRegisterMetaType<Direction>("Direction");

  m_config = new Config(this);
  // cleanup shit
  connect(this, &MainWindow::destroyed, this , &MainWindow::cleanup);
  // Qt::QueuedConnection is recommended here otherwise close might not happen.
  connect(this, &MainWindow::shutdown, this , &MainWindow::close, Qt::QueuedConnection);

#if defined(TESTDIALOG)
  // TEMPORARY TEST STUFF
  auto builder = new LanguageTagBuilderDialog(m_config, this);
  if (builder->exec() == QDialog::Accepted) {
    auto data = builder->tag();
    qDebug() << QString("Tag : ").arg(data);
  }

  emit shutdown(0);

  // TEMPORARY TEST STUFF
#elif
  initGui();

  connect(
    m_config, &Config::sendStatusMessage, this, &MainWindow::setStatusMessage);
  connect(m_config, &Config::sendLogMessage, this, &MainWindow::setLogMessage);
  connect(m_editor,
          &EPubEdit::sendStatusMessage,
          this,
          &MainWindow::setStatusMessage);
  connect(
    m_editor, &EPubEdit::sendLogMessage, this, &MainWindow::setLogMessage);

  const auto sr = qApp->screens().at(0)->availableGeometry();
  const QRect wr({}, this->frameSize().boundedTo(sr.size()));
  const auto offset = sr.center() - wr.center();

  setGeometry(offset.x(), offset.y(), m_width, m_height);
  setStatusLineAndCol(0, 0);

  // Version 2.0
#if defined(VERSION_2)
  m_editor->loadDocument(
    "/home/simonmeaden/workspace/epubedit/book/Who/Algis Budrys - Who.epub");

#elif defined(VERSION_3_0)

#elif defined(VERSION_3_1)
  // Version 3.1
  m_editor->loadDocument(
    "/home/simonmeaden/workspace/epubedit/book/mobydick/moby-dick.epub");
#elif defined(VERSION_3_2)

#elif defined(VERSION_3_3)

#endif

  m_editor->saveDocument();
#endif
}

MainWindow::~MainWindow() {}

void
MainWindow::newEpub()
{
  m_editor->newDocument();
}

void
MainWindow::openFile()
{
  auto filename = QFileDialog::getOpenFileName(
    this, tr("Select file to open"), ".", tr("EPub Files (*.epub)"));
  if (!filename.isEmpty()) {
    m_editor->loadDocument(filename);
  }
}

void
MainWindow::saveFile()
{
  // TODO
}

void
MainWindow::cleanup()
{
  // TODO cleanup code
  qWarning();
}

void
MainWindow::initGui()
{
  auto mainFrame = new QFrame(this);
  setCentralWidget(mainFrame);
  auto layout = new QGridLayout;
  mainFrame->setLayout(layout);

  auto tabs = new QTabWidget(this);

  m_editor = new EPubEdit(m_config, this);
  tabs->addTab(m_editor, tr("Editor"));

  m_logPage = new QPlainTextEdit(this);
  m_logPage->setReadOnly(true);
  tabs->addTab(m_logPage, tr("Logs"));

  auto undoView = m_editor->undoView();
  tabs->addTab(undoView, tr("Undo View"));

  layout->addWidget(tabs, 0, 0);

  initActions();
  initMenus();

  m_lineLbl = new QLabel(this);
  m_colLbl = new QLabel(this);
  m_msgLbl = new QLabel(this);

  statusBar()->addPermanentWidget(m_msgLbl, 1);
  statusBar()->addPermanentWidget(m_lineLbl);
  statusBar()->addPermanentWidget(m_colLbl);
}

void
MainWindow::initFileActions()
{
  m_fileNewAct = new QAction(tr("&New"), this);
  m_fileNewAct->setShortcuts(QKeySequence::New);
  m_fileNewAct->setStatusTip(tr("Create a new file"));
  connect(m_fileNewAct, &QAction::triggered, this, &MainWindow::newEpub);

  m_fileOpenAct = new QAction(tr("&Open"), this);
  m_fileOpenAct->setShortcuts(QKeySequence::Open);
  m_fileOpenAct->setStatusTip(tr("Open an Epub file"));
  connect(m_fileOpenAct, &QAction::triggered, this, &MainWindow::openFile);

  m_fileSaveAct = new QAction(tr("&Save"), this);
  m_fileSaveAct->setShortcuts(QKeySequence::Save);
  m_fileSaveAct->setStatusTip(tr("Save Epub file"));
  connect(m_fileSaveAct, &QAction::triggered, this, &MainWindow::saveFile);

  m_fileExitAct = new QAction(tr("&Quit"), this);
  m_fileExitAct->setShortcuts(QKeySequence::Close);
  m_fileExitAct->setStatusTip(tr("Exit Application"));
  connect(m_fileExitAct, &QAction::triggered, this, &MainWindow::cleanup);
}

void
MainWindow::initEditActions()
{ //, *, *, *,*, *
  m_editUndoAct = m_editor->undoAction();
  m_editUndoAct->setShortcuts(QKeySequence::Undo);
  connect(m_editUndoAct, &QAction::triggered, this, &MainWindow::editUndo);

  m_editRedoAct = m_editor->redoAction();
  m_editRedoAct->setShortcuts(QKeySequence::Redo);
  connect(m_editRedoAct, &QAction::triggered, this, &MainWindow::editRedo);

  m_editDeleteAct = new QAction(tr("Delete"), this);
  m_editDeleteAct->setShortcuts(QKeySequence::Delete);
  connect(m_editDeleteAct, &QAction::triggered, this, &MainWindow::editDelete);

  m_editCutAct = new QAction(tr("Cut"), this);
  m_editCutAct->setShortcuts(QKeySequence::Cut);
  connect(m_editCutAct, &QAction::triggered, this, &MainWindow::editCut);

  m_editCopyAct = new QAction(tr("Copy"), this);
  m_editCopyAct->setShortcuts(QKeySequence::Copy);
  connect(m_editCopyAct, &QAction::triggered, this, &MainWindow::editCopy);

  m_editPasteAct = new QAction(tr("Redo"), this);
  m_editPasteAct->setShortcuts(QKeySequence::Redo);
  connect(m_editPasteAct, &QAction::triggered, this, &MainWindow::editPaste);
}

void
MainWindow::initToolsActions()
{
  m_toolsPrefAct = new QAction(tr("&Options..."), this);
  m_toolsPrefAct->setShortcuts(QKeySequence::Preferences);
  m_toolsPrefAct->setStatusTip(tr("Configuration edit dialog"));
  connect(m_toolsPrefAct, &QAction::triggered, this, &MainWindow::preferences);
}

void
MainWindow::initHelpActions()
{
  m_helpContentsAct = new QAction(tr("Contents"), this);
  connect(
    m_helpContentsAct, &QAction::triggered, this, &MainWindow::helpContents);

  m_helpIndexAct = new QAction(tr("Index"), this);
  connect(m_helpIndexAct, &QAction::triggered, this, &MainWindow::helpIndex);

  helpContextAct = new QAction(tr("Context Help"), this);
  helpContextAct->setShortcuts(QKeySequence::HelpContents);
  connect(helpContextAct, &QAction::triggered, this, &MainWindow::helpContext);

  m_helpAboutAct = new QAction(tr("About EPubEdit..."), this);
  connect(m_helpAboutAct, &QAction::triggered, this, &MainWindow::helpAbout);

  m_helpAboutPluginsAct = new QAction(tr("About Plugins..."), this);
  connect(m_helpAboutPluginsAct,
          &QAction::triggered,
          this,
          &MainWindow::helpAboutPlugins);
}

void
MainWindow::initActions()
{
  initFileActions();
  initEditActions();
  initToolsActions();
  initHelpActions();
}

void
MainWindow::initFileMenu()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_fileNewAct);
  m_fileMenu->addAction(m_fileOpenAct);
  m_fileMenu->addAction(m_fileSaveAct);
  //  fileMenu->addAction(printAct);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_fileExitAct);
}

void
MainWindow::initEditMenu()
{
  m_editMenu = menuBar()->addMenu(tr("&Edit"));
  m_editMenu->addAction(m_editUndoAct);
  m_editMenu->addAction(m_editRedoAct);
  m_editMenu->addSeparator();
  m_editMenu->addAction(m_editDeleteAct);
  m_editMenu->addAction(m_editCutAct);
  m_editMenu->addAction(m_editCopyAct);
  m_editMenu->addAction(m_editPasteAct);
}

void
MainWindow::initToolsMenu()
{
  m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
  m_toolsMenu->addAction(m_toolsPrefAct);
}

void
MainWindow::initHelpMenu()
{
  m_helpMenu = menuBar()->addMenu(tr("&Help"));
  m_helpMenu->addAction(m_helpContentsAct);
  m_helpMenu->addAction(m_helpIndexAct);
  m_helpMenu->addAction(helpContextAct);
  m_helpMenu->addSeparator();
  m_helpMenu->addAction(m_helpAboutAct);
  m_helpMenu->addAction(m_helpAboutPluginsAct);
}

void
MainWindow::initMenus()
{
  initFileMenu();
  initEditMenu();
  initToolsMenu();
  initHelpMenu();
}

void
MainWindow::center(const QScreen* s)
{
  const QRect sr = s->availableGeometry();
  const QRect wr({}, this->frameSize().boundedTo(sr.size()));

  move(sr.center() - wr.center());
}

void
MainWindow::setStatusLineAndCol(int line, int col)
{
  m_lineLbl->setText(tr("Line: %1").arg(line));
  m_colLbl->setText(tr("Col: %1").arg(col));
}

void
MainWindow::setStatusMessage(const QString& message, int timeout)
{
  auto t = timeout;
  if (t == 0)
    t = m_config->statusTimeout();

  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::clearStatusMessage);
  timer->start(t * 1000);
  m_msgLbl->setText(message);
}

void
MainWindow::setLogMessage(const QString& message)
{
  m_logPage->moveCursor(QTextCursor::End);
  m_logPage->appendPlainText(message);
}

void
MainWindow::clearStatusMessage()
{
  m_msgLbl->clear();
}

void
MainWindow::editUndo()
{}

void
MainWindow::editRedo()
{
  // TODO
}

void
MainWindow::editDelete()
{
  // TODO
}

void
MainWindow::editCut()
{
  // TODO
}

void
MainWindow::editCopy()
{
  // TODO
}

void
MainWindow::editPaste()
{
  // TODO
}

void
MainWindow::preferences()
{
  auto configEdit = new ConfigurationEditor(m_config, this);
  if (configEdit->exec() == QDialog::Accepted) {
    if (!m_config->save()) {
      setLogMessage(tr("Configuration save failed."));
    }
  }
}

void
MainWindow::helpContents()
{
  // TODO
}

void
MainWindow::helpIndex()
{
  // TODO
}

void
MainWindow::helpContext()
{
  // TODO
}

void
MainWindow::helpAbout()
{
  // TODO
}

void
MainWindow::helpAboutPlugins()
{
  // TODO
}
