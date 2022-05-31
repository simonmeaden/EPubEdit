#include "infowidget.h"
#include "config.h"

//====================================================================
//=== InfoWidget
//====================================================================
InfoWidget::InfoWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : DockWidget(parent)
  , m_undoStack(undoStack)
  , m_config(config)
{
  setContentsMargins(0, 0, 0, 0);

  auto p = palette();
  p.setColor(QPalette::Window, QColor("lightgreen"));
  setPalette(p);

  //  addHeader();

  //  m_header = new HeaderWidget(this);
  //  layout->addWidget(m_header, BorderLayout::North);

  //  QImage img(":/icons/hideH");
  //  m_header->addIconButton(
  //    WidgetPosition::Left, img, tr("Toggle visibility of first"));

  //  img = QImage(":/icons/hideV");
  //  m_header->addIconButton(
  //    WidgetPosition::Left, img, tr("Toggle visibility of second"));

  //  img = QImage(":/icons/hideV");
  //  m_header->addIconButton(
  //    WidgetPosition::Right, img, tr("Toggle visibility of second"));
  //  img = QImage(":/icons/hideH");
  //  m_header->addIconButton(
  //    WidgetPosition::Right, img, tr("Toggle visibility of first"));

  //  m_splitter = new QSplitter(this);
  //  m_splitter->setContentsMargins(0, 0, 0, 0);
  //  connect(
  //    m_splitter, &QSplitter::splitterMoved, this,
  //    &InfoWidget::splitterHasMoved);
  //  m_splitter->setSizes(m_config->iSplitterSizes());
  //  setCentreWidget(m_splitter);

  //  m_logPage = new QPlainTextEdit(this);
  //  m_logPage->setContentsMargins(0, 0, 0, 0);
  //  m_logPage->setReadOnly(true);
  //  m_splitter->addWidget(m_logPage);

  //  m_undoView = new QUndoView(m_undoStack);
  //  m_undoView->setContentsMargins(0, 0, 0, 0);
  //  m_undoView->setWindowTitle(tr("Metadata Undo List"));
  //  m_splitter->addWidget(m_undoView);
}

QUndoView*
InfoWidget::undoView()
{
  return m_undoView;
}

QPlainTextEdit*
InfoWidget::logPage() const
{
  return m_logPage;
}

void
InfoWidget::splitterHasMoved()
{
  m_config->setISplitterSizes(m_splitter->sizes());
}
