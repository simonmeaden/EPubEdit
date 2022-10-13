#ifndef HTMLHIGHLIGHTER_H
#define HTMLHIGHLIGHTER_H

//#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QTextDocument>

#include "utilities/x11colors.h"
#include "widgets/lnplaintextedit.h"

class HtmlScanner;
class HtmlEdit;
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
                           LNPlainTextEdit* parent = nullptr);

  //! This method should be called by the using editor whenever the editor is
  //! clicked.
  void cursorPosHasChanged();

  //! Returns the line number of the current cursor position.
  int currentLineNumber() const;

  //! Returns the Tag at the current cursor position
  Tag* currentTag() const;
  //! Sets the current tag.
  void setCurrentTag(Tag* currentTag);

  //! Returns the background colour.
  QColor background() { return m_backgroundColor; }
  //! Sets the background colour for all non current line blocks.
  void setBackground(const QColor& background);

  //! Gets the colour for bracket tag/matching
  QColor matchColor() { return m_matchColor; }
  //! Sets the foreground and, optionally, the background colour for
  //! tag start (<) / tag end (>) and bracket matching
  void setMatchColor(
    const QColor& matchColor,
    const QColor& matchBackgound = QColorConstants::X11::darkslategray);

  //! Gets the colour for bracket tag/matching
  QColor nameColor() { return m_nameColor; }
  //! Sets the foreground colour for tag names
  void setNameColor(const QColor& nameColor);

  //! Gets the foreground colour for attribute names
  QColor attrColor() { return m_attrColor; }
  //! Sets the foreground colour for attribute names
  void setAttrColor(const QColor& attrColor);

  //! Gets the foreground colour for attribute values
  QColor valueColor() { return m_valueColor; }
  //! Sets the foreground colour for attribute values
  void setValueColor(const QColor& valueColor);

  //! Gets the foreground colour for values within single quotes
  QColor sQuoteColor() { return m_sQuoteColor; }
  //! Sets the foreground colour for values within single quotes
  void setSQuoteColor(const QColor& sQuoteColor);

  //! Gets the foreground colour for values within double quotes
  QColor dQuoteColor() { return m_dQuoteColor; }
  //! Sets the foreground colour for values within double quotes
  void setDQuoteColor(const QColor& dQuoteColor);

  //! Gets the foreground colour for comments
  QColor commentColor() { return m_commentColor; }
  //! Sets the foreground colour for comments
  void setCommentColor(const QColor& comment);

  //! Gets the foreground colour for non tag text
  QColor textColor() { return m_textColor; }
  //! Sets the foreground colour for non tag text
  void setTextColor(const QColor& textColor);

  //! Gets the foreground colour for detected errors
  QColor errorColor() { return m_errorColor; }
  //! Sets the foreground colour for detected errors
  void setErrorColor(const QColor& error);

  //! \brief Gets the foreground color for special characters such a ⮒ which
  //! is used to display a new line character.
  QColor charColor() { return m_charForegroundColor; }
  //! \brief Sets the foreground color for special characters such a ⮒ which
  //! is used to display a new line character.
  void setCharForegroundColor(const QColor& CharForegroundColor);

  //! \brief Gets the background color for special characters such a ⮒ which
  //! is used to display a new line character.
  QColor charBackgroundColor() { return m_charBackgroundColor; }
  //! \brief Sets the background color for special characters such a ⮒ which
  //! is used to display a new line character.
  void setCharBackgroundColor(const QColor& CharBackgroundColor);

  //! \brief Gets the underline color for tag matching.
  const QColor& tagMatchUnderlineColor() const;
  //! \brief Sets the underline color for tag matching.
  void setTagMatchUnderlineColor(const QColor& tagMatchUnderlineColor);

  //! Sets the start position for bracket and quotation matching
  void setBracketMatchStart(int brktMatchStart);
  //! Sets the end position for bracket and quotation matching
  void setBracketMatchEnd(int brktMatchEnd);
  //! Clears the position data bracket and quotation matching
  void clearBracketMatch();

  //! Returns the start tag for tag matching
  Tag* startTagMatch() const;
  //! Sets the start tag for tag matching
  void setStartTagMatch(Tag* startTagMatch);

  //! Returns the end tag for tag matching
  Tag* endTagMatch() const;
  //! Sets the end tag for tag matching
  void setEndTagMatch(Tag* endTagMatch);

signals:

protected:
  //! \reimplements{QSyntaxHighlighter::highlightBlock}
  void highlightBlock(const QString&);

private:
  LNPlainTextEdit* m_editor = nullptr;
  HtmlScanner* m_scanner;
  Tag* m_currentTag = nullptr;
  Tag* m_nextTag = nullptr;
  Tag* m_prevTag = nullptr;
  int m_currentIndex = -1;
  Tag* m_matchingTag = nullptr;
  int m_currentLineNumber = 0;

  int m_brktMatchStart = -1;
  int m_brktMatchEnd = -1;

  Tag* m_startTagMatch = nullptr;
  Tag* m_endTagMatch = nullptr;
  QList<QTextCharFormat> m_startTagFormats;
  QList<QTextCharFormat> m_endTagFormats;
  bool m_startTagMatchModified = false;
  bool m_endTagMatchModified = false;

  QColor m_textColor;
  QColor m_backgroundColor;
  QColor m_matchColor;
  QColor m_matchBackgroundColor;
  QColor m_nameColor;
  QColor m_attrColor;
  QColor m_valueColor;
  QColor m_sQuoteColor;
  QColor m_dQuoteColor;
  QColor m_commentColor;
  QColor m_errorColor;
  QColor m_charForegroundColor;
  QColor m_charBackgroundColor;
  QColor m_tagMatchUnderlineColor;

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
  QTextCharFormat m_tagMatchFormat;

  void setDoubleQuoteToError(Attribute* att);
  void clearDoubleQuotesFromError();
  void setSingleQuoteToError(Attribute* att);
  void clearSingleQuotesFromError();
  bool formatComment(Tag* tag,
                     const QString& text,
                     int blockStart,
                     int blockEnd,
                     int blockLength,
                     int tagStart,
                     int tagEnd,
                     int tagOffset,
                     int tagLength);
};

#endif // HTMLHIGHLIGHTER_H
