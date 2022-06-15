#ifndef LANGUANGEVIEW_H
#define LANGUANGEVIEW_H

#include <QTableView>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QList>
#include <QTableView>
#include <QScrollBar>
#include <QVariant>
#include <QSharedPointer>

#include "document/bookpointers.h"

class BCP47Languages;

class TwoColModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  TwoColModel(QWidget* parent = nullptr);
  ~TwoColModel();

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

  void insertRowData(int row, const QString& text1, const QString& text2);

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;


  bool swapWithFirst(int row);

  QStringList firstCol();
  QStringList secondCol();
  QList<bool> modifiedCol();

  QString firstAt(int row);
  QString secondAt(int row);
  bool isModified(int row);

  bool setFirstAt(int row, const QString& author);
  bool setSecondAt(int row, const QString& text);
  bool setModifiedAt(int row, const bool& modified);

  void clear();

private:
  QStringList m_firstCol;
  QStringList m_secondCol;
  QList<bool> m_modified;
};

class TwoColView : public QTableView
{
  Q_OBJECT
public:
  TwoColView(QWidget* parent = nullptr);
  ~TwoColView();

  bool insertRow(int row);
  bool removeRow(int row);

  QModelIndex primaryAuthorIndex();
  bool swapToPrimaryPosition(int row);

  QSize sizeHint() const override;

  void resizeTableVertically();

  bool setFirstAt(int row, const QString& author);
  bool setSecondAt(int row, const QString& text);
  bool setModifiedAt(int row, const bool& modified);

  QString firstAt(int row);
  QString secondAt(int row);
  bool modifiedAt(int row);

  void clear();

  bool addRow(const QString &text1, const QString &text2);
signals:
  void rowRemoved(int row);

private:
  TwoColModel* m_model;
  QSize m_hint;
};
#endif // LANGUANGEVIEW_H
