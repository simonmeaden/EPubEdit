#include "docker/docksplitter.h"

DockSplitter::DockSplitter(QWidget* parent)
  : QSplitter(parent)
{}

DockSplitter::DockSplitter(Qt::Orientation orientation, QWidget* parent)
  : QSplitter(orientation, parent)
{}

void
DockSplitter::widgetSizeChanged(QWidget* widget, const QSize& size)
{
  auto index = indexOf(widget);
  auto c = count();
  if (index < 0 || index > c || c < 2)
    return;

  auto s = sizes();
  int diff = s.at(index) - size.height();

  s.replace(index, size.height());
  if (index > 0) {
    s.replace(index - 1, s.at(index - 1) + diff);
  } else {
    s.replace(index + 1, s.at(index - 1) + diff);
  }
  setSizes(s);
}
