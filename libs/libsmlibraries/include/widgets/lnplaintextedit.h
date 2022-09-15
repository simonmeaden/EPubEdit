#ifndef LNPLAINTEXTEDIT_H
#define LNPLAINTEXTEDIT_H

#include <QAbstractItemModel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QStandardPaths>
#include <QTableView>
#include <QTextBlock>

#include "utilities/keymap.h"

class LabelledSpinBox;
class LabelledLineEdit;
class BookmarkArea;
class LineNumberArea;
class HoverWidget;

/*!
  \ingroup widgets
  \file lnplaintextedit.h
  \class LNPlainTextEdit lnplaintextedit.h
  \brief A QPlainTextEdit withline number and bookmark display capability.
  \author Simon Meaden
  \date 2022-05-10

  A QPlainTextEdit extension that displays line numbers and bookmarks.
 */

class BookmarkData;
/*!
 * \class BookmarkEditDialog lnplaintextedit.h
 * \brief The BookmarkEditDialog class allows the user to edit bookmark data
 */
class BookmarkEditDialog : public QDialog
{
  Q_OBJECT
public:
  //! Constructor for BookmarkEditDialog
  explicit BookmarkEditDialog(QWidget* parent = nullptr);

  //! Sets the dialog text to edit.
  void setText(const QString& text);
  //! Returns the edited dialog text
  QString text();
  //! sets the line number to edit.
  void setLineNumber(int linenumber);
  //! Returns the edited line number
  int lineNumber();

private:
  LabelledSpinBox* m_linenumberEdit;
  LabelledLineEdit* m_textEdit;
  QDialogButtonBox* m_buttonBox;
};

/*!
 * \class GoToBookmarkDialog lnplaintextedit.h
 * \brief The GoToBookmarkDialog class allows the user to move to a bookmark
 */
class GoToBookmarkDialog : public QDialog
{
  Q_OBJECT
public:
  //! Constructor for GoToBookmarkDialog
  explicit GoToBookmarkDialog(QMap<int, BookmarkData*>* bookmarks,
                              QWidget* parent = nullptr);

  //! Returns the bookmark index
  int bookmark();
  //! Returns the bookmark text
  QString text();

private:
  QGroupBox* m_group;
  QTableView* m_bookmarkView;
  QDialogButtonBox* m_buttonBox;
  int m_bookmarkIndex;
  QString m_text;

  void handleClicked(const QModelIndex& index);
};

/*!
 * \ingroup widgetsQElapsedTimer
 * \class LNPlainTextEdit lnplaintextedit.h "includes/widgets/lnplaintextedit.h"
 * \brief The LNPlainTextEdit class an extension of QPlainTextEdit that displays
 * line numbers and bookmarks.
 */
class LNPlainTextEdit : public QPlainTextEdit
{
  Q_OBJECT

  class CommonArea : public QWidget
  {
  public:
    CommonArea(LNPlainTextEdit* editor);

    bool isEnabled() const;
    void setEnabled(bool enabled);

  protected:
    LNPlainTextEdit* m_editor = nullptr;
    bool m_enabled = true;

    void mousePressEvent(QMouseEvent* event) override;
    //! Implements a context menu for bookmark/linenumber/fold areas
    void contextMenuEvent(QContextMenuEvent* event) override;

    static const int WIDTH = 15;
  };

  /*!
   * \brief The BookmarkData class stores data about the bookmark.
   */
  struct BookmarkData
  {
    //! Constructor for BookmarkData
    BookmarkData(QString str = QString())
    {
      text = str;
      if (!text.isEmpty())
        valid = true;
    }
    //! Copy constructor for BookmarkData
    BookmarkData(const BookmarkData& data)
    {
      text = data.text;
      rect = data.rect;
      pixmap = data.pixmap;
      valid = data.valid;
    }

    //! Returns true if the bookmark data is valid.
    bool isValid() { return valid; }

    QString text;       //!< The bookmark tex
    QRect rect;         //!< The display rectangle.
    int pixmap = 0;     //!< The optional icon pixmap
    bool valid = false; //!< Validity flag.
  };

  class LineNumberArea : public CommonArea
  {
  public:
    LineNumberArea(LNPlainTextEdit* editor);

    QSize sizeHint() const override;

  protected:
    void paintEvent(QPaintEvent* event) override;
  };

  class BookmarkArea : public CommonArea
  {
    struct HoverItem
    {
      int iconId;
      QString title;
      QString text;
      QRect rect;
    };

  public:
    BookmarkArea(LNPlainTextEdit* editor = nullptr);
    ~BookmarkArea();

