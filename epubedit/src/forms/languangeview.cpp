#include "languangeview.h"
#include "config.h"
#include "document/metadata.h"
#include "languages.h"

//====================================================================
//=== LanguageView
//====================================================================
LanguageView::LanguageView(PConfig config, QWidget* parent)
  : QTableView(parent)
{
  m_model = new LanguageModel(config, this);
  setModel(m_model);
  verticalHeader()->hide();
  horizontalHeader()->setStretchLastSection(true);
  horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  resizeTableVertically();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

LanguageView::~LanguageView() {}

void
LanguageView::initialiseData(PMetadata metadata)
{
  m_model->initialiseData(metadata);
  setCurrentIndex(m_model->index(0, 0));
  resizeTableVertically();
}

bool
LanguageView::insertRow(int row)
{
  auto success = m_model->insertRows(row, 1);
  if (success)
    resizeTableVertically();
  return success;
}

bool
LanguageView::removeRow(int row)
{
  auto success = m_model->removeRow(row, QModelIndex());
  if (success) {
    emit rowRemoved(row);
    resizeTableVertically();
  }
  return success;
}

bool
LanguageView::setFirstAt(int row, const QString& text)
{
  return m_model->setFirstAt(row, text);
}

bool
LanguageView::setSecondAt(int row, const QString& text)
{
  return m_model->setSecondAt(row, text);
}

bool
LanguageView::setModifiedAt(int row, const bool& modified)
{
  return m_model->setModifiedAt(row, modified);
}

QModelIndex
LanguageView::primaryAuthorIndex()
{
  return m_model->index(0, 0);
}

bool
LanguageView::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSize
LanguageView::sizeHint() const
{
  return m_hint;
}

QString
LanguageView::firstAt(int row)
{
  auto index = m_model->index(row, 0);
  return m_model->data(index).toString();
}

QString
LanguageView::secondAt(int row)
{
  auto index = m_model->index(row, 1);
  return m_model->data(index).toString();
}

bool
LanguageView::modifiedAt(int row)
{
  auto index = m_model->index(row, 2);
  return m_model->data(index).toBool();
}

void
LanguageView::resizeTableVertically()
{
  auto height = viewportMargins().top();
  height += contentsMargins().top();

  if (horizontalScrollBar()->isVisible()) {
    height += horizontalScrollBar()->height();
  }
  if (horizontalHeader()->isVisible()) {
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

//====================================================================
//=== LanguageModel
//====================================================================
LanguageModel::LanguageModel(PConfig config, QWidget* parent)
  : QAbstractTableModel(parent)
{
  m_languageDB = config->languages();
}

LanguageModel::~LanguageModel() {}

int
LanguageModel::rowCount(const QModelIndex&) const
{
  return m_firstCol.size();
}

int
LanguageModel::columnCount(const QModelIndex&) const
{
  return 2;
}

QVariant
LanguageModel::headerData(int section, Qt::Orientation orientation, int) const
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
LanguageModel::data(const QModelIndex& index, int role) const
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
LanguageModel::setData(const QModelIndex& index,
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
LanguageModel::insertRowData(int row,
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
LanguageModel::initialiseData(PMetadata metadata)
{
  m_metadata = metadata;
  auto languages = m_metadata->languages();
  beginResetModel();
  m_firstCol.clear();
  m_secondCol.clear();
  m_modified.clear();
  for (auto& language : languages) {
    auto subtag = language->language;
    auto bcp47 = m_languageDB->languageFromSubtag(subtag);
    auto desc = bcp47->description();
    m_firstCol.append(subtag);
    m_secondCol.append(desc);
    m_modified.append(false);
  }
  endResetModel();
}

bool
LanguageModel::insertRows(int row, int count, const QModelIndex& parent)
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
LanguageModel::removeRows(int row, int count, const QModelIndex& parent)
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
LanguageModel::firstCol()
{
  return m_firstCol;
}

QStringList
LanguageModel::secondCol()
{
  return m_secondCol;
}

QList<bool>
LanguageModel::thirdCol()
{
  return m_modified;
}

bool
LanguageModel::areModified()
{
  for (auto& modified : m_modified) {
    if (modified)
      return true;
  }
  return false;
}

bool
LanguageModel::isModified(int row)
{
  if (row >= 0 && row < m_modified.size())
    return m_modified.at(row);
  return false;
}

QMap<int, QString>
LanguageModel::modifiedRows()
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
LanguageModel::swapWithFirst(int row)
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
LanguageModel::setFirstAt(int row, const QString& text)
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
LanguageModel::setSecondAt(int row, const QString& text)
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
LanguageModel::setModifiedAt(int row, const bool& modified)
{
  if (row >= 0 && row < m_modified.size()) {
    beginResetModel();
    m_modified.replace(row, modified);
    endResetModel();
    return true;
  }
  return false;
}
