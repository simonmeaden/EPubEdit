#ifndef ABSTRACTDOCKWIDGETPRIVATE_H
#define ABSTRACTDOCKWIDGETPRIVATE_H

#include <QBrush>
#include <QColor>
#include <QHBoxLayout>
#include <QToolTip>

#include "docker/abstractdockwidget.h"

class WidgetItem;
class AbstractDockItem;

/*!
 * \internal
 */
class AbstractDockWidgetPrivate
{
  Q_DECLARE_PUBLIC(AbstractDockWidget)
public:
  explicit AbstractDockWidgetPrivate(AbstractDockWidget* dockWidget);

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

  const QRect& availableRect() const;
  void setAvailableRect(const QRect& newAvailableRect);

  bool dockItemHoverCheck(AbstractDockItem* item, QPoint pos);
  void mouseClickCheck(AbstractDockItem* item, QPoint pos);
  void checkForOpenListWidgets(AbstractDockItem* item);

  virtual AbstractDockWidget* clone(AbstractDockWidget* widget);

  QWidget* widget() const;
  QWidget* setWidget(QWidget* widget);

  AbstractDockWidget* q_ptr = nullptr;
  /// \cond DO_NOT_DOCUMENT
  QWidget* m_parent = nullptr;
  QWidget* m_widget = nullptr;
  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QColor m_textColor;
  QColor m_spacerColor;
  WidgetItem* m_hoverItem = nullptr;
  QRect m_availableRect;
  QHBoxLayout* m_layout = nullptr;
  /// \endcond DO_NOT_DOCUMENT
};

#endif // ABSTRACTDOCKWIDGETPRIVATE_H
