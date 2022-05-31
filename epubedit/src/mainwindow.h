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
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include <QToolBar>
#include <QUndoStack>

class EPubEditor;

#include "document/bookpointers.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

signals:
  void shutdown(int returnCode = 0);

protected:
private:
  PConfig m_config;
  int m_width, m_height;
  QAction *m_fileNewAct, *m_fileOpenAct, *m_fileSaveAct, *m_fileExitAct;
  QAction *m_editUndoAct, *m_editRedoAct, *m_editDeleteAct, *m_editCutAct,
    *m_editCopyAct, *m_editPasteAct;
  QAction* m_toolsPrefAct;
  QAction *m_helpContentsAct, *m_helpIndexAct, *helpContextAct, *m_helpAboutAct,
    *m_helpAboutPluginsAct;
  QMenu *m_fileMenu, *m_editMenu, *m_toolsMenu, *m_helpMenu;
  EPubEditor* m_editor = nullptr;
  QPlainTextEdit* m_logPage = nullptr;
  QUndoStack* m_undoStack = nullptr;

  void newEpub();
  void openFile();
  void saveFile();
  void saveAsFile();
  void cleanup();

  bool loadDocument(const QString& filename);

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
  void setLogMessage(const QString& message);
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
