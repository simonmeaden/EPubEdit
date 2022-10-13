#ifndef DRAGGABLEBUTTONWIDGET_H
#define DRAGGABLEBUTTONWIDGET_H

#include "widgets/htmledit.h"
#include <docker/buttonwidget.h>

class DraggableButtonWidgetPrivate;
class DraggableButtonWidget : public ButtonWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DraggableButtonWidget)
public:
  Q_INVOKABLE DraggableButtonWidget(AbstractDockWidget* parent);

  QPoint dragStartPosition() const;
  void setDragStartPosition(QPoint dragStartPosition);

  const QString& mimeType() const;
  //  void setMimeType(const QString &mimeType);

  HtmlButtonType data(const QString& mimetype) const;
  void setData(const QString& mimetype, HtmlButtonType data);

protected:
  QPoint m_dragStartPosition;
  QString m_mimeType;
  HtmlButtonType m_data;

  //! d_ptr constructor.
  DraggableButtonWidget(DraggableButtonWidgetPrivate& d);
};

#endif // DRAGGABLEBUTTONWIDGET_H
