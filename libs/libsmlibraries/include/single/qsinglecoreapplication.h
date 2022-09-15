#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QCoreApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>

QT_BEGIN_NAMESPACE

/*!
 * \class QSingleCoreApplication qsinglecoreapplication.h "single/qsinglecoreapplication.h"
 * \brief The QSingleApplication class limits the application to a single instance.
 *
 * Extends QCoreApplication and checks for an already existing instance
 * on the computer. If it detects one it closes the new attempt. To use
 * just extend QSingleCoreApplication rather than QCoreApplication.
 *
 * Based, closely, on the tutorial by Evgenij Legotskoj <https://evileg.com/en/post/147/>
 */
class QSingleCoreApplication : public QCoreApplication
{
  Q_OBJECT
public:
  //! Constructor for SingleApp.
  explicit QSingleCoreApplication(int& argc, char** argv);

signals:

private:
  QString m_uniq1, m_uniq2;

  // Checks the shared memory for existence of another instance of
  // the application and returns true if another exists, otherwise
  // return false.
  bool checkForExistingInstance();

};

QT_END_NAMESPACE

#endif // SINGLEAPPLICATION_H
