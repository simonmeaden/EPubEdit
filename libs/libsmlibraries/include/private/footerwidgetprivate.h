#ifndef FOOTERWIDGETPRIVATE_H
#define FOOTERWIDGETPRIVATE_H

#include "private/abstractdockwidgetprivate.h"
#include "footerwidget.h"

class DockFooter;

class FooterWidgetPrivate : public AbstractDockWidgetPrivate
{
  Q_DECLARE_PUBLIC(FooterWidget)
public:
  FooterWidgetPrivate(FooterWidget *parent);

  //! Returns the DockFooter attached to this FooterWidget
  DockFooter *footer();
  QWidget* setWidget(QWidget* widget);
  void hideWidget();
  void showWidget();
  bool isWidgetVisible();
  AbstractDockWidget *clone(AbstractDockWidget* widget);

protected:
  /// \cond DO_NOT_DOCUMENT
  DockFooter* m_footer;
  /// \endcond DO_NOT_DOCUMENT

  void paint(QPainter &painter) ;
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);
  void mousePress(QPoint pos) ;
//  void mouseReleaseEvent(QMouseEvent*) override;
//  bool event(QEvent* event) override;
//  void resizeEvent(QResizeEvent* event) override;
//  void focusInEvent(QFocusEvent* event) override;
//  void focusOutEvent(QFocusEvent* event) override;

  void widgetWasClicked(QPoint);

private:
  int m_width = 0;
  int m_height = 0;
  int m_footerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;

  void calculateGeometry(const QRect& rect);
};

#endif // FOOTERWIDGETPRIVATE_H
