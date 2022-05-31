#ifndef MULTISPLITTER_H
#define MULTISPLITTER_H

#include <QMetaType>
#include <QSplitter>
#include <QWidget>

#include "dockwidget.h"

class MultiSplitter : public QWidget
{
  Q_OBJECT
public:
  explicit MultiSplitter(DockWidget *widget, QWidget* parent = nullptr);

  void split(QWidget *widget=nullptr);
  void splitSideBySide(QWidget *widget=nullptr);

  static QWidget* create(const QString& type);
  static QWidget* create(QWidget* sister);

signals:

private:
  QList<DockWidget*> m_windows;
  QSplitter* m_splitter;

  DockWidget* m_currentDocker;

  DockWidget* createDocker(QWidget* widget);
  void createSplit();
};

#endif // MULTISPLITTER_H