    QSize sizeHint() const override;

    int width() const;
    void setWidth(int width) { m_width = width; }

    const QRect& rect() const;

    void insertHover(int lineNumber, int icon, const QString &title, const QString& text);
    bool hasHover(int lineNumber);
    void clearHovers();

    QString hoverTitle(int lineNumber);
    QString hoverText(int lineNumber);
    int hoverIcon(int lineNumber);

    // Has to be here as I need to access it from LNPlainTextEdit
    //    void mouseMoveEvent(QMouseEvent* event) override;

  protected:
    //! \reimplements{QPlainTextEdit::paintEvent(QPaintEvent*)
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    //    bool event(QEvent *event);

    //    void hoverEnter(QHoverEvent *event);
    //    void hoverLeave(QHoverEvent *event);
    //    void hoverMove(QHoverEvent *event);

  private:
    int m_width;
    QRect m_rect;
    int m_lineNumber = 0;
    int m_lineCount = 0;

    QMap<int, HoverItem*> m_hovers;
  };

  class FoldArea : public CommonArea
  {
  public:
    FoldArea(LNPlainTextEdit* editor);
    QSize sizeHint() const override;

    //    void load();
    //    void save();

    const QList<int> folds();
    void setFolds(QList<int> folds);
    bool setFolds(QList<int> folds, QList<bool> folded);
    bool setFolds(QList<int> folds, QList<bool> folded, QList<QRect> rects);
    void insertFold(int lineNumber);
    void insertFold(int lineNumber, bool folded);
    void insertFold(int lineNumber, bool folded, QRect rect);
    void clearFolds();
    bool hasFold(int lineNumber);
    bool isFolded(int lineNumber);
    QRect rect(int lineNumber);
    void set(int lineNumber, bool folded);
    void set(int lineNumber, QRect rect);
    void set(int lineNumber, bool folded, QRect rect);

    int width() const;

  protected:
    //! \reimplements{QPlainTextEdit::paintEvent(QPaintEvent*)
    void paintEvent(QPaintEvent* event) override;
    //! \reimplements{CommonArea::mousePressEvent(QMouseEvent*)
    void mousePressEvent(QMouseEvent* event) override;

  private:
    int m_width;
    QRect m_rect;
    QMap<int, QPair<bool, QRect>> m_folds;
  };

  /*!
   * \brief Model for a QTableView in class GoToBookmarkDialog
   */
  class BookmarkModel : public QAbstractTableModel
  {
  public:
    //! Constructor for BookmarkModel
    explicit BookmarkModel(QMap<int, BookmarkData*>* bookmarks);

    //! \reimplements{QAbstractTableModel::columnCount}
    int columnCount(const QModelIndex& = QModelIndex()) const;
    //! \reimplements{QAbstractTableModel::rowCount}
    int rowCount(const QModelIndex& = QModelIndex()) const;
    //! \reimplements{QAbstractTableModel::data}
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    //! \reimplements{QAbstractTableModel::flags}
    Qt::ItemFlags flags(const QModelIndex& index) const;
    //! \reimplements{QAbstractTableModel::headerData}
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int = Qt::DisplayRole) const;

  private:
    QMap<int, BookmarkData*>* m_bookmarks;
  };

