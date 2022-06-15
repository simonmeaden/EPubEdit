#include "infowidget.h"
#include "config.h"
#include "dockitem.h"
#include "widgetitem.h"

#include "common.h"

//====================================================================
//=== InfoWidget
//====================================================================
InfoWidget::InfoWidget(PConfig config, QUndoStack* undoStack, QWidget* parent)
  : HeaderWidget(parent)
  , m_undoStack(undoStack)
  , m_config(config)
{
  setContentsMargins(0, 0, 0, 0);

  //  auto p = palette();
  //  p.setColor(QPalette::Window, QColor("lightgreen"));
  //  setPalette(p);

  auto img = QImage(":/icons/RemoveSplitDown");
  auto size = img.size();
  auto pix = QPixmap::fromImage(img);
  m_removeSplitDown = QIcon(pix);

  img = QImage(":/icons/RemoveSplitUp");
  pix = QPixmap::fromImage(img);
  m_removeSplitUp = QIcon(pix);

  m_removeSplitWidget = qobject_cast<ButtonWidget*>(
    m_header->addIconButton(End, m_removeSplitDown, size));
  setShowHeaderOnHide(false);
  connect(m_removeSplitWidget,
          &WidgetItem::widgetClicked,
          this,
          &InfoWidget::toggleVisible);

  auto editor = createEditor(this);
  setCentralWidget(editor);
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

void InfoWidget::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (m_config->infoIsVisible()) {
    toggleVisible();
  }
}

void
InfoWidget::toggleVisible()
{
  if (isWidgetVisible()) {
    hideWidget();
    m_removeSplitWidget->setIcon(m_removeSplitUp);
    m_config->setInfoIsVisible(false);
  } else {
    showWidget();
    m_removeSplitWidget->setIcon(m_removeSplitDown);
    m_config->setInfoIsVisible(true);
  }
}
