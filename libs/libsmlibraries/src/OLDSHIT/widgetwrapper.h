#ifndef WIDGETWRAPPER_H
#define WIDGETWRAPPER_H

#include <QIcon>
#include <QPainter>
#include <QWidget>

#include "widgetenum.h"

class WidgetWrapper : public QObject
{
  Q_OBJECT
public:
  WidgetWrapper(QObject* parent = nullptr);

signals:
  void widgetClicked();
  /*!
   * \brief  This signal is emitted when the widget is changed in any way that
   *         would modify the layout. This could be a size change, a margin
   *         change or an arrangement change.
   */
  void widgetChanged();

public:
  static const int TOPMARGIN = 3;
  static const int BOTTOMMARGIN = 3;
  static const int LEFTMARGIN = 3;
  static const int RIGHTMARGIN = 3;
  static const int TEXT_SPACER = 2;

  const QMargins& margins() const;
  void setMargins(const QMargins& newMargins);
  int leftMargin();
  void setLeftMargin(int margin);
  int rightMargin();
  void setRightMargin(int margin);
  int topMargin();
  void setTopMargin(int margin);
  int bottomMargin();
  void setBottomMargin(int margin);

  bool isEnabled() const;
  void setEnabled(bool newEnabled);

  bool isSelected() const;
  void setSelected(bool newSelected);

  const QRect& rect() const;
  void setRect(const QRect& newRect);

  WidgetType type() const;
  void setType(WidgetType newType);

  WidgetPosition position() const;
  void setPosition(WidgetPosition newPosition);

  Arrangement arrangement() const;
  void setArrangement(Arrangement newArrangement);

  const QString& tooltip() const;
  void setTooltip(const QString& newTooltip);

  bool isHoverOver() const;
  void setHoverOver(bool newHoverOver);

  virtual const QSize calcSize() = 0;
  virtual void paintWidget(QPainter& painter) = 0;
  virtual void resizeEvent(QRect& r, int& min, int& max) = 0;

  void paintBackground(QPainter& painter);

  const QBrush& backColor() const;
  void setBackColor(const QBrush& newBackColor);

  const QBrush& hoverBackColor() const;
  void setHoverBackColor(const QBrush& newHoverBackColor);

  const QBrush& selectedColor() const;
  void setSelectedColor(const QBrush& newSelectedColor);

  const QSize &content() const;

protected:
  QBrush m_backColor;
  QBrush m_hoverBackColor;
  QBrush m_selectedColor;
  QRect m_rect;
  WidgetType m_type = Button;
  WidgetPosition m_position = Left;
  Arrangement m_arrangement = IconOnly;
  QString m_tooltip;
  bool m_hoverOver = false;
  bool m_selected = false;
  bool m_enabled = true;
  QMargins m_margins;
  QSize m_content;

  int halfDifference(int large, int small);
};

class SpacerWrapper : public WidgetWrapper
{
  Q_OBJECT
public:
  SpacerWrapper(QObject* parent = nullptr);

  const QColor& color() const;
  void setColor(const QColor& newColor);

  int thickness() const;
  void setThickness(int newThickness);

  // WidgetWrapper interface
  const QSize calcSize();
  void paintWidget(QPainter& painter);
  void resizeEvent(QRect& r, int& min, int& max);

private:
  QColor m_color;
  int m_thickness = 1;
};

class CustomWidgetWrapper : public WidgetWrapper
{};

class ButtonWrapper : public WidgetWrapper
{
  Q_OBJECT
public:
  ButtonWrapper(QObject* parent = nullptr);

  const QRect& iconRect() const;
  void setIconRect(const QRect& newIconRect);

  const QIcon& icon() const;
  void setIcon(const QIcon& newIcon);

  const QSize& iconSize() const;
  void setIconSize(const QSize& newIconSize);

  const QRect& textRect() const;
  void setTextRect(const QRect& newTextRect);

  const QString& text() const;
  void setText(const QString& newText);

  int spacer() const;
  void setSpacer(int newSpacer);

  const QColor& textColor() const;
  void setTextColor(const QColor& newTextColor);

  // WidgetWrapper interface
  const QSize calcSize();
  void paintWidget(QPainter& painter);
  void resizeEvent(QRect& r, int& min, int& max);

private:
  QRect m_iconRect;
  QIcon m_icon;
  QSize m_iconSize;
  QRect m_textRect;
  QString m_text;
  int m_spacer = TEXT_SPACER;
  QColor m_textColor;
};

#endif // WIDGETWRAPPER_H
