#include "multisplitter.h"

#include <QDebug>

//====================================================================
//=== static functions
//====================================================================
QWidget*
MultiSplitter::create(const QString& type)
{
  void* myClassPtr = nullptr;
  int id = QMetaType::type(type.toStdString().c_str());
  if (id) {
    myClassPtr = QMetaType::create(id);
  }
  return static_cast<QWidget*>(myClassPtr);
}

QWidget*
MultiSplitter::create(QWidget* sister)
{
  void* myClassPtr = nullptr;
  int id = QMetaType::type(sister->metaObject()->className());
  if (id) {
    myClassPtr = QMetaType::create(id, sister);
  }
  return static_cast<QWidget*>(myClassPtr);
}

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

void
MultiSplitter::setCurrentWidget(QWidget* widget)
{
  m_currentWidget = widget;
  m_currentSplitter->addWidget(widget);
}

void
MultiSplitter::saveState()
{}

QWidget*
MultiSplitter::currentWidget() const
{
  return m_currentWidget;
}

void
MultiSplitter::createSplit(Qt::Orientation orientation)
{
  switch (orientation) {
    case Qt::Horizontal: {
      if (m_currentSplitter->orientation() == Qt::Horizontal) {
        auto widget = MultiSplitter::create(m_currentWidget);
        m_widgets.append(widget);
        m_currentSplitter->addWidget(widget);
      } else if (m_currentSplitter->count() == 1) {
        m_currentSplitter->setOrientation(Qt::Horizontal);
        auto widget = MultiSplitter::create(m_currentWidget);
        m_widgets.append(widget);
        m_currentSplitter->addWidget(widget);
      }
      break;
    }
    case Qt::Vertical: {

      break;
    }
  }
}

void
MultiSplitter::splitToWindow()
{
  qWarning();
}

QList<SplitterData*>
MultiSplitter::splitterSizes()
{
  QList<SplitterData*> dataList;
  for (auto i = 0; i < m_splitters.count(); i++) {
    auto splitter = m_splitters.at(i);
    auto data = new SplitterData;
    data->parent = i - 1;
    data->sizes = m_splitter->sizes();
    dataList.append(data);
  }
  return dataList;
}

void
MultiSplitter::setSplitterSizes(QList<SplitterData*> dataList)
{
  QSplitter* splitter;
  QSplitter* parent;
  for (auto data : dataList) {
    parent = m_splitters.at(data->parent);
    splitter = new QSplitter(this);
    m_splitters.append(splitter);
  }
}
