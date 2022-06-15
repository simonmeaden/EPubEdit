#include "abstractdockwidget.h"
#include "dockitem.h"
#include "widgetitem.h"
#include "x11colors.h"

AbstractDockWidget::AbstractDockWidget(QWidget* parent)
  : QWidget{ parent }
  , m_backColor(QColor(64, 65, 66))
  , m_hoverBackColor(QColorConstants::X11::grey43)
  , m_selectedColor(QColorConstants::X11::grey18)
  , m_spacerColor(QColorConstants::X11::DimGrey)
{}

const QColor&
AbstractDockWidget::textColor() const
{
  return m_textColor;
}

void
AbstractDockWidget::setTextColor(const QColor& newTextColor)
{
  m_textColor = newTextColor;
}

const QColor&
AbstractDockWidget::spacerColor() const
{
  return m_spacerColor;
}

void
AbstractDockWidget::setSpacerColor(const QColor& newSpacerColor)
{
  m_spacerColor = newSpacerColor;
}

const QBrush&
AbstractDockWidget::backColor() const
{
  return m_backColor;
}

void
AbstractDockWidget::setBackColor(const QBrush& newBackColor)
{
  m_backColor = newBackColor;
}

const QBrush&
AbstractDockWidget::hoverBackColor() const
{
  return m_hoverBackColor;
}

void
AbstractDockWidget::setHoverBackColor(const QBrush& newHoverBackColor)
{
  m_hoverBackColor = newHoverBackColor;
}

const QBrush&
AbstractDockWidget::selectedColor() const
{
  return m_selectedColor;
}

void
AbstractDockWidget::setSelectedColor(const QBrush& newSelectedColor)
{
  m_selectedColor = newSelectedColor;
}

bool
AbstractDockWidget::dockItemHoverCheck(DockItem* item, QPoint pos)
{
  if (item) {
    auto widgets = item->widgets();
    for (auto& w : widgets) {
      if (w->rect().contains(pos)) {
        if (m_hoverItem && w != m_hoverItem) {
          m_hoverItem->setHoverOver(false);
          QToolTip::hideText();
          m_hoverItem = nullptr;
        }
        w->setHoverOver(true);
        QToolTip::showText(mapToGlobal(pos), w->tooltip(), this, w->rect());
        m_hoverItem = w;
        repaint();
        return true;
      }
    }
  }
  return false;
}

void
AbstractDockWidget::mouseClickCheck(DockItem* item, QPoint pos)
{
  auto widgets = item->widgets();

  for (auto& w : widgets) {
    if (w->rect().contains(pos)) {
      if (w->isEnabled()) {
        switch (w->type()) {
          case Button: {
            auto bl = w->rect().bottomLeft();
            emit w->widgetClicked(bl);
            w->setSelected(true);
            break;
          }
          case List: {
            //            emit w->widgetClicked();
            auto listWidget = qobject_cast<ListWidget*>(w);
            w->setSelected(true);
            if (listWidget) {
              listWidget->show(pos, rect());
            }
            break;
          }
          default:
            break;
        }
      }
    }
  }
}
