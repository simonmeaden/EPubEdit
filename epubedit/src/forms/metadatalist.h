#ifndef METADATALIST_H
#define METADATALIST_H

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QList>
#include <QTableView>
#include <QScrollBar>
#include <QVariant>

#include "document/bookpointers.h"
#include "document/epubdocument.h"
#include "document/metadata.h"
#include "document/marcrelator.h"

class MetadataModel : public QAbstractTableModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  MetadataModel(QWidget* parent = nullptr);
  ~MetadataModel();
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

  void initialiseData(PMetadata metadata);

  bool areModified();
  bool isModified(int row);
  QMap<int, QString> modifiedRows();

  bool swapWithFirst(int row);

  QStringList firstCol();
  QStringList secondCol();
  QList<bool> thirdCol();

  bool setFirstAt(int row, const QString& author);
  bool setSecondAt(int row, const QString& text);
  bool setModifiedAt(int row, const bool& modified);

private:
  QStringList m_firstCol;
  QStringList m_secondCol;
  QList<bool> m_modified;
  PMetadata m_metadata;
};

class MetadataView : public QTableView
{
  Q_OBJECT
public:
  MetadataView(QWidget* parent = nullptr);
  ~MetadataView();

  void initialiseData(PMetadata metadata);

  bool insertRow(int row);
  bool removeRow(int row);

  QString firstAt(int row);

  QModelIndex primaryAuthorIndex();
  bool swapToPrimaryPosition(int row);

  QSize sizeHint() const override;

  void resizeTableVertically();

  bool setFirstAt(int row, const QString& author);
  bool setSecondAt(int row, const QString& text);
  bool setModifiedAt(int row, const bool& modified);

  QString secondAt(int row);
  bool modifiedAt(int row);
signals:
  void rowRemoved(int row);

private:
  MetadataModel* m_model;
  QSize m_hint;
};

#endif // METADATALIST_H
