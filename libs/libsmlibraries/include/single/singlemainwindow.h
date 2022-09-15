#ifndef SINGLEMAINWINDOW_H
#define SINGLEMAINWINDOW_H

#include <QMainWindow>
#include <QSystemSemaphore>
#include <QSharedMemory>

QT_BEGIN_NAMESPACE

/*!
 * \class SingleMainWindow singlemainwindow.h "single/singlemainwindow.h"
 * \brief The QSingleApplication class limits the application to a single instance.
 *
 * Extends QMainWindow and checks for an already existing instance
 * on the computer. If it detects one it closes the new attempt. To use
 * just extend SingleMainWindow rather than MainWindow.
 *
 * Based, closely, on the tutorial by Evgenij Legotskoj <https://evileg.com/en/post/147/>
 */
class SingleMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  //! Constructor for SingleMainWindow
  explicit SingleMainWindow(QWidget *parent = nullptr);

signals:

private:
  QString m_uniq1, m_uniq2;

  // Checks the shared memory for existence of another instance of
  // the application and returns true if another exists, otherwise
  // return false.
  bool checkForExistingInstance();

};

QT_END_NAMESPACE

#endif // SINGLEMAINWINDOW_H
