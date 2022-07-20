#include "abstractepubeditor.h"
#include "private/abstractepubeditorprivate.h"

AbstractEPubEditor::AbstractEPubEditor(QWidget *parent)
  : HeaderWidget(parent)
{
}

AbstractEPubEditor::AbstractEPubEditor(PConfig config, QWidget* parent)
  : HeaderWidget(*new AbstractEPubEditorPrivate(config, this), parent)
{
}

AbstractEPubEditor::AbstractEPubEditor(AbstractEPubEditorPrivate& d)
  : HeaderWidget(d)
{
}

AbstractEPubEditor::AbstractEPubEditor(AbstractEPubEditorPrivate& d,
                                       QWidget* parent)
  : HeaderWidget(d, parent)
{
}

void
AbstractEPubEditor::mousePressEvent(QMouseEvent* event)
{
  HeaderWidget::mousePressEvent(event);
}
