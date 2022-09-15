#ifndef CLONEABLEINTERFACE_H
#define CLONEABLEINTERFACE_H

#include <QWidget>

/*!
 * \ingroup widget
 * \brief Base class for all cloneable classes.
 *
 * There are two primary Cloneable interfaces, One for QWidget based bojects and
 * one for QObject based objects.
 *
 * \sa CloneableWidgetInterface
 * \sa CloneableObjectInterface
 */
class CloneableInterface
{
public:
  CloneableInterface() {}
};

/*!
 * \ingroup widget
 * \brief Creates an interface that allows your QWidget extensions to clone
 *        themselves.
 *
 * Intended for use by the MultiSplitter widget. When the splitter split's
 * a QWidget it checks the existing widget, and if it extends the CloneableWidgetInterface
 * class it will be cloned, placing a copy of the widget in each split. Otherwise
 * it will attempt to create a copy of the QWidget.
 */
class CloneableWidgetInterface : public CloneableInterface
{
public:
  CloneableWidgetInterface() {}

  /*!
   * \brief implement this method in your cloneable classes. This will allow MultiSplitter
   * to create a clone of your QWidget.
   */
  virtual QWidget* clone(QWidget* widget) = 0;
};

/*!
 * \ingroup widget
 * \brief Creates an interface that allows your QObject extensions to clone
 *        themselves.
 *
 * Intended for use by the MultiSplitter widget. When the splitter split's
 * a QWidget it checks the existing widget, and if it extends the CloneableObjectInterface
 * class it will be cloned, placing a copy of the widget in each split. Otherwise
 * it will attempt to create a copy of the QWidget.
 */
class CloneableObjectInterface : public CloneableInterface
{
public:
  CloneableObjectInterface() {}

  /*!
   * \brief implement this method in your cloneable classes. This will allow MultiSplitter
   * to create a clone of your QWidget.
   */
  virtual void clone(QObject* object) = 0;
};

#endif // CLONEABLEINTERFACE_H
