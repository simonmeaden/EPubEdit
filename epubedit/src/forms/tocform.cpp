#include "tocform.h"
#include "document/metadata.h"
#include "navitem.h"

#include <QDebug>

//====================================================================
//=== TocForm
//====================================================================
TocForm::TocForm(QWidget* parent)
  : QWidget{ parent }
{
  setContentsMargins(0, 0, 0, 0);
  auto layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_treeView = new QTreeView(this);
  m_treeView->setHeaderHidden(true);
  layout->addWidget(m_treeView);
  connect(m_treeView, &QTreeView::clicked, this, &TocForm::tocClicked);

  m_model = new TocModel(this);
  m_treeView->setModel(m_model);
}

void
TocForm::setMetadata(PMetadata metadata)
{
  m_metadata = metadata;
  auto navList = m_metadata->navList();
  m_model->setData(navList);
}

void
TocForm::tocClicked(const QModelIndex& index)
{
  auto navItem = static_cast<NavItem*>(index.internalPointer());
  auto valueItem = dynamic_cast<NavValueItem*>(navItem);
  if (valueItem) {
    auto href = valueItem->findFirstHref();
    if (!href.isEmpty())
      emit itemSelected(href);
  }
  // TODO handle list clicked.
}

//====================================================================
//=== TocModel
//====================================================================
TocModel::TocModel(QObject* parent)
  : QAbstractItemModel(parent)
{}

TocModel::~TocModel() {}

void
TocModel::setData(NavListItem* data)
{
  beginResetModel();
  m_data = data;
  endResetModel();
}

bool
TocModel::hasIndex(int row, int column, const QModelIndex& parent) const
{
  if (row >= 0 && row < m_data->itemCount()) {
    return true;
  }
  return false;
}

QModelIndex
TocModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  auto navItem = m_data->item(row);
  auto index = createIndex(row, column, navItem);

  // TODO handle sub lists
  return index;
}

QModelIndex
TocModel::parent(const QModelIndex& child) const
{
  return QModelIndex();
}

int
TocModel::rowCount(const QModelIndex& parent) const
{
  return m_data->itemCount();
}

int
TocModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return static_cast<NavItem*>(parent.internalPointer())->columnCount();
  return 1;
}

QVariant
TocModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  //  auto row = index.row();
  //  auto col = index.column();

  auto item = static_cast<NavItem*>(index.internalPointer());

  switch (role) {
    case Qt::DisplayRole: {
      auto valueItem = dynamic_cast<NavValueItem*>(item);
      if (valueItem)
        return valueItem->value();
      auto listItem = dynamic_cast<NavListItem*>(item);
      if (listItem) {
        // TODO handle sub lists
        auto navItem = listItem->item(index.column());
        auto value = dynamic_cast<NavValueItem*>(navItem);
        if (value)
          return value->value();
        return QVariant();
      }
    }
  }

  return QVariant();
}

bool
TocModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  // TODO first display it then work out how to modify it.
  return false;
}

bool
TocModel::insertRows(int row, int count, const QModelIndex& parent)
{
  // TODO first display it then work out how to modify it.
  return false;
}

bool
TocModel::removeRows(int row, int count, const QModelIndex& parent)
{
  // TODO first display it then work out how to modify it.
  return false;
}

bool
TocModel::moveRows(const QModelIndex& sourceParent,
                   int sourceRow,
                   int count,
                   const QModelIndex& destinationParent,
                   int destinationChild)
{
  // TODO first display it then work out how to modify it.
  return false;
}
