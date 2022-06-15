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
#include <QScrollBar>
#include <QSharedPointer>
#include <QTableView>
#include <QVariant>

#include "document/bookpointers.h"
#include "document/uniquestring.h"


class ThreeColModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  ThreeColModel(QWidget* parent = nullptr);
  ~ThreeColModel();

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

  void insertRowData(int row,
                     const QString& text1,
                     const QString& text2,
                     const QString& text3);

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

  QMap<int, QString> modifiedRows();

  bool swapWithFirst(int row);

  QString firstAt(int row);
  QString secondAt(int row);
  QString thirdAt(int row);
  bool isModified(int row);

  bool setFirstAt(int row, const QString& text);
  bool setSecondAt(int row, const QString& text);
  bool setThirdAt(int row, const QString& text);
  bool setModifiedAt(int row, bool modified);

  QStringList firstCol();
  QStringList secondCol();
  QStringList thirdCol();
  QList<bool> modifiedCol();

  void clear();

private:
  QStringList m_firstCol;
  QStringList m_secondCol;
  QStringList m_thirdCol;
  QList<bool> m_modified;

  //  void initialiseData(QList<PTitle> data);
  friend class ThreeColView;
};

class ThreeColView : public QTableView
{
  Q_OBJECT
public:
  ThreeColView(QWidget* parent = nullptr);
  ~ThreeColView();

  bool insertRow(int row);
  bool removeRow(int row);

  bool setTitle(int row, const QString& title);
  bool setThird(int row, const UniqueString& id);

  bool swapToPrimaryPosition(int row);

  bool setFirstAt(int row, const QString& text);
  bool setSecondAt(int row, const QString& text);
  bool setThirdAt(int row, const QString& text);
  bool setModifiedAt(int row, bool modified);

  QString firstAt(int row);
  QString secondAt(int row);
  QString thirdAt(int row);
  QModelIndex primaryTitleIndex();

  bool addRow(const QString& text1, const QString& text2, const QString& text3);

  QSize sizeHint() const override;

  void clear();

  void resizeTableVertically();

signals:
  void rowRemoved(int row);

private:
  ThreeColModel* m_model;
  PMetadata m_metadata;
  QSize m_hint;
};

#endif // TITLEVIEW_H
