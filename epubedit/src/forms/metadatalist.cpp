#include "metadatalist.h"

//====================================================================
//=== MetadataModel
//====================================================================
MetadataModel::MetadataModel(QWidget* parent)
  : QAbstractTableModel(parent)
{}

MetadataModel::~MetadataModel() {}

int
MetadataModel::rowCount(const QModelIndex&) const
{
  return m_firstCol.size();
}

int
MetadataModel::columnCount(const QModelIndex&) const
{
  return 2;
}

QVariant
MetadataModel::headerData(int section, Qt::Orientation orientation, int) const
{
  switch (orientation) {
    case Qt::Horizontal: {
      switch (section) {
        case 0:
          return tr("First");
        case 1:
          return tr("Second");
        case 2:
          return tr("Third");
      }
      break;
    }
    case Qt::Vertical:
      return QVariant();
  }
  return QVariant();
}

QVariant
MetadataModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch (role) {
    case Qt::DisplayRole: {
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

  return QVariant();
}

bool
MetadataModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
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
MetadataModel::insertRowData(int row,
                             const QString& text1,
                             const QString& text2)
{
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_firstCol.insert(row, text1);
  m_secondCol.insert(row, text2);
  m_modified.insert(row, true);
  endInsertRows();
}

void
MetadataModel::initialiseData(PMetadata metadata)
{
  m_metadata = metadata;
  auto creatorList = m_metadata->creatorList();
  auto contributorList = m_metadata->contributorList();
  beginResetModel();
  m_firstCol.clear();
  m_secondCol.clear();
  m_modified.clear();
  for (auto& creator : creatorList) {
    auto data = m_metadata->creatorFromName(creator);
    m_firstCol.append(creator);
    auto relator = data->relator;
    auto description = relator->relatorToDescription(relator->type());
    m_secondCol.append(description);
    m_modified.append(false);
  }
  for (auto& contributor : contributorList) {
    auto data = m_metadata->contributorFromName(contributor);
    m_firstCol.append(contributor);
    auto relator = data->relator;
    auto description = relator->relatorToDescription(relator->type());
    m_secondCol.append(description);
    m_modified.append(false);
  }
  endResetModel();
}

bool
MetadataModel::insertRows(int row, int count, const QModelIndex& parent)
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
MetadataModel::removeRows(int row, int count, const QModelIndex& parent)
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
MetadataModel::firstCol()
{
  return m_firstCol;
}

QStringList
MetadataModel::secondCol()
{
  return m_secondCol;
}

QList<bool>
MetadataModel::thirdCol()
{
  return m_modified;
}

bool
MetadataModel::areModified()
{
  for (auto& modified : m_modified) {
    if (modified)
      return true;
  }
  return false;
}

bool
MetadataModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

QMap<int, QString>
MetadataModel::modifiedRows()
{
  QMap<int, QString> map;
  for (int i = 0; i < m_firstCol.size(); i++) {
    if (m_modified[i]) {
      map.insert(i, m_firstCol.at(i));
    }
  }
  return map;
}

bool
MetadataModel::swapWithFirst(int row)
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
MetadataModel::setFirstAt(int row, const QString& text)
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
MetadataModel::setSecondAt(int row, const QString& text)
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
MetadataModel::setModifiedAt(int row, const bool& modified)
{
  if (row >= 0 && row < m_modified.size()) {
    beginResetModel();
    m_modified.replace(row, modified);
    endResetModel();
    return true;
  }
  return false;
}

// bool
// MetadataModel::modifyFirstCol(int row, const QString& text)
//{
////  auto rows = rowCount(QModelIndex());
//  if (row >= 0 && row < m_firstCol.size()) {
//    m_firstCol.replace(row, text);
//    m_modified.replace(row, true);
//    return true;
//  }
//  return false;
//}
// bool
// MetadataModel::setSecondCol(int row, const QString& text)
//{
//  if (row >= 0 && row < m_secondCol.size()) {
//    beginResetModel();
//    m_secondCol.replace(row, text);
//    endResetModel();
//    return true;
//  }
//  return false;
//}

// bool
// MetadataModel::modifySecondCol(int row, const QString& text)
//{
////  auto rows = rowCount(QModelIndex());
//  if (row >= 0 && row < m_secondCol.size()) {
//    m_secondCol.replace(row, text);
//    m_modified.replace(row, true);
//    return true;
//  }
//  return false;
//}

//====================================================================
//=== MetadataView
//====================================================================
MetadataView::MetadataView(QWidget* parent)
  : QTableView(parent)
{
  m_model = new MetadataModel(this);
  setModel(m_model);
  verticalHeader()->hide();
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  resizeTableVertically();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MetadataView::~MetadataView() {}

void
MetadataView::initialiseData(PMetadata metadata)
{
  m_model->initialiseData(metadata);
  setCurrentIndex(m_model->index(0, 0));
  resizeTableVertically();
}

bool
MetadataView::insertRow(int row)
{
  auto success = m_model->insertRows(row, 1);
  if (success)
    resizeTableVertically();
  return success;
}

bool
MetadataView::removeRow(int row)
{
  auto success = m_model->removeRow(row, QModelIndex());
  if (success) {
    emit rowRemoved(row);
    resizeTableVertically();
  }
  return success;
}

bool
MetadataView::setFirstAt(int row, const QString& text)
{
  return m_model->setFirstAt(row, text);
}

bool
MetadataView::setSecondAt(int row, const QString& text)
{
  return m_model->setSecondAt(row, text);
}

bool
MetadataView::setModifiedAt(int row, const bool& modified)
{
  return m_model->setModifiedAt(row, modified);
}

QModelIndex
MetadataView::primaryAuthorIndex()
{
  return m_model->index(0, 0);
}

bool
MetadataView::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSize
MetadataView::sizeHint() const
{
  return m_hint;
}

QString
MetadataView::firstAt(int row)
{
  auto index = m_model->index(row, 0);
  return m_model->data(index).toString();
}

QString
MetadataView::secondAt(int row)
{
  auto index = m_model->index(row, 1);
  return m_model->data(index).toString();
}

bool
MetadataView::modifiedAt(int row)
{
  auto index = m_model->index(row, 2);
  return m_model->data(index).toBool();
}

void
MetadataView::resizeTableVertically()
{
  auto height = viewportMargins().top();
  height += contentsMargins().top();

  if (horizontalScrollBar()->isVisible()){
    height += horizontalScrollBar()->height();
  }
  if (horizontalHeader()->isVisible()){
    height += horizontalHeader()->height();
  }
  for (int i = 0; i < verticalHeader()->count(); ++i) {
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
