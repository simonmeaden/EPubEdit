#ifndef HEADERFOOTERWIDGETPRIVATE_H
#define HEADERFOOTERWIDGETPRIVATE_H

#include <QPainter>
#include <QWidget>

#include "docker/private/abstractdockwidgetprivate.h"
#include "docker/headerfooterwidget.h"

class DockHeader;
class DockFooter;

class HeaderFooterWidgetPrivate : public AbstractDockWidgetPrivate
{
  Q_DECLARE_PUBLIC(HeaderFooterWidget)
public:
  HeaderFooterWidgetPrivate(HeaderFooterWidget *parent);

  DockHeader *header();
  DockFooter* footer();

  void hideWidget();
  void showWidget();
  bool isWidgetVisible();

  HeaderFooterWidget* clone(HeaderFooterWidget* widget);

  void calculateGeometry(const QRect& rect);

  void paint(QPainter& painter);
  //  //!< Implements a Hover Enter event
  void hoverEnter(QPoint pos);
  //  //!< Implements a Hover Leave event
  void hoverLeave(QPoint);
  //  //!< Implements a Hover Move event
  void hoverMove(QPoint pos);

  void mousePress(QMouseEvent* event) override;
  void mouseMove(QMouseEvent* event) override;
  void mouseRelease(QMouseEvent* event) override;

  //  //!< \reimplements{QWidget::mouseReleaseEvent}
  //  void mouseReleaseEvent(QMouseEvent*) override;
  //  //!< \reimplements{QWidget::event}
  //  bool event(QEvent* event) override;
  //  //!< \reimplements{QWidget::resizeEvent}
  //  void resizeEvent(QResizeEvent* event) override;
  //  //!< Implements QWidget::focusInEvent
  //  void focusInEvent(QFocusEvent* event) override;
  //  //!< Implements QWidget::focusOutEvent
  //  void focusOutEvent(QFocusEvent* event) override;

protected:
  /// \cond DO_NOT_DOCUMENT
  DockHeader* m_header;
  DockFooter* m_footer;
  /// \endcond DO_NOT_DOCUMENT

  /// \cond DO_NOT_DOCUMENT
  void widgetWasClicked(QPoint);
  /// \endcond DO_NOT_DOCUMENT
  ///
private:
  HeaderFooterWidget* d_ptr;
  int m_width = 0;
  int m_height = 0;
  int m_headerHeight = 0;
  int m_footerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;
};

#endif // HEADERFOOTERWIDGETPRIVATE_H
