#ifndef BUTTONWIDGETPRIVATE_H
#define BUTTONWIDGETPRIVATE_H

#include <QIcon>

#include "buttonwidget.h"
#include "widgetitemprivate.h"

class ButtonWidgetPrivate : public WidgetItemPrivate
{
  Q_DECLARE_PUBLIC(ButtonWidget)
public:
  ButtonWidgetPrivate(AbstractDockWidget* parent, WidgetItem* qptr);

  const QIcon& icon();
//  void setIcon(const QIcon& newIcon);

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
  QIcon m_icon;
  QIcon::Mode m_iconMode = QIcon::Normal;
  QSize m_iconSize;
  QRect m_textRect;
  QString m_text;
  int m_spacer = 0;
  QColor m_textColor;
  QList<QMetaObject::Connection> m_connections;
};

#endif // BUTTONWIDGETPRIVATE_H
