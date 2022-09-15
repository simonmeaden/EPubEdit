#ifndef EPUBEDITORPRIVATE_H
#define EPUBEDITORPRIVATE_H

#include "config.h"
#include "document/bookpointers.h"
#include "document/bookstructs.h"
#include "forms/epubedit.h"
#include "forms/epubeditor.h"
#include "forms/private/abstractepubeditorprivate.h"
#include "listbuttonwidget.h"
#include "listwidget.h"

class EPubEditorPrivate : public AbstractEPubEditorPrivate
{
  Q_DECLARE_PUBLIC(EPubEditor)
public:
  EPubEditorPrivate(PConfig config, EPubEditor* parent);

  void setDocument(PDocument document);

  void saveConfig();
  void loadConfig();

  AbstractDockWidget* clone(AbstractDockWidget* editor);

  //  bool isLoaded() const;

  void setHtml(const QString& html);

  //  QUndoView* undoView();
  //  QAction* undoAction();
  //  QAction* redoAction();

  void openCloseClicked();

  QTextCursor currentCursor();

  QSize minimumSize() const;

//  void loadHref(const QString& href);

  void removeSplit();
  void dragFile();
  void goBack();
  void goForward();
  void menuClicked(int index, const QString&);
  void verticalSplit();
  void horizontalSplit();
  void toWindow();
  void fileListClicked(int index, const QString& href);
  QPoint adjustWidgetPositionInsideWidget(QPoint originalPos,
                                          QSize requiredSize);

private:
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
  EPubEditor* createAndCloneEditor();
};

#endif // EPUBEDITORPRIVATE_H
