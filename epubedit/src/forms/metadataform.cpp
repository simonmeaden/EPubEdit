#include "forms/metadataform.h"

#include "config.h"
#include "document/authors.h"
#include "document/metadata.h"
#include "forms/metadatalist.h"
#include "forms/threecolview.h"
#include "forms/twocolview.h"
#include "labelled/labelledlineedit.h"
#include "language/languages.h"

//====================================================================
//=== MetadataForm
//====================================================================
const QString MetadataForm::TITLESTR = "%1%2";

MetadataForm::MetadataForm(PConfig config, QWidget* parent)
  : QFrame(parent)
  , m_config(config)
  , m_currentTitleIndex(QModelIndex())
  , m_currentAuthorIndex(QModelIndex())
{
  initGui();
}

MetadataForm::~MetadataForm() {}

void
MetadataForm::setMetadata(PMetadata metadata)
{
  m_metadata = metadata;
  initialiseTitleView();
  auto date = m_metadata->date();
  m_date->setText(date.toString(Qt::TextDate));
  initialiseMetadataView();
  auto rights = m_metadata->rights();
  m_rights->setText(rights->name);
  initialiseLanguageView();
}

void
MetadataForm::initGui()
{
  setContentsMargins(0, 0, 0, 0);
  auto layout = new QGridLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  QStringList titles;
  QList<QWidget*> widgets;
  titles << tr("Main Metadata") << tr("Secondary");

  auto typeBox = new QComboBox(this);
  typeBox->addItems(titles);
  layout->addWidget(typeBox, 0, 0);

  auto widget = new QWidget(this);
  widget->setContentsMargins(0, 0, 0, 0);
  widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto typeLayout = new QStackedLayout;
  typeLayout->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(typeLayout);
  connect(typeBox,
          qOverload<int>(&QComboBox::currentIndexChanged),
          typeLayout,
          &QStackedLayout::setCurrentIndex);
  layout->addWidget(widget, 1, 0);

  typeLayout->addWidget(initMainDataFrame());
  typeLayout->addWidget(initSecondaryDataFrame());

  auto btnFrame = new QFrame(this);
  auto btnLayout = new QHBoxLayout;
  btnFrame->setLayout(btnLayout);

  m_lockBtn = new QPushButton(tr("Unlock"), this);
  connect(m_lockBtn, &QPushButton::clicked, this, &MetadataForm::lock);
  btnLayout->addWidget(m_lockBtn);

  auto acceptBtn = new QPushButton(tr("Save Changes"), this);
  connect(acceptBtn, &QPushButton::clicked, this, &MetadataForm::acceptChanges);
  btnLayout->addWidget(acceptBtn);

  auto cancelBtn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancelBtn, &QPushButton::clicked, this, &MetadataForm::cancelChanges);
  btnLayout->addWidget(cancelBtn);

  layout->addWidget(btnFrame, 2, 0);
}

