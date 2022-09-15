#include "docker/dockfooter.h"
#include "docker/abstractdockwidget.h"
#include "docker/listwidget.h"
#include "docker/private/abstractdockitemprivate.h"

class DockFooterPrivate : public AbstractDockItemPrivate
{
  Q_DECLARE_PUBLIC(DockFooter)
public:
  explicit DockFooterPrivate(AbstractDockWidget* parent,
                             AbstractDockItem* item);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  WidgetItem* addListWidget(WidgetPosition pos,
                            const QString& tooltip = QString());

  void clone(AbstractDockItem* item);

protected:
private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;
};

//====================================================================
//=== DockFooterPrivate
//====================================================================

DockFooterPrivate::DockFooterPrivate(AbstractDockWidget* parent,
                                     AbstractDockItem* item)
  : AbstractDockItemPrivate(South, parent, item)
{
  setPreferredSize(WIDTH, HEIGHT);
}

WidgetItem*
DockFooterPrivate::addListWidget(WidgetPosition pos, const QString& tooltip)
{
  auto widget = new ListWidget(m_parentDocker, q_ptr);
  widget->setTooltip(tooltip);
  int height = 0;
  auto textRect = widget->textRect();
  auto fm = m_parentDocker->fontMetrics();
  textRect.setHeight(fm.height());
  widget->setTextRect(textRect);
  height = widget->topMargin() + fm.height() + widget->bottomMargin();
  m_maxWidgetHeight = std::max(height, m_maxWidgetHeight);

  widget->setType(List);
  widget->setWidgetPosition(pos);
  m_widgets.append(widget);

  setPreferredSize(m_maxWidgetWidth, m_maxWidgetHeight);
  calcMaxWidgetSizes();
  q_ptr->connect(
    widget, &WidgetItem::widgetChanged, q_ptr, &DockFooter::calcMaxWidgetSizes);
  return widget;
}

void
DockFooterPrivate::clone(AbstractDockItem* item)
{
  if (item)
    AbstractDockItemPrivate::clone(item);
}

QSize
DockFooterPrivate::sizeHint() const
{
  auto w = m_width;
  auto h = m_height;
  for (auto widget : m_widgets) {
    auto s = widget->sizeHint();
    w += s.width();
    h = (h > s.height() ? h : s.height());
  }
  return QSize(w, h);
}

void
DockFooterPrivate::paint(QPainter& painter)
{
  if (m_visible) {
    AbstractDockItemPrivate::paint(painter);

    auto pen = painter.pen();
    pen.setColor(QColor(55, 56, 56));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.x(), m_rect.height());
    painter.drawLine(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.y());
  }
}

//====================================================================
//=== DockFooter
//====================================================================
DockFooter::DockFooter(AbstractDockWidget* parent)
  : AbstractDockItem(*new DockFooterPrivate(parent, this))
{
  setPreferredSize(WIDTH, HEIGHT);
}

DockFooter::DockFooter(DockFooterPrivate& d)
  : AbstractDockItem(d)
{
}

WidgetItem*
DockFooter::addListWidget(WidgetPosition pos, const QString& tooltip)
{
  Q_D(DockFooter);
  return d->addListWidget(pos, tooltip);
}

void
DockFooter::clone(QObject *item)
{
  auto anobject = qobject_cast<DockFooter*>(item);
  if (anobject)
    d_ptr->clone(anobject);
}

QSize
DockFooter::sizeHint() const
{
  return d_ptr->sizeHint();
}

void
DockFooter::paint(QPainter& painter)
{
  d_ptr->paint(painter);
}
