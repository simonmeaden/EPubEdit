#ifndef FOOTERWIDGET_H
#define FOOTERWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "abstractdockwidget.h"

class DockItem;

class FooterWidget : public AbstractDockWidget
{
  Q_OBJECT
public:
  explicit FooterWidget(QWidget *parent = nullptr);

  DockItem* footer() const;

  /*!
   * \brief Sets the central widget of the DockWidget.
   *
   * If the new QWidget is replacing an existing widget a pointer tothe old
   * QWidget is returned, otherwise a nullptr is returned. The old QWidget is
   * NOT deleted.
   *
   * \note If the QWidget is replaced with itself, the pointer is returned but
   * nothing else is done. You will need to delete it yourself.
   */
  QWidget* setCentralWidget(QWidget* centralWidget);

  void hideWidget();
  void showWidget();
  bool isWidgetVisible();

  bool showFooterOnHide() const;
  void setShowFooterOnHide(bool show);

signals:
  void sizeChanged(QWidget* widget, const QSize& size);

protected:
  DockItem* m_footer;

  void paintEvent(QPaintEvent*) override;
  void hoverEnterEvent(QHoverEvent* event);
  void hoverLeaveEvent(QHoverEvent*);
  void hoverMoveEvent(QHoverEvent* event);
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  bool event(QEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;

private:
  QWidget* m_centralWidget = nullptr;
  QHBoxLayout* m_layout = nullptr;
  int m_width = 0;
  int m_height = 0;
  int m_footerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;
  bool m_showFooterOnHide = true;

  void calculateGeometry(const QRect& rect);
};

#endif // FOOTERWIDGET_H
