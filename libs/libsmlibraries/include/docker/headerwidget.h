#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "docker/abstractdockwidget.h"

class DockHeader;

/*!
 * \ingroup docker
 * \class HeaderWidget headerwidget.h "includes/headerwidget.h"
 * \brief The HeaderWidget class defines a HeaderWidget object.
 *
 * A HeaderWidget is a QWidget with a footer bar in which various widgets
 * can be placed. The actual widgets are stored in a DockHeader which can
 * be accessed via the header() method.
 *
 * \sa DockHeader
 */
class HeaderWidgetPrivate;
class HeaderWidget : public AbstractDockWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(HeaderWidget)
public:
  /*!
   * \brief Constructs a HeaderWidget which is a child of parent.
   */
  explicit HeaderWidget(QWidget* parent = nullptr);

  //! Returns the DockHeader attached to this HeaderWidget
  DockHeader* header();

  //  /*!
  //   * \brief Sets the central widget of the DockWidget.
  //   *
  //   * If the new QWidget is replacing an existing widget a pointer tothe old
  //   * QWidget is returned, otherwise a nullptr is returned. The old QWidget
  //   is
  //   * NOT deleted.
  //   *
  //   * \note If the QWidget is replaced with itself, the pointer is returned
  //   but
  //   * nothing else is done. You will need to delete it yourself.
  //   */
  //  QWidget* setWidget(QWidget* centralWidget);

  //! \brief Hides the widget.
  void hideWidget();
  //! Shows the widget.
  void showWidget();
  //! True if the widget is visible, otherwise false.
  bool isWidgetVisible();

  /*!
   * \brief Creates a clone of the HeaderWidget, passing it's variable
   * and settings into the supplied 'master'. If the widget parameter is
   * nullptr then it will create a clone of itself.
   */
  QWidget* clone(QWidget* widget = nullptr) override;

  //! \brief Called by external widgets when they're mouse clicked.
  //!
  //! This slot should be called whenever the widget is clicked by the mouse
  //! if they are using either a ListWidget or a ListButtonWidget.
  //!
  void widgetWasClicked(QPoint pos);

signals:
  //! Triggered when the widgets size changes.
  void sizeChanged(QWidget* widget, const QSize& size);
  //! Triggered when focus is lost.
  void lostFocus(QWidget*);
  //! Triggered whenfocus is gained.
  void gotFocus(QWidget*);

protected:
  //! d_ptr constructor
  HeaderWidget(HeaderWidgetPrivate& d);
  //! d_ptr constructor
  HeaderWidget(HeaderWidgetPrivate& d, QWidget* parent);

  /// \cond DO_NOT_DOCUMENT
  //  DockHeader* m_header;
  /// \endcond DO_NOT_DOCUMENT

  //! \reimplements{QWidget::paintEvent}
  void paintEvent(QPaintEvent* event) override;
  //  //! Implements a Hover Enter event
  //  void hoverEnterEvent(QHoverEvent* event);
  //  //! Implements a Hover Leave event
  //  void hoverLeaveEvent(QHoverEvent*);
  //  //! Implements a Hover Move event
  //  void hoverMoveEvent(QHoverEvent* event);
  //! \reimplements{QWidget::mousePressEvent}
  void mousePressEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::mouseMoveEvent}
  void mouseMoveEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::mouseReleaseEvent}
  void mouseReleaseEvent(QMouseEvent* event) override;
  //! \reimplements{QWidget::event}
  bool event(QEvent* event) override;
  //! \reimplements{QWidget::resizeEvent}
  void resizeEvent(QResizeEvent* event) override;
  //! Implements QWidget::focusInEvent
  void focusInEvent(QFocusEvent* event) override;
  //! Implements QWidget::focusOutEvent
  void focusOutEvent(QFocusEvent* event) override;

private:
  void calculateGeometry(const QRect& rect);
};

#endif // HEADERWIDGET_H
