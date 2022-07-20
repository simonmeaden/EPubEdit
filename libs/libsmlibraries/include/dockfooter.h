#ifndef DOCKFOOTER_H
#define DOCKFOOTER_H

#include "abstractdockitem.h"

/*!
 * \class DockFooter dockitem.h "include/dockitem"
 * \brief The DockFooter class is a special case of DockItem that is
 *        fixed at the bottom
 *
 * Please note that DockFooter buttons only allow icons, any text is ignored.
 */
class DockFooterPrivate;
class DockFooter : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockFooter)
public:
  explicit DockFooter(AbstractDockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  /*!
   * \brief Adds a ListWidget to the Footer widget with an optional tooltip.
   *
   * Items must be added to the list later as each item can have an optional
   * QIcon. Use ListWidget::addItem(QString), ListWidget::addItem(QIcon,
   * QString) or ListWidget::setItems(QStringList) to add items to the
   * ListWidget. ListWidget::setText(int, QString), ListWidget::setIcon(int,
   * QIcon) and ListWidget::setItem(int, QString, QIcon) to set or reset the
   * values of particular items.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addListWidget(WidgetPosition pos,
                            const QString& tooltip = QString());

  /*!
   * \brief Creates a clone of the DockFooter into item.
   */
  void clone(AbstractDockItem* item);

protected:
  DockFooter(DockFooterPrivate& d);

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class DockWidget;
};

#endif // DOCKFOOTER_H
