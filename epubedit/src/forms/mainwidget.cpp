#include "mainwidget.h"
#include "centralwidget.h"
#include "dockitem.h"
#include "epubeditor.h"
#include "widgetitem.h"

MainWidget::MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : DockWidget(parent)
{
  setContentsMargins(0, 0, 0, 0);

  m_centralWidget = new CentralWidget(config, undoStack, this);
  setWidget(m_centralWidget);

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
  m_leftSidebarWidget =
    footer->addIconButton(Start, icon, size, tr("Show Left Sidebar"));
  connect(m_leftSidebarWidget,
          &WidgetItem::widgetClicked,
          this,
          &MainWidget::toggleLeftSidebar);

  img = QImage(":/icons/ShowRight");
  size = img.size();
  pix = QPixmap::fromImage(img);
  icon = QIcon(pix);
  m_rightSidebarWidget =
    footer->addIconButton(End, icon, size, tr("Show Right Sidebar"));
  connect(m_rightSidebarWidget,
          &WidgetItem::widgetClicked,
          this,
          &MainWidget::toggleRightSidebar);

  m_lineNoWidget =
    footer->addTextLabel(End, tr("Line: 0, Col: 0"), tr("Show Right Sidebar"));

  widget = footer->addTextButton(End, tr("Show Info"));
  connect(widget,
          &ButtonWidget::widgetClicked,
          m_centralWidget,
          &CentralWidget::widgetClicked);

  widget = footer->addListWidget(End, tr("Show Right Sidebar"));
  m_listNoWidget = qobject_cast<ListWidget*>(widget);
  m_listNoWidget->addItem(tr("First"));
  m_listNoWidget->addItem(tr("Second"));
  m_listNoWidget->addItem(tr("Third"));
  m_listNoWidget->addItem(tr("Very long item"));
  m_listNoWidget->addItem(tr("Fifth"));
  m_listNoWidget->addItem(tr("Sixth"));
  m_listNoWidget->addItem(tr("Seventh"));
  connect(
    m_listNoWidget, &ListWidget::itemClicked, this, &MainWidget::listClicked);

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
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::newClicked);

  img = QImage(":/icons/GOpen");
  widget = toolbar->addIconTextButton(Start,
                                      QIcon(QPixmap::fromImage(img)),
                                      img.size(),
                                      tr("Open"),
                                      Arrangement::TextBelowIcon,
                                      tr("Open an epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::openClicked);

  img = QImage(":/icons/GSave");
  widget = toolbar->addIconTextButton(Start,
                                      QIcon(QPixmap::fromImage(img)),
                                      img.size(),
                                      tr("Save"),
                                      Arrangement::TextBelowIcon,
                                      tr("Save the epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::saveClicked);

  //  widget = toolbar->addIconTextButton(End,
  //                                      QImage(":/icons/GNew"),
  //                                      tr("New"),
  //                                      Arrangement::TextBelowIcon,
  //                                      tr("Create a new epub file."));
  //  connect(widget, &WidgetItem::widgetClicked, this,
  //  &MainWidget::newClicked);

  //  widget = toolbar->addIconTextButton(End,
  //                                      QImage(":/icons/GOpen"),
  //                                      tr("Open"),
  //                                      Arrangement::TextBelowIcon,
  //                                      tr("Open epub file."));

  //  widget = toolbar->addTextButton(End, tr("Test"), tr("Open epub file."));

  //  widget = toolbar->addTextButton(Start, tr("Test Top"), tr("Open epub
  //  file."));

  //  //====================================================================
  //  //=== Header widgets
  //  //====================================================================
  /*
   * auto header = addHeader();

  widget = header->addIconButton(
    End, QImage(":/icons/Splitscreen"), tr("Create a new epub file."));
  connect(widget, &WidgetItem::widgetClicked, this, &MainWidget::splitScreen);
  */

  //====================================================================
  //=== North toolbar widgets
  //====================================================================
  /*
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
  */

  //====================================================================
  //=== South toolbar widgets
  //====================================================================
  /*
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
  */

  //====================================================================
  //=== East toolbar widgets
  //====================================================================
  /*
  auto toolbar = addToolbar(East);
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
  */
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
MainWidget::load(const QString& filename)
{
  m_centralWidget->loadDocument(filename);
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
  if (m_leftSidebarWidget) {
    auto visible = m_centralWidget->toggleLeftSidebar();
    m_leftSidebarWidget->setSelected(visible);
    repaint();
  }
}

void
MainWidget::toggleRightSidebar()
{
  if (m_rightSidebarWidget) {
    auto visible = m_centralWidget->toggleRightSidebar();
    m_rightSidebarWidget->setSelected(visible);
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
  auto widget = qobject_cast<LabelWidget*>(m_lineNoWidget);
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
