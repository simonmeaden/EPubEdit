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
#include <QTimer>
#include <QToolBar>

#include "forms/config.h"

class EPubEdit;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

protected:
private:
  Config m_config;
  int m_width, m_height;
  QAction *m_fileNewAct, *m_fileOpenAct, *m_fileSaveAct, *m_fileExitAct;
  QAction *m_editUndoAct, *m_editRedoAct, *m_editDeleteAct, *m_editCutAct,
    *m_editCopyAct, *m_editPasteAct;
  QAction* m_toolsPrefAct;
  QAction *m_helpContentsAct, *m_helpIndexAct, *helpContextAct, *m_helpAboutAct,
    *m_helpAboutPluginsAct;
  QMenu *m_fileMenu, *m_editMenu, *m_toolsMenu, *m_helpMenu;
  QLabel *m_lineLbl, *m_colLbl, *m_msgLbl;
  EPubEdit* m_editor;

  void newEpub();
  void openFile();
  void saveFile();
  void exit();

  bool loadEpubDocument(const QString& filename);

  void initGui();

  void initActions();
  void initFileActions();
  void initEditActions();
  void initToolsActions();
  void initHelpActions();

  void initMenus();
  void initFileMenu();
  void initEditMenu();
  void initToolsMenu();
  void initHelpMenu();

  void center(const QScreen* s);
  void setStatusLineAndCol(int line, int col);
  void setStatusMessage(const QString& message, int timeout = 0);
  void clearStatusMessage();

  void editUndo();
  void editRedo();
  void editDelete();
  void editCut();
  void editCopy();
  void editPaste();

  void preferences();

  void helpContents();
  void helpIndex();
  void helpContext();
  void helpAbout();
  void helpAboutPlugins();
};
#endif // MAINWINDOW_H
