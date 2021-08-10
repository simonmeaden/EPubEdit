#include "forms/metadataform.h"

#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/epubmetadata.h"

//====================================================================
//=== MetadataForm
//====================================================================
const QString MetadataForm::TITLESTR = "%1%2";

MetadataForm::MetadataForm(QUndoStack* undoStack, QWidget* parent)
  : QFrame(parent)
  , m_undoStack(undoStack)
  , m_currentIndex(QModelIndex())
{

  initGui();
}

MetadataForm::~MetadataForm() {}

void
MetadataForm::initGui()
{
  //  setStyleSheet("background-color: green;");
  auto layout = new QGridLayout;
  setLayout(layout);

  auto* tabs = new QTabWidget(this);
  tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(tabs, 0, 0);

  tabs->addTab(initMaindataFrame(), tr("Title/Author"));

  auto btnFrame = new QFrame(this);
  auto btnLayout = new QHBoxLayout;
  btnFrame->setLayout(btnLayout);

  auto acceptBtn = new QPushButton(tr("Save Changes"), this);
  connect(acceptBtn, &QPushButton::clicked, this, &MetadataForm::acceptChanges);
  btnLayout->addWidget(acceptBtn);

  auto cancelBtn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancelBtn, &QPushButton::clicked, this, &MetadataForm::cancelChanges);
  btnLayout->addWidget(cancelBtn);

  layout->addWidget(btnFrame, 1, 0);
}

QWidget*
MetadataForm::initMaindataFrame()
{
  auto frame = new QFrame(this);
  auto layout = new QFormLayout;
  frame->setLayout(layout);

  //  setStyleSheet("background-color: lightgreen;");
  layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

  m_titleView = new TitleView(this);
  m_titleView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_titleView->setContextMenuPolicy(Qt::CustomContextMenu);
  //  connect(m_titleView, &TitleView::clicked, this,
  //  &MetadataForm::titleClicked);
  connect(m_titleView,
          &TitleView::customContextMenuRequested,
          this,
          &MetadataForm::titleContextMenu);
  connect(
    m_titleView, &TitleView::titleRemoved, this, &MetadataForm::titleRemoved);
  layout->addRow(tr("Title(s) :"), m_titleView);

  m_authorView = new AuthorView(m_uniqueIdList, this);
  m_authorView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout->addRow(tr("Author(s) :"), m_authorView);

  return frame;
}

void
MetadataForm::acceptChanges()
{
  if (m_modifications != MetadataForm::NO_CHANGES) {
    if (m_modifications.testFlag(MetadataForm::TITLES_CHANGED)) {
      //      m_document->setTitle();
    }
    if (m_modifications.testFlag(MetadataForm::AUTHORS_CHANGED)) {
    }
  }
}

void
MetadataForm::cancelChanges()
{
  // TODO
}

void
MetadataForm::addTitle()
{
  auto text =
    QInputDialog::getText(this, tr("Enter new title"), tr("Title : "));
  if (!text.isEmpty()) {
    auto row = m_titleView->model()->rowCount();
    // adding the command to the stack calls redo() on the command so we don't
    // actually need to add the title manually.
    auto addCommand = new AddTitleCommand(m_titleView, text, row);
    m_undoStack->push(addCommand);
    m_modifications.setFlag(TITLES_CHANGED, true);
    emit sendStatusMessage(
      tr("Adding new title \"%1\"").arg(m_titleView->titleStrAt(row)));
    m_titleView->resizeTableVertically();
  }
}

void
MetadataForm::titleContextMenu(QPoint pos)
{
  m_currentIndex = m_titleView->indexAt(pos);

  auto menu = new QMenu(this);
  auto addTitleAct = new QAction("Add title", this);
  menu->addAction(addTitleAct);
  connect(addTitleAct, &QAction::triggered, this, &MetadataForm::addTitle);
  auto modifyAct = new QAction("Modify title", this);
  menu->addAction(modifyAct);
  connect(modifyAct, &QAction::triggered, this, &MetadataForm::modifyTitle);
  auto removeAct = new QAction("Remove title", this);
  menu->addAction(removeAct);
  connect(removeAct, &QAction::triggered, this, &MetadataForm::removeTitle);
  auto setIdAct = new QAction("Set id", this);
  menu->addAction(setIdAct);
  connect(setIdAct, &QAction::triggered, this, &MetadataForm::setId);
  auto primaryAct = new QAction("Set title as Primary", this);
  menu->addAction(primaryAct);
  connect(
    primaryAct, &QAction::triggered, this, &MetadataForm::setPrimaryTitle);
  if (m_titleView->hasId(m_currentIndex.row()) && m_currentIndex.row() > 0) {
    auto idToPrimaryAct = new QAction("Move Id to Primary", this);
    menu->addAction(idToPrimaryAct);
    connect(idToPrimaryAct,
            &QAction::triggered,
            this,
            &MetadataForm::moveIdToPrimary);
  }
  menu->addSeparator();
  auto editTitleAct = new QAction("Edit Title values", this);
  menu->addAction(editTitleAct);
  connect(primaryAct, &QAction::triggered, this, &MetadataForm::editTitle);

  menu->popup(m_titleView->viewport()->mapToGlobal(pos));
}

