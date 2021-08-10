#ifndef EXTENDEDTABWIDGET_GLOBAL_H
#define EXTENDEDTABWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SM_CORE_LIBRARY)
  #define SM_CORE_SHARED_EXPORT Q_DECL_EXPORT
#else
  #define SM_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTENDEDTABWIDGET_GLOBAL_H
