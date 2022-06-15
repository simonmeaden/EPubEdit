#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QCoreApplication>
#include <QSharedPointer>
#include <QString>
#include <QUndoCommand>

class ThreeColView;
class TwoColView;
struct EPubTitle;

#include "document/uniquestring.h"
#include "document/bookpointers.h"

class UndoMetadataCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(UndoMetadataCommand)

public:
  UndoMetadataCommand(PMetadata metadata,
                      QUndoCommand* parent = nullptr);

private:
  PMetadata m_metadata;
};

class AddTitleCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  AddTitleCommand(ThreeColView* view,
                  const QString& title,
                  int row,
                  QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  QString m_title;
  int m_row;
};

class AddAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  AddAuthorCommand(TwoColView* view,
                   PMetadata metadata,
                   const QString& author,
                   int row,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TwoColView* m_view;
  QString m_author;
  int m_row;
};

class SetIdCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SetIdCommand(ThreeColView* view,
               const QString& newId,
               const QString& oldId,
               int row,
               QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  QString m_newId;
  QString m_oldId;
  int m_row;
};

class RemoveTitleCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  RemoveTitleCommand(ThreeColView* view,
                     QString title,
                     int pos,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  QString m_title;
  int m_row;
};

class RemoveAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(RemoveAuthorCommand)

public:
  RemoveAuthorCommand(TwoColView* view,
                      PMetadata metadata,
                      const QString& author,
                      int row,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TwoColView* m_view;
  QString m_author;
  int m_row;
};

class SwapTitleCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapTitleCommand(ThreeColView* view,
                   QString title1,
                   int row1,
                   QString title2,
                   int row2,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  QString m_title1;
  int m_row1;
  QString m_title2;
  int m_row2;
};

class SwapAuthorCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapAuthorCommand(TwoColView* view,
                    const QString& author1,
                    int row1,
                    const QString& author2,
                    int row2,
                    QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TwoColView* m_view;
  QString m_author1;
  int m_row1;
  QString m_author2;
  int m_row2;
};

class SwapIdCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapIdCommand(ThreeColView* view,
                UniqueString id1,
                int row1,
                UniqueString id2,
                int row2,
                QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  UniqueString m_id1;
  int m_row1;
  UniqueString m_id2;
  int m_row2;
};

class ModifyTitleCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  ModifyTitleCommand(ThreeColView* view,
                     PMetadata metadata,
                     const QString& newTitle,
                     int pos,
                     const QString& oldTitle,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  ThreeColView* m_view;
  QString m_newTitle;
  QString m_oldTitle;
  PMetadata m_metadata;
  int m_row;
};

class ModifyAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  ModifyAuthorCommand(TwoColView* view,
                      PMetadata metadata,
                      const QString& newAuthor,
                      int row,
                      const QString& oldAuthor,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TwoColView* m_view;
  QString m_newAuthor;
  QString m_oldAuthor;
  int m_row;
};

#endif // UNDOCOMMANDS_H
