#ifndef HTMLHIGHLIGHTER_H
#define HTMLHIGHLIGHTER_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTextDocument>

class HtmlScanner;

class CodeEdit;
class Tag;
class Attribute;

/*!
   \file htmlhighlighter.h "widgets/htmlhighlighter.h"
   \class HtmlHighlighter htmlhighlighter.h
   \brief A text highlighter for HTML 5 files.
   \since 5.15.0
   \license{The MIT License}
   \copyright © 2021 - 2022 Simon Meaden. All rights reserved.
   \requires{HtmlScanner to parse the html file}
   \sa HtmlScanner
*/
class HtmlHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  //! Constructor for HtmlHighlighter
  explicit HtmlHighlighter(HtmlScanner* scanner,
                           QPlainTextEdit* parent = nullptr);

  //! This method should be called by the using editor whenever the editor is
  //! clicked.
  void cursorPosHasChanged();

  //! Returns the line number of the current cursor position.
  int currentLineNumber() const;

  //! Returns the Tag at the current cursor position
  Tag* currentTag() const;
  //! Sets the current tag.
  void setCurrentTag(Tag* currentTag);

  //! Sets the background colour for all non current line blocks.
  void setBackground(const QColor& background);
  //! Sets the background colour for cuttent line blocks
  void setLineBackground(const QColor& lineBackground);
  //! Sets the foreground colour for tag/bracket matching
  void setMatchColor(const QColor& matchColor);
  //! Sets the foreground colour for tag names
  void setNameColor(const QColor& nameColor);
  //! Sets the foreground colour for attribute names
  void setAttrColor(const QColor& attrColor);
  //! Sets the foreground colour for attribute values
  void setValueColor(const QColor& valueColor);
  //! Sets the foreground colour for values within single quotes
  void setSQuoteColor(const QColor& sQuoteColor);
  //! Sets the foreground colour for values within double quotes
  void setDQuoteColor(const QColor& dQuoteColor);
  //! Sets the foreground colour for comments
  void setCommentColor(const QColor& comment);
  //! Sets the foreground colour for non tag text
  void setTextColor(const QBrush& newTextColor);
  //! Sets the foreground colour for detected errors
  void setErrorColor(const QColor& newError);
  //! \brief Sets the foreground color for special characters such a ⮒ which
  //! is used to display a new line character.
  void setCharForegroundColor(const QColor& CharForegroundColor);
  //! \brief Sets the background color for special characters such a ⮒ which
  //! is used to display a new line character.
  void setCharBackgroundColor(const QColor& CharBackgroundColor);

signals:

protected:
  //! \reimplements{QSyntaxHighlighter::highlightBlock}
  void highlightBlock(const QString&);

private:
  QPlainTextEdit* m_editor = nullptr;
  HtmlScanner* m_scanner;
  Tag* m_currentTag = nullptr;
  Tag* m_nextTag = nullptr;
  Tag* m_prevTag = nullptr;
  int m_currentIndex = -1;
  Tag* m_matchingTag = nullptr;
  int m_currentLineNumber = 0;

  QBrush m_textColor;
  QBrush m_backgroundColor;
  QColor m_matchColor;
  QColor m_nameColor;
  QColor m_attrColor;
  QColor m_valueColor;
  QColor m_sQuoteColor;
  QColor m_dQuoteColor;
  QBrush m_lineBackgroundColor;
  QColor m_commentColor;
  QColor m_errorColor;
  QColor m_charForegroundColor;
  QColor m_charBackgroundColor;

  QTextCharFormat m_lineTextFormat;
  QTextCharFormat m_lineMatchFormat;
  QTextCharFormat m_lineNameFormat;
  QTextCharFormat m_lineAttrFormat;
  QTextCharFormat m_lineValueFormat;
  QTextCharFormat m_lineSQuoteFormat;
  QTextCharFormat m_lineDQuoteFormat;
  QTextCharFormat m_lineCommentFormat;
  QTextCharFormat m_lineErrorFormat;
  QTextCharFormat m_textFormat;
  QTextCharFormat m_matchFormat;
  QTextCharFormat m_nameFormat;
  QTextCharFormat m_attrFormat;
  QTextCharFormat m_valueFormat;
  QTextCharFormat m_sQuoteFormat;
  QTextCharFormat m_dQuoteFormat;
  QTextCharFormat m_commentFormat;
  QTextCharFormat m_errorFormat;
  QTextCharFormat m_charFormat;

  void setDoubleQuoteToError(Attribute* att);
  void clearDoubleQuotesFromError();
  void setSingleQuoteToError(Attribute* att);
  void clearSingleQuotesFromError();
};

#endif // HTMLHIGHLIGHTER_H
