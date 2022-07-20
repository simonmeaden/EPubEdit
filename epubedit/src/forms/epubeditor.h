#ifndef EPUBEDITOR_H
#define EPUBEDITOR_H

#include <QMenu>
#include <QObject>
#include <QTextDocument>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>

#include <Logger.h>

#include "abstractepubeditor.h"
#include "config.h"
#include "headerwidget.h"

#include "document/bookpointers.h"
#include "document/bookstructs.h"

class ContentsFrame;
class EPubDocument;
class EPubEdit;
class ListWidget;

class EPubEditorPrivate;
class EPubEditor : public AbstractEPubEditor
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(EPubEditor)
public:
//  explicit EPubEditor(QWidget* parent = nullptr);
  explicit EPubEditor(PConfig config, QWidget* parent);
  ~EPubEditor();

  void setDocument(PDocument document);
  PDocument document();

  void saveConfig();
  void loadConfig();

  /*!
   * \brief Creates a clone of the EPubEditor.
   *
   * Asthe highest parent of the clone this creates an EPubEditor and
   * using lower level clone() methods copies various values into it.
   */
  EPubEditor* clone();

  bool isLoaded();

  void setHtml(const QString& html);

  //  QUndoView* undoView();
  //  QAction* undoAction();
  //  QAction* redoAction();

  void openCloseClicked();

  void loadHref(const QString& href);

  QSize minimumSize();

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void splitWidget(Qt::Orientation orientation, EPubEditor* widget = nullptr);
  void splitToWindow(QWidget* widget = nullptr);

protected:
  explicit EPubEditor(EPubEditorPrivate& d);
  // Both these methods are used by clone().
  //  EPubEditor(PConfig config,
  //             POptions options,
  //             PLibraryDB libraryDB,
  //             PSeriesDB seriesDB,
  //             PAuthorsDB authorsDB,
  //             PDocument document,
  //             QString href,
  //             QWidget* parent = nullptr);
  EPubEditorPrivate* d_ptr;

private:
  Logger* m_logger;

  void removeSplit(QPoint);
  void dragFile();
  void goBack();
  void goForward();
  void menuClicked(int index, const QString& text = QString());
  void verticalSplit();
  void horizontalSplit();
  void toWindow();
  void fileListClicked(int index, const QString& href);
  void typeChanged(int index, const QString& text);
  //  QPoint adjustWidgetPositionInsideWidget(QPoint originalPos,
  //                                          QSize requiredSize);
  void hasGotFocus(QWidget*);
  void hasLostFocus(QWidget* widget);
};

// Q_DECLARE_METATYPE(EPubEditor)

#endif // EPUBEDITOR_H
