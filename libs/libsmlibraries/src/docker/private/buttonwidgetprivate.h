#ifndef BUTTONWIDGETPRIVATE_H
#define BUTTONWIDGETPRIVATE_H

#include <QIcon>

#include "docker/buttonwidget.h"
#include "docker/private/widgetitemprivate.h"

class ButtonWidgetPrivate : public WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(ButtonWidget)
public:
  ButtonWidgetPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  const QSize& iconSize();
  void setIconSize(const QSize& newIconSize);

  const QString& text();
  void setText(const QString& newText);

  int spacer();
  void setSpacer(int newSpacer);

  const QColor& textColor();
  void setTextColor(const QColor& newTextColor);

  Arrangement arrangement();
  void setArrangement(Arrangement newArrangement);

  WidgetItem* clone(WidgetItem* widget = nullptr);

  bool isShowBorder() const;
  void setShowBorder(bool ShowBorder);

protected:
  // WidgetWrapper interface
  const QSize calcMinimumSize();
  void paint(QPainter& painter);
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect);
  const QRect& iconRect();
  void setIconRect(const QRect& newIconRect);
  const QRect& textRect();
  void setTextRect(const QRect& newTextRect);

private:
  Arrangement m_arrangement = IconOnly;
  QRect m_iconRect;
  QPixmap m_pixmap;
  QIcon::Mode m_iconMode = QIcon::Normal;
  bool m_showBorder = true;
  QSize m_iconSize;
  QRect m_textRect;
  QString m_text;
  int m_spacer = 0;
  QColor m_textColor;
  QList<QMetaObject::Connection> m_connections;

  static int BUTTON_ROUND;
};

#endif // BUTTONWIDGETPRIVATE_H
