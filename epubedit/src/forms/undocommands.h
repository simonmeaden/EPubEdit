#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QSharedPointer>
#include <QString>
#include <QUndoCommand>

class TitleView;
class AuthorList;
class EPubTitle;

#include "document/uniquestring.h"

class AddTitleCommand : public QUndoCommand
{
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

class AddAuthorCommand : public QUndoCommand
{
public:
  AddAuthorCommand(AuthorList* view,
                   const QString& author,
                   int row,
                   QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  AuthorList* m_view;
  QString m_author;
  int m_row;
};

class SetIdCommand : public QUndoCommand
{
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

class RemoveAuthorCommand : public QUndoCommand
{
public:
  RemoveAuthorCommand(AuthorList* view,
                      const QString& author,
                      int row,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  AuthorList* m_view;
  QString m_author;
  int m_row;
};

class SwapTitleCommand : public QUndoCommand
{
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
public:
  SwapAuthorCommand(AuthorList* view,
                    const QString& author1,
                    int row1,
                    const QString& author2,
                    int row2,
                    QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  AuthorList* m_view;
  QString m_author1;
  int m_row1;
  QString m_author2;
  int m_row2;
};

class SwapIdCommand : public QUndoCommand
{
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

class ModifyTitleCommand : public QUndoCommand
{
public:
  ModifyTitleCommand(TitleView* view,
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
  int m_row;
};

class ModifyAuthorCommand : public QUndoCommand
{
public:
  ModifyAuthorCommand(AuthorList* view,
                      const QString& newAuthor,
                      int row,
                      const QString& oldAuthor,
                      QUndoCommand* parent = nullptr);

  void undo() override;
  void redo() override;

private:
  AuthorList* m_view;
  QString m_newAuthor;
  QString m_oldAuthor;
  int m_row;
};

#endif // UNDOCOMMANDS_H
