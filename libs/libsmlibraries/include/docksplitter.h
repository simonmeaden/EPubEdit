#ifndef DOCKSPLITTER_H
#define DOCKSPLITTER_H

#include <QSplitter>

class DockSplitter : public QSplitter
{
  Q_OBJECT
public:
  DockSplitter(QWidget* parent);
  DockSplitter(Qt::Orientation orientation, QWidget* parent);

  void widgetSizeChanged(QWidget*widget, const QSize &size);

signals:  void manualSplitterMove();
};

#endif // DOCKSPLITTER_H
