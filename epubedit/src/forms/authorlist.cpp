#include "authorlist.h"

//====================================================================
//=== AuthorModel
//====================================================================
AuthorModel::AuthorModel(QWidget* parent)
  : QAbstractTableModel(parent)
{}

AuthorModel::~AuthorModel() {}

int
AuthorModel::rowCount(const QModelIndex&) const
{
  return m_authors.size();
}

int
AuthorModel::columnCount(const QModelIndex&) const
{
  return 1;
}

QVariant
AuthorModel::headerData(int section, Qt::Orientation orientation, int) const
{
  switch (orientation) {
    case Qt::Horizontal: {
      switch (section) {
        case 0:
          return tr("Author");
      }
      break;
    }
    default:
      break;
  }
  return QVariant();
}

QVariant
AuthorModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch (role) {
    case Qt::DisplayRole: {
      int row = index.row();
      int col = index.column();
      switch (col) {
        case 0:
          return m_authors[row];
      }
    }
  }

  return QVariant();
}

bool
AuthorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid()) {
    return false;
  }

  if (role == Qt::EditRole) {
    m_authors.replace(index.row(), value.toString());
    m_modified.replace(index.row(), true);
    return true;
  }

  return false;
}

void
AuthorModel::modifyRowData(int row, const QString& value)
{
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_authors.append(value);
  m_modified.append(true);
  endInsertRows();
}

void
AuthorModel::initialiseData(QStringList authors)
{
  beginResetModel();
  m_authors.clear();
  for (auto& author : authors) {
    m_authors.append(author);
    m_modified.append(false);
  }
  endResetModel();
}

bool
AuthorModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (row < 0 || row > rowCount(parent))
    return false;

  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int i = 0; i < count; i++) {
    m_authors.insert(row, QString());
    m_modified.insert(row, false);
  }
  endInsertRows();

  return true;
}

bool
AuthorModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row >= 0 || row < rowCount(parent)) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = row; i < row + count; i++) {
      m_authors.removeAt(row);
      m_modified.removeAt(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

QStringList
AuthorModel::authors()
{
  return m_authors;
}

bool
AuthorModel::areModified()
{
  for (auto modified : m_modified) {
    if (modified)
      return true;
  }
  return false;
}

bool
AuthorModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

QMap<int, QString>
AuthorModel::modifiedAuthors()
{
  QMap<int, QString> map;
  for (int i = 0; i < m_authors.size(); i++) {
    if (m_modified[i]) {
      map.insert(i, m_authors.at(i));
    }
  }
  return map;
}

bool
AuthorModel::swapWithFirst(int row)
{
  if (row > 0 && row < m_authors.size()) {
    auto first = m_authors.first();
    auto swapper = m_authors.at(row);
    beginResetModel();
    m_authors.replace(0, swapper);
    m_authors.replace(row, first);
    endResetModel();
    return true;
  }
  return false;
}

bool
AuthorModel::setAuthor(int row, const QString& author)
{
  if (row >= 0 && row < m_authors.size()) {
    beginResetModel();
    m_authors.replace(row, author);
    endResetModel();
    return true;
  }
  return false;
}

bool
AuthorModel::modifyAuthor(int row, const QString& author)
{
  auto rows = rowCount(QModelIndex());
  if (row < 0 || row > rows)
    return false;

  if (row >= m_authors.size()) {
    return false;
  }
  m_authors.replace(row, author);
  m_modified.replace(row, true);
  return true;
}

//====================================================================
//=== AuthorList
//====================================================================
AuthorList::AuthorList(QWidget* parent)
  : QTableView(parent)
{
  horizontalHeader()->setStretchLastSection(true);
  m_model = new AuthorModel(this);
  setModel(m_model);
}

AuthorList::~AuthorList() {}

void
AuthorList::initialiseData(QStringList authors)
{
  m_model->initialiseData(authors);
  setCurrentIndex(m_model->index(0, 0));
}

bool
AuthorList::removeAuthor(int row)
{
  auto success = m_model->removeRow(1, QModelIndex());
  if (success) {
    emit authorRemoved(row);
  }
  return success;
}

bool
AuthorList::insertAuthor(int row, const QString& author)
{
  auto success = m_model->insertRow(row, QModelIndex());
  if (success) {
    m_model->setAuthor(row, author);
  }
  return success;
}

bool
AuthorList::modifyAuthor(int row, const QString& author)
{
  auto success = m_model->modifyAuthor(row, author);
  auto index = m_model->index(row, 0);
  dataChanged(index, index);
  return success;
}

QModelIndex
AuthorList::primaryAuthorIndex()
{
  return m_model->index(0, 0);
}

bool
AuthorList::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSize
AuthorList::sizeHint() const
{
  return m_hint;
}

void
AuthorList::resizeTableVertically()
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
AuthorList::setAuthor(int row, const QString& author)
{
  auto index = m_model->index(row, 0);
  if (index.isValid()) {
    return m_model->setData(index, QVariant::fromValue(author));
  }
  return false;
}

QString
AuthorList::authorAt(int row)
{
  auto index = m_model->index(row, 0);
  return m_model->data(index).toString();
}
