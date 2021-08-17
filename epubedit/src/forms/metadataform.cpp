#include "forms/metadataform.h"

#include "document/authors.h"
#include "document/epubdocument.h"
#include "document/epubmetadata.h"
#include "forms/authorlist.h"
#include "forms/titleview.h"

//====================================================================
//=== MetadataForm
//====================================================================
const QString MetadataForm::TITLESTR = "%1%2";

MetadataForm::MetadataForm(QUndoStack* undoStack, QWidget* parent)
  : QFrame(parent)
  , m_undoStack(undoStack)
  , m_currentTitleIndex(QModelIndex())
  , m_currentAuthorIndex(QModelIndex())
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

  QStringList titles;
  QList<QWidget*> widgets;
  titles << tr("Main Metadata") << tr("Secondary");
  widgets << initMaindataFrame() << new QFrame();

  auto typeBox = new QComboBox(this);
  typeBox->addItems(titles);
  layout->addWidget(typeBox, 0, 0);

  auto frame = new QFrame();
  frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto typeLayout = new QStackedLayout;
  frame->setLayout(typeLayout);
  connect(typeBox,
          &QComboBox::currentIndexChanged,
          typeLayout,
          &QStackedLayout::setCurrentIndex);
  layout->addWidget(frame, 1, 0);

  for (auto widget : widgets) {
    typeLayout->addWidget(widget);
  }

  auto btnFrame = new QFrame(this);
  auto btnLayout = new QHBoxLayout;
  btnFrame->setLayout(btnLayout);

  auto acceptBtn = new QPushButton(tr("Save Changes"), this);
  connect(acceptBtn, &QPushButton::clicked, this, &MetadataForm::acceptChanges);
  btnLayout->addWidget(acceptBtn);

  auto cancelBtn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancelBtn, &QPushButton::clicked, this, &MetadataForm::cancelChanges);
  btnLayout->addWidget(cancelBtn);

  layout->addWidget(btnFrame, 2, 0);
}

QWidget*
MetadataForm::initMaindataFrame()
{
  auto frame = new QFrame(this);
  auto layout = new QFormLayout;
  frame->setLayout(layout);

  layout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

  m_titleView = new TitleView(this);
  m_titleView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_titleView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_titleView, &TitleView::clicked, this, &MetadataForm::titleClicked);
  connect(m_titleView,
          &TitleView::customContextMenuRequested,
          this,
          &MetadataForm::titleContextMenu);
  connect(
    m_titleView, &TitleView::titleRemoved, this, &MetadataForm::titleRemoved);
  layout->addRow(tr("Title(s) :"), m_titleView);

  m_authorView = new AuthorList(this);
  m_authorView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_authorView->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(
    m_authorView, &AuthorList::clicked, this, &MetadataForm::authorClicked);
  connect(m_authorView,
          &AuthorList::customContextMenuRequested,
          this,
          &MetadataForm::authorContextMenu);
  connect(m_authorView,
          &AuthorList::authorRemoved,
          this,
          &MetadataForm::authorRemoved);
  layout->addRow(tr("Author(s) :"), m_authorView);

  return frame;
}

void
MetadataForm::titleClicked()
{
  m_currentTitleIndex = m_titleView->currentIndex();
}

void
MetadataForm::authorClicked()
{
  m_currentAuthorIndex = m_authorView->currentIndex();
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
    m_titleView->setCurrentIndex(m_titleView->model()->index(row, 2));
    m_titleView->resizeTableVertically();
  }
}

void
MetadataForm::titleContextMenu(QPoint pos)
{
  m_currentTitleIndex = m_titleView->indexAt(pos);

  auto menu = new QMenu(this);
  auto addTitleAct = new QAction("Add title", this);
  menu->addAction(addTitleAct);
  connect(addTitleAct, &QAction::triggered, this, &MetadataForm::addTitle);
  auto removeAct = new QAction("Remove title", this);
  menu->addAction(removeAct);
  connect(removeAct, &QAction::triggered, this, &MetadataForm::removeTitle);
  auto primaryAct = new QAction("Set title as Primary", this);
  menu->addAction(primaryAct);
  connect(
    primaryAct, &QAction::triggered, this, &MetadataForm::setPrimaryTitle);
  auto idToPrimaryAct = new QAction("Move Id to Primary", this);
  menu->addAction(idToPrimaryAct);
  connect(idToPrimaryAct,
          &QAction::triggered,
          this,
          &MetadataForm::moveTitleIdToPrimary);
  if (m_titleView->hasId(m_currentTitleIndex.row()) &&
      m_currentTitleIndex.row() > 0) {
    idToPrimaryAct->setEnabled(true);
  } else {
    idToPrimaryAct->setEnabled(false);
  }
  menu->addSeparator();
  auto editTitleAct = new QAction("Edit Title values", this);
  menu->addAction(editTitleAct);
  connect(editTitleAct, &QAction::triggered, this, &MetadataForm::editTitle);

  menu->popup(m_titleView->viewport()->mapToGlobal(pos));
}

