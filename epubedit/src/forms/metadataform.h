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
#include <QScrollArea>

#include <QUndoCommand>
#include <QUndoStack>

class MetadataView;
class ThreeColView;
class TwoColView;
class LabelledLineEdit;

#include "document/bookpointers.h"
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

  MetadataForm(PConfig config, QWidget* parent = nullptr);
  ~MetadataForm();

  void setMetadata(PMetadata metadata);

  QWidget* initMainDataFrame();
  QWidget* initSecondaryDataFrame();

signals:
  void dataHasChanged(MetadataForm::Modifications mods);
  void sendStatusMessage(const QString& message, int timeout = 20);
  void pushUndoAction(QUndoCommand* undoCommand);

private:
  PConfig m_config;
  PMetadata m_metadata;
  ThreeColView* m_titleView = nullptr;
  TwoColView* m_metadataView = nullptr;
  QLineEdit* m_rights = nullptr;
  LabelledLineEdit* m_date = nullptr;
  TwoColView*m_languageView;
  QPushButton* m_lockBtn = nullptr;
  QModelIndex m_currentTitleIndex, m_currentAuthorIndex;
  Modifications m_modifications = NO_CHANGES;
  bool locked = true;

  static const QString TITLESTR;

  void initGui();
  void acceptChanges();
  void cancelChanges();
  void addTitle();
  void addAuthor();
  void modifyAuthor();
  void removeAuthor();
  void setPrimaryAuthor();

  void lock();
  //  void titleClicked(const QModelIndex& index);
  void titleContextMenu(QPoint pos);
  void authorContextMenu(QPoint pos);
  void langContextMenu(QPoint pos);
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
  void initialiseLanguageView();
  void initialiseMetadataView();
  void initialiseTitleView();
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(MetadataForm::Types)

class TitleEditDialog : public QDialog
{
  Q_OBJECT

public:
  TitleEditDialog(PTitle ptitle,
                  const QString& title,
                  const QString& id,
                  QWidget* parent);

  PTitle title();

private:
  QLabel* m_label;
  QDialogButtonBox* m_box;
  PTitle m_ptitle;

  void initGui();
  void idChanged(const QString& text);
  void titleChanged(const QString& text);
  void directionChanged(int index);
  void langChanged(const QString& text);
  void dateChanged(const QDate& date);
};

#endif // METADATAFORM_H
