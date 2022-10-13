#ifndef DOCKFOOTER_H
#define DOCKFOOTER_H

#include "docker/abstractdockitem.h"

/*!
 * \ingroup docker
 * \class DockFooter dockfooter.h "include/docker/dockfooter.h"
 * \brief The DockFooter class is a special case of DockItem that is
 *        fixed at the bottom
 *
 * Please note that DockFooter buttons only allow icons, any text is ignored.
 */
class DockFooterPrivate;
class DockFooter
  : public AbstractDockItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(DockFooter)
public:
  //! d_ptr constructor
  explicit DockFooter(AbstractDockWidget* parent);

  /*!
   * \brief Adds an icon only ButtonWidget to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconButton(WidgetPosition pos,
                              QPixmap pixmap,
                              const QString& tooltip = QString());
  /*!
   * \brief Adds an draggable icon only ButtonWidget to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addDragIconButton(WidgetPosition pos,
                              QPixmap pixmap,
                              const QString& tooltip = QString());
  /*!
   * \brief Adds an icon only ListButtonWidget to the Header/Footer/Toolbar
   * widget with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconListButton(WidgetPosition pos,
                                  QPixmap pixmap,
                                  const QString& tooltip = QString());

  /*!
   * \brief Adds an icon with text ButtonWidget to the Header/Footer/Toolbar
   * widget and an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconTextButton(WidgetPosition pos,
    QPixmap pixmap,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());
  /*!
   * \brief Adds an icon with text ListButtonWidget to the Header/Footer/Toolbar
   * widget and an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addIconTextListButton(WidgetPosition pos,
    QPixmap pixmap,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());

  /*!
   * \brief Adds an text only ButtonWidget to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addTextButton(WidgetPosition pos,
                              const QString& text,
                              const QString& tooltip = QString());
  /*!
   * \brief Adds an text only ListButtonWidget to the Header/Footer/Toolbar
   * widget with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  ButtonWidget* addTextListButton(WidgetPosition pos,
                                  const QString& text,
                                  const QString& tooltip = QString());

  /*!
   * \brief Adds a text label to the Header/Footer/Toolbar widget
   * with an optional tooltip.
   *
   * \return a pointer to the stored wrapper.
   */
  WidgetItem* addTextLabel(WidgetPosition pos,
                           const QString& text,
                           const QString& tooltip = QString());

  /*!
   * \brief addSpacer
   * \return
   */
  WidgetItem* addSeperator(WidgetPosition);

  /*!
   * \brief Adds a custom widget to the Header/Footer/Toolbar widget.
   */
  WidgetItem* addCustomWidget(CustomWidget* w);

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
  void clone(QObject* item);

protected:
  //! d_ptr constructor
  DockFooter(DockFooterPrivate& d);

private:
  static const int HEIGHT = 26;
  static const int WIDTH = 26;

  friend class HeaderWidget;
  friend class FooterWidget;
  friend class DockWidget;
};

#endif // DOCKFOOTER_H
