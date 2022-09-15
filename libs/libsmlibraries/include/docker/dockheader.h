#ifndef DOCKHEADER_H
#define DOCKHEADER_H

#include "docker/abstractdockitem.h"

/*!
 * \ingroup docker
 * \class DockHeader dockheader.h "include/docker/dockheader.h"
 * \brief The DockHeader class is a special case of DockItem that is
 *        fixed at the top
 */
class DockHeaderPrivate;
class DockHeader : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockHeader)
public:
  //! d_ptr constructor
  explicit DockHeader(AbstractDockWidget* parent);

  QSize sizeHint() const;

  void paint(QPainter& painter);

  /*!
   * \brief Adds a ListWidget to the Header widget with an optional tooltip.
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
   * \brief Creates a clone of the DockHeader, passing it's variable
   * and settings into the supplied 'master'.
   */
   void clone(QObject *item);

protected:
   //! d_ptr constructor
   DockHeader(DockHeaderPrivate& d);

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class DockWidget;
};

#endif // DOCKHEADER_H