QWidget*
MetadataForm::initMainDataFrame()
{
  auto widget = new QWidget(this);
  widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  widget->setContentsMargins(0, 0, 0, 0);
  auto layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(layout);

  auto lbl = new QLabel(tr("Title(s) :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addWidget(lbl, Qt::AlignLeft);

  m_titleView = new ThreeColView(this);
  m_titleView->setSizePolicy(QSizePolicy::Expanding,
                             QSizePolicy::MinimumExpanding);
  m_titleView->setContextMenuPolicy(Qt::CustomContextMenu);
  m_titleView->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  layout->addWidget(m_titleView);

  m_date = new LabelledLineEdit(tr("Published Date:"), this);
  m_date->setLabelAlignment(Qt::AlignLeft);
  m_date->setReadOnly(true);
  m_date->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout->addWidget(m_date, Qt::AlignLeft);

  lbl = new QLabel(tr("Metadata :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addWidget(lbl, Qt::AlignLeft);

  m_metadataView = new TwoColView(this);
  m_metadataView->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::MinimumExpanding);
  m_metadataView->setContextMenuPolicy(Qt::CustomContextMenu);
  layout->addWidget(m_metadataView);

  lbl = new QLabel(tr("Rights :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addWidget(lbl, Qt::AlignLeft);

  m_rights = new QLineEdit(this);
  m_rights->setReadOnly(true);
  m_rights->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout->addWidget(m_rights, Qt::AlignLeft);

  lbl = new QLabel(tr("Languages :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  layout->addWidget(lbl, Qt::AlignLeft);

  m_languageView = new TwoColView(this);
  m_languageView->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::MinimumExpanding);
  m_languageView->setContextMenuPolicy(Qt::CustomContextMenu);
  layout->addWidget(m_languageView, Qt::AlignLeft);

  auto spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer);

//  return scrollArea;
  return widget;
}

void
MetadataForm::initialiseTitleView()
{
  m_titleView->clear();
  auto ptitles = m_metadata->orderedTitles();

  for (auto i = 0; i < ptitles.size(); i++) {
    auto ptitle = ptitles.at(i);
    auto title = ptitle->title;
    auto id = ptitle->id.toString();
    auto s = tr("Alternate title");
    if (i == 0) {
      s = tr("Primary Title");
    }
    m_titleView->addRow(s, title, id);
  }
}

void
MetadataForm::initialiseLanguageView()
{
  m_languageView->clear();
  auto languages = m_metadata->languages();
  auto languageDB = m_config->languages();

  for (auto& language : languages) {
    auto subtag = language->language;
    auto bcp47 = languageDB->languageFromSubtag(subtag);
    if (bcp47){
      auto desc = bcp47->description();
      m_languageView->addRow(subtag, desc);
    }
  }
}

void
MetadataForm::initialiseMetadataView()
{
  m_metadataView->clear();
  auto creatorList = m_metadata->creatorList();
  auto contributorList = m_metadata->contributorList();

  for (auto& creator : creatorList) {
    auto data = m_metadata->creatorFromName(creator);
    auto relator = data->relator;
    auto description = relator->relatorToDescription(relator->type());
    m_metadataView->addRow(creator, description);
  }
  for (auto& contributor : contributorList) {
    auto data = m_metadata->contributorFromName(contributor);
    auto relator = data->relator;
    auto description = relator->relatorToDescription(relator->type());
    m_metadataView->addRow(contributor, description);
  }
}

QWidget*
MetadataForm::initSecondaryDataFrame()
{
  auto frame = new QFrame(this);
  auto layout = new QGridLayout;
  frame->setLayout(layout);

  return frame;
}

void
MetadataForm::lock()
{
  if (locked) {
    locked = false;
    m_lockBtn->setText(tr("Lock"));
    connect(
      m_titleView, &ThreeColView::clicked, this, &MetadataForm::titleClicked);
    connect(m_titleView,
            &ThreeColView::customContextMenuRequested,
            this,
            &MetadataForm::titleContextMenu);
    connect(m_titleView,
            &ThreeColView::rowRemoved,
            this,
            &MetadataForm::titleRemoved);
    connect(
      m_metadataView, &TwoColView::clicked, this, &MetadataForm::authorClicked);
    connect(m_metadataView,
            &TwoColView::customContextMenuRequested,
            this,
            &MetadataForm::authorContextMenu);
    connect(m_metadataView,
            &TwoColView::rowRemoved,
            this,
            &MetadataForm::authorRemoved);
    connect(m_languageView,
            &ThreeColView::customContextMenuRequested,
            this,
            &MetadataForm::langContextMenu);
    m_date->setReadOnly(false);
    m_rights->setReadOnly(false);
  } else {
    locked = true;
    m_lockBtn->setText(tr("Unlock"));
    disconnect(
      m_titleView, &ThreeColView::clicked, this, &MetadataForm::titleClicked);
    disconnect(m_titleView,
               &ThreeColView::customContextMenuRequested,
               this,
               &MetadataForm::titleContextMenu);
    disconnect(m_titleView,
               &ThreeColView::rowRemoved,
               this,
               &MetadataForm::titleRemoved);
    disconnect(
      m_metadataView, &TwoColView::clicked, this, &MetadataForm::authorClicked);
    disconnect(m_metadataView,
               &TwoColView::customContextMenuRequested,
               this,
               &MetadataForm::authorContextMenu);
    disconnect(m_metadataView,
               &TwoColView::rowRemoved,
               this,
               &MetadataForm::authorRemoved);
    disconnect(m_languageView,
               &ThreeColView::customContextMenuRequested,
               this,
               &MetadataForm::langContextMenu);
    m_date->setReadOnly(true);
    m_rights->setReadOnly(true);
  }
}

void
MetadataForm::titleClicked()
{
  m_currentTitleIndex = m_titleView->currentIndex();
}

void
MetadataForm::authorClicked()
{
  m_currentAuthorIndex = m_metadataView->currentIndex();
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
    emit pushUndoAction(addCommand);
    m_modifications.setFlag(TITLES_CHANGED, true);
    emit sendStatusMessage(
      tr("Adding new title \"%1\"").arg(m_titleView->secondAt(row)));
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
  if (!m_titleView->firstAt(m_currentTitleIndex.row()).isEmpty() &&
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
MetadataForm::langContextMenu(QPoint pos)
{
  // TODO
  //  auto menu = new QMenu(this);
  //  auto addAuthorAct = new QAction("Add author", this);
  //  menu->addAction(addAuthorAct);
  //  connect(addAuthorAct, &QAction::triggered, this,
  //  &MetadataForm::addAuthor); auto modifyAct = new QAction("Modify author",
  //  this); menu->addAction(modifyAct); connect(modifyAct, &QAction::triggered,
  //  this, &MetadataForm::modifyAuthor); auto removeAct = new QAction("Remove
  //  author", this); menu->addAction(removeAct); connect(removeAct,
  //  &QAction::triggered, this, &MetadataForm::removeAuthor); auto primaryAct =
  //  new QAction("Set author as Primary", this); menu->addAction(primaryAct);
  //  connect(
  //    primaryAct, &QAction::triggered, this, &MetadataForm::setPrimaryAuthor);
  //  menu->addSeparator();

  //  menu->popup(m_titleView->viewport()->mapToGlobal(pos));
}

void
MetadataForm::modifyTitle()
{
  if (m_currentTitleIndex.isValid()) {
    auto row = m_currentTitleIndex.row();
    auto title = m_titleView->firstAt(row);
    auto titleStr1 = m_titleView->secondAt(row);
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
      emit pushUndoAction(new ModifyTitleCommand(
        m_titleView, m_metadata, titleStr2, row, titleStr1));
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
    auto title = m_titleView->firstAt(row);
    auto idStr1 = m_titleView->thirdAt(row);
    auto idStr2 = QInputDialog::getText(
      this, tr("modify ID"), tr("Enter a new value for the id"));
    if (!idStr2.isEmpty() && idStr1 != idStr2) {
      if (UniqueString::exists(idStr2)) {
        QMessageBox::warning(this,
                             tr("Non-Unique ID value!"),
                             tr("Attempting to use an ID more than once.\n"
                                "ID's must be unique."),
                             QMessageBox::Ok,
                             QMessageBox::Ok);
      } else {
        emit pushUndoAction(new SetIdCommand(m_titleView, idStr1, idStr2, row));
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
      auto title = m_titleView->firstAt(row);
      int btn =
        QMessageBox::question(this,
                              tr("Removing Alternative Title"),
                              tr("You are attempting to remove an alternate\n"
                                 "title. \"%1\"\n"
                                 "Are you sure? Press \"Yes\" to continue\n"
                                 "or \"No\" to cancel the operation.")
                                .arg(title),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);
      if (btn == QMessageBox::Yes) {
        //        m_titleView->removeTitle(row);
        emit pushUndoAction(new RemoveTitleCommand(
          m_titleView, title, m_currentTitleIndex.row()));
        m_currentTitleIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("Removed title \"%1\"")
            .arg(m_titleView->secondAt(m_currentTitleIndex.row())));
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
        auto title1 = m_titleView->firstAt(0);
        auto row2 = m_currentTitleIndex.row();
        auto title2 = m_titleView->firstAt(row2);
        // adding the command to the stack calls redo() on the command so we
        // don't actually need to swap the title manually.
        emit pushUndoAction(
          new SwapTitleCommand(m_titleView, title1, 0, title2, row2));
        m_currentTitleIndex = m_titleView->primaryTitleIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("\"%1\" is now the primary title!")
            .arg(m_titleView->secondAt(m_currentTitleIndex.row())));
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
      auto id1 = m_titleView->thirdAt(0);
      auto row2 = m_currentTitleIndex.row();
      auto id2 = m_titleView->thirdAt(row2);
      // adding the command to the stack calls redo() on the command so we
      // don't actually need to swap the title manually.
      emit pushUndoAction(new SwapIdCommand(m_titleView, id1, 0, id2, row2));
      m_currentTitleIndex = m_titleView->primaryTitleIndex();
      m_modifications.setFlag(ID_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" is now the primary title!")
          .arg(m_titleView->secondAt(m_currentTitleIndex.row())));
    }
  }
}

void
MetadataForm::addAuthor()
{
  auto text =
    QInputDialog::getText(this, tr("Enter new author"), tr("Author : "));
  if (!text.isEmpty()) {
    auto row = m_metadataView->model()->rowCount();
    // adding the command to the stack calls redo() on the command so we don't
    // actually need to add the title manually.
    auto addCommand =
      new AddAuthorCommand(m_metadataView, m_metadata, text, row);
    emit pushUndoAction(addCommand);
    m_modifications.setFlag(AUTHORS_CHANGED, true);
    emit sendStatusMessage(
      tr("Adding new title \"%1\"").arg(m_titleView->secondAt(row)));
    m_metadataView->setCurrentIndex(m_metadataView->model()->index(row, 0));
    m_metadataView->resizeTableVertically();
  }
}

void
MetadataForm::modifyAuthor()
{
  m_currentAuthorIndex = m_metadataView->currentIndex();
  if (m_currentAuthorIndex.isValid()) {
    auto row = m_currentAuthorIndex.row();
    auto oldAuthor = m_metadataView->firstAt(row);
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
      emit pushUndoAction(new ModifyAuthorCommand(
        m_metadataView, m_metadata, oldAuthor, row, newAuthor));
      m_modifications.setFlag(AUTHORS_CHANGED, true);
      emit sendStatusMessage(
        tr("\"%1\" has been changed to \"%2\".").arg(oldAuthor, newAuthor));
    }
  }
}

void
MetadataForm::removeAuthor()
{
  m_currentAuthorIndex = m_metadataView->currentIndex();
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
      auto author = m_metadataView->firstAt(row);
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
        emit pushUndoAction(new RemoveAuthorCommand(
          m_metadataView, m_metadata, author, m_currentAuthorIndex.row()));
        m_currentAuthorIndex = m_metadataView->primaryAuthorIndex();
        m_modifications.setFlag(AUTHORS_CHANGED, true);
        emit sendStatusMessage(
          tr("Removed author \"%1\"")
            .arg(m_metadataView->firstAt(m_currentAuthorIndex.row())));
        m_metadataView->resizeTableVertically();
      }
    }
  }
}

void
MetadataForm::setPrimaryAuthor()
{
  m_currentAuthorIndex = m_metadataView->currentIndex();
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
        auto author1 = m_metadataView->firstAt(0);
        auto row2 = m_currentAuthorIndex.row();
        auto author2 = m_metadataView->firstAt(row2);
        // adding the command to the stack calls redo() on the command so we
        // don't actually need to swap the title manually.
        emit pushUndoAction(
          new SwapAuthorCommand(m_metadataView, author1, 0, author2, row2));
        m_currentAuthorIndex = m_metadataView->primaryAuthorIndex();
        m_modifications.setFlag(TITLES_CHANGED, true);
        emit sendStatusMessage(
          tr("\"%1\" is now the primary author!")
            .arg(m_metadataView->firstAt(m_currentAuthorIndex.row())));
      }
    }
  }
}

