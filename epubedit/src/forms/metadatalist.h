#ifndef METADATALIST_H
#define METADATALIST_H

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QTableView>
#include <QVariant>
#include <QList>

#include "document/epubdocument.h"
#include "document/epubmetadata.h"
#include "document/marcrelator.h"
#include "document/bookpointers.h"

class MetadataModel : public QAbstractTableModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  MetadataModel(QWidget* parent = nullptr);
  ~MetadataModel();
  int rowCount(const QModelIndex&) const override;
  int columnCount(const QModelIndex&) const override;

  QVariant headerData(int, Qt::Orientation,
                      int) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;

  void modifyRowData(int row, const QString& value);
  void initialiseData(PDocument document);

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

  QStringList names();
  bool areModified();
  bool isModified(int row);
  QMap<int, QString> modifiedAuthors();
  bool setNameData(int row, const QString& author);
  bool modifyNameData(int row, const QString& author);

  bool swapWithFirst(int row);

private:
  QStringList m_names;
  QList<QSharedPointer<EPubCreatorContributorBase>> m_data;
  QList<bool> m_modified;
  PMetadata m_metadata;

};

class MetadataList : public QTableView
{
  Q_OBJECT
public:
  MetadataList(QWidget* parent = nullptr);
  ~MetadataList();

  void initialiseData(PDocument document);

  bool removeAuthor(int row);
  bool insertAuthor(int row, const QString& author);
  bool modifyAuthor(int row, const QString& author);
  bool setAuthor(int row, const QString& author);
  QString authorAt(int row);

  QModelIndex primaryAuthorIndex();
  bool swapToPrimaryPosition(int row);

  QSize sizeHint() const override;

  void resizeTableVertically();

signals:
  void authorRemoved(int row);

private:
  MetadataModel* m_model;
  QStringList m_authorList;
  QSize m_hint;
};

#endif // METADATALIST_H
