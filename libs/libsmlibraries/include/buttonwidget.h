#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include "widgetitem.h"

/*!
 * \class ButtonWidget  widgetitem.h widgetitem.cpp
 * \brief The ButtonWidget class supplies Buttons for the various DockItem's.
 *
 * ButtonWidget's act in a similar fashion to QToolButton's but are intended to
 * interact with the various DockItem subclasses. ButtonWidget's can have one of
 * several forms. A simple icon button, a text button or combined text/icon
 * buttons. For combined text/icon buttons text can be above, below or to the
 * left or right.
 *
 * Preferably one of the various DockItem::add... methods, such as
 * addIconTextButton(...) should be used to create the buttons as they
 * assure the buttons are properly created.
 *
 * \note text to the left/right on vertical toolbars or above/below on
 * horizontal toolbars/headers/footers are possible but cause the
 * toolbar/header/footer to widen to accomodate the text.
 *
 * \sa ListButtonWidget
 */
class ButtonWidgetPrivate;
class ButtonWidget : public WidgetItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(ButtonWidget)
public:
  /*!
   * Construct a ButtonWidget that is a child of parent.
   *
   * parent connot be a nullptr, the item must have a parent widget.
   */
  ButtonWidget(AbstractDockWidget* parent);

  //! Returns the icon of the label.
  const QIcon& icon();
  //! Sets the icon of the label.
  void setIcon(const QIcon& icon);

  /*!
   * \brief Returns the size the icon of the label.
   *
   * This needs to be specified as icons can have multiple sizes.
   */
  const QSize& iconSize() ;
  /*!
   * \brief Sets the size the icon of the label.
   *
   * This needs to be specified as icons can have multiple sizes.
   */
  void setIconSize(const QSize& iconSize);

  //! Returns the text of the button
  const QString& text();
  //! Sets the text of the button
  void setText(const QString& newText);

  //! Returns the spacer width.
  int spacer();
  /*! \brief Sets the spacer width.
   *
   *   This only sets the space between icon and text so is not used
   *   if text only or icon only buttons are created.
   */
  void setSpacer(int spacer);

  //! returns the text color of the button
  const QColor& textColor();
  //! Sets the text color of the button
  void setTextColor(const QColor& textColor);

  //! Returns the arrangement of the text/icon combination.
  Arrangement arrangement();
  /*!
   * \brief Sets the arrangement of the text/icon combination.
   *
   * Valid values are   TextAboveIcon, TextBelowIcon, TextAboveAndBelow,
   * TextToRight or TextToLeft.
   *
   * \note TextAboveAndBelow is not yet implemented.
   */
  void setArrangement(Arrangement arrangement);

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  WidgetItem* clone(WidgetItem* widget = nullptr) override;

  void paint(QPainter& painter) override;

protected:
  /// \cond DO_NOT_DOCUMENT
  ButtonWidget(ButtonWidgetPrivate& d);

  // WidgetWrapper interface
  const QSize calcMinimumSize() override;
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect) override;
  const QRect& iconRect();
  void setIconRect(const QRect& iconRect);
  const QRect& textRect() ;
  void setTextRect(const QRect& textRect);
  /// \endcond DO_NOT_DOCUMENT
};

#endif // BUTTONWIDGET_H
