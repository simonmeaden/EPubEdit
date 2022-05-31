#include "multisplitter.h"

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

MultiSplitter::MultiSplitter(DockWidget* widget, QWidget* parent)
  : QWidget{ parent }
{
  auto p = palette();
  p.setColor(QPalette::Window, QColor("red"));
  setPalette(p);

  m_splitter = new QSplitter(this);
  m_currentDocker = widget;
  m_splitter->addWidget(widget);
  m_windows.append(m_currentDocker);
}

void
MultiSplitter::split(QWidget* widget)
{
  auto w = widget;
  DockWidget* docker = nullptr;
  if (!widget) {
    docker = createDocker(m_currentDocker->centreWidget());
  } else {
    docker = createDocker(widget);
  }
  m_splitter->addWidget(docker);
}

void
MultiSplitter::splitSideBySide(QWidget* widget)
{}

DockWidget*
MultiSplitter::createDocker(QWidget* widget)
{
  auto docker = new DockWidget(this);
  docker->setCentalWidget(create(widget));
  return docker;
}

void
MultiSplitter::createSplit()
{}