void
MetadataForm::modifyTitle()
{
  if (m_currentIndex.isValid()) {
    auto row = m_currentIndex.row();
    auto title = m_titleView->titleAt(row);
    auto titleStr1 = title->title;
    auto titleStr2 = QInputDialog::getText(
      this,
      tr("modify Title"),
      tr("You are attempting to modify the title \"%1\".\n"
         "Enter a new value for the title and press \"OK\"\n"
         "or press \"Cancel\" to abort change."),
      QLineEdit::Normal,
      titleStr1);
    if (!titleStr2.isEmpty() && titleStr1 != titleStr2) {
      // adding the command to the stack calls redo() on the command so we don't
      // actually need to modify the title manually.
      m_undoStack->push(
        new ModifyTitleCommand(m_titleView, titleStr2, row, titleStr1));
      m_modifications.setFlag(TITLES_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" has been changed to \"%2\".").arg(titleStr1, titleStr2));
    }
  }
}

void
MetadataForm::setId()
{
  if (m_currentIndex.isValid()) {
    auto row = m_currentIndex.row();
    auto title = m_titleView->titleAt(row);
    auto idStr1 = title->id.toString();
    auto idStr2 = QInputDialog::getText(
      this, tr("modify ID"), tr("Enter a new value for the id"));
    if (!idStr2.isEmpty() && idStr1 != idStr2) {
      if (m_uniqueIdList->contains(idStr2)) {
        QMessageBox::warning(this,
                             tr("Non-Unique ID value!"),
                             tr("Attempting to use an ID more than once.\n"
                                "ID's must be unique."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
      } else {
        m_undoStack->push(new SetIdCommand(m_titleView, idStr1, idStr2, row));
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("ID \"%1\" has been changed to \"%2\".").arg(idStr1, idStr2));
      }
    }
  }
}

void
MetadataForm::removeTitle()
{
  if (m_currentIndex.isValid()) {
    if (m_currentIndex.row() == 0) {
      QMessageBox::warning(this,
                           tr("Removing Primary Title"),
                           tr("You are attempting to remove the first,\n"
                              "or primary title. This is not allowed, you\n"
                              "must first set another title (if any) as the\n"
                              "primary title. Use the \"Set title as Primary\""
                              "menu item."));
    } else {
      auto row = m_currentIndex.row();
      auto title = m_titleView->titleAt(row);
      int btn =
        QMessageBox::question(this,
                              tr("Removing Alternative Title"),
                              tr("You are attempting to remove an alternate\n"
                                 "title. \"%1\"\n"
                                 "Are you sure? Press \"Yes\" to continue\n"
                                 "or \"No\" to cancel the operation.")
                                .arg(title->title),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);
      if (btn == QMessageBox::Yes) {
        //        m_titleView->removeTitle(row);
        m_undoStack->push(
          new RemoveTitleCommand(m_titleView, title, m_currentIndex.row()));
        m_currentIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("Removed title \"%1\"")
            .arg(m_titleView->titleStrAt(m_currentIndex.row())));
        m_titleView->resizeTableVertically();
      }
    }
  }
}

void
MetadataForm::titleRemoved(int row)
{
  if (row >= 0 && row < m_titleView->model()->rowCount(QModelIndex())) {
    m_currentIndex = QModelIndex();
  }
}

