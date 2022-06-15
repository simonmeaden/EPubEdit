#ifndef LEFTSIDEBAR_H
#define LEFTSIDEBAR_H

#include <QFrame>
#include <QHBoxLayout>
#include <QComboBox>
#include <QStackedLayout>

#include "document/bookpointers.h"

class TocForm;

class LeftSidebar : public QFrame
{
  Q_OBJECT
public:
  explicit LeftSidebar(QWidget* parent = nullptr);

  void updateMetadataForm(PMetadata metadata);

signals:
  void tocClicked(const QString& href);

private:
  TocForm* m_tocForm;

  void initGui();

};

#endif // LEFTSIDEBAR_H
