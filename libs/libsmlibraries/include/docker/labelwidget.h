#ifndef LABELWIDGET_H
#define LABELWIDGET_H

#include <QColor>

#include "docker/widgetitem.h"

/*!
 * \ingroup docker
 * \class LabelWidget  widgetitem.h widgetitem.cpp
 * \brief The LabelWidget class supplies a simple label widget.
 */
class LabelWidgetPrivate;
class LabelWidget : public WidgetItem
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(LabelWidget)
public:
  /*!
   * Construct a LabelWidget that is a child of parent.
   *
   * parent connot be a nullptr, the item must have a parent widget.
   */
  Q_INVOKABLE LabelWidget(AbstractDockWidget* parent);

  //! Returns the text of the label.
  const QString& text();
  //! Sets the text of the label.
  void setText(const QString& text);

  //! Returns the text color of the label.
  const QColor& textColor();
  //! Sets the text color of the label.
  void setTextColor(const QColor& textColor);

  /*!
   * \brief Creates a clone of the WidgetItem, passing it's variables
   * and settings into the supplied 'master'.
   */
  void clone(QObject* widget = nullptr) override;

  void paint(QPainter& painter) override;

protected:
  /// \cond DO_NOT_DOCUMENT
  LabelWidget(LabelWidgetPrivate& d);

  // WidgetWrapper interface
  const QSize calcMinimumSize() override;
  void setGeometry(const QRect& widgetRect, const QRect& contentsRect) override;
  QSize sizeHint();
  /// \endcond DO_NOT_DOCUMENT
};

#endif // LABELWIDGET_H
