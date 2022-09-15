#ifndef SM_WIDGETS_GLOBAL_H
#define SM_WIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SM_WIDGETS_LIBRARY)
  #define SM_WIDGETS_SHARED_EXPORT Q_DECL_EXPORT
#else
  #define SM_WIDGETS_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SM_WIDGETS_GLOBAL_H
