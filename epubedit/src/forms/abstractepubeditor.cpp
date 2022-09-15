#include "abstractepubeditor.h"
//#include "private/abstractepubeditorprivate.h"

AbstractEPubEditor::AbstractEPubEditor(QWidget* parent)
  : HeaderWidget(parent)
{
}

AbstractEPubEditor::AbstractEPubEditor(PConfig config, QWidget* parent)
  : HeaderWidget(parent)
  , m_config(config)
{
}

void
AbstractEPubEditor::mousePressEvent(QMouseEvent* event)
{
  HeaderWidget::mousePressEvent(event);
}
