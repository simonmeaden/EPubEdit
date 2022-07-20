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

class MultiSplitter : public QSplitter
{
  Q_OBJECT

public:
  explicit MultiSplitter(QWidget* parent = nullptr);

  void saveState();

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

  QWidget *createSplit(Qt::Orientation orientation, QWidget* newWidget = nullptr);
  void createSplitToWindow();

  QList<QWidget*> widgets();

  QSplitter* splitter(QWidget* widget) const;

signals:

protected:
  QHBoxLayout* m_layout;
  QMap<QWidget*, QSplitter*> m_widgetMap;
  QWidget* m_currentWidget = nullptr;
  QSplitter* m_baseSplitter;

  QSplitter* currentSplitter();

private:

};

#endif // MULTISPLITTER_H
