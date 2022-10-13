#include "draggablebuttonwidgetprivate.h"
#include "docker/abstractdockitem.h"
#include "docker/abstractdockwidget.h"

//====================================================================
//=== DraggableButtonWidgetPrivate
//====================================================================
DraggableButtonWidgetPrivate::DraggableButtonWidgetPrivate(
  AbstractDockWidget* parent,
  WidgetItem* q_ptr)
  : ButtonWidgetPrivate(parent, q_ptr)
{
}

WidgetItem*
DraggableButtonWidgetPrivate::clone(WidgetItem* widget)
{
  if (!widget) {
    widget = new DraggableButtonWidget(m_parentWidget);
  }

  auto button = qobject_cast<DraggableButtonWidget*>(widget);
  if (button) {
    ButtonWidgetPrivate::clone(button);
  }
  return button;
}