void
MetadataForm::authorRemoved(int row)
{
  if (row >= 0 && row < m_metadataView->model()->rowCount(QModelIndex())) {
    m_currentAuthorIndex = QModelIndex();
  }
}

void
MetadataForm::editTitle()
{
  m_currentTitleIndex = m_titleView->currentIndex();
  auto row = m_currentTitleIndex.row();
  if (m_currentTitleIndex.isValid()) {
    auto title = m_titleView->firstAt(row);
    auto id = m_titleView->thirdAt(row);
    auto ptitles = m_metadata->orderedTitles();
    auto ptitle = ptitles.at(row);

    auto dlg = new TitleEditDialog(ptitle, title, id, this);
    if (dlg->exec() == QDialog::Accepted) {
      ptitle = dlg->title();
      m_titleView->setFirstAt(row, ptitle->title);
    }
  }
}

//====================================================================
//=== TitleEditDialog
//====================================================================
TitleEditDialog::TitleEditDialog(PTitle ptitle,
                                 const QString& title,
                                 const QString& id,
                                 QWidget* parent)
  : QDialog(parent)
  , m_ptitle(ptitle)
{
  m_ptitle->title = title;
  m_ptitle->id = id;
  setWindowTitle(tr("Edit Title Values"));
  initGui();
}

