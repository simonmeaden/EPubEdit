#ifndef ABSTRACTDOCKWIDGET_H
#define ABSTRACTDOCKWIDGET_H

#include <QWidget>
#include <QToolTip>

#include "docktypes.h"

class DockItem;
class WidgetItem;

class AbstractDockWidget : public QWidget
{
  Q_OBJECT
public:
  explicit AbstractDockWidget(QWidget *parent = nullptr);

  const QBrush& backColor() const;
  void setBackColor(const QBrush& newBackColor);

  const QBrush& hoverBackColor() const;
  void setHoverBackColor(const QBrush& newHoverBackColor);

  const QBrush& selectedColor() const;
  void setSelectedColor(const QBrush& newSelectedColor);

  const QColor& textColor() const;
  void setTextColor(const QColor& newTextColor);

  const QColor& spacerColor() const;
  void setSpacerColor(const QColor& newSpacerColor);

signals:

protected:
  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QColor m_textColor;
  QColor m_spacerColor;
  WidgetItem* m_hoverItem = nullptr;

  bool dockItemHoverCheck(DockItem* item, QPoint pos);
  void mouseClickCheck(DockItem* item, QPoint pos);


};

#endif // ABSTRACTDOCKWIDGET_H
