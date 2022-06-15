#include "twocolview.h"
#include "config.h"
#include "document/metadata.h"
#include "languages.h"

//====================================================================
//=== TwoColView
//====================================================================
TwoColView::TwoColView(QWidget* parent)
  : QTableView(parent)
{
  m_model = new TwoColModel(this);
  setModel(m_model);
  verticalHeader()->hide();
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  resizeTableVertically();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TwoColView::~TwoColView() {}

void
TwoColView::clear()
{
  m_model->clear();
}

bool
TwoColView::insertRow(int row)
{
  auto success = m_model->insertRows(row, 1);
  if (success)
    resizeTableVertically();
  return success;
}

bool
TwoColView::removeRow(int row)
{
  auto success = m_model->removeRows(row, 1);
  if (success) {
    emit rowRemoved(row);
    resizeTableVertically();
  }
  return success;
}

bool
TwoColView::addRow(const QString& text1, const QString& text2)
{
  int row = m_model->rowCount(QModelIndex());
  insertRow(row);
  m_model->setFirstAt(row, text1);
  m_model->setSecondAt(row, text2);
  m_model->setModifiedAt(row, false);
  return true;
}

bool
TwoColView::setFirstAt(int row, const QString& text)
{
  return m_model->setFirstAt(row, text);
}

bool
TwoColView::setSecondAt(int row, const QString& text)
{
  return m_model->setSecondAt(row, text);
}

bool
TwoColView::setModifiedAt(int row, const bool& modified)
{
  return m_model->setModifiedAt(row, modified);
}

QModelIndex
TwoColView::primaryAuthorIndex()
{
  return m_model->index(0, 0);
}

bool
TwoColView::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSize
TwoColView::sizeHint() const
{
  return m_hint;
}

QString
TwoColView::firstAt(int row)
{
  auto index = m_model->index(row, 0);
  return m_model->data(index).toString();
}

QString
TwoColView::secondAt(int row)
{
  auto index = m_model->index(row, 1);
  return m_model->data(index).toString();
}

bool
TwoColView::modifiedAt(int row)
{
  auto index = m_model->index(row, 2);
  return m_model->data(index).toBool();
}

void
TwoColView::resizeTableVertically()
{
  auto height = viewportMargins().top();
  height += contentsMargins().top();

  if (horizontalScrollBar()->isVisible()) {
    height += horizontalScrollBar()->height();
  }

  if (horizontalHeader()->isVisible()) {
    height += horizontalHeader()->height();
  }

  auto count = verticalHeader()->count();
  for (int i = 0; i < count; ++i) {
    if (!verticalHeader()->isSectionHidden(i)) {
      height += verticalHeader()->sectionSize(i);
    }
  }

  height += viewportMargins().bottom();
  height += contentsMargins().bottom();

  setMinimumHeight(height);
  setMaximumHeight(height);
  auto g = geometry();
  g.setHeight(height);
  m_hint = g.size();
  setGeometry(g);
  updateGeometry();
}

//====================================================================
//=== TwoColModel
//====================================================================
TwoColModel::TwoColModel(QWidget* parent)
  : QAbstractTableModel(parent)
{}

TwoColModel::~TwoColModel() {}

int
TwoColModel::rowCount(const QModelIndex&) const
{
  return m_firstCol.size();
}

int
TwoColModel::columnCount(const QModelIndex&) const
{
  return 2;
}

QVariant
TwoColModel::headerData(int section, Qt::Orientation orientation, int) const
{
  switch (orientation) {
    case Qt::Horizontal: {
      switch (section) {
        case 0:
          return tr("Code");
        case 1:
          return tr("name");
      }
      break;
    }
    case Qt::Vertical:
      return QVariant();
  }
  return QVariant();
}

QVariant
TwoColModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch (role) {
    case Qt::DisplayRole: {
      if (m_firstCol.size() == 0) {
        return QString();
      } else {
        int row = index.row();
        int col = index.column();
        switch (col) {
          case 0:
            return m_firstCol[row];
          case 1:
            return m_secondCol[row];
        }
      }
    }
  }

  return QVariant();
}

bool
TwoColModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid()) {
    return false;
  }

  if (role == Qt::EditRole) {
    switch (index.column()) {
      case 0: {
        m_firstCol.replace(index.row(), value.toString());
        m_modified.replace(index.row(), true);
        return true;
      }
      case 1: {
        // TODO update authorData.
      }
    }
  }

  return false;
}

void
TwoColModel::insertRowData(int row, const QString& text1, const QString& text2)
{
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_firstCol.insert(row, text1);
  m_secondCol.insert(row, text2);
  m_modified.insert(row, true);
  endInsertRows();
}

void
TwoColModel::clear()
{
  m_firstCol.clear();
  m_secondCol.clear();
  m_modified.clear();
}

bool
TwoColModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (row < 0 || row > rowCount(parent))
    return false;

  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int i = 0; i < count; i++) {
    m_firstCol.insert(row, QString());
    m_secondCol.insert(row, QString());
    m_modified.insert(row, false);
  }
  endInsertRows();

  return true;
}

bool
TwoColModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row >= 0 || row < rowCount(parent)) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = row; i < row + count; i++) {
      m_firstCol.removeAt(row);
      m_secondCol.removeAt(row);
      m_modified.removeAt(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

QStringList
TwoColModel::firstCol()
{
  return m_firstCol;
}

QStringList
TwoColModel::secondCol()
{
  return m_secondCol;
}

QList<bool>
TwoColModel::modifiedCol()
{
  return m_modified;
}

QString
TwoColModel::firstAt(int row)
{
  if (row >= 0 && row < m_firstCol.size())
    return m_firstCol.at(row);
  return QString();
}

QString
TwoColModel::secondAt(int row)
{
  if (row >= 0 && row < m_firstCol.size())
    return m_secondCol.at(row);
  return QString();
}

bool
TwoColModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

bool
TwoColModel::swapWithFirst(int row)
{
  if (row > 0 && row < m_firstCol.size()) {
    auto first = m_firstCol.first();
    auto second = m_secondCol.first();
    auto third = m_modified.first();
    auto swapper1 = m_firstCol.at(row);
    auto swapper2 = m_secondCol.at(row);
    auto swapper3 = m_modified.at(row);
    beginResetModel();
    m_firstCol.replace(0, swapper1);
    m_secondCol.replace(0, swapper2);
    m_modified.replace(0, swapper3);
    m_firstCol.replace(row, first);
    m_secondCol.replace(row, second);
    m_modified.replace(row, third);
    endResetModel();
    return true;
  }
  return false;
}

bool
TwoColModel::setFirstAt(int row, const QString& text)
{
  if (row >= 0 && row < m_firstCol.size()) {
    beginResetModel();
    m_firstCol.replace(row, text);
    endResetModel();
    return true;
  }
  return false;
}

bool
TwoColModel::setSecondAt(int row, const QString& text)
{
  if (row >= 0 && row < m_secondCol.size()) {
    beginResetModel();
    m_secondCol.replace(row, text);
    endResetModel();
    return true;
  }
  return false;
}

bool
TwoColModel::setModifiedAt(int row, const bool& modified)
{
  if (row >= 0 && row < m_modified.size()) {
    beginResetModel();
    m_modified.replace(row, modified);
    endResetModel();
    return true;
  }
  return false;
}
