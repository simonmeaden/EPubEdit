#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_width(800)
  , m_height(400)
{
  //  initGui();

  //  const auto sr = qApp->screens().at(0)->availableGeometry();
  //  const QRect wr({}, this->frameSize().boundedTo(sr.size()));
  //  const auto offset = sr.center() - wr.center();

  //  setGeometry(offset.x(), offset.y(), m_width, m_height);
  //  setStatusLineAndCol(0, 0);
  m_editor->loadDocument(
    "/home/simonmeaden/workspace/epubedit/book/mobydick/moby-dick.epub");
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
{}

void
MainWindow::exit()
{
  close();
}

void
MainWindow::initGui()
{
  initActions();
  initMenus();

  auto mainFrame = new QFrame(this);
  setCentralWidget(mainFrame);
  auto layout = new QGridLayout;
  mainFrame->setLayout(layout);

  m_editor = new EPubEdit(this);
  layout->addWidget(m_editor, 0, 0);

  m_lineLbl = new QLabel();
  m_colLbl = new QLabel();
  statusBar()->addPermanentWidget(m_lineLbl);
  statusBar()->addPermanentWidget(m_colLbl);
}

void
MainWindow::initActions()
{
  m_newAct = new QAction(tr("&New"), this);
  m_newAct->setShortcuts(QKeySequence::New);
  m_newAct->setStatusTip(tr("Create a new file"));
  connect(m_newAct, &QAction::triggered, this, &MainWindow::newEpub);

  m_openAct = new QAction(tr("&Open"), this);
  m_openAct->setShortcuts(QKeySequence::Open);
  m_openAct->setStatusTip(tr("Open an Epub file"));
  connect(m_openAct, &QAction::triggered, this, &MainWindow::openFile);

  m_saveAct = new QAction(tr("&Save"), this);
  m_saveAct->setShortcuts(QKeySequence::Save);
  m_saveAct->setStatusTip(tr("Save Epub file"));
  connect(m_saveAct, &QAction::triggered, this, &MainWindow::saveFile);

  m_exitAct = new QAction(tr("&Quit"), this);
  m_exitAct->setShortcuts(QKeySequence::Close);
  m_exitAct->setStatusTip(tr("Exit Application"));
  connect(m_exitAct, &QAction::triggered, this, &MainWindow::exit);
}

void
MainWindow::initMenus()
{
  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_newAct);
  m_fileMenu->addAction(m_openAct);
  m_fileMenu->addAction(m_saveAct);
  //  fileMenu->addAction(printAct);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_exitAct);

  m_editMenu = menuBar()->addMenu(tr("&Edit"));

  m_helpMenu = menuBar()->addMenu(tr("&Help"));
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
  m_lineLbl->setText(QString("Line: %1").arg(line));
  m_colLbl->setText(QString("Col: %1").arg(col));
}
