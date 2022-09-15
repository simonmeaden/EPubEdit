#ifndef SEPERATORWIDGETPRIVATE_H
#define SEPERATORWIDGETPRIVATE_H

#include <QColor>

#include "docker/seperatorwidget.h"
#include "docker/private/widgetitemprivate.h"

class SeperatorWidgetPrivate : public WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(SeperatorWidget)
public:
  SeperatorWidgetPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  QColor& color();
  void setColor(const QColor& color);

  int thickness();
  void setThickness(int thickness);

  WidgetItem* clone(WidgetItem* widget = nullptr);

  const QSize calcMinimumSize();

  void paint(QPainter& painter);

private:
  QColor m_color;
  int m_thickness = 1;
};

#endif // SEPERATORWIDGETPRIVATE_H
