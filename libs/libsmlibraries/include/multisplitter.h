#ifndef MULTISPLITTER_H
#define MULTISPLITTER_H

#include <QHBoxLayout>
#include <QMetaType>
#include <QSplitter>
#include <QWidget>

struct SplitterData {
  int parent = -1;
  QList<int> sizes;
};

class MultiSplitter : public QSplitter
{
  Q_OBJECT


public:
  explicit MultiSplitter(QWidget* parent = nullptr);

  void saveState();

  QWidget* currentWidget();
  void setCurrentWidget(QWidget* widget);

  QWidget* currentWidget() const;

  void createSplit(Qt::Orientation orientation);
  void splitToWindow();

signals:

private:
  QHBoxLayout *m_layout;
  QList<QWidget*> m_widgets;
  QList<QSplitter*> m_splitters;
  QSplitter* m_splitter = nullptr;
  QSplitter* m_currentSplitter = nullptr;
  QWidget* m_currentWidget = nullptr;

  QList<SplitterData*> splitterSizes();
  void setSplitterSizes(QList<SplitterData *> dataList);


  static QWidget* create(const QString& type);
  static QWidget* create(QWidget* sister);
};

#endif // MULTISPLITTER_H
