#ifndef IMAGEEDIT_H
#define IMAGEEDIT_H

#include <QWidget>

#include "document/bookpointers.h"
#include "forms/iepubeditor.h"

class ImageEdit : public QWidget, public IEPubEditor
{
  Q_OBJECT
public:
  explicit ImageEdit(PConfig config, QWidget *parent = nullptr);

  // IEPubEditor interface
  const QString &href() const;
  void loadHref(const QString &href);

signals:

protected:
  PConfig m_config;
  QString m_href;

};

#endif // IMAGEEDIT_H