public:
  friend class BookmarkArea;
  friend class LineNumberArea;
  enum IconId
  {
    Bookmark = 0,
    Cross,
    Bar,
    FoldedCross,
    UnfoldedCross,
    FoldedArrow,
    UnfoldedArrow,
    Warning,
    BlackCircle,
    WhiteCircle,
    BlueCircle,
    DarkBlueCircle,
    RedCircle,
    DarkRedCircle,
    GreenCircle,
    DarkGreenCircle,
    YellowCircle,
    OrangeCircle,
    PinkCircle,
    CyanCircle,
    HighPixmap, // Just used to indicate last standard pixmap.
    CustomPixmap = 0x100,
  };
  enum Style
  {
    ArrowStyle,
    CrossStyle,
    CustomStyle,
  };

  //! Constructor for LNPlainTextEdit
  LNPlainTextEdit(QWidget* parent = nullptr);

  int addCustomPixmap(const QPixmap pixmap);
  const QPixmap pixmap(int id);

  static QPixmap unfoldedPixmap(Style style, int pixmap = -1);

  static QPixmap foldedPixmap(Style style, int pixmap = -1);

  /*!
   * \brief Returns a list of the fold line numbers.
   */
  const QList<int> folds();
  /*!
   * \brief Sets a list of the fold line numbers.
   *
   * \note This does not set the folded flag, all folds are set to
   * false (unfolded). To set the folded flag to folded use either
   * the setFolds(QList<int>, QList<bool>) method to block set or
   * use setFold(int, bool) to set individual folds.
   */
  void setFolds(QList<int> folds);
  /*!
   * \brief Sets a list of the fold line numbers with attacted set of folded
   * flags.
   *
   * \note Both lists must be the same size otherwise this returns with a value
   * of false.
   */
  bool setFolds(QList<int> folds, QList<bool> folded);
  //! Add a single fold, unfolded by default.
  void insertFold(int lineNumber);
  //! Add a single fold, with a folded flag value.
  void insertFold(int lineNumber, bool folded);
  //! Clears all folds.
  //!
  //! \note There is no checking before deletion. This is up to the programmer.
  void clearFolds();

  /*!
   * \brief Inserts a hoveritem into the bookmark area.
   *
   * Hover items place an icon into the bookmark area and displays
   * the supplied text as a hover item when the cursor hovers over
   * the icon.
   */
  void insertHoverItem(int lineNumber, int icon, const QString &title, const QString& text);
  //! Checks whether there is an hover item at thte supplied line number.
  bool hasHoverItem(int lineNumber);
  //! Clears the hover items from the bookmark area.
  //!
  //! \note There is no checking before deletion. This is up to the programmer.
  void clearHoverItems();

  //! Returns true if there is a fold set at this line.
  //!
  //! \note This does not indicate that it is folde or unfolded. To get
  //! the folded flag use isFolded(int).
  bool hasFold(int lineNumber);
  //! Returns true if there is a is folded, otherwise false.
  //!
  //! \note If no fold at this linenumber then this returns false.
  bool isFolded(int lineNumber);
  //! Set the folded flag fo a linenumber. If there is no fold at
  //! this lineNumber it is created.
  void setFold(int lineNumber, bool folded);

  //! Returns the background color for the current line number.
  const QColor& lineNumberBackColor() const;
  //! Sets the background color for the current line number.
  void setLineNumberBackColor(const QColor& backColor);
  //! Returns the foreground color for the current line number.
  const QColor& lnAreaTextColor() const;
  //! Sets the foreground color for the current line number.
  void setLNAreaTextColor(const QColor& foreColor);
  //! Returns the background color for the selected line.https://cpp.hotexamples.com/examples/-/QFont/setWeight/cpp-qfont-setweight-method-examples.html
  const QColor& lnAreaSelectedBackColor() const;
  //! Sets the background color for the current line number.
  void setLNAreaSelectedBackColor(const QColor& backColor);
  //! Returns the foreground color for selected text.
  const QColor& lnAreaSelectedTextColor() const;
  //! Sets the foreground color for selected text.
  void setLNAreaSelectedTextColor(const QColor& textColor);

  //! Returns the value of the highlight line flag.
  bool highlightLine() const;
  //! Sets the value of the highlight line flag.
  void setHighlightLine(bool highlightLine);

  //! Move the cursor to the selected line.
  void goToLine(int lineNumber);
  //! Move the cursor to the previous bookmark
  bool goToPreviousBookmark();
  //! Move the cursor to the next bookmark
  bool goToNextBookmark();

  void goToBookmark(int bookmark = -1);

  //! Sets the key map for the current editor.
  void setKeyMap(const QMap<KeyEventMapper, KeyMapper>& KeyMap);

  //! Enables or disables the bookmark area.
  //!
  //! If enable is true the bookmark area is displayed, otherwise it is hidden.
  void enableBookmarks(bool enable);
  //! Returns the value of the bookmark area enablement flag.
  bool isBookmarksEnabled();

  //! Enables or disables the fold area.
  //!
  //! If enable is true the fold area is displayed, otherwise it is hidden.
  void enableFolds(bool enable);
  //! Returns the value of the fold area enablement flag.
  bool isFoldsEnabled();

  //! Enables or disables the linenumber area.
  //!
  //! If enable is true the linenumber area is displayed, otherwise it is
  //! hidden.m_bookmarkArea
  void enableLineNumbers(bool enable);
  //! Returns the value of the linenumber area enablement flag.
  bool isLineNumbersEnabled();

  void setFoldStyle(Style foldStyle);

  bool isShowNewline();
  void setShowNewline(bool showNewline);
  void showNewline();
  void clearNewline();

  bool isShowTabs();
  void setShowTabs(bool showTabs);
  void showTabs();
  void clearTabs();

  //! returns all known fixed width font families
  const QStringList& getFontFamilies() const;

