#include "threecolview.h"
#include "document/metadata.h"

//====================================================================
//=== TitleView
//====================================================================
ThreeColView::ThreeColView(QWidget* parent)
  : QTableView(parent)
{
  m_model = new ThreeColModel(this);
  setModel(m_model);
  verticalHeader()->hide();
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  resizeTableVertically();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ThreeColView::~ThreeColView() {}

bool
ThreeColView::insertRow(int row)
{
  auto success = m_model->insertRows(row, 1);
  if (success)
    resizeTableVertically();
  return success;
}

bool
ThreeColView::removeRow(int row)
{
  auto success = m_model->removeRows(row, 1);
  if (success) {
    emit rowRemoved(row);
    resizeTableVertically();
  }
  return success;
}

bool
ThreeColView::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

bool
ThreeColView::setFirstAt(int row, const QString& text)
{
  return m_model->setFirstAt(row, text);
}

bool
ThreeColView::setSecondAt(int row, const QString& text)
{
  return m_model->setSecondAt(row, text);
}

bool
ThreeColView::setThirdAt(int row, const QString& text)
{
  return m_model->setThirdAt(row, text);
}

bool
ThreeColView::setModifiedAt(int row, bool modified)
{
  return m_model->setModifiedAt(row, modified);
}

QString
ThreeColView::firstAt(int row)
{
  return m_model->firstAt(row);
}

QString
ThreeColView::secondAt(int row)
{
  return m_model->secondAt(row);
}

QString
ThreeColView::thirdAt(int row)
{
  return m_model->thirdAt(row);
}

QModelIndex
ThreeColView::primaryTitleIndex()
{
  return m_model->index(0, 0);
}

bool
ThreeColView::addRow(const QString& text1,
                     const QString& text2,
                     const QString& text3)
{
  int row = m_model->rowCount(QModelIndex());
  insertRow(row);
  m_model->setFirstAt(row, text1);
  m_model->setSecondAt(row, text2);
  m_model->setThirdAt(row, text3);
  m_model->setModifiedAt(row, false);
  return true;
}

QSize
ThreeColView::sizeHint() const
{
  return m_hint;
}

void
ThreeColView::clear()
{
  m_model->clear();
}

void
ThreeColView::resizeTableVertically()
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
//=== TitleModel
//====================================================================
ThreeColModel::ThreeColModel(QWidget* parent)
  : QAbstractTableModel(parent)
{}

ThreeColModel::~ThreeColModel() {}

int
ThreeColModel::rowCount(const QModelIndex&) const
{
  return m_firstCol.size();
}

int
ThreeColModel::columnCount(const QModelIndex&) const
{
  return 3;
}

QVariant
ThreeColModel::headerData(int section, Qt::Orientation orientation, int) const
{
  switch (orientation) {
    case Qt::Horizontal: {
      switch (section) {
        case 1:
          return tr("Title");
        case 2:
          return tr("id");
      }
      break;
    }
    default:
      break;
  }
  return QVariant();
}

QVariant
ThreeColModel::data(const QModelIndex& index, int role) const
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
            return m_firstCol.at(row);
          case 1:
            return m_secondCol.at(row);
          case 2:
            return m_thirdCol.at(row);
        }
      }
    }
  }

  return QVariant();
}

bool
ThreeColModel::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role)
{
  if (!index.isValid()) {
    return false;
  }

  if (role == Qt::EditRole) {
    switch (index.column()) {
      case 0: {
        auto first = value.toString();
        m_firstCol.replace(index.row(), first);
        m_modified.replace(index.row(), true);
        return true;
      }
      case 1: {
        auto second = value.toString();
        m_secondCol.replace(index.row(), second);
        m_modified.replace(index.row(), true);
        return true;
      }
      case 2: {
        auto third = value.toString();
        m_thirdCol.replace(index.row(), third);
        m_modified.replace(index.row(), true);
        return true;
      }
    }
  }

  return false;
}