void
MetadataForm::setPrimaryTitle()
{
  if (m_currentIndex.isValid()) {
    if (m_currentIndex.row() == 0) {
      QMessageBox::warning(this,
                           tr("Setting Primary Title"),
                           tr("You are attempting to set the first,\n"
                              "title as the primary title. Please\n"
                              "note that it ALREADY the primary title\n"
                              "so nothing will be done."));
    } else {
      int btn =
        QMessageBox::question(this,
                              tr("Setting Primary Title"),
                              tr("You are attempting to move an alternate\n"
                                 "title to the primary position.\n"
                                 "Are you sure? Press \"Yes\" to continue\n"
                                 "or \"No\" to cancel the operation."),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);
      if (btn == QMessageBox::Yes) {
        auto title1 = m_titleView->titleAt(0);
        auto row2 = m_currentIndex.row();
        auto title2 = m_titleView->titleAt(row2);
        // adding the command to the stack calls redo() on the command so we
        // don't actually need to swap the title manually.
        m_undoStack->push(
          new SwapTitleCommand(m_titleView, title1, 0, title2, row2));
        m_currentIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("\"%1\" is now the primary title!")
            .arg(m_titleView->titleStrAt(m_currentIndex.row())));
      }
    }
  }
}

void
MetadataForm::moveIdToPrimary()
{
  if (m_currentIndex.isValid()) {
    int btn =
      QMessageBox::question(this,
                            tr("Moving current ID to  Primary"),
                            tr("You are attempting to move an alternate\n"
                               "ID to the primary position.\n"
                               "Are you sure? Press \"Yes\" to continue\n"
                               "or \"No\" to cancel the operation."),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No);
    if (btn == QMessageBox::Yes) {
      auto id1 = m_titleView->idAt(0);
      auto row2 = m_currentIndex.row();
      auto id2 = m_titleView->idAt(row2);
      // adding the command to the stack calls redo() on the command so we
      // don't actually need to swap the title manually.
      m_undoStack->push(new SwapIdCommand(m_titleView, id1, 0, id2, row2));
      m_currentIndex = m_titleView->primaryTitleIndex();
      m_modifications.setFlag(ID_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" is now the primary title!")
          .arg(m_titleView->titleStrAt(m_currentIndex.row())));
    }
  }
}

void
MetadataForm::editTitle()
{}

void
MetadataForm::setDocument(QSharedPointer<EPubDocument> document)
{
  m_document = document;
  auto list = m_document->metadata()->orderedTitles();
  m_uniqueIdList = m_document->uniqueIdList();
  m_titleView->initialiseData(list, m_uniqueIdList);
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
      m_titles.remove(row);
      m_modified.remove(row);
    }
    endRemoveRows();

    return true;
  }
  return false;
}

//====================================================================
//=== AuthorModel
//====================================================================
AuthorModel::AuthorModel(QSharedPointer<UniqueStringList> uniqueIdList,
                         QWidget* parent)
  : QAbstractTableModel(parent)
  , m_uniqueIdList(uniqueIdList)
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
AuthorModel::modifyRowData(int row, QString value)
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
    return m_model->setData(index, title);
  }
  return false;
}