void
MetadataForm::authorContextMenu(QPoint pos)
{
  auto menu = new QMenu(this);
  auto addAuthorAct = new QAction("Add author", this);
  menu->addAction(addAuthorAct);
  connect(addAuthorAct, &QAction::triggered, this, &MetadataForm::addAuthor);
  auto modifyAct = new QAction("Modify author", this);
  menu->addAction(modifyAct);
  connect(modifyAct, &QAction::triggered, this, &MetadataForm::modifyAuthor);
  auto removeAct = new QAction("Remove author", this);
  menu->addAction(removeAct);
  connect(removeAct, &QAction::triggered, this, &MetadataForm::removeAuthor);
  auto primaryAct = new QAction("Set author as Primary", this);
  menu->addAction(primaryAct);
  connect(
    primaryAct, &QAction::triggered, this, &MetadataForm::setPrimaryAuthor);
  menu->addSeparator();

  menu->popup(m_titleView->viewport()->mapToGlobal(pos));
}

void
MetadataForm::modifyTitle()
{
  if (m_currentTitleIndex.isValid()) {
    auto row = m_currentTitleIndex.row();
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
  if (m_currentTitleIndex.isValid()) {
    auto row = m_currentTitleIndex.row();
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
  if (m_currentTitleIndex.isValid()) {
    if (m_currentTitleIndex.row() == 0) {
      QMessageBox::warning(this,
                           tr("Removing Primary Title"),
                           tr("You are attempting to remove the first,\n"
                              "or primary title. This is not allowed, you\n"
                              "must first set another title (if any) as the\n"
                              "primary title. Use the \"Set title as Primary\""
                              "menu item."));
    } else {
      auto row = m_currentTitleIndex.row();
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
        m_undoStack->push(new RemoveTitleCommand(
          m_titleView, title, m_currentTitleIndex.row()));
        m_currentTitleIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("Removed title \"%1\"")
            .arg(m_titleView->titleStrAt(m_currentTitleIndex.row())));
        m_titleView->resizeTableVertically();
      }
    }
  }
}

void
MetadataForm::titleRemoved(int row)
{
  if (row >= 0 && row < m_titleView->model()->rowCount(QModelIndex())) {
    m_currentTitleIndex = QModelIndex();
  }
}

void
MetadataForm::setPrimaryTitle()
{
  if (m_currentTitleIndex.isValid()) {
    if (m_currentTitleIndex.row() == 0) {
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
        auto row2 = m_currentTitleIndex.row();
        auto title2 = m_titleView->titleAt(row2);
        // adding the command to the stack calls redo() on the command so we
        // don't actually need to swap the title manually.
        m_undoStack->push(
          new SwapTitleCommand(m_titleView, title1, 0, title2, row2));
        m_currentTitleIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("\"%1\" is now the primary title!")
            .arg(m_titleView->titleStrAt(m_currentTitleIndex.row())));
      }
    }
  }
}

void
MetadataForm::moveTitleIdToPrimary()
{
  if (m_currentTitleIndex.isValid()) {
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
      auto row2 = m_currentTitleIndex.row();
      auto id2 = m_titleView->idAt(row2);
      // adding the command to the stack calls redo() on the command so we
      // don't actually need to swap the title manually.
      m_undoStack->push(new SwapIdCommand(m_titleView, id1, 0, id2, row2));
      m_currentTitleIndex = m_titleView->primaryTitleIndex();
      m_modifications.setFlag(ID_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" is now the primary title!")
          .arg(m_titleView->titleStrAt(m_currentTitleIndex.row())));
    }
  }
}

void
MetadataForm::addAuthor()
{
  auto text =
    QInputDialog::getText(this, tr("Enter new author"), tr("Author : "));
  if (!text.isEmpty()) {
    auto row = m_authorView->model()->rowCount();
    // adding the command to the stack calls redo() on the command so we don't
    // actually need to add the title manually.
    auto addCommand = new AddAuthorCommand(m_authorView, text, row);
    m_undoStack->push(addCommand);
    m_modifications.setFlag(AUTHORS_CHANGED, true);
    emit sendStatusMessage(
      tr("Adding new title \"%1\"").arg(m_titleView->titleStrAt(row)));
    m_authorView->setCurrentIndex(m_authorView->model()->index(row, 0));
    m_authorView->resizeTableVertically();
  }
}

