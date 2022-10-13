#ifndef LNSTATUSBAR_H
#define LNSTATUSBAR_H

#include <QLabel>
#include <QStatusBar>
#include <QToolButton>

class LNStatusBar : public QStatusBar
{
  Q_OBJECT
public:
  explicit LNStatusBar(QWidget* parent = nullptr);

  void setLocation(int line, int lineCount, int column);

  void addPermanentWidget(QWidget* widget, int stretch = 0);
signals:
  void settingsRequested();

protected:
  QLabel* m_posLbl;

  void settingsClicked();
};

#endif // LNSTATUSBAR_H
