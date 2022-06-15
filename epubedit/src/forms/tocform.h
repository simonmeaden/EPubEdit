#ifndef TOCFORM_H
#define TOCFORM_H

#include <QAbstractItemModel>
#include <QHBoxLayout>
#include <QTreeView>
#include <QWidget>

#include "document/bookpointers.h"

class NavListItem;

class TocModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  TocModel(QObject* parent = nullptr);
  ~TocModel();

  void setData(NavListItem* data);
  bool hasIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex index(int row, int column, const QModelIndex& parent) const;
  QModelIndex parent(const QModelIndex& child) const;
  int rowCount(const QModelIndex& parent) const;
  int columnCount(const QModelIndex& parent) const;
  QVariant data(const QModelIndex& index, int role) const;
  bool setData(const QModelIndex& index, const QVariant& value, int role);
  bool insertRows(int row, int count, const QModelIndex& parent);
  bool removeRows(int row, int count, const QModelIndex& parent);
  bool moveRows(const QModelIndex& sourceParent,
                int sourceRow,
                int count,
                const QModelIndex& destinationParent,
                int destinationChild);

private:
  NavListItem* m_data = nullptr;
};

class TocForm : public QWidget
{
  Q_OBJECT
public:
  explicit TocForm(QWidget* parent = nullptr);

  void setMetadata(PMetadata metadata);

signals:
  void itemSelected(const QString& href);

private:
  PMetadata m_metadata;
  QTreeView* m_treeView;
  TocModel* m_model;

  void tocClicked(const QModelIndex &index);
};

#endif // TOCFORM_H
