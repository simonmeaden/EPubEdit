#include "docker/seperatorwidget.h"
#include "docker/abstractdockitem.h"
#include "docker/private/seperatorwidgetprivate.h"

//====================================================================
//=== SpacerWrapper
//====================================================================
SeperatorWidget::SeperatorWidget(AbstractDockWidget* parent)
  : WidgetItem(*new SeperatorWidgetPrivate(parent, this))
{
  qRegisterMetaType<SeperatorWidget*>();
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

void SeperatorWidget::clone(QObject *item)
{
  Q_D(SeperatorWidget);
  auto anobject = qobject_cast<SeperatorWidget*>(item);
  if (anobject)
    d->clone(anobject);
}

SeperatorWidget::SeperatorWidget(SeperatorWidgetPrivate& d)
  : WidgetItem(d)
{
}
