#ifndef SEPERATORWIDGET_H
#define SEPERATORWIDGET_H

#include "docker/widgetitem.h"

/*!
 * \ingroup docker
 * \class SeperatorWidget  widgetitem.h widgetitem.cpp
 * \brief The SeperatorWidget class is used as a visual indicator of seperate
 * widget types.
 */
class SeperatorWidgetPrivate;
class SeperatorWidget : public WidgetItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(SeperatorWidget)
public:
  /*!
   * Construct a SeperatorWidget that is a child of parent.
   *
   * parent connot be a nullptr, the item must have a parent widget.
   */
  Q_INVOKABLE SeperatorWidget(AbstractDockWidget* parent);

  //! Returns the color of the seperator bar.
   const QColor& color();
  //! Sets the color of the seperator bar.
  void setColor(const QColor& color);

  //! Returns the thickness of the seperator bar.
  int thickness();
  //! Sets the thickness of the seperator bar.
  void setThickness(int newThickness);

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(QObject* item = nullptr) override;

  void paint(QPainter& painter) override;

protected:
  /// \cond DO_NOT_DOCUMENT
  SeperatorWidget(SeperatorWidgetPrivate& d);

  // WidgetWrapper interface
  const QSize calcMinimumSize() override;
  /// \endcond DO_NOT_DOCUMENT
};
#endif // SEPERATORWIDGET_H
