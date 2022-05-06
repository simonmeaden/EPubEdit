#ifndef EPUBCONTENTS_H
#define EPUBCONTENTS_H

#include <QWidget>
#include <QListWidget>
#include <QPainter>
#include <QHoverEvent>

#include "borderlayout.h"
#include "document/bookpointers.h"
#include "document/epubmetadata.h"
#include "x11colors.h"

class HeaderWidget;

class ContentsFrame : public QWidget
{
  Q_OBJECT


public:
  explicit ContentsFrame(QWidget* parent = nullptr);

  void setDocument(PDocument document);
  void closeBtnClicked();

private:
  HeaderWidget* m_header;

  void initGui();

  friend class HeaderWidget;
};

#endif // EPUBCONTENTS_H
