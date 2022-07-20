#include "authorlist.h"

#include "document/epubmetadata.h"

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
  return 3;
}

QVariant
MetadataModel::headerData(int section, Qt::Orientation orientation, int) const
{
//  switch (orientation) {
//    case Qt::Horizontal: {
//      switch (section) {
//        case 0:
//          return tr("Name");
//        case 1:
//          return tr("File As...");
//      }
//      break;
//    }
//    default:
//      break;
//  }
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
          auto data = m_secondCol[row];
          if (data) {
            return data->fileAs->name;
          }
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
MetadataModel::modifyRowData(int row, const QString& value)
{
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_firstCol.append(value);
  m_modified.append(true);
  endInsertRows();
}

void
MetadataModel::initialiseData(PMetadata metadata)
{
  m_metadata = metadata;
  auto authorList = m_metadata->creatorList();
  auto rights = m_metadata->rights();
  beginResetModel();
  m_firstCol.clear();
  for (auto& author : authorList) {
    auto authorData = m_metadata->creatorFromName(author);
    m_firstCol.append(author);
    m_secondCol.append(authorData);
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
    m_secondCol.insert(row, QSharedPointer<EPubCreator>(new EPubCreator()));
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
MetadataModel::authors()
{
  return m_firstCol;
}

bool
MetadataModel::areModified()
{
  for (auto modified : m_modified) {
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
MetadataModel::modifiedAuthors()
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
    auto swapper = m_firstCol.at(row);
    beginResetModel();
    m_firstCol.replace(0, swapper);
    m_firstCol.replace(row, first);
    endResetModel();
    return true;
  }
  return false;
}

bool
MetadataModel::setAuthor(int row, const QString& author)
{
  if (row >= 0 && row < m_firstCol.size()) {
    beginResetModel();
    m_firstCol.replace(row, author);
    endResetModel();
    return true;
  }
  return false;
}

bool
MetadataModel::modifyAuthor(int row, const QString& author)
{
  auto rows = rowCount(QModelIndex());
  if (row < 0 || row > rows)
    return false;

  if (row >= m_firstCol.size()) {
    return false;
  }
  m_firstCol.replace(row, author);
  m_modified.replace(row, true);
  return true;
}

//====================================================================
//=== MetadataList
//====================================================================
MetadataList::MetadataList(QWidget* parent)
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

MetadataList::~MetadataList() {}

void
MetadataList::initialiseData(PMetadata metadata)
{
  m_model->initialiseData(metadata);

  setCurrentIndex(m_model->index(0, 0));
}

bool
MetadataList::removeAuthor(int row)
{
  auto success = m_model->removeRow(1, QModelIndex());
  if (success) {
    emit authorRemoved(row);
  }
  return success;
}

bool
MetadataList::insertAuthor(int row, const QString& author)
{
  auto success = m_model->insertRow(row, QModelIndex());
  if (success) {
    m_model->setAuthor(row, author);
  }
  return success;
}

bool
MetadataList::modifyAuthor(int row, const QString& author)
{
  auto success = m_model->modifyAuthor(row, author);
  auto index = m_model->index(row, 0);
  dataChanged(index, index);
  return success;
}

QModelIndex
MetadataList::primaryAuthorIndex()
{
  return m_model->index(0, 0);
}

bool
MetadataList::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSize
MetadataList::sizeHint() const
{
  return m_hint;
}

void
MetadataList::resizeTableVertically()
{
  auto vMargins = viewportMargins();
  auto count = m_model->rowCount(QModelIndex());
  auto height = vMargins.top();
  for (int row = 0; row < count; row++) {
    height += verticalHeader()->sectionSize(row);
  }
  setMinimumHeight(height);
  setMaximumHeight(height);
  auto g = geometry();
  g.setHeight(height);
  m_hint = g.size();
  setGeometry(g);
  updateGeometry();
}

bool
MetadataList::setAuthor(int row, const QString& author)
{
  auto index = m_model->index(row, 0);
  if (index.isValid()) {
    return m_model->setData(index, QVariant::fromValue(author));
  }
  return false;
}

QString
MetadataList::authorAt(int row)
{
  auto index = m_model->index(row, 0);
  return m_model->data(index).toString();
}
