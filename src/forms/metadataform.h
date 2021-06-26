#ifndef METADATAFORM_H
#define METADATAFORM_H

#include <QAbstractTableModel>
#include <QFormLayout>
#include <QFrame>
#include <QLineEdit>
#include <QListView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTableView>

#include "libepubedit/ebookbasemetadata.h"

class TitleModel : public QAbstractTableModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex&) const override;
  int columnCount(const QModelIndex&) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role) override;
  void setData(OrderedTitleMap data);
  bool setData(const QModelIndex& index, Title data);

  OrderedTitleMap titles();
  bool areModified();
  bool isModified(int row);
  OrderedTitleMap modifiedTitles();

private:
  QList<Title> m_titles;
  QList<bool> m_modified;
};

class AuthorModel : public QAbstractListModel
{
  Q_OBJECT
  // QAbstractItemModel interface
public:
  int rowCount(const QModelIndex&) const override;
  int columnCount(const QModelIndex&) const override;

  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role) override;

  void setData(const QModelIndex& index, QString value);
  void setData(QStringList authors);

  QStringList authors();
  bool areModified();
  bool isModified(int row);
  QMap<int, QString> modifiedAuthors();

private:
  QStringList m_authors;
  QList<bool> m_modified;
};

class MetadataForm : public QFrame
{
  Q_OBJECT
public:
  //  enum Type
  //  {
  //    NO_CHANGES = 0,
  //    TITLES = 1,
  //    AUTHORS = 2,

  //    ALL = TITLES | AUTHORS,
  //  };
  //  Q_DECLARE_FLAGS(Types, Type)
  //  Q_FLAG(MetadataForm::Types)

  MetadataForm(QWidget* parent);

  const OrderedTitleMap titles() const;
  bool titlesModified() const;
  void setTitles(OrderedTitleMap titles);

  const QStringList authors() const;
  bool authorsModified() const;
  void setAuthors(const QStringList& authors);

signals:
  void dataHasChanged();

private:
  QTableView* m_titleView;
  QListView* m_authorView;
  //  Types m_changes = NO_CHANGES;
  QStringList m_authors;
  OrderedTitleMap m_titles;

  static const QString TITLESTR;

  void acceptChanges();
  void cancelChanges();
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(MetadataForm::Types)

#endif // METADATAFORM_H
