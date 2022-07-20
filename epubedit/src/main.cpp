#include "mainwindow.h"

#include <QApplication>

#include <Logger.h>
#include <FileAppender.h>
#include "signalappender.h"

int
main(int argc, char* argv[])
{

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

