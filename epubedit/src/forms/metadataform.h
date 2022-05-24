#ifndef METADATAFORM_H
#define METADATAFORM_H

#include <QAbstractTableModel>
#include <QAction>
#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QFrame>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QStackedLayout>
#include <QString>
#include <QTableView>

#include <QUndoCommand>
#include <QUndoStack>

class Metadata;
class EPubTitle;
class MetadataList;
class TitleView;

#include "document/bookpointers.h"
#include "document/epubmetadata.h"
#include "forms/undocommands.h"

class MetadataForm : public QFrame
{
  Q_OBJECT
public:
  enum Modified
  {
    NO_CHANGES = 0,
    TITLES_CHANGED = 1,
    AUTHORS_CHANGED = 2,
    ID_CHANGED = 4,

    ALL = TITLES_CHANGED | AUTHORS_CHANGED,
  };
  Q_DECLARE_FLAGS(Modifications, Modified)
  Q_FLAG(MetadataForm::Modifications)

  MetadataForm(QWidget* parent = nullptr);
  ~MetadataForm();

  void setDocument(PDocument document);

  QWidget* initMaindataFrame();

signals:
  void dataHasChanged(MetadataForm::Modifications mods);
  void sendStatusMessage(const QString& message, int timeout = 20);
  void pushUndoAction(QUndoCommand*undoCommand);

private:
  TitleView* m_titleView;
  MetadataList* m_metadataView;
  PDocument m_document;
  QModelIndex m_currentTitleIndex, m_currentAuthorIndex;
  Modifications m_modifications = NO_CHANGES;

  static const QString TITLESTR;

  void initGui();
  void acceptChanges();
  void cancelChanges();
  void addTitle();
  void addAuthor();
  void modifyAuthor();
  void removeAuthor();
  void setPrimaryAuthor();

  //  void titleClicked(const QModelIndex& index);
  void titleContextMenu(QPoint pos);
  void authorContextMenu(QPoint pos);
  void modifyTitle();
  void setId();
  void removeTitle();
  void titleRemoved(int row);
  void setPrimaryTitle();
  void moveTitleIdToPrimary();
  void editTitle();
  void authorRemoved(int row);
  void titleClicked();
  void authorClicked();
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(MetadataForm::Types)

#endif // METADATAFORM_H
