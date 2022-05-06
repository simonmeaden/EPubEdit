#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QList>
#include <QPainter>
#include <QWidget>

#include "borderlayout.h"
#include "epubedit.h"
#include "footerwidget.h"
#include "toolbarwidget.h"
#include "x11colors.h"



class CentralWidget;

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MainWidget(PConfig config, QWidget* parent = nullptr);

  EPubEdit* editor();
  QPlainTextEdit* logPage() const;

signals:

protected:
private:
  ToolbarWidget* m_toolbar;
  CentralWidget* m_centreFrame;

  friend class FooterWidget;
  friend class ToolbarWidget;
};

class CentralWidget : public QWidget
{
  Q_OBJECT

public:
  CentralWidget(PConfig config, QWidget* parent = nullptr);

  EPubEdit* editor();
  QPlainTextEdit* logPage() const;

private:
  FooterWidget* m_footer;
  MainWidget* m_mainWidget;
  QSplitter* m_mainSplitter;
  QSplitter* m_infoSplitter;
  EPubEdit* m_editor;
  QPlainTextEdit* m_logPage;
  //  InformationWidget*m_infoWidget;
  PConfig m_config;

  void splitterHasMoved();
};

#endif // MAINWIDGET_H
