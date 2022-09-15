#ifndef MULTISPLITTER_H
#define MULTISPLITTER_H

#include <QHBoxLayout>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QSplitter>
#include <QWidget>
#include <QFocusEvent>

class AbstractEPubEditor;

/*!
 * \ingroup widgets
 * \class MultiSplitter multisplitter.h "include/multisplitter/multisplitter.h"
 * \brief A multidirectional splitter that works in a similar way to the editor
 *        panes in QtCreator.
 *
 * The MultiSplitter is actaully an array of QWidget's and QSplitters. Initially it
 * holds a single QWidget. When the widget is split it replaces the widget with a
 * QSplitter, and places the QWidget and a second QWidget.
 *
 * Classes that intend to be used in MultiSplitter should implement the
 * CloneableWidgetInterface. This creates a cloneable widget so that when a widget is
 * 'split' either vertically or horizontally a clone of the forst widget is created
 * and placed into the second split.
 */
class MultiSplitter : public QSplitter
{
  Q_OBJECT

public:
  //! Constructor for MultiSplitter.
  explicit MultiSplitter(QWidget* parent = nullptr);

  //! Saves the splitter state to file.
  virtual void saveState();

  //! returns the currently selected QWidget.
  QWidget* currentWidget();

  /*!
   * \brief Sets the new QWidget at the current position.
   *
   * If there is a current widget then the current widget is replaced.
   * The previous current QWidget will be returned, or nullptr if there
   * was no set widget.
   */
  QWidget* setCurrentWidget(QWidget *widget);
  /*!
   * \brief Returns the current QWidget.
   */
  QWidget* currentWidget() const;

  /*!
   * \brief Splits the current widget into two, depending on the orientation.
   */
  QWidget *createSplit(Qt::Orientation orientation, QWidget* newWidget = nullptr);

  //! Removes the current widget from the splitter and places into a new window.
  void openToNewWindow();

  /*!
   * \brief Returns the collection of QWidget objects in this spiltter.
   */
  QList<QWidget*> widgets();

  //! returns the actual splitter that contains this QWidget.
  QSplitter* splitter(QWidget* widget) const;

signals:

protected:
  QHBoxLayout* m_layout;
  QMap<QWidget*, QSplitter*> m_widgetMap;
  QWidget* m_currentWidget = nullptr;
  QSplitter* m_baseSplitter;

private:
  QSplitter* currentSplitter();

};

#endif // MULTISPLITTER_H