bool
TitleView::setId(int row, const UniqueString& id)
{
  auto index = m_model->index(row, 1);
  if (index.isValid()) {
    return m_model->setData(index, id.toString());
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
//=== AuthorView
//====================================================================
AuthorView::AuthorView(QSharedPointer<UniqueStringList> uniqueIdList,
                       QWidget* parent)
  : QListView(parent)
  , m_uniqueIdList(uniqueIdList)
{
  setModel(new AuthorModel(m_uniqueIdList, this));
}

AuthorView::~AuthorView() {}

//====================================================================
//=== AddTitleCommand
//====================================================================
AddTitleCommand::AddTitleCommand(TitleView* view,
                                 const QString& title,
                                 int row,
                                 QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title(title)
  , m_row(row)
{
  auto text = QObject::tr("Add %1:%2").arg(m_row).arg(m_title);
  setText(text);
}

void
AddTitleCommand::undo()
{
  m_view->removeTitle(m_row);
}

void
AddTitleCommand::redo()
{
  auto success = m_view->model()->insertRow(m_row, QModelIndex());
  if (success) {
    m_view->setTitle(m_row, m_title);
  }
}

//====================================================================
//=== RemoveTitleCommand
//====================================================================
RemoveTitleCommand::RemoveTitleCommand(TitleView* view,
                                       QSharedPointer<EPubTitle> title,
                                       int row,
                                       QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title(title)
  , m_row(row)
{
  auto text = QObject::tr("remove %1:%2").arg(m_row).arg(m_title->title);
  setText(text);
}

void
RemoveTitleCommand::undo()
{
  m_view->insertTitle(m_row, m_title);
}

void
RemoveTitleCommand::redo()
{
  m_view->removeTitle(m_row);
}

//====================================================================
//=== SwapTitleCommand
//====================================================================
SwapTitleCommand::SwapTitleCommand(TitleView* view,
                                   QSharedPointer<EPubTitle> title1,
                                   int row1,
                                   QSharedPointer<EPubTitle> title2,
                                   int row2,
                                   QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title1(title1)
  , m_row1(row1)
  , m_title2(title2)
  , m_row2(row2)
{
  setText(QObject::tr("Swap %1:\"%2\" with %3:\"%4\"")
            .arg(row1)
            .arg(title1->title)
            .arg(row2)
            .arg(title2->title));
}

void
SwapTitleCommand::undo()
{
  m_view->swapToPrimaryPosition(m_row2);
}

void
SwapTitleCommand::redo()
{
  m_view->swapToPrimaryPosition(m_row2);
}

//====================================================================
//=== SwapTitleCommand
//====================================================================
SwapIdCommand::SwapIdCommand(TitleView* view,
                             UniqueString id1,
                             int row1,
                             UniqueString id2,
                             int row2,
                             QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_id1(id1)
  , m_row1(row1)
  , m_id2(id2)
  , m_row2(row2)
{}

void
SwapIdCommand::undo()
{
  m_view->setId(0, m_id1);
  m_view->setId(m_row2, m_id2);
}

void
SwapIdCommand::redo()
{
  m_view->setId(0, m_id2);
  m_view->setId(m_row2, UniqueString());
}

//====================================================================
//=== ModifyTitleCommand
//====================================================================
ModifyTitleCommand::ModifyTitleCommand(TitleView* view,
                                       const QString& newTitle,
                                       int row,
                                       const QString& oldTitle,
                                       QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_newTitle(newTitle)
  , m_oldTitle(oldTitle)
  , m_row(row)
{
  setText(QObject::tr("Modify %1:\"%2\" => \"%3\"")
            .arg(row)
            .arg(oldTitle)
            .arg(newTitle));
}

void
ModifyTitleCommand::undo()
{
  m_view->modifyTitle(m_row, m_oldTitle);
}

void
ModifyTitleCommand::redo()
{
  m_view->modifyTitle(m_row, m_newTitle);
}

//====================================================================
//=== SetIdCommand
//====================================================================
SetIdCommand::SetIdCommand(TitleView* view,
                           const QString& newId,
                           const QString& oldId,
                           int row,
                           QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_newId(newId)
  , m_oldId(oldId)
  , m_row(row)
{
  setText(QObject::tr("ID for %1:\"%2\" => \"%3\"")
            .arg(row)
            .arg(m_oldId)
            .arg(m_newId));
}

void
SetIdCommand::undo()
{
  auto title = m_view->titleAt(m_row);
  title->id = m_view->uniqueIdList()->append(m_newId, -1);
}

void
SetIdCommand::redo()
{
  auto title = m_view->titleAt(m_row);
  title->id = m_view->uniqueIdList()->append(m_oldId, -1);
}

//====================================================================
//=== TitleEditDialog
//====================================================================
void
TitleEditDialog::initGui()
{
  auto layout = new QFormLayout;
  setLayout(layout);

  auto idEdit = new QLineEdit(this);
  layout->addRow(tr("ID :"), idEdit);
  connect(idEdit, &QLineEdit::textChanged, this, &TitleEditDialog::idChanged);

  auto titleEdit = new QLineEdit(this);
  layout->addRow(tr("Title :"), titleEdit);
  connect(
    titleEdit, &QLineEdit::textChanged, this, &TitleEditDialog::titleChanged);

  m_dirEdit = new QComboBox(this);
  m_dirEdit->addItem(tr("Left to Right"), "LTR");
  m_dirEdit->addItem(tr("Right to Left"), "RTL");
  layout->addRow(tr("Direction :"), m_dirEdit);
  connect(m_dirEdit,
          &QComboBox::currentTextChanged,
          this,
          &TitleEditDialog::directionChanged);

  auto langEdit = new QLineEdit(this);
  layout->addRow(tr("Language :"), langEdit);
  connect(
    langEdit, &QLineEdit::textChanged, this, &TitleEditDialog::langChanged);

  auto dateEdit = new QDateEdit(this);
  layout->addRow(tr("Date :"), dateEdit);
  connect(
    dateEdit, &QDateEdit::dateChanged, this, &TitleEditDialog::dateChanged);
}

void
TitleEditDialog::idChanged(const QString& text)
{
  m_title->id = text;
}

void
TitleEditDialog::titleChanged(const QString& text)
{
  m_title->title = text;
}

void
TitleEditDialog::directionChanged(const QString& text)
{
  auto dir = m_dirEdit->currentData().toString();
  if (dir == "LTR") {
    m_title->dir = Direction::LTR;
  } else {
    m_title->dir = Direction::RTL;
  }
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