void
ThreeColModel::insertRowData(int row,
                             const QString& text1,
                             const QString& text2,
                             const QString& text3)
{
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_firstCol.insert(row, text1);
  m_secondCol.insert(row, text2);
  m_thirdCol.insert(row, text3);
  m_modified.insert(row, true);
  endInsertRows();
}

bool
ThreeColModel::swapWithFirst(int row)
{
  if (row > 0 && row < m_firstCol.size()) {
    auto oldFirst = m_firstCol.first();
    auto oldSecond = m_secondCol.first();
    auto oldThird = m_thirdCol.first();
    auto newFirst = m_firstCol.at(row);
    auto newSecond = m_secondCol.at(row);
    auto newThird = m_thirdCol.at(row);
    beginResetModel();
    m_firstCol.replace(0, newFirst);
    m_secondCol.replace(0, newSecond);
    m_thirdCol.replace(0, newThird);
    m_firstCol.replace(row, oldFirst);
    m_secondCol.replace(row, oldSecond);
    m_thirdCol.replace(row, oldThird);
    endResetModel();
    return true;
  }
  return false;
}

bool
ThreeColModel::setFirstAt(int row, const QString& text)
{
  if (row >= 0 && row < m_firstCol.size()) {
    beginResetModel();
    m_firstCol.replace(row, text);
    m_modified.replace(row, true);
    endResetModel();
    return true;
  }
  return false;
}

bool
ThreeColModel::setSecondAt(int row, const QString& text)
{
  if (row >= 0 && row < m_secondCol.size()) {
    beginResetModel();
    m_secondCol.replace(row, text);
    m_modified.replace(row, true);
    endResetModel();
    return true;
  }
  return false;
}

bool
ThreeColModel::setThirdAt(int row, const QString& text)
{
  if (row >= 0 && row < m_thirdCol.size()) {
    beginResetModel();
    m_thirdCol.replace(row, text);
    m_modified.replace(row, true);
    endResetModel();
    return true;
  }
  return false;
}

bool
ThreeColModel::setModifiedAt(int row, bool modified)
{
  if (row >= 0 && row < m_modified.size()) {
    beginResetModel();
    m_modified.replace(row, modified);
    endResetModel();
    return true;
  }
  return false;
}

QString
ThreeColModel::firstAt(int row)
{
  if (row >= 0 && row < m_firstCol.size())
    return m_firstCol.at(row);
  return QString();
}

QString
ThreeColModel::secondAt(int row)
{
  if (row >= 0 && row < m_firstCol.size()) {
    return m_firstCol.at(row);
  }
  return QString();
}

QString
ThreeColModel::thirdAt(int row)
{
  if (row >= 0 && row < m_thirdCol.size()) {
    return m_thirdCol.at(row);
  }
  return QString();
}

QStringList
ThreeColModel::firstCol()
{
  return m_firstCol;
}
QStringList
ThreeColModel::secondCol()
{
  return m_secondCol;
}
QStringList
ThreeColModel::thirdCol()
{
  return m_thirdCol;
}

QList<bool>
ThreeColModel::modifiedCol()
{
  return m_modified;
}

bool
ThreeColModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

bool
ThreeColModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (row < 0 || row > rowCount(parent))
    return false;

  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int i = 0; i < count; i++) {
    m_firstCol.insert(row, QString());
    m_secondCol.insert(row, QString());
    m_thirdCol.insert(row, QString());
    m_modified.insert(row, false);
  }
  endInsertRows();

  return true;
}

bool
ThreeColModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row >= 0 || row < rowCount(parent)) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = row; i < row + count; i++) {
      m_firstCol.removeAt(row);
      m_secondCol.removeAt(row);
      m_thirdCol.removeAt(row);
      m_modified.removeAt(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

void
ThreeColModel::clear()
{
  m_firstCol.clear();
  m_secondCol.clear();
  m_thirdCol.clear();
  m_modified.clear();
}
