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
#include "docker/headerwidget.h"
#include "widgets/iepubeditor.h"

#include "document/bookpointers.h"
#include "document/bookstructs.h"
#include "config.h"

class ContentsFrame;
class EPubDocument;
class EPubEdit;
class HtmlEdit;
class ImageEdit;
class ListWidget;

// class EPubEditorPrivate;
class EPubEditor : public AbstractEPubEditor
{
  Q_OBJECT
  //  Q_DECLARE_PRIVATE(EPubEditor)
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
  EPubEditor* clone(IEPubEditor *editor);

  bool isLoaded();

//  void setHtml(const QString& html);

  //  QUndoView* undoView();
  //  QAction* undoAction();
  //  QAction* redoAction();

  void openCloseClicked();

  void loadHref(const QString& href);

  QSize minimumSize();

  void setCurrentEditor(IEPubEditor::Type type);

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void splitWidget(Qt::Orientation orientation, EPubEditor* widget = nullptr);
  void splitToWindow(QWidget* widget = nullptr);

protected:
  //  explicit EPubEditor(EPubEditorPrivate& d);
  // Both these methods are used by clone().
  //  EPubEditor(PConfig config,
  //             POptions options,
  //             PLibraryDB libraryDB,
  //             PSeriesDB seriesDB,
  //             PAuthorsDB authorsDB,
  //             PDocument document,
  //             QString href,
  //             QWidget* parent = nullptr);
  //  EPubEditorPrivate* d_ptr;

private:
  HtmlEdit* m_codeedit = nullptr;
  EPubEdit* m_epubEdit = nullptr;
  ImageEdit* m_imgEdit = nullptr;
  IEPubEditor* m_currentEditor = nullptr;
  PDocument m_document;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  ListWidget* m_listWidget = nullptr;
  ListWidget* m_typeWidget = nullptr;
  QStringList m_docFileList;

  QString m_currentBookFilename;
  bool m_loaded;
  QList<UniqueString> m_pageOrder;

  void removeSplit(QPoint);
  void dragFile();
  void goBack();
  void goForward();
  void menuClicked(int index, const QString& = QString());
  void split(int index, const QString&);
  void verticalSplit();
  void horizontalSplit();
  void toWindow();
  void fileListClicked(int index, const QString& href);
  void typeChanged(int index, const QString& text);
  //  QPoint adjustWidgetPositionInsideWidget(QPoint originalPos,
  //                                          QSize requiredSize);
  void hasGotFocus(QWidget*);
  void hasLostFocus(QWidget* widget);
  EPubEditor* createAndCloneEditor();
  void setupToolbar();
};

// Q_DECLARE_METATYPE(EPubEditor)

#endif // EPUBEDITOR_H
