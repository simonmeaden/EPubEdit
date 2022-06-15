#include "undocommands.h"
#include "twocolview.h"
#include "threecolview.h"
#include "document/metadata.h"

//====================================================================
//=== UndoMetadataCommand
//====================================================================
UndoMetadataCommand::UndoMetadataCommand(PMetadata metadata,
                                         QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_metadata(metadata)
{}

//====================================================================
//=== AddTitleCommand
//====================================================================
AddTitleCommand::AddTitleCommand(ThreeColView* view,
                                 const QString& title,
                                 int row,
                                 QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title(title)
  , m_row(row)
{
  auto text = tr("Add %1:%2").arg(m_row).arg(m_title);
  setText(text);
}

void
AddTitleCommand::undo()
{
//  m_view->removeRow(m_row);
}

void
AddTitleCommand::redo()
{
//  auto success = m_view->model()->insertRow(m_row, QModelIndex());
//  if (success) {
//    m_view->setTitle(m_row, m_title);
//  }
}

//====================================================================
//=== AddAuthorCommand
//====================================================================
AddAuthorCommand::AddAuthorCommand(TwoColView *view,
                                   PMetadata metadata,
                                   const QString& author,
                                   int row,
                                   QUndoCommand* parent)
  : UndoMetadataCommand(metadata, parent)
  , m_view(view)
  , m_author(author)
  , m_row(row)
{
  auto text = tr("Add %1:%2").arg(m_row).arg(m_author);
  setText(text);
}

void
AddAuthorCommand::undo()
{
//  m_view->removeRow(m_row);
}

void
AddAuthorCommand::redo()
{
//  auto success = m_view->model()->insertRow(m_row, QModelIndex());
//  if (success) {
//    m_view->setFirstAt(m_row, m_author);
//  }
}

//====================================================================
//=== RemoveTitleCommand
//====================================================================
RemoveTitleCommand::RemoveTitleCommand(ThreeColView* view,
                                       QString title,
                                       int row,
                                       QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title(title)
  , m_row(row)
{
  auto text = tr("remove %1:%2").arg(m_row).arg(title);
  setText(text);
}

void
RemoveTitleCommand::undo()
{
//  m_view->insertTitle(m_row, m_title);
}

void
RemoveTitleCommand::redo()
{
//  m_view->removeTitle(m_row);
}

//====================================================================
//=== RemoveAuthorCommand
//====================================================================
RemoveAuthorCommand::RemoveAuthorCommand(TwoColView *view,
                                         PMetadata metadata,
                                         const QString& author,
                                         int row,
                                         QUndoCommand* parent)
  : UndoMetadataCommand(metadata, parent)
  , m_view(view)
  , m_author(author)
  , m_row(row)
{}

void
RemoveAuthorCommand::undo()
{
//  m_view->insertAtFirst(m_row, m_author);
}

void
RemoveAuthorCommand::redo()
{
//  m_view->removeSecond(m_row);
}

//====================================================================
//=== SwapTitleCommand
//====================================================================
SwapTitleCommand::SwapTitleCommand(ThreeColView* view,
                                   QString title1,
                                   int row1,
                                   QString title2,
                                   int row2,
                                   QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_title1(title1)
  , m_row1(row1)
  , m_title2(title2)
  , m_row2(row2)
{
  setText(tr("Swap %1:\"%2\" with %3:\"%4\"")
            .arg(row1)
            .arg(title1)
            .arg(row2)
            .arg(title2));
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
//=== SwapAuthorCommand
//====================================================================
SwapAuthorCommand::SwapAuthorCommand(TwoColView *view,
                                     const QString& author1,
                                     int row1,
                                     const QString& author2,
                                     int row2,
                                     QUndoCommand* parent)
  : QUndoCommand(parent)
  , m_view(view)
  , m_author1(author1)
  , m_row1(row1)
  , m_author2(author2)
  , m_row2(row2)
{
  setText(tr("Swap %1:\"%2\" with %3:\"%4\"")
            .arg(row1)
            .arg(author1)
            .arg(row2)
            .arg(author2));
}

void
SwapAuthorCommand::undo()
{
  m_view->swapToPrimaryPosition(m_row2);
}

void
SwapAuthorCommand::redo()
{
  m_view->swapToPrimaryPosition(m_row2);
}

//====================================================================
//=== SwapIdCommand
//====================================================================
SwapIdCommand::SwapIdCommand(ThreeColView* view,
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
//  m_view->setThird(0, m_id1.toString());
//  m_view->setThird(m_row2, m_id2.toString());
}

void
SwapIdCommand::redo()
{
//  m_view->setThird(0, m_id2.toString());
//  m_view->setThird(m_row2, QString());
}

//====================================================================
//=== ModifyTitleCommand
//====================================================================
ModifyTitleCommand::ModifyTitleCommand(ThreeColView* view,
                                       PMetadata metadata,
                                       const QString& newTitle,
                                       int row,
                                       const QString& oldTitle,
                                       QUndoCommand* parent)
  : UndoMetadataCommand(metadata, parent)
  , m_view(view)
  , m_newTitle(newTitle)
  , m_oldTitle(oldTitle)
  , m_row(row)
{
  setText(tr("Modify %1:\"%2\" => \"%3\"").arg(row).arg(oldTitle, newTitle));
}

void
ModifyTitleCommand::undo()
{
//  m_view->modifyTitle(m_row, m_oldTitle);
}

void
ModifyTitleCommand::redo()
{
//  m_view->modifyTitle(m_row, m_newTitle);
}

//====================================================================
//=== ModifyTitleCommand
//====================================================================
ModifyAuthorCommand::ModifyAuthorCommand(TwoColView *view,
                                         PMetadata metadata,
                                         const QString& newAuthor,
                                         int row,
                                         const QString& oldAuthor,
                                         QUndoCommand* parent)
  : UndoMetadataCommand(metadata, parent)
  , m_view(view)
  , m_newAuthor(newAuthor)
  , m_oldAuthor(oldAuthor)
  , m_row(row)
{
  setText(tr("Modify %1:\"%2\" => \"%3\"").arg(row).arg(oldAuthor, newAuthor));
}

void
ModifyAuthorCommand::undo()
{
//  m_view->modifySecondcol(m_row, m_oldAuthor);
}

void
ModifyAuthorCommand::redo()
{
//  m_view->modifySecondcol(m_row, m_newAuthor);
}

//====================================================================
//=== SetIdCommand
//====================================================================
SetIdCommand::SetIdCommand(ThreeColView* view,
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
  setText(tr("ID for %1:\"%2\" => \"%3\"").arg(row).arg(m_oldId, m_newId));
}

void
SetIdCommand::undo()
{
//  auto title = m_view->titleAt(m_row);
//  title->id = m_view->uniqueIdList()->append(m_newId, -1);
}

void
SetIdCommand::redo()
{
//  auto title = m_view->titleAt(m_row);
//  title->id = m_view->uniqueIdList()->append(m_oldId, -1);
}