signals:
  void lineNumberChanged(int);
  void lineCountChanged(int);
  void columnChanged(int);
  void locationChanged(int line, int lineCount, int column);
  //! triggered when an unfold icon is clicked
  void unfold(int lineNumber);
  //! triggered when an fold icon is clicked
  void fold(int lineNumber);

protected:
  //! \reimplements{QPlainTextEdit::mousePressEvent(QMouseEvent*)
  void mousePressEvent(QMouseEvent* event) override;
  //  //! \reimplements{QPlainTextEdit::mouseMoveEvent(QMouseEvent*)
  //  void mouseMoveEvent(QMouseEvent* event) override;
  //! \reimplements{QPlainTextEdit::eventFilter(QObject*, QEvent*)
  bool eventFilter(QObject* obj, QEvent* event) override;
  //! \reimplements{QPlainTextEdit::resizeEvent(QResizeEvent*)
  void resizeEvent(QResizeEvent* event) override;
  //! \reimplements{QPlainTextEdit::contextMenuEvent(QContextMenuEvent*)
  void contextMenuEvent(QContextMenuEvent* event) override;

  bool event(QEvent* event) override;

//  void hoverEnter(QHoverEvent* event);
  void hoverLeave(QHoverEvent* event);
  void hoverMove(QHoverEvent* event);

  void setFontToSourceCodePro();

private:
  QStringList m_fontFamilies;
  BookmarkArea* m_bookmarkArea = nullptr;
  LineNumberArea* m_lineNumberArea = nullptr;
  FoldArea* m_foldArea = nullptr;
  QColor m_lnAreaBackColor;
  QColor m_lnAreaTextColor;
  QColor m_lnAreaSelectedBackColor;
  QColor m_lnAreaSelectedTextColor;
  bool m_highlightLine = true;
  QMap<KeyEventMapper, KeyMapper> m_keyMap;
  bool m_bookmarkAreaEnabled = true;
  bool m_lineNumberAreaEnabled = true;
  bool m_foldAreaEnabled = true;
  static int m_customPixmap;
  static QMap<int, QPixmap> m_pixmaps;
  Style m_foldStyle = ArrowStyle;
  QStringList m_fontfontFamilies;
  int m_lineNumber = 1, m_lineCount = 0;
  QMap<int, BookmarkData> m_bookmarks;
  QMap<int, QString> m_oldBookmarks;
  HoverWidget* m_hoverWidget = nullptr;

  bool m_showNewline = false;
  bool m_showTabs = false;

  //  //! Loads custom pixmaps.
  //  void load();
  //  //! Saves custom pixmaps.
  //  void save();

  void goToBookmark(bool triggered);

  void updateViewableAreaWidth(int blockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect& rect, int dy);
  void updateBookmarkArea(const QRect& rect, int dy);
  void updateFoldArea(const QRect& rect, int dy);

  void paintBookmarkArea(QPaintEvent* event);

  void paintLNArea(QPaintEvent* event);
  int lineNumberAreaWidth();

  void paintFoldArea(QPaintEvent* event);

  void handleBookmarkContextMenu(QContextMenuEvent* event);
  void handleAddBookmark(bool);
  void handleRemoveBookmark(bool);
  void handleEditBookmark(bool);
  void handleClearBookmarks(bool);

  QString bookmarkText(int bookmark);
  void setBookmarks(QMap<int, BookmarkData>* bookmarks);
  bool hasBookmarks();
  bool hasBookmark(int bookmark);
  void insertBookmark(int bookmark, const QString& text = QString());
  void editBookmark(int lineNumber);
  void removeBookmark(int bookmark);
  void clearBookmarks();
  void toggleBookmark(int bookmark);
  bool hasBookmarkText(int bookmark);
  int bookmarkCount();
  void loadBookmarks();
  void saveBookmarks();
  BookmarkData bookmark(int number);

  void bookmarkHoverFinished();

  //  int isInBookmark(QPoint pos);

  void gotoLineNumber();

  int calculateColumn();
  QPair<int, int> calculateLineNumber(QTextCursor textCursor);
  QAction* actionForKey(const QString& text, KeyEventMapper mapper);

  void appendIfNotInList(QStringList list, QStringList& fontFamilies)
  {
    for (auto family : list) {
      if (!fontFamilies.contains(family)) {
        fontFamilies.append(family);
      }
    }
  }
  void loadStandardPixmaps();
};

#endif // LNPLAINTEXTEDIT_H
