#include "mainwidget.h"
#include "centralwidget.h"

MainWidget::MainWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : DockWidget(parent)
{
  setContentsMargins(0, 0, 0, 0);

  m_centralWidget = new CentralWidget(config, undoStack, this);
  setCentreWidget(m_centralWidget);

  setCornerType(Box, Width, Height, Height);

  addFooter();
  //====================================================================
  //=== Upper widgets
  //====================================================================
  addToolbar(DockLayout::West);

  //  auto widget = addToolbarIconTextButton(West,
  //                                         Start,
  //                                         QImage(":/icons/GNew"),
  //                                         tr("New"),
  //                                         Arrangement::TextToRight,
  //                                         tr("Create a new epub file."));
  //  connect(widget, &WidgetWrapper::widgetClicked, this,
  //  &MainWidget::newClicked); widget = addToolbarIconTextButton(West,
  //                                    Start,
  //                                    QImage(":/icons/GOpen"),
  //                                    tr("Open"),
  //                                    Arrangement::TextToRight,
  //                                    tr("Open an epub file."));
  //  connect(
  //    widget, &WidgetWrapper::widgetClicked, this, &MainWidget::openClicked);
  //  widget = addToolbarIconTextButton(West,
  //                                    WidgetPosition::Start,
  //                                    QImage((":/icons/GSave")),
  //                                    tr("Save"),
  //                                    Arrangement::TextToRight,
  //                                    tr("Save the epub file."));
  //  connect(
  //    widget, &WidgetWrapper::widgetClicked, this, &MainWidget::saveClicked);

  //  widget = addToolbarIconButton(
  //    West, Start, QImage((":/icons/Save")), tr("Save the epub file."));

  //  widget = addToolbarTextButton(West, Start, tr("TEST TOP"));

  //====================================================================
  //=== Lower widgets
  //====================================================================
  //  widget = addToolbarIconTextButton(West,
  //                                    End,
  //                                    QImage(":/icons/New"),
  //                                    tr("New"),
  //                                    Arrangement::TextToRight,
  //                                    tr("Create a new epub file."));
  //  widget = addToolbarIconTextButton(West,
  //                                    End,
  //                                    QImage(":/icons/Open"),
  //                                    tr("Open"),
  //                                    Arrangement::TextToRight,
  //                                    tr("Open an epub file."));
  //  widget = addToolbarIconTextButton(West,
  //                                    End,
  //                                    QImage((":/icons/Save")),
  //                                    tr("Save"),
  //                                    Arrangement::TextToRight,
  //                                    tr("Save the epub file."));
  //  widget = addToolbarIconButton(
  //    West, End, QImage((":/icons/Save")), tr("Save the epub file."));

  //  widget = addToolbarTextButton(
  //    West, End, tr("BOTTOM TEST"), tr("Save the epub file."));

  //  widget = addToolbarIconButton(
  //    West, End, QImage((":/icons/GCut")), tr("Cut to clipboard."));

  //  addToolbarSpacer(West, End);

  auto widget = addFooterIconTextButton(Start,
                                        QImage(":/icons/GNew"),
                                        tr("New"),
                                        Arrangement::TextToRight,
                                        tr("Create a new epub file."));
}

EPubEdit*
MainWidget::editor()
{
  return m_centralWidget->editor();
}

QPlainTextEdit*
MainWidget::logPage() const
{
  return m_centralWidget->logPage();
}

QUndoView*
MainWidget::undoView()
{
  return m_centralWidget->undoView();
}
