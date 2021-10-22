#include "titleview.h"

//====================================================================
//=== TitleView
//====================================================================
TitleView::TitleView(QWidget* parent)
  : QTableView(parent)
{
  m_model = new TitleModel(this);
  setModel(m_model);
  horizontalHeader()->setStretchLastSection(true);
  resizeTableVertically();
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TitleView::~TitleView() {}

void
TitleView::initialiseData(QList<QSharedPointer<EPubTitle>> data,
                          QSharedPointer<UniqueStringList> uniqueIdList)
{
  m_uniqueIdList = uniqueIdList;
  m_model->initialiseData(data, uniqueIdList);
  setCurrentIndex(m_model->index(0, 0));
  resizeTableVertically();
}

bool
TitleView::removeTitle(int row)
{
  auto success = m_model->removeRow(1, QModelIndex());
  if (success) {
    emit titleRemoved(row);
  }
  return success;
}

bool
TitleView::insertTitle(int row, QSharedPointer<EPubTitle> title)
{
  auto success = m_model->insertRow(row, QModelIndex());
  if (success) {
    m_model->setTitle(row, title);
  }
  return success;
}

bool
TitleView::modifyTitle(int row, const QString& title)
{
  return m_model->modifyTitle(row, title);
}

bool
TitleView::setTitle(int row, const QString& title)
{
  auto index = m_model->index(row, 2);
  if (index.isValid()) {
    auto success = m_model->setData(index, QVariant::fromValue(title));
    auto index = m_model->index(row, 2);
    dataChanged(index, index);
    return success;
  }
  return false;
}

bool
TitleView::setId(int row, const UniqueString& id)
{
  auto index = m_model->index(row, 1);
  if (index.isValid()) {
    auto success = m_model->setData(index, QVariant::fromValue(id.toString()));
    auto index = m_model->index(row, 1);
    dataChanged(index, index);
    return success;
  }
  return false;
}

UniqueString
TitleView::id(int row)
{
  return m_model->idStrAt(row);
}

bool
TitleView::swapToPrimaryPosition(int row)
{
  return m_model->swapWithFirst(row);
}

QSharedPointer<EPubTitle>
TitleView::titleAt(int row)
{
  return m_model->titleAt(row);
}

UniqueString
TitleView::idAt(int row)
{
  return m_model->idStrAt(row);
}

QString
TitleView::titleStrAt(int row)
{
  return m_model->titleStrAt(row);
}

QModelIndex
TitleView::primaryTitleIndex()
{
  return m_model->index(0, 0);
}

bool
TitleView::hasId(int row)
{
  return m_model->hasId(row);
}

QSize
TitleView::sizeHint() const
{
  return m_hint;
}

QSharedPointer<UniqueStringList>
TitleView::uniqueIdList() const
{
  return m_uniqueIdList;
}

void
TitleView::resizeTableVertically()
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

void
TitleView::setUniqueIdList(const QSharedPointer<UniqueStringList>& uniqueIdList)
{
  m_uniqueIdList = uniqueIdList;
}

//====================================================================
//=== TitleModel
//====================================================================
TitleModel::TitleModel(QWidget* parent)
  : QAbstractTableModel(parent)
{}

TitleModel::~TitleModel() {}

int
TitleModel::rowCount(const QModelIndex&) const
{
  auto size = m_titles.size();
  if (size == 0)
    return 1;
  return size;
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
      auto size = m_titles.size();
      if (size == 0) {
        return QString();
      } else {
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
            return m_titles.at(row)->id.toString();
          case 2:
            return m_titles.at(row)->title;
        }
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
        title->id = m_uniqueIdList->append(value.toString(), -1);
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
TitleModel::initialiseData(QList<QSharedPointer<EPubTitle>> titles,
                           QSharedPointer<UniqueStringList> uniqueIdList)
{
  m_uniqueIdList = uniqueIdList;
  beginResetModel();
  m_titles.clear();
  for (auto& title : titles) {
    m_titles.append(title);
    m_originalTitles.append(title);
    m_modified.append(false);
  }
  endResetModel();
}

bool
TitleModel::modifyTitle(int row, const QString& titleStr)
{
  auto rows = rowCount(QModelIndex());
  if (row < 0 || row > rows)
    return false;

  if (row >= m_titles.size()) {
    return false;
  }
  auto title = m_titles.at(row);
  title->title = titleStr;
  m_modified.replace(row, true);
  return true;
}

bool
TitleModel::modifyId(int row, const QString& idStr)
{
  auto rows = rowCount(QModelIndex());
  if (row < 0 || row > rows)
    return false;

  if (row >= m_titles.size()) {
    return false;
  }
  auto title = m_titles.at(row);
  title->id = m_uniqueIdList->append(idStr, -1);
  m_modified.replace(row, true);
  return true;
}

bool
TitleModel::swapWithFirst(int row)
{
  if (row > 0 && row < m_titles.size()) {
    auto first = m_titles.first();
    auto swapper = m_titles.at(row);
    beginResetModel();
    m_titles.replace(0, swapper);
    m_titles.replace(row, first);
    endResetModel();
    return true;
  }
  return false;
}

bool
TitleModel::setTitle(int row, QSharedPointer<EPubTitle> title)
{
  if (row >= 0 && row < m_titles.size()) {
    beginResetModel();
    m_titles.replace(row, title);
    endResetModel();
    return true;
  }
  return false;
}

bool
TitleModel::hasId(int row)
{
  if (row >= 0 || row < m_titles.size())
    return !m_titles.at(row)->id.isEmpty();
  return false;
}

QSharedPointer<EPubTitle>
TitleModel::titleAt(int row)
{
  if (row >= 0 && row < m_titles.size())
    return m_titles.at(row);
  return QSharedPointer<EPubTitle>(new EPubTitle());
}

QString
TitleModel::titleStrAt(int row)
{
  if (row >= 0 && row < m_titles.size())
    return m_titles.at(row)->title;
  return QString();
}

UniqueString
TitleModel::idStrAt(int row)
{
  if (row >= 0 && row < m_titles.size())
    return m_titles.at(row)->id;
  return UniqueString();
}

QList<QSharedPointer<EPubTitle>>
TitleModel::titles()
{
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

QList<QSharedPointer<EPubTitle>>
TitleModel::modifiedTitles()
{
  return m_titles;
}

bool
TitleModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (row < 0 || row > rowCount(parent))
    return false;

  beginInsertRows(QModelIndex(), row, row + count - 1);
  for (int i = 0; i < count; i++) {
    m_titles.insert(row, QSharedPointer<EPubTitle>(new EPubTitle()));
    m_modified.insert(row, false);
  }
  endInsertRows();

  return true;
}

bool
TitleModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row >= 0 || row < rowCount(parent)) {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = row; i < row + count; i++) {
      m_titles.removeAt(row);
      m_modified.removeAt(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

//====================================================================
//=== TitleEditDialog
//====================================================================
TitleEditDialog::TitleEditDialog(QSharedPointer<EPubTitle> title,
                                 QSharedPointer<UniqueStringList> uniqueIdList,
                                 QWidget* parent)
  : QDialog(parent)
  , m_title(title)
  , m_uniqueIdList(uniqueIdList)
{
  setWindowTitle(tr("Edit Title Values"));
  initGui();
}

void
TitleEditDialog::initGui()
{
  auto layout = new QFormLayout;
  setLayout(layout);

  m_label = new QLabel(tr("Modify values :"), this);
  m_label->setStyleSheet("QLabel {color: black;}");
  layout->addRow(m_label);

  auto titleEdit = new QLineEdit(this);
  titleEdit->setText(m_title->title);
  layout->addRow(tr("Title :"), titleEdit);
  connect(
    titleEdit, &QLineEdit::textChanged, this, &TitleEditDialog::titleChanged);

  auto idEdit = new QLineEdit(this);
  idEdit->setText(m_title->id.toString());
  idEdit->setStyleSheet("color: black;");
  layout->addRow(tr("ID :"), idEdit);
  connect(idEdit, &QLineEdit::textChanged, this, &TitleEditDialog::idChanged);

  auto dirEdit = new QComboBox(this);
  dirEdit->setCurrentIndex(int(m_title->dir));
  dirEdit->addItem(tr("Left to Right"));
  dirEdit->addItem(tr("Right to Left"));
  layout->addRow(tr("Direction :"), dirEdit);
  connect(dirEdit,
          qOverload<int>(&QComboBox::currentIndexChanged),
          this,
          &TitleEditDialog::directionChanged);

  auto langEdit = new QLineEdit(this);
  langEdit->setText(m_title->lang);
  layout->addRow(tr("Language :"), langEdit);
  connect(
    langEdit, &QLineEdit::textChanged, this, &TitleEditDialog::langChanged);

  auto dateEdit = new QDateEdit(this);
  dateEdit->setDate(m_title->date);
  layout->addRow(tr("Date :"), dateEdit);
  connect(
    dateEdit, &QDateEdit::dateChanged, this, &TitleEditDialog::dateChanged);

  m_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok |
                                 QDialogButtonBox::Help,
                               this);
  connect(m_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  layout->addRow(m_box);
}

void
TitleEditDialog::idChanged(const QString& text)
{
  if (m_uniqueIdList->contains(text)) {
    m_label->setText(tr("ID value supplied id NOT unique!\n"
                        "%1 has been used elsewhere.")
                       .arg(text));
    m_label->setStyleSheet("QLabel {color: red;}");
    m_box->button(QDialogButtonBox::Ok)->setEnabled(false);
  } else {
    m_label->setText(tr("ID value is unique."));
    m_label->setStyleSheet("QLabel {color: green;}");
    m_title->id = text;
    m_box->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}

void
TitleEditDialog::titleChanged(const QString& text)
{
  m_title->title = text;
}

void
TitleEditDialog::directionChanged(int index)
{
  m_title->dir = Direction(index);
}

void
TitleEditDialog::langChanged(const QString& text)
{
  m_title->lang = text;
}

void
TitleEditDialog::dateChanged(const QDate& date)
{
  m_title->date = date;
}