void
MetadataForm::modifyAuthor()
{
  m_currentAuthorIndex = m_authorView->currentIndex();
  if (m_currentAuthorIndex.isValid()) {
    auto row = m_currentAuthorIndex.row();
    auto oldAuthor = m_authorView->authorAt(row);
    auto newAuthor = QInputDialog::getText(
      this,
      tr("modify Author"),
      tr("You are attempting to modify the author name \"%1\".\n"
         "Enter a new value for the author and press \"OK\"\n"
         "or press \"Cancel\" to abort change."),
      QLineEdit::Normal,
      oldAuthor);
    if (!newAuthor.isEmpty() && newAuthor != oldAuthor) {
      // adding the command to the stack calls redo() on the command so we don't
      // actually need to modify the title manually.
      m_undoStack->push(
        new ModifyAuthorCommand(m_authorView, oldAuthor, row, newAuthor));
      m_modifications.setFlag(AUTHORS_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" has been changed to \"%2\".").arg(oldAuthor, newAuthor));
    }
  }
}

void
MetadataForm::removeAuthor()
{
  m_currentAuthorIndex = m_authorView->currentIndex();
  if (m_currentAuthorIndex.isValid()) {
    if (m_currentAuthorIndex.row() == 0) {
      QMessageBox::warning(
        this,
        tr("Removing Primary Author"),
        tr("You are attempting to remove the first,\n"
           "or primary author. This is not allowed, you\n"
           "must first set another author (if any) as the\n"
           "primary author. Use the \"Set Author as Primary\""
           "menu item."));
    } else {
      auto row = m_currentAuthorIndex.row();
      auto author = m_authorView->authorAt(row);
      int btn =
        QMessageBox::question(this,
                              tr("Removing Alternative Author"),
                              tr("You are attempting to remove an alternate\n"
                                 "author. \"%1\"\n"
                                 "Are you sure? Press \"Yes\" to continue\n"
                                 "or \"No\" to cancel the operation.")
                                .arg(author),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);
      if (btn == QMessageBox::Yes) {
        m_undoStack->push(new RemoveAuthorCommand(
          m_authorView, author, m_currentAuthorIndex.row()));
        m_currentAuthorIndex = m_authorView->primaryAuthorIndex();
        m_modifications.setFlag(AUTHORS_CHANGED, true);
        emit sendStatusMessage(
          tr("Removed author \"%1\"")
            .arg(m_authorView->authorAt(m_currentAuthorIndex.row())));
        m_authorView->resizeTableVertically();
      }
    }
  }
}

void
MetadataForm::setPrimaryAuthor()
{
  m_currentAuthorIndex = m_authorView->currentIndex();
  if (m_currentAuthorIndex.isValid()) {
    if (m_currentAuthorIndex.row() == 0) {
      QMessageBox::warning(this,
                           tr("Setting Primary Author"),
                           tr("You are attempting to set the first,\n"
                              "author as the primary author. Please\n"
                              "note that it ALREADY the primary author\n"
                              "so nothing will be done."));
    } else {
      int btn =
        QMessageBox::question(this,
                              tr("Setting Primary Author"),
                              tr("You are attempting to move an alternate\n"
                                 "author to the primary position.\n"
                                 "Are you sure? Press \"Yes\" to continue\n"
                                 "or \"No\" to cancel the operation."),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);
      if (btn == QMessageBox::Yes) {
        auto author1 = m_authorView->authorAt(0);
        auto row2 = m_currentAuthorIndex.row();
        auto author2 = m_authorView->authorAt(row2);
        // adding the command to the stack calls redo() on the command so we
        // don't actually need to swap the title manually.
        m_undoStack->push(
          new SwapAuthorCommand(m_authorView, author1, 0, author2, row2));
        m_currentAuthorIndex = m_authorView->primaryAuthorIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("\"%1\" is now the primary author!")
            .arg(m_authorView->authorAt(m_currentAuthorIndex.row())));
      }
    }
  }
}

void
MetadataForm::authorRemoved(int row)
{
  if (row >= 0 && row < m_authorView->model()->rowCount(QModelIndex())) {
    m_currentAuthorIndex = QModelIndex();
  }
}

void
MetadataForm::editTitle()
{
  m_currentTitleIndex = m_titleView->currentIndex();
  auto row = m_currentTitleIndex.row();
  if (m_currentTitleIndex.isValid()) {
    auto title = m_titleView->titleAt(row);
    auto dlg = new TitleEditDialog(title, m_uniqueIdList, this);
    if (dlg->exec() == QDialog::Accepted) {
      title = dlg->title();
      m_titleView->insertTitle(row, title);
    }
  }
}

void
MetadataForm::setDocument(QSharedPointer<EPubDocument> document)
{
  m_document = document;

  m_uniqueIdList = m_document->uniqueIdList();
  auto titleList = m_document->metadata()->orderedTitles();
  m_titleView->initialiseData(titleList, m_uniqueIdList);
  m_currentTitleIndex = m_titleView->currentIndex();

  auto authorList = m_document->metadata()->creatorList();
  m_authorView->initialiseData(authorList);
  m_currentAuthorIndex = m_authorView->currentIndex();
}
