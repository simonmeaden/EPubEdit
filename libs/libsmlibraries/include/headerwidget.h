#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QHBoxLayout>
#include <QWidget>

#include "abstractdockwidget.h"

class DockItem;

class HeaderWidget : public AbstractDockWidget
{
  Q_OBJECT
public:
  explicit HeaderWidget(QWidget* parent = nullptr);

  DockItem* header() const;

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

  bool showHeaderOnHide() const;
  void setShowHeaderOnHide(bool show);

signals:
  void sizeChanged(QWidget* widget, const QSize& size);

protected:
  DockItem* m_header;

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
  int m_headerHeight = 0;
  QSize m_hiddenSize;
  QSize m_visibleSize;
  bool m_showHeaderOnHide = true;

  void calculateGeometry(const QRect& rect);
};

#endif // HEADERWIDGET_H
