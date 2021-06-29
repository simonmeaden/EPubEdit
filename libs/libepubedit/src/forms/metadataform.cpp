#include "forms/metadataform.h"

#include "document/authors.h"
#include "document/ebookbasemetadata.h"

const QString MetadataForm::TITLESTR = "%1%2";

MetadataForm::MetadataForm(QWidget* parent)
  : QFrame(parent)
{
  QFrame* mainFrame = new QFrame(this);
  auto mainLayout = new QGridLayout;
  mainFrame->setLayout(mainLayout);

  auto layout = new QFormLayout;
  mainLayout->addLayout(layout, 0, 0);

  //  setStyleSheet("background-color: lightgreen;");
  layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

  m_titleView = new QTableView(this);
  m_titleView->setModel(new TitleModel());
  m_titleView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addRow(tr("Title(s) :"), m_titleView);

  m_authorView = new QListView(this);
  m_authorView->setModel(new AuthorModel());
  m_authorView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addRow(tr("Author(s) :"), m_authorView);

  auto btnFrame = new QFrame(this);
  auto btnLayout = new QHBoxLayout;
  btnFrame->setLayout(btnLayout);

  auto acceptBtn = new QPushButton(tr("Accept Changes"), this);
  connect(acceptBtn, &QPushButton::clicked, this, &MetadataForm::acceptChanges);
  btnLayout->addWidget(acceptBtn);

  auto cancelBtn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancelBtn, &QPushButton::clicked, this, &MetadataForm::cancelChanges);
  btnLayout->addWidget(cancelBtn);

  mainLayout->addWidget(btnFrame, 1, 0);
}

void
MetadataForm::acceptChanges()
{
  emit dataHasChanged();
}

void
MetadataForm::cancelChanges()
{
  // TODO
}

Metadata
MetadataForm::metadata() const
{
  return m_metadata;
}

void
MetadataForm::setMetadata(const Metadata& metadata)
{
  m_metadata = metadata;
  auto list = m_metadata->orderedTitles();
  qobject_cast<TitleModel*>(m_titleView->model())->setData(list);
}

//=== TitleModel ===================================================
int
TitleModel::rowCount(const QModelIndex&) const
{
  return m_titles.size();
}

int
TitleModel::columnCount(const QModelIndex&) const
{
  return 3;
}

QVariant
TitleModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  switch (role) {
    case Qt::DisplayRole: {
      int row = index.row();
      int col = index.column();
      switch (col) {
        case 0:
          if (row == 0) {
            return tr("Primary Title");
          } else {
            return tr("Alternate title");
          }
          break;
        case 1:
          return m_titles.at(row)->id;
        case 2:
          return m_titles.at(row)->title;
      }
    }
  }

  return QVariant();
}

QVariant
TitleModel::headerData(int section, Qt::Orientation orientation, int) const
{
  switch (orientation) {
    case Qt::Horizontal: {
      switch (section) {
        case 1:
          return tr("id");
        case 2:
          return tr("Title");
      }
      break;
    }
    default:
      break;
  }
  return QVariant();
}

bool
TitleModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid()) {
    return false;
  }

  if (role == Qt::EditRole) {
    auto title = m_titles[index.row()];
    switch (index.column()) {
      case 1:
        title->id = value.toString();
        m_titles.replace(index.row(), title);
        m_modified.replace(index.row(), true);
        return true;
      case 2:
        title->title = value.toString();
        m_titles.replace(index.row(), title);
        m_modified.replace(index.row(), true);
        return true;
    }
  }

  return false;
}

void
TitleModel::setData(TitleList titles)
{
  beginResetModel();
  m_titles.clear();
  auto row = 0;
  for (auto& title : titles) {
    m_titles.append(title);
    m_originalTitles.append(title);
    m_modified.append(false);
    row++;
  }
  endResetModel();
}

bool
TitleModel::setData(const QModelIndex& index, Title data)
{
  if (!index.isValid())
    return false;

  if (index.row() < m_titles.size()) {
    m_titles[index.row()] = data;
    m_modified.replace(index.row(), true);
  } else {
    beginInsertRows(QModelIndex(), index.row() + 1, index.row() + 1);
    m_titles.append(data);
    m_modified.append(true);
    endInsertRows();
  }
  return true;
}

TitleList
TitleModel::titles()
{
  //  OrderedTitleMap map;
  //  for (int i = 0; i < m_titles.size(); i++) {
  //    map.insert(i, m_titles.at(i));
  //  }
  //  return map;
  return m_titles;
}

bool
TitleModel::areModified()
{
  for (auto modified : m_modified) {
    if (modified)
      return true;
  }
  return false;
}

bool
TitleModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

TitleList
TitleModel::modifiedTitles()
{
  //  OrderedTitleMap map;
  //  for (int i = 0; i < m_titles.size(); i++) {
  //    if (m_modified[i]) {
  //      map.insert(i, m_titles.at(i));
  //    }
  //  }
  //  return map;
  return m_titles;
}

//=== AuthorModel ===================================================
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
AuthorModel::setData(const QModelIndex& index, QString value)
{
  beginInsertRows(QModelIndex(), index.row() + 1, index.row() + 1);
  m_authors.append(value);
  m_modified.append(true);
  endInsertRows();
}

void
AuthorModel::setData(QStringList authors)
{
  beginResetModel();
  m_authors.clear();
  for (auto& author : authors) {
    m_authors.append(author);
    m_modified.append(false);
  }
  endResetModel();
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
