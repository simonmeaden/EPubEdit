#ifndef METADATAFORM_H
#define METADATAFORM_H

#include <QAbstractTableModel>
#include <QAction>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QFrame>
#include <QHeaderView>
#include <QInputDialog>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QString>
#include <QTableView>

#include <QUndoCommand>
#include <QUndoStack>
#include <QUndoView>

class EPubDocument;
class EPubMetadata;
class EPubTitle;

#include "document/epubmetadata.h"

class TitleEditDialog : public QDialog
{
  Q_OBJECT
public:
  TitleEditDialog(QSharedPointer<EPubTitle> title, QWidget* parent)
    : QDialog(parent)
    , m_title(title)
  {
    initGui();
  }

  inline QSharedPointer<EPubTitle> title() { return m_title; }

private:
  QComboBox* m_dirEdit;
  QSharedPointer<EPubTitle> m_title;

  void initGui();
  void idChanged(const QString& text);
  void titleChanged(const QString& text);
  void directionChanged(const QString& text);
  void langChanged(const QString& text);
  void dateChanged(const QDate& date);
};

class TitleModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  TitleModel(QWidget* parent = nullptr);
  ~TitleModel();
  int rowCount(const QModelIndex&) const override;
  int columnCount(const QModelIndex&) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;
  //! Change the Title at row, or if row == rowCount() append it.
  //!
  //! On success true is returned otherwise
  //! if row < 0 or row > rowCount() then nothing is done with the data
  //! and false is returned.
  bool modifyTitle(int row, const QString& titleStr);
  bool modifyId(int row, const QString& idStr);
  bool swapWithFirst(int row);
  bool setTitle(int row, QSharedPointer<EPubTitle> title);

  QSharedPointer<EPubTitle> titleAt(int row);
  QString titleStrAt(int row);

  QList<QSharedPointer<EPubTitle>> titles();
  bool areModified();
  bool isModified(int row);
  QList<QSharedPointer<EPubTitle>> modifiedTitles();

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

private:
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  QList<QSharedPointer<EPubTitle>> m_titles;
  QList<QSharedPointer<EPubTitle>> m_originalTitles;
  QList<bool> m_modified;

  void initialiseData(QList<QSharedPointer<EPubTitle>> data,
                      QSharedPointer<UniqueStringList> uniqueIdList);
  friend class TitleView;

  // QAbstractItemModel interface
};

class TitleView : public QTableView
{
  Q_OBJECT
public:
  TitleView(QWidget* parent = nullptr);
  ~TitleView();

  void initialiseData(QList<QSharedPointer<EPubTitle>> data,
                      QSharedPointer<UniqueStringList> uniqueIdList);
  //  bool addTitle(const QString& text, const UniqueString& id =
  //  UniqueString());
  bool removeTitle(int row);
  bool insertTitle(int row, QSharedPointer<EPubTitle> title);
  bool modifyTitle(int row, const QString& title);
  bool setTitle(int row, const QString& title);
  bool setId(int row, const UniqueString& id);
  bool swapToPrimaryPosition(int row);
  QSharedPointer<EPubTitle> titleAt(int row);
  QString titleStrAt(int row);
  QModelIndex primaryTitleIndex();

  QSize sizeHint() const override;

  QSharedPointer<UniqueStringList> uniqueIdList() const;
  void setUniqueIdList(const QSharedPointer<UniqueStringList>& uniqueIdList);

  void resizeTableVertically();

signals:
  void updateSize(int rowCount);
  void titleRemoved(int row);

private:
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  TitleModel* m_model;
  QSize m_hint;
};

class AuthorModel : public QAbstractTableModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  AuthorModel(QSharedPointer<UniqueStringList> uniqueIdList,
              QWidget* parent = nullptr);
  ~AuthorModel();
  int rowCount(const QModelIndex&) const override;
  int columnCount(const QModelIndex&) const override;

  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role) override;

  void modifyRowData(int row, QString value);
  void initialiseData(QStringList authors);

  QStringList authors();
  bool areModified();
  bool isModified(int row);
  QMap<int, QString> modifiedAuthors();

private:
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  QStringList m_authors;
  QList<bool> m_modified;
};

class AuthorView : public QListView
{
  Q_OBJECT
public:
  AuthorView(QSharedPointer<UniqueStringList> uniqueIdList,
             QWidget* parent = nullptr);
  ~AuthorView();

private:
  QSharedPointer<UniqueStringList> m_uniqueIdList;
};

class AddTitleCommand : public QUndoCommand
{
public:
  AddTitleCommand(TitleView* view,
                  const QString& title,
                  int row,
                  QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_title;
  int m_row;
};

class SetIdCommand : public QUndoCommand
{
public:
  SetIdCommand(TitleView* view,
               const QString& newId,
               const QString& oldId,
               int row,
               QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_newId;
  QString m_oldId;
  int m_row;
};

class RemoveTitleCommand : public QUndoCommand
{
public:
  RemoveTitleCommand(TitleView* view,
                     QSharedPointer<EPubTitle> title,
                     int pos,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QSharedPointer<EPubTitle> m_title;
  int m_row;
};

class SwapTitleCommand : public QUndoCommand
{
public:
  SwapTitleCommand(TitleView* view,
                   QSharedPointer<EPubTitle> title1,
                   int row1,
                   QSharedPointer<EPubTitle> title2,
                   int row2,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QSharedPointer<EPubTitle> m_title1;
  int m_row1;
  QSharedPointer<EPubTitle> m_title2;
  int m_row2;
};

class ModifyTitleCommand : public QUndoCommand
{
public:
  ModifyTitleCommand(TitleView* view,
                     const QString& newTitle,
                     int pos,
                     const QString& oldTitle,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_newTitle;
  QString m_oldTitle;
  int m_row;
};

class MetadataForm : public QFrame
{
  Q_OBJECT
public:
  enum Modified
  {
    NO_CHANGES = 0,
    TITLES_CHANGED = 1,
    AUTHORS_CHANGED = 2,

    ALL = TITLES_CHANGED | AUTHORS_CHANGED,
  };
  Q_DECLARE_FLAGS(Modifications, Modified)
  Q_FLAG(MetadataForm::Modifications)

  MetadataForm(QUndoStack* undoStack, QWidget* parent = nullptr);
  ~MetadataForm();

  void setDocument(QSharedPointer<EPubDocument> document);

  QWidget* initMaindataFrame();

signals:
  void dataHasChanged(Modifications mods);
  void sendStatusMessage(const QString& message, int timeout = 20);

private:
  QUndoStack* m_undoStack;
  TitleView* m_titleView;
  AuthorView* m_authorView;
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  QSharedPointer<EPubDocument> m_document;
  QModelIndex m_currentIndex;
  Modifications m_modifications = NO_CHANGES;

  static const QString TITLESTR;

  void initGui();
  void acceptChanges();
  void cancelChanges();
  void addTitle();
  //  void titleClicked(const QModelIndex& index);
  void titleContextMenu(QPoint pos);
  void modifyTitle();
  void setId();
  void removeTitle();
  void titleRemoved(int row);
  void setPrimaryTitle();
  void editTitle();
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(MetadataForm::Types)

#endif // METADATAFORM_H
