#include "mainwidget.h"
#include "centralwidget.h"
#include "epubeditor.h"

MainWidget::MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : DockWidget(parent)
{
  //  auto p = palette();
  //  p.setColor(QPalette::Window, QColor("lightgreen"));
  //  setPalette(p);

  //  auto editor = new EPubEditor(this);
//  m_centralWidget = new CentralWidget(config, undoStack, this);
//  setCentreWidget(m_centralWidget);
  auto f = new QFrame(this);
  auto p = palette();
  p.setColor(QPalette::Window, QColor("blue"));
  f->setPalette(p);
  setCentalWidget(f);

  // TODO make connections.
  //  connect(m_centralWidget,
  //          &CentralWidget::geometryChanged,
  //          this,
  //          &MainWidget::internalGeometryUpdate);

  setCorner(NorthWest, Box);
  setCorner(SouthWest, Box);
  setCorner(SouthEast, Box);
  setCorner(NorthEast, Box);

  //  setCornerHeight(NorthWest, 30);

  WidgetItem* widget;

  //  //====================================================================
  //  //=== Header widgets
  //  //====================================================================
  auto header = addHeader();

  widget = header->addIconButton(
    End, QImage(":/icons/Splitscreen"), tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::splitScreen);

  //====================================================================
  //=== Footer widgets
  //====================================================================
  auto footer = addFooter();

  widget = footer->addIconButton(
    Start, QImage(":/icons/ShowLeft"), tr("Show Left Sidebar"));
  connect(
    widget, &WidgetItem::widgetClicked, this, &MainWidget::toggleLeftSidebar);

  widget = footer->addIconButton(
    End, QImage(":/icons/ShowRight"), tr("Show Right Sidebar"));
  connect(
    widget, &WidgetItem::widgetClicked, this, &MainWidget::toggleRightSidebar);

  //====================================================================
  //=== North toolbar widgets
  //====================================================================
  auto toolbar = addToolbar(North);

  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open epub file."));

  widget = toolbar->addTextButton(End, tr("Test"), tr("Open epub file."));

  widget = toolbar->addTextButton(Start, tr("Test Top"), tr("Open epub file."));

  //====================================================================
  //=== South toolbar widgets
  //====================================================================
  toolbar = addToolbar(South);
  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open an epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::openClicked);

  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open epub file."));
  widget = toolbar->addTextButton(End, tr("Test"), tr("Open epub file."));

  widget = toolbar->addTextButton(Start, tr("Test Top"), tr("Open epub file."));
  //====================================================================
  //=== East toolbar widgets
  //====================================================================
  toolbar = addToolbar(East);
  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open an epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::openClicked);

  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open epub file."));

  widget = toolbar->addTextButton(End, tr("Test"), tr("Open epub file."));

  widget = toolbar->addTextButton(Start, tr("Test Top"), tr("Open epub file."));
  //====================================================================
  //=== West toolbar widgets
  //====================================================================
  toolbar = addToolbar(West);
  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open an epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::openClicked);
  widget = toolbar->addIconTextButton(Start,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GNew"),
                                      tr("New"),
                                      Arrangement::TextBelowIcon,
                                      tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  widget = toolbar->addIconTextButton(End,
                                      QImage(":/icons/GOpen"),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open epub file."));

  widget = toolbar->addTextButton(End, tr("Test"), tr("Open epub file."));

  widget = toolbar->addTextButton(Start, tr("Test Top"), tr("Open epub file."));
}

EPubEditor*
MainWidget::editor()
{
  if (m_centralWidget)
    return m_centralWidget->editor();
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
  if (m_centralWidget)
    return m_centralWidget->undoView();
  return nullptr;
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
  qWarning();
}

void
MainWidget::toggleRightSidebar()
{
  qWarning();
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
