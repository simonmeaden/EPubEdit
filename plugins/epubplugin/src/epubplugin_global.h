#ifndef EPUBPLUGIN_GLOBAL_H
#define EPUBPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EPUBPLUGIN_LIBRARY)
#  define EPUBPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EPUBPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EPUBPLUGIN_GLOBAL_H