PTitle
TitleEditDialog::title()
{
  return m_ptitle;
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
  titleEdit->setText(m_ptitle->title);
  layout->addRow(tr("Title :"), titleEdit);
  connect(
    titleEdit, &QLineEdit::textChanged, this, &TitleEditDialog::titleChanged);

  auto idEdit = new QLineEdit(this);
  idEdit->setText(m_ptitle->id.toString());
  idEdit->setStyleSheet("color: black;");
  layout->addRow(tr("ID :"), idEdit);
  connect(idEdit, &QLineEdit::textChanged, this, &TitleEditDialog::idChanged);

  auto dirEdit = new QComboBox(this);
  dirEdit->setCurrentIndex(int(m_ptitle->dir));
  dirEdit->addItem(tr("Left to Right"));
  dirEdit->addItem(tr("Right to Left"));
  layout->addRow(tr("Direction :"), dirEdit);
  connect(dirEdit,
          qOverload<int>(&QComboBox::currentIndexChanged),
          this,
          &TitleEditDialog::directionChanged);

  auto langEdit = new QLineEdit(this);
  langEdit->setText(m_ptitle->lang);
  layout->addRow(tr("Language :"), langEdit);
  connect(
    langEdit, &QLineEdit::textChanged, this, &TitleEditDialog::langChanged);

  auto dateEdit = new QDateEdit(this);
  dateEdit->setDate(m_ptitle->date);
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
  if (UniqueString::exists(text)) {
    m_label->setText(tr("ID value supplied id NOT unique!\n"
                        "%1 has been used elsewhere.")
                       .arg(text));
    m_label->setStyleSheet("QLabel {color: red;}");
    m_box->button(QDialogButtonBox::Ok)->setEnabled(false);
  } else {
    m_label->setText(tr("ID value is unique."));
    m_label->setStyleSheet("QLabel {color: green;}");
    m_ptitle->id = text;
    m_box->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}

void
TitleEditDialog::titleChanged(const QString& text)
{
  m_ptitle->title = text;
}

void
TitleEditDialog::directionChanged(int index)
{
  m_ptitle->dir = Direction(index);
}

void
TitleEditDialog::langChanged(const QString& text)
{
  m_ptitle->lang = text;
}

void
TitleEditDialog::dateChanged(const QDate& date)
{
  m_ptitle->date = date;
}
