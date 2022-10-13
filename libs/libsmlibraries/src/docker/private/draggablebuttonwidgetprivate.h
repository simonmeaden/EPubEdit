#ifndef DRAGGABLEBUTTONWIDGETPRIVATE_H
#define DRAGGABLEBUTTONWIDGETPRIVATE_H

#include "docker/private/buttonwidgetprivate.h"
#include "docker/draggablebuttonwidget.h"

class DraggableButtonWidgetPrivate : public ButtonWidgetPrivate
{
public:
  DraggableButtonWidgetPrivate(AbstractDockWidget *parent, WidgetItem *widgetItem);

  WidgetItem* clone(WidgetItem* widget);

private:
};

#endif // DRAGGABLEBUTTONWIDGETPRIVATE_H
