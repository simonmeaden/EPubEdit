#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyle>

#include <QFileDialog>
#include <QGridLayout>
#include <QImageReader>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMoveEvent>
#include <QStatusBar>
#include <QToolBar>

#include "document/epubdocument.h"
#include "libepubedit/epubedit.h"

typedef QSharedPointer<QDomDocument> DomDocument;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

protected:
private:
  int m_width, m_height;
  QAction *m_newAct, *m_openAct, *m_saveAct, *m_exitAct;
  QMenu *m_fileMenu, *m_editMenu, *m_helpMenu;
  QLabel *m_lineLbl, *m_colLbl;
  EPubEdit* m_editor;

  Document m_document;

  void newEpub();
  void openFile();
  void saveFile();
  void exit();

  bool loadEpubDocument(const QString& filename);

  void initGui();
  void initActions();
  void initMenus();
  void center(const QScreen* s);
  void setStatusLineAndCol(int line, int col);
};
#endif // MAINWINDOW_H
