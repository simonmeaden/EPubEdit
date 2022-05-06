#ifndef EPUBEDIT_H
#define EPUBEDIT_H

#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFormLayout>
#include <QImageReader>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QSharedPointer>
#include <QSplitter>
#include <QStandardPaths>
#include <QTabWidget>
#include <QTextEdit>
#include <QUndoStack>
#include <QUndoView>
#include <QWidget>

#include "borderlayout.h"
#include "config.h"
#include "document/bookpointers.h"
#include "forms/metadataform.h"

class EPubEditor;
class ContentsFrame;

struct Page
{
  UniqueString idref;
  QString mediaType;
  QString page; // possibly entire page??
  QString path;
};

class FooterWidget;

class EPubEdit : public QWidget
{
  Q_OBJECT

//  class FooterWidget : public QWidget
//  {
//  public:
//    explicit FooterWidget(QWidget* parent = nullptr);

//  protected:
//    void paintEvent(QPaintEvent* event);
//    void resizeEvent(QResizeEvent* event);
//    void hoverEnter(QHoverEvent* event);
//    void hoverLeave(QHoverEvent* event);
//    void hoverMove(QHoverEvent* event);
//    void mousePressEvent(QMouseEvent* event);
//    void mouseReleaseEvent(QMouseEvent* event);
//    bool event(QEvent* event);

//  private:
//    EPubEdit* m_parent;
//    QRect m_buttonRect, m_frameRect;
//    QBrush m_back, m_hoverBack;
//    bool m_hoverOverButton = false;

//    static const int HEIGHT = 20;
//    static const int BUTTON_WIDTH = 19;
//  };

public:
  explicit EPubEdit(PConfig config, QWidget* parent = nullptr);
  ~EPubEdit();

  void saveConfig();
  void loadConfig();

  bool loadDocument(const QString& filename);
  bool saveDocument(const QString& filename = QString());
  bool newDocument();

  bool isLoaded() const;

  QUndoView* undoView();
  QAction* undoAction();
  QAction* redoAction();

  void openCloseClicked();

signals:
  void sendStatusMessage(const QString& message,
                         int timeout = Config::StatusTimeout);
  void sendLogMessage(const QString& message);

private:
  QSplitter* m_splitter;
  FooterWidget* m_footer;
  MetadataForm* m_metadataForm;
  ContentsFrame* m_contentsFrame;
  EPubEditor* m_editor;

  PConfig m_config;
  POptions m_options;
  PLibraryDB m_libraryDB;
  PSeriesDB m_seriesDB;
  PAuthorsDB m_authorsDB;
  PDocument m_document;

  QString m_currentBookFilename;

  QUndoStack* m_undoStack;
  QUndoView* m_undoView;
//  QTabWidget* m_tabs;
  bool m_loaded;
  QList<UniqueString> m_pageOrder;
  QMap<UniqueString, Page> m_pages;
  Page m_page;
  void initGui();
  void updateMetadataForm();
  void metadataHasChanged(MetadataForm::Modifications modifications);
  //  PBookData insertNewBook(const QString &filename, const QString& title,
  //  QStringList creators);
  void loadFileIntoTextDocument(const QString& zipfile,
                                const QString& imageName);
  void splitterHasMoved(int, int);
  void toggleOpenClicked();
};

#endif // EPUBEDIT_H
