#ifndef ABSTRACTDOCKITEMPRIVATE_H
#define ABSTRACTDOCKITEMPRIVATE_H

#include "docker/abstractdockitem.h"

class DraggableButtonWidget;
class AbstractDockItemPrivate
{
  Q_DECLARE_PUBLIC(AbstractDockItem)
public:
  AbstractDockItemPrivate(DockPosition position,
                          AbstractDockWidget* parent,
                          AbstractDockItem* qptr);

  ButtonWidget* addIconButton(WidgetPosition pos,
                              QPixmap pixmap,
                              const QString& tooltip = QString());
  DraggableButtonWidget* addDragIconButton(WidgetPosition pos,
                              QPixmap pixmap,
                               const QString& tooltip = QString());
  ButtonWidget* addIconListButton(WidgetPosition pos,
                                  QPixmap pixmap,
                                  const QString& tooltip = QString());
  ButtonWidget* addIconTextButton(WidgetPosition pos,
    QPixmap pixmap,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());
  ButtonWidget* addIconTextListButton(
    WidgetPosition pos,
    QPixmap pixmap,
    const QString& text,
    Arrangement textPos = Arrangement::TextBelowIcon,
    const QString& tooltip = QString());
  ButtonWidget* addTextButton(WidgetPosition pos,
                              const QString& text,
                              const QString& tooltip = QString());
  ButtonWidget* addTextListButton(WidgetPosition pos,
                                  const QString& text,
                                  const QString& tooltip = QString());
  WidgetItem* addTextLabel(WidgetPosition pos,
                           const QString& text,
                           const QString& tooltip = QString());

  WidgetItem* addSeperator(WidgetPosition);
  WidgetItem* addCustomWidget(CustomWidget* w);

  void setText(int index, const QString& text);
  void setToolTip(int index, const QString& tooltip);
  WidgetItem* takeAt(int index);
  WidgetItem* at(int index);
  int count();
  bool replace(int index, WidgetItem* w);
  bool widgetEnabled(int index);
  bool enableWidget(int index, bool value);
  bool selected(int index);
  bool setSelected(int index, bool value);
  QMargins widgetMargins(int index);
  void setWidgetMargins(int index, int left, int top, int right, int bottom);
  void setPreferredSize(int width, int height);
  void setPreferredSize(QSize size);
  void setPreferredWidth(int width);
  void setPreferredHeight(int height);

  DockPosition dockPosition() const;
  void setDockPosition(DockPosition position);

  int maxWidgetWidth() const;
  int maxWidgetHeight() const;

  int height() const;
  int width() const;
  void setWidth(int width);
  void setHeight(int height);

  bool isVisible() const;
  void setVisible(bool newShow);
  void show();
  void hide();

  virtual void paint(QPainter& painter);

  const QList<WidgetItem*>& widgets() const;

  virtual void calcMaxWidgetSizes();

  const QRect& rect() const;

  virtual QSize sizeHint() const { return QSize(); }

  void clone(AbstractDockItem* item);

  QMargins calcMaxMargins();
  QSize calcMaxContentsSize();
  void calculateGeometry(const QRect& rect);

  const QMargins &margins() const;
  void setMargins(const QMargins &margins);

  WidgetItem* addItem(WidgetItem* item);

  ButtonWidget* createButtonItem(WidgetType type,
                                 WidgetPosition pos,
                                 QPixmap icon,
                                 const QString& text,
                                 Arrangement textPos,
                                 const QString& tooltip);
  void setupButton(ButtonWidget* bw,
                   QPixmap icon,
                   const QString& text,
                   Arrangement textPos,
                   const QString& tooltip);

  AbstractDockItem* q_ptr = nullptr;
  AbstractDockWidget* m_parentDocker = nullptr;
  DockPosition m_dockPosition;
  QList<WidgetItem*> m_widgets;
  bool m_visible = true;
  QRect m_rect;

  int m_width;
  int m_height;
  int m_maxWidgetWidth = 0;
  int m_maxWidgetHeight = 0;
  //! margins
  QMargins m_margins;
  int m_spacer = SPACER; //!< space between widgets

  int spacer() const;
  void setSpacer(int Spacer);

private:
  void calculateMaxSize(ButtonWidget* bw, QString text, Arrangement textPos);
};

#endif // ABSTRACTDOCKITEMPRIVATE_H
