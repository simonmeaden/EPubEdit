#ifndef FOOTERWIDGET_H
#define FOOTERWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "docker/abstractdockwidget.h"

class DockFooter;

/*!
 * \ingroup docker
 * \class FooterWidget footerwidget.h "includes/footerwidget.h"
 * \brief The FooterWidget class defines a FooterWidget object.
 *
 * A FooterWidget is a QWidget with a header bar in which various widgets
 * can be placed. The actual widgets are stored in a DockFooter which can
 * be accessed via the footer() method.
 *
 * \sa DockFooter
 */
class FooterWidgetPrivate;
class FooterWidget : public AbstractDockWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(FooterWidget)
public:
  /*!
   * \brief Constructs a FooterWidget which is a child of parent.
   */
  explicit FooterWidget(QWidget* parent = nullptr);

  //! Returns the DockFooter attached to this FooterWidget
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

  //! \brief Hides the widget.
  void hideWidget();
  //! Shows the widget.
  void showWidget();
  //! True if the widget is visible, otherwise false.
  bool isWidgetVisible();

  /*!
   * \brief Creates a clone of the DockWidget, passing it's variable
   * and settings into the supplied 'master'. If the widget parameter is
   * nullptr then it will create a clone of itself.
   */
  QWidget *clone(QWidget *widget) override;

signals:
  //! Triggered when the widgets size changes.
  void sizeChanged(QWidget* widget, const QSize& size);
  //! Triggered when focus is lost.
  void lostFocus(QWidget*);
  //! Triggered whenfocus is gained.
  void gotFocus(QWidget*);

protected:
  //! d_ptr constructor
  FooterWidget(FooterWidgetPrivate& d);
  FooterWidgetPrivate* d_ptr; //!< d_ptr instance

  /// \cond DO_NOT_DOCUMENT
  DockFooter* m_footer;
  /// \endcond DO_NOT_DOCUMENT

  //! \reimplements{QWidget::paintEvent}
  void paintEvent(QPaintEvent* event) override;
  //! Implements a Hover Enter event
  void hoverEnterEvent(QHoverEvent* event);
  //! Implements a Hover Leave event
  void hoverLeaveEvent(QHoverEvent*);
  //! Implements a Hover Move event
  void hoverMoveEvent(QHoverEvent* event);
  //! \reimplements{QWidget::mousePressEvent}
  void mousePressEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::mouseMoveEvent}
  void mouseMoveEvent(QMouseEvent*event) override;
  //! \reimplements{QWidget::mouseReleaseEvent}
  void mouseReleaseEvent(QMouseEvent*event) override;
  //! \reimplements{QWidget::event}
  bool event(QEvent* event) override;
  //! \reimplements{QWidget::resizeEvent}
  void resizeEvent(QResizeEvent* event) override;
  //! Implements QWidget::focusInEvent
  void focusInEvent(QFocusEvent* event) override;
  //! Implements QWidget::focusOutEvent
  void focusOutEvent(QFocusEvent* event) override;

  //! called whenever the widget was clicked
  void widgetWasClicked(QPoint pos);

private:
  int m_width = 0;
  int m_height = 0;
  int m_footerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;

  void calculateGeometry(const QRect& rect);
};

#endif // FOOTERWIDGET_H
