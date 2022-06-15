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
#include <QScrollBar>
#include <QVariant>

#include "document/bookpointers.h"
#include "document/uniquestring.h"

class TitleEditDialog : public QDialog
{
  Q_OBJECT

public:
  TitleEditDialog(PTitle title,
                   QWidget* parent);

  PTitle title();

private:
  QLabel* m_label;
  QDialogButtonBox* m_box;
  PTitle m_title;

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
  bool setTitle(int row, PTitle title);
  bool hasId(int row);

  PTitle titleAt(int row);
  QString titleStrAt(int row);
  UniqueString idStrAt(int row);

  QList<PTitle> titles();
  bool areModified();
  bool isModified(int row);
  QList<PTitle> modifiedTitles();

  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

private:
  QList<PTitle> m_titles;
  QList<PTitle> m_originalTitles;
  QList<bool> m_modified;

  void initialiseData(QList<PTitle> data);
  friend class TitleView;
};

class TitleView : public QTableView
{
  Q_OBJECT
public:
  TitleView(QWidget* parent = nullptr);
  ~TitleView();

  void initialiseData(PMetadata metadata);
  bool removeTitle(int row);
  bool insertTitle(int row, PTitle title);
  bool modifyTitle(int row, const QString& title);
  bool setTitle(int row, const QString& title);
  bool setId(int row, const UniqueString& id);
  UniqueString id(int row);
  bool swapToPrimaryPosition(int row);
  PTitle titleAt(int row);
  UniqueString idAt(int row);
  QString titleStrAt(int row);
  QModelIndex primaryTitleIndex();
  bool hasId(int row);

  QSize sizeHint() const override;

  void resizeTableVertically();

signals:
  void titleRemoved(int row);

private:
  TitleModel* m_model;
  PMetadata m_metadata;
  QSize m_hint;
};

#endif // TITLEVIEW_H
