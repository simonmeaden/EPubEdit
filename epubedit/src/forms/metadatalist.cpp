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
  return m_names.size();
}

int
MetadataModel::columnCount(const QModelIndex&) const
{
  return 3;
}

QVariant
MetadataModel::headerData(int /*section*/,
                          Qt::Orientation /*orientation*/,
                          int) const
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
  //    case Qt::Vertical:
  //      return QVariant();
  //  }
  return QVariant();
}

QVariant
MetadataModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  //  auto type
  switch (role) {
    case Qt::DisplayRole: {
      int row = index.row();
      int col = index.column();
      auto name = m_names.at(row);
      auto data = m_data.at(row);
      auto type = data->relator->type();
      switch (col) {
        case 0: {
          return name;
        }
        case 1: {
          if (data) {
            if (type == MarcRelator::AUTHOR || type == MarcRelator::CREATOR) {
              return data->fileAs->name;
            }
          }
        }
        case 2: { // This depends on type
          return MarcRelator::relatorToDescription(type);
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
        m_names.replace(index.row(), value.toString());
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
  m_names.append(value);
  m_modified.append(true);
  endInsertRows();
}

void
MetadataModel::initialiseData(PDocument document)
{
  m_metadata = document->metadata();
  auto authorList = m_metadata->creatorList();
  auto contributorList = m_metadata->contributorList();
  beginResetModel();
  m_names.clear();
  for (auto& author : authorList) {
    auto authorData = document->metadata()->creatorFromName(author);
    m_names.append(author);
    m_data.append(authorData);
  }
  for (auto& contributor : contributorList) {
    auto contibutorData = m_metadata->contributorFromName(contributor);
    m_names.append(contributor);
    m_data.append(contibutorData);
  }
  endResetModel();
  m_modified.append(false);
}

bool
MetadataModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (row < 0 || row > rowCount(parent))
    return false;

  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int i = 0; i < count; i++) {
    m_names.insert(row, QString());
    //    m_data.insert(row, QSharedPointer<EPubCreator>(new EPubCreator()));
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
      m_names.removeAt(row);
      m_data.removeAt(row);
      m_modified.removeAt(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

QStringList
MetadataModel::names()
{
  return m_names;
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
  for (int i = 0; i < m_names.size(); i++) {
    if (m_modified[i]) {
      map.insert(i, m_names.at(i));
    }
  }
  return map;
}

bool
MetadataModel::swapWithFirst(int row)
{
  if (row > 0 && row < m_names.size()) {
    auto first = m_names.first();
    auto swapper = m_names.at(row);
    beginResetModel();
    m_names.replace(0, swapper);
    m_names.replace(row, first);
    endResetModel();
    return true;
  }
  return false;
}

bool
MetadataModel::setNameData(int row, const QString& name)
{
  if (row >= 0 && row < m_names.size()) {
    m_names.replace(row, name);

    endResetModel();
    return true;
  }
  return false;
}

bool
MetadataModel::modifyNameData(int row, const QString& author)
{
  auto rows = rowCount(QModelIndex());
  if (row < 0 || row > rows)
    return false;

  if (row >= m_names.size()) {
    return false;
  }
  m_names.replace(row, author);
  m_modified.replace(row, true);
  return true;
}

//====================================================================
//=== AuthorList
//====================================================================
MetadataList::MetadataList(QWidget* parent)
  : QTableView(parent)
{
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_model = new MetadataModel(this);
  setModel(m_model);
}

MetadataList::~MetadataList() {}

void
MetadataList::initialiseData(PDocument document)
{
  m_model->initialiseData(document);
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
    m_model->setNameData(row, author);
  }
  return success;
}

bool
MetadataList::modifyAuthor(int row, const QString& author)
{
  auto success = m_model->modifyNameData(row, author);
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
