#ifndef POINTERS_H
#define POINTERS_H

#include <QScopedPointer>
#include <QSharedPointer>

#include "authors.h"
#include "config.h"
#include "library.h"
#include "options.h"
#include "series.h"
//#include "epubmetadata.h"

// QSharedpointer typedefs for epubedit.
typedef QSharedPointer<Config> PConfig;

typedef QSharedPointer<EBookOptions> POptions;

typedef QSharedPointer<EBookSeriesDB> PSeriesDB;

typedef QSharedPointer<EBookAuthorsDB> PAuthorsDB;

typedef QSharedPointer<EBookLibraryDB> PLibraryDB;

typedef QSharedPointer<BookData> PBookData;

typedef QSharedPointer<EPubDocument> PDocument;

typedef QSharedPointer<Metadata> PMetadata;

typedef QSharedPointer<EPubFileAs> PFileAs;

typedef QSharedPointer<Calibre> PCalibre;

typedef QSharedPointer<EPubManifestItem> PManifestItem;
typedef QSharedPointer<Manifest> PManifest;

#endif // POINTERS_H
