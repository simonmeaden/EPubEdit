#include "infowidget.h"
#include "docker/abstractdockitem.h"
#include "docker/buttonwidget.h"
#include "config.h"
#include "docker/dockheader.h"
#include "docker/widgetitem.h"

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

  //    auto p = palette();
  //    p.setColor(QPalette::Window, QColor("lightgreen"));
  //    setPalette(p);

  auto img = QImage(":/icons/RemoveSplitDown");
  auto size = img.size();
  auto pix = QPixmap::fromImage(img);
  m_removeSplitDown = QIcon(pix);

  img = QImage(":/icons/RemoveSplitUp");
  pix = QPixmap::fromImage(img);
  m_removeSplitUp = QIcon(pix);

  m_removeSplitWidget = qobject_cast<ButtonWidget*>(
    header()->addIconButton(End, m_removeSplitDown, size));
  connect(m_removeSplitWidget,
          &WidgetItem::widgetClicked,
          this,
          &InfoWidget::toggleVisible);

  m_logPage = new QPlainTextEdit(this);
  setWidget(m_logPage);
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
InfoWidget::appendLog(const QString& text)
{
  m_logPage->appendPlainText(text);
}

void
InfoWidget::setVisible(bool visible)
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
