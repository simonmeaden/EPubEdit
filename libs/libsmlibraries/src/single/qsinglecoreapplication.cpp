#include "single/qsinglecoreapplication.h"

QT_BEGIN_NAMESPACE

QSingleCoreApplication::QSingleCoreApplication(int& argc, char** argv)
  : QCoreApplication{ argc, argv }
{
  m_uniq1 = "<uniq id>";
  m_uniq2 = "<uniq id 2>";
  /*
   * Raise the semaphore, barring other instances to work
   *  with shared memory.
   */
  if (checkForExistingInstance()) {
    exit();
  }
}

bool
QSingleCoreApplication::checkForExistingInstance()
{
  QSystemSemaphore semaphore(m_uniq1, 1); // create semaphore
  semaphore.acquire();                    //
                                          //

#ifndef Q_OS_WIN32
  /*
   * in linux / unix shared memory is not freed when the application
   * terminates abnormally, so you need to get rid of the garbage
   */
  QSharedMemory nixFixSharedMemory(m_uniq2);
  if (nixFixSharedMemory.attach()) {
    nixFixSharedMemory.detach();
  }
#endif

  // Create a copy of the shared memory
  QSharedMemory sharedMemory(m_uniq2);
  bool isRunning; // variable to test the already running application
  if (sharedMemory.attach()) {
    /*
     We are trying to attach a copy of the shared
     memory To an existing segment If successful, it determines that there is
     already a running instance
     */
    isRunning = true;
  } else {
    sharedMemory.create(1); // Otherwise allocate 1 byte of memory
    isRunning = false; // And determines that another instance is not running
  }
  semaphore.release();
  return isRunning;
}

QT_END_NAMESPACE
