#ifndef DOCKSPLITTER_H
#define DOCKSPLITTER_H

#include <QSplitter>

/*!
 * \ingroup docker
 * \class DockSplitter docksplitter.h "include/docksplitter.h"
 *
 * Extension to QSplitter to handle size changes of the
 * DockWidget / HeaderWidget / FooterWidget / HeaderFooterWidget
 * classes.
 */
class DockSplitter : public QSplitter
{
  Q_OBJECT
public:
  /*!
   * \brief Default constructor for DockSplitter which is a child of parent.
   */
  DockSplitter(QWidget* parent);
  /*!
   * \brief Constructs a DockSplitter which is a child of parent.
   */
  DockSplitter(Qt::Orientation orientation, QWidget* parent);

  /*!
   * \brief Should be called by parent classes when their size changes.
   *
   * \code
   *   // InfoWidget has a sizeChanged signal
   *   m_widget = new InfoWidget(this);
   *   connect(m_widget, &InfoWidget::sizeChanged,
   *           m_mainSplitter, &DockSplitter::widgetSizeChanged);
   * \endcode
   */
  void widgetSizeChanged(QWidget*widget, const QSize &size);

signals:
  //! Emitted when the splitter is dragged.
  void manualSplitterMove();
};

#endif // DOCKSPLITTER_H
