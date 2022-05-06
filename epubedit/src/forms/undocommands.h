#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QCoreApplication>
#include <QSharedPointer>
#include <QString>
#include <QUndoCommand>

class TitleView;
class MetadataList;
class EPubTitle;

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
  AddTitleCommand(TitleView* view,
                  const QString& title,
                  int row,
                  QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_title;
  int m_row;
};

class AddAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  AddAuthorCommand(MetadataList* view,
                   PMetadata metadata,
                   const QString& author,
                   int row,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  MetadataList* m_view;
  QString m_author;
  int m_row;
};

class SetIdCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SetIdCommand(TitleView* view,
               const QString& newId,
               const QString& oldId,
               int row,
               QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_newId;
  QString m_oldId;
  int m_row;
};

class RemoveTitleCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  RemoveTitleCommand(TitleView* view,
                     QSharedPointer<EPubTitle> title,
                     int pos,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QSharedPointer<EPubTitle> m_title;
  int m_row;
};

class RemoveAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(RemoveAuthorCommand)

public:
  RemoveAuthorCommand(MetadataList* view,
                      PMetadata metadata,
                      const QString& author,
                      int row,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  MetadataList* m_view;
  QString m_author;
  int m_row;
};

class SwapTitleCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapTitleCommand(TitleView* view,
                   QSharedPointer<EPubTitle> title1,
                   int row1,
                   QSharedPointer<EPubTitle> title2,
                   int row2,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QSharedPointer<EPubTitle> m_title1;
  int m_row1;
  QSharedPointer<EPubTitle> m_title2;
  int m_row2;
};

class SwapAuthorCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapAuthorCommand(MetadataList* view,
                    const QString& author1,
                    int row1,
                    const QString& author2,
                    int row2,
                    QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  MetadataList* m_view;
  QString m_author1;
  int m_row1;
  QString m_author2;
  int m_row2;
};

class SwapIdCommand : public QUndoCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  SwapIdCommand(TitleView* view,
                UniqueString id1,
                int row1,
                UniqueString id2,
                int row2,
                QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  UniqueString m_id1;
  int m_row1;
  UniqueString m_id2;
  int m_row2;
};

class ModifyTitleCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  ModifyTitleCommand(TitleView* view,
                     PMetadata metadata,
                     const QString& newTitle,
                     int pos,
                     const QString& oldTitle,
                     QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  TitleView* m_view;
  QString m_newTitle;
  QString m_oldTitle;
  PMetadata m_metadata;
  int m_row;
};

class ModifyAuthorCommand : public UndoMetadataCommand
{
  Q_DECLARE_TR_FUNCTIONS(ModifyTitleCommand)

public:
  ModifyAuthorCommand(MetadataList* view,
                      PMetadata metadata,
                      const QString& newAuthor,
                      int row,
                      const QString& oldAuthor,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  MetadataList* m_view;
  QString m_newAuthor;
  QString m_oldAuthor;
  int m_row;
};

#endif // UNDOCOMMANDS_H
