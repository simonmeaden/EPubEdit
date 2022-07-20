#include "seperatorwidget.h"
#include "abstractdockitem.h"
#include "private/seperatorwidgetprivate.h"

//====================================================================
//=== SpacerWrapper
//====================================================================
SeperatorWidget::SeperatorWidget(AbstractDockWidget* parent)
  : WidgetItem(*new SeperatorWidgetPrivate(parent, this))
{
}

/// \cond DO_NOT_DOCUMENT
const QSize
SeperatorWidget::calcMinimumSize()
{
  return d_ptr->calcMinimumSize();
}
/// \endcond DO_NOT_DOCUMENT

const QColor &SeperatorWidget::color()
{
  Q_D(SeperatorWidget);
  return d->m_color;
}

void
SeperatorWidget::setColor(const QColor& color)
{
  Q_D(SeperatorWidget);
  d->m_color = color;
}

int
SeperatorWidget::thickness()
{
  Q_D(SeperatorWidget);
  return d->m_thickness;
}

void
SeperatorWidget::setThickness(int thickness)
{
  Q_D(SeperatorWidget);
  d->m_thickness = thickness;
}

/// \cond DO_NOT_DOCUMENT
void
SeperatorWidget::paint(QPainter& painter)
{
  Q_D(SeperatorWidget);
  d->paint(painter);
}
/// \endcond DO_NOT_DOCUMENT

WidgetItem*
SeperatorWidget::clone(WidgetItem* item)
{
  return d_ptr->clone(item);
}

SeperatorWidget::SeperatorWidget(SeperatorWidgetPrivate& d)
  : WidgetItem(d)
{
}
