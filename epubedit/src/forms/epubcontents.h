#ifndef EPUBCONTENTS_H
#define EPUBCONTENTS_H

#include <QWidget>
#include <QListWidget>
#include <QPainter>
#include <QHoverEvent>

#include "document/bookpointers.h"
#include "document/metadata.h"
#include "x11colors.h"


class ContentsFrame : public QWidget
{
  Q_OBJECT


public:
  explicit ContentsFrame(QWidget* parent = nullptr);

  void setDocument(PDocument document);
  void closeBtnClicked();

private:

  void initGui();

  friend class DockHeader;
};

#endif // EPUBCONTENTS_H
