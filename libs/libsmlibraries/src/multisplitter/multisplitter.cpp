#include "multisplitter/multisplitter.h"

#include <QDebug>

//====================================================================
//=== MultiSplitter
//====================================================================
MultiSplitter::MultiSplitter(QWidget* parent)
  : QSplitter{ parent }
{
  setContentsMargins(0, 0, 0, 0);

  m_layout = new QHBoxLayout;
  setLayout(m_layout);
}

QWidget*
MultiSplitter::currentWidget()
{
  return m_currentWidget;
}

QWidget*
MultiSplitter::setCurrentWidget(QWidget* widget)
{
  if (widget == m_currentWidget)
    return nullptr;

  if (!m_currentWidget) { // first widget.
    m_currentWidget = widget;
    auto splitter = new QSplitter(this);
    m_baseSplitter = splitter;
    m_layout->addWidget(splitter);
    splitter->addWidget(widget);
    splitter->setStretchFactor(0, 1);
    splitter->setSizes({ 10000 });
    m_widgetMap.insert(widget, splitter);
  } else {
    m_currentWidget = widget;
  }
  return nullptr;
}

QSplitter*
MultiSplitter::currentSplitter()
{
  if (m_currentWidget) {
    auto splitter = m_widgetMap.value(m_currentWidget);
    return splitter;
  }
  return nullptr;
}

void
MultiSplitter::saveState()
{
  // TODO
}

QWidget*
MultiSplitter::currentWidget() const
{
  return m_currentWidget;
}

QWidget*
MultiSplitter::createSplit(Qt::Orientation orientation, QWidget* widget)
{
  if (!m_currentWidget)
    return setCurrentWidget(widget);

  auto currentSplitter = this->currentSplitter();
  if (widget) {
    if (currentSplitter->count() == 1) {
      // only one item in splitter so just add another.
      currentSplitter->setOrientation(orientation);
      currentSplitter->addWidget(widget);
      auto size = int((orientation == Qt::Vertical ? m_currentWidget->height()
                                                   : m_currentWidget->width()) /
                      2);
      for (auto i = 0; i < currentSplitter->count(); i++) {
        currentSplitter->setStretchFactor(i, 1);
      }
      currentSplitter->setSizes({ size, size });
      m_widgetMap.insert(widget, currentSplitter);
    } else if (currentSplitter->orientation() == orientation) {
      // insert the cloned widget at the correct place then set
      // the size of the current widget and it's clone to half.
      auto sizes = currentSplitter->sizes();
      auto index = currentSplitter->indexOf(m_currentWidget);
      auto size = int(sizes.at(index) / 2) + 1;
      sizes.replace(index, size);
      sizes.insert(index, size);
      currentSplitter->insertWidget(index + 1, widget);
      currentSplitter->setSizes(sizes);
      m_widgetMap.insert(widget, currentSplitter);
    } else {
      // replace the current widget with a splitter containing the original
      // widget and it's clone then reset their sizes to half.
      auto sizes = currentSplitter->sizes();
      auto index = currentSplitter->indexOf(m_currentWidget);
      auto size = int((orientation == Qt::Vertical ? m_currentWidget->height()
                                                   : m_currentWidget->width()) /
                      2);
      auto splitter = new QSplitter(orientation, this);
      auto oldWidget = currentSplitter->replaceWidget(index, splitter);
      splitter->addWidget(oldWidget);
      splitter->addWidget(widget);
      for (auto i = 0; i < currentSplitter->count(); i++) {
        currentSplitter->setStretchFactor(i, 1);
      }
      splitter->setSizes({ size, size });
      // need to reset the sizes of the original sizes otherwise something
      // quirky sometimes happens. Probably because of min/max size hints.
      currentSplitter->setSizes(sizes);
      m_widgetMap.insert(oldWidget, splitter);
      m_widgetMap.insert(widget, splitter);
    }
  }
  setCurrentWidget(widget);
  return m_currentWidget;
}

void
MultiSplitter::openToNewWindow()
{
  // TODO
  qWarning();
}

QList<QWidget*>
MultiSplitter::widgets()
{
  return m_widgetMap.keys();
}

QSplitter*
MultiSplitter::splitter(QWidget* widget) const
{
  return m_widgetMap.value(widget);
}
