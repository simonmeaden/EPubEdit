#ifndef TITLEVIEW_H
#define TITLEVIEW_H

#include <QAbstractTableModel>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSharedPointer>
#include <QTableView>
#include <QVariant>

#include "document/epubmetadata.h"

class TitleEditDialog : public QDialog
{
  Q_OBJECT

public:
  TitleEditDialog(QSharedPointer<EPubTitle> title,
                  QSharedPointer<UniqueStringList> uniqueIdList,
                  QWidget* parent);

  inline QSharedPointer<EPubTitle> title() { return m_title; }

private:
  QLabel* m_label;
  QDialogButtonBox* m_box;
  QSharedPointer<EPubTitle> m_title;
  QSharedPointer<UniqueStringList> m_uniqueIdList;

  void initGui();
  void idChanged(const QString& text);
  void titleChanged(const QString& text);
  void directionChanged(int index);
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
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
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
  bool hasId(int row);

  QSharedPointer<EPubTitle> titleAt(int row);
  QString titleStrAt(int row);
  UniqueString idStrAt(int row);

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
};

class TitleView : public QTableView
{
  Q_OBJECT
public:
  TitleView(QWidget* parent = nullptr);
  ~TitleView();

  void initialiseData(QList<QSharedPointer<EPubTitle>> data,
                      QSharedPointer<UniqueStringList> uniqueIdList);
  bool removeTitle(int row);
  bool insertTitle(int row, QSharedPointer<EPubTitle> title);
  bool modifyTitle(int row, const QString& title);
  bool setTitle(int row, const QString& title);
  bool setId(int row, const UniqueString& id);
  UniqueString id(int row);
  bool swapToPrimaryPosition(int row);
  QSharedPointer<EPubTitle> titleAt(int row);
  UniqueString idAt(int row);
  QString titleStrAt(int row);
  QModelIndex primaryTitleIndex();
  bool hasId(int row);

  QSize sizeHint() const override;

  QSharedPointer<UniqueStringList> uniqueIdList() const;
  void setUniqueIdList(const QSharedPointer<UniqueStringList>& uniqueIdList);

  void resizeTableVertically();

signals:
  void titleRemoved(int row);

private:
  QSharedPointer<UniqueStringList> m_uniqueIdList;
  TitleModel* m_model;
  QSize m_hint;
};

#endif // TITLEVIEW_H
