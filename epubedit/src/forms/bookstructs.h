#ifndef BOOKSTRUCTS_H
#define BOOKSTRUCTS_H

struct Page
{
  UniqueString idref;
  QString mediaType;
  QString page; // possibly entire page??
  QString path;
};

#endif // BOOKSTRUCTS_H
