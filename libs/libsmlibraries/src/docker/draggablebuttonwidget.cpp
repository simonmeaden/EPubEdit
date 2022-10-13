#include "draggablebuttonwidget.h"
#include "private/draggablebuttonwidgetprivate.h"

DraggableButtonWidget::DraggableButtonWidget(AbstractDockWidget* parent)
  : ButtonWidget(*new DraggableButtonWidgetPrivate(parent, this))
{
}

DraggableButtonWidget::DraggableButtonWidget(DraggableButtonWidgetPrivate& d)
  : ButtonWidget(d)
{
}

QPoint
DraggableButtonWidget::dragStartPosition() const
{
  return m_dragStartPosition;
}

void
DraggableButtonWidget::setDragStartPosition(QPoint dragStartPosition)
{
  m_dragStartPosition = dragStartPosition;
}

const QString&
DraggableButtonWidget::mimeType() const
{
  return m_mimeType;
}

// void DraggableButtonWidget::setMimeType(const QString &mimeType)
//{
//   m_mimeType = mimeType;
// }

HtmlButtonType
DraggableButtonWidget::data(const QString& mimetype) const
{
  if (mimetype == m_mimeType)
    return m_data;
  return NoButton;
}

void
DraggableButtonWidget::setData(const QString& mimetype, HtmlButtonType data)
{
  m_mimeType = mimetype;
  m_data = data;
}
