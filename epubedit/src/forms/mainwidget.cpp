#include "mainwidget.h"
#include "abstractdockitem.h"
#include "buttonwidget.h"
#include "editorwidget.h"
#include "dockfooter.h"
#include "docktoolbar.h"
#include "epubeditor.h"
#include "labelwidget.h"
#include "widgetitem.h"

MainWidget::MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : DockWidget(parent)
  , m_config(config)
{
  setContentsMargins(0, 0, 0, 0);

//  m_editorTabs = new QTabWidget(this);

  m_currentWidget = new EditorWidget(config, undoStack, this);
  setWidget(m_currentWidget);

  //  auto f = new QFrame(this);
  //  auto p = f->palette();
  //  p.setColor(QPalette::Window, QColor("red"));
  //  f->setPalette(p);

  //  f->setFrameStyle(QFrame::Box);
  //  f->setLineWidth(10);
  //  setWidget(f);

  setCorner(NorthWest, Box);
  setCorner(SouthWest, Box);
  setCorner(SouthEast, Box);
  setCorner(NorthEast, Box);

  //  setCornerHeight(NorthWest, 30);

  WidgetItem* widget;

  //====================================================================
  //=== Footer widgets
  //====================================================================
  auto footer = addFooter();

  auto img = QImage(":/icons/ShowLeft");
  auto size = img.size();
  auto pix = QPixmap::fromImage(img);
  auto icon = QIcon(pix);
  m_leftSidebarBtn =
    footer->addIconButton(Start, icon, size, tr("Show Left Sidebar"));
  connect(m_leftSidebarBtn,
          &WidgetItem::widgetClicked,
          this,
          &MainWidget::toggleLeftSidebar);

  img = QImage(":/icons/ShowRight");
  size = img.size();
  pix = QPixmap::fromImage(img);
  icon = QIcon(pix);
  m_rightSidebarBtn =
    footer->addIconButton(End, icon, size, tr("Show Right Sidebar"));
  connect(m_rightSidebarBtn,
          &WidgetItem::widgetClicked,
          this,
          &MainWidget::toggleRightSidebar);

  m_lineNoLbl =
    footer->addTextLabel(End, tr("Line: 0, Col: 0"), tr("Show Right Sidebar"));

  widget = footer->addTextButton(End, tr("Show Info"));
  connect(widget,
          &ButtonWidget::widgetClicked,
          m_currentWidget,
          &EditorWidget::widgetClicked);

  //====================================================================
  //=== West toolbar widgets
  //====================================================================
  auto toolbar = addToolbar(West);

  img = QImage(":/icons/GNew");
  widget = toolbar->addIconTextButton(Start,
                                      QIcon(QPixmap::fromImage(img)),
                                      img.size(),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newEpub);

  img = QImage(":/icons/GOpen");
  widget = toolbar->addIconTextButton(Start,
                                      QIcon(QPixmap::fromImage(img)),
                                      img.size(),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open an epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::openFile);

  img = QImage(":/icons/GSave");
  widget = toolbar->addIconTextButton(Start,
                                      QIcon(QPixmap::fromImage(img)),
                                      img.size(),
                                      tr("Save"),
                                      Arrangement::TextBelowIcon,
                                      tr("Save the epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::saveFile);
}

void
MainWidget::openFile()
{
  //  auto filename = QFileDialog::getOpenFileName(
  //    this, tr("Select file to open"), ".", tr("EPub Files (*.epub)"));
  auto typeList = m_config->fileTypes();
  QString types = tr("EPub Files (");
  for (auto& type : typeList) {
    if (!types.isEmpty())
      types += " ";
    types += type;
  }
  types += ")";

  auto dlg = new QFileDialog(
    this, tr("Open EPub file"), m_config->libraryDirectory(), types);
  dlg->setFileMode(QFileDialog::ExistingFiles);
  dlg->setAcceptMode(QFileDialog::AcceptOpen);

  if (dlg->exec() == QDialog::Accepted) {
    auto filenames = dlg->selectedFiles();
    m_currentWidget->loadDocument(filenames.at(0));
  }
}

void
MainWidget::saveFile()
{
  // TODO
  qWarning();
}

void
MainWidget::saveAsFile()
{
  // TODO
  qWarning();
}

EPubEditor*
MainWidget::editor()
{
  if (m_currentWidget)
    return m_currentWidget->editor();
  return nullptr;
}

QPlainTextEdit*
MainWidget::logPage() const
{
  //  if (m_centralWidget)
  //    return m_centralWidget->logPage();
  return nullptr;
}

QUndoView*
MainWidget::undoView()
{
  if (m_currentWidget)
    return m_currentWidget->undoView();
  return nullptr;
}

void
MainWidget::load(const QString& filename)
{
  m_currentWidget->loadDocument(filename);
}

void
MainWidget::newEpub()
{
  m_currentWidget->newDocument();
}

void
MainWidget::internalGeometryUpdate(int north, int south, int east, int west)
{
  setCornerSize(NorthWest, QSize(west, north));
  setCornerSize(NorthEast, QSize(east, north));
  setCornerSize(SouthWest, QSize(west, south));
  setCornerSize(SouthEast, QSize(east, south));
}

void
MainWidget::splitScreen()
{
  qWarning();
}

void
MainWidget::toggleLeftSidebar()
{
  if (m_leftSidebarBtn) {
    auto visible = m_currentWidget->toggleLeftSidebar();
    m_leftSidebarBtn->setSelected(visible);
    repaint();
  }
}

void
MainWidget::toggleRightSidebar()
{
  if (m_rightSidebarBtn) {
    auto visible = m_currentWidget->toggleRightSidebar();
    m_rightSidebarBtn->setSelected(visible);
    repaint();
  }
}

void
MainWidget::listClicked(int index, const QString& text)
{
  qWarning();
}

void
MainWidget::setLineNoText(int lineNo, int column)
{
  auto widget = qobject_cast<LabelWidget*>(m_lineNoLbl);
  if (widget) {
    auto text = tr("Line: %1, Col: %2").arg(lineNo).arg(column);
    widget->setText(text);
  }
}

void
MainWidget::hideLeftSidebar()
{
  qWarning();
}

void
MainWidget::showLeftSidebar()
{
  qWarning();
}

void
MainWidget::hideRightSidebar()
{
  qWarning();
}

void
MainWidget::showRightSidebar()
{
  qWarning();
}
