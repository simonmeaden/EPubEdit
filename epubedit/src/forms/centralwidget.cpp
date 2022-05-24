#include "forms/centralwidget.h"
#include "forms/epubedit.h"
#include "forms/infowidget.h"

//====================================================================
//=== CentralWidget
//====================================================================
CentralWidget::CentralWidget(PConfig config,
                             QUndoStack* undoStack,
                             QWidget* parent)
  : DockWidget{ parent }
  , m_config(config)
  , m_undoStack(undoStack)
{
  setContentsMargins(0, 0, 0, 0);

  initGui(config, undoStack);
}

void
CentralWidget::initGui(PConfig config, QUndoStack* undoStack)
{
//  addHeader();

  m_splitter = new QSplitter(Qt::Vertical, this);
  m_splitter->setContentsMargins(0, 0, 0, 0);
  connect(m_splitter,
          &QSplitter::splitterMoved,
          this,
          &CentralWidget::splitterHasMoved);
  setCentreWidget(m_splitter);

  m_splitter->setSizes(config->vSplitterSizes());

  m_editor = new EPubEdit(config, this);
  m_splitter->addWidget(m_editor);

  //== Information frame ==//
  m_infoWidget = new InfoWidget(config, undoStack, this);
  m_splitter->addWidget(m_infoWidget);


  //== Main Footer ==//

//  addIconButton(Footer,
//                WidgetPosition::Left,
//                QImage(":/icons/hideH"),
//                tr("Toggle visibility of first"));

//  addIconButton(Footer,
//                WidgetPosition::Left,
//                QImage(":/icons/hideV"),
//                tr("Toggle visibility of second"));

//  addIconButton(Footer,
//                WidgetPosition::Right,
//                QImage(":/icons/Copy"),
//                tr("Toggle visibility of second"));

//  addIconTextButton(Footer,
//                    WidgetPosition::Left,
//                    QImage((":/icons/SaveAs")),
//                    tr("Save As..."),
//                    Arrangement::TextToLeft,
//                    tr("Save the epub file."));

//  addIconButton(Footer,
//                WidgetPosition::Right,
//                QImage(":/icons/Paste"),
//                tr("Toggle visibility of first"));

//  addIconTextButton(Footer,
//                    WidgetPosition::Right,
//                    QImage((":/icons/Save")),
//                    tr("Save"),
//                    Arrangement::TextToRight,
//                    tr("Save the epub file."));
}

EPubEdit*
CentralWidget::editor()
{
  return m_editor;
}

QPlainTextEdit*
CentralWidget::logPage() const
{
  return m_infoWidget->logPage();
}

QUndoView*
CentralWidget::undoView()
{
  return m_infoWidget->undoView();
}

void
CentralWidget::splitterHasMoved()
{
  m_config->setVSplitterSizes(m_splitter->sizes());
}
