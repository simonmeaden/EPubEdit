#ifndef ABSTRACTEPUBEDITORPRIVATE_H
#define ABSTRACTEPUBEDITORPRIVATE_H

#include "../abstractepubeditor.h"
#include "document/bookpointers.h"
#include "private/headerwidgetprivate.h"

class AbstractEPubEditorPrivate : public HeaderWidgetPrivate
{
  Q_DECLARE_PUBLIC(AbstractEPubEditor)
public:
  AbstractEPubEditorPrivate(PConfig config, AbstractEPubEditor* parent);

  QTextDocument* document();
  QTextCursor currentCursor();

  const QString& href() const;
  void loadHref(const QString& href);

protected:
  PConfig m_config;
  QString m_href;
  QAbstractScrollArea* m_editor;
};

#endif // ABSTRACTEPUBEDITORPRIVATE_H
