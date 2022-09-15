#ifndef HEADERFOOTERWIDGET_H
#define HEADERFOOTERWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "docker/abstractdockwidget.h"

class DockHeader;
class DockFooter;

/*!
 * \ingroup docker
 * \class HeaderFooterWidget headerfooterwidget.h
 * "includes/headerfooterwidget.h" \brief The HeaderWidget class defines a
 * HeaderWidget object.
 *
 * A HeaderFooterWidget is a QWidget with both a header a footer bar in which
 * various widgets* can be placed. The actual widgets are stored in a DockHeader
 * and a DockFooter which can be accessed via the header() and footer() methods
 * respectively.
 *
 * \sa DockHeader
 * \sa DockFooter
 */
class HeaderFooterWidgetPrivate;
class HeaderFooterWidget : public AbstractDockWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(HeaderFooterWidget)
public:
  /*!
   * \brief Constructs a HeaderFooterWidget which is a child of parent.
   */
  HeaderFooterWidget(QWidget* parent);

  //! Returns the DockHeader attached to this HeaderWidget
  DockHeader *header();
  //! Returns the DockFooter attached to this HeaderWidget
  DockFooter* footer();

//  /*!
//   * \brief Sets the central widget of the DockWidget.
//   *
//   * If the new QWidget is replacing an existing widget a pointer tothe old
//   * QWidget is returned, otherwise a nullptr is returned. The old QWidget is
//   * NOT deleted.
//   *
//   * \note If the QWidget is replaced with itself, the pointer is returned but
//   * nothing else is done. You will need to delete it yourself.
//   */
//  QWidget* setWidget(QWidget* widget);

  /*! \brief Hides the widget.
   *  if showHeaderOnHide is true then the header remains visible, otherwise
   *  the entire widget is hidden.
   */
  void hideWidget();
  //! Shows the widget.
  void showWidget();
  //! True if the widget is visible, otherwise false.
  bool isWidgetVisible();

  /*!
   * \brief Creates a clone of the HeaderFooterWidget, passing it's variable
   * and settings into the supplied 'master'. If the widget parameter is
   * nullptr then it will create a clone of itself.
   */
  QWidget *clone(QWidget *widget) override;

signals:
  //! Triggered when the widgets size changes.
  void sizeChanged(QWidget* widget, const QSize& size);
  //! Triggered when focus uis lost.
  void lostFocus(QWidget*);
  //! Triggered whenfocus is gained.
  void gotFocus(QWidget*);

protected:
  //! d_ptr constructor
  HeaderFooterWidget(HeaderFooterWidgetPrivate& d);
  //! \reimplements{QWidget::paintEvent}
  void paintEvent(QPaintEvent* event) override;
  //! Implements a Hover Enter event
  void hoverEnterEvent(QHoverEvent* event);
  //! Implements a Hover Leave event
  void hoverLeaveEvent(QHoverEvent* event);
  //! Implements a Hover Move event
  void hoverMoveEvent(QHoverEvent* event);
  //! \reimplements{QWidget::mousePressEvent}
  void mousePressEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::mouseReleaseEvent}
//  void mouseReleaseEvent(QMouseEvent*) override;
  //! \reimplements{QWidget::event}
  bool event(QEvent* event) override;
  //! \reimplements{QWidget::resizeEvent}
  void resizeEvent(QResizeEvent* event) override;
  //! Implements QWidget::focusInEvent
  void focusInEvent(QFocusEvent* event) override;
  //! Implements QWidget::focusOutEvent
  void focusOutEvent(QFocusEvent* event) override;

  /// \cond DO_NOT_DOCUMENT
  void widgetWasClicked(QPoint);
  /// \endcond DO_NOT_DOCUMENT
  ///
private:
  //  void calculateGeometry(const QRect& rect);
};

#endif // HEADERFOOTERWIDGET_H
