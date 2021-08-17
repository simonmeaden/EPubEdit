#ifndef AUTHORLIST_H
#define AUTHORLIST_H

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QTableView>

class AuthorModel : public QAbstractTableModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  AuthorModel(QWidget* parent = nullptr);
  ~AuthorModel();
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

  void modifyRowData(int row, const QString& value);
  void initialiseData(QStringList authors);

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

  QStringList authors();
  bool areModified();
  bool isModified(int row);
  QMap<int, QString> modifiedAuthors();
  bool setAuthor(int row, const QString& author);
  bool modifyAuthor(int row, const QString& author);

  bool swapWithFirst(int row);

private:
  QStringList m_authors;
  QList<bool> m_modified;
};

class AuthorList : public QTableView
{
  Q_OBJECT
public:
  AuthorList(QWidget* parent = nullptr);
  ~AuthorList();

  void initialiseData(QStringList authors);

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
  AuthorModel* m_model;
  QStringList m_authorList;
  QSize m_hint;
};

#endif // AUTHORLIST_H
