#include "widgets/lnplaintextedit.h"
#include "labelled/labelledlineedit.h"
#include "labelled/labelledspinbox.h"
#include "qyamlcpp.h"
#include "utilities/ContainerUtil.h"
#include "utilities/MathUtilities.h"
#include "utilities/characters.h"
#include "utilities/paths.h"
#include "utilities/x11colors.h"
#include "widgets/hoverwidget.h"

//#include <algorithm>

//====================================================================
//=== GoToBookmarkDialog
//====================================================================
GoToBookmarkDialog::GoToBookmarkDialog(QMap<int, BookmarkData*>* bookmarks,
                                       QWidget* parent)
  : QDialog(parent)
  , m_bookmarkIndex(-1)
  , m_text(QString())
{
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);

  m_group = new QGroupBox(tr("Bookmarks"), this);
  //  m_group->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_group);

  QVBoxLayout* grpLayout = new QVBoxLayout;
  grpLayout->setContentsMargins(0, 0, 0, 0);
  m_group->setLayout(grpLayout);

  //  auto bookmarkModel = new BookmarkModel(bookmarks);
  //  m_bookmarkView = new QTableView(this);
  //  m_bookmarkView->setModel(bookmarkModel);
  //  m_bookmarkView->verticalHeader()->hide();
  //  m_bookmarkView->horizontalHeader()->setVisible(true);
  //  m_bookmarkView->horizontalHeader()->setStretchLastSection(true);
  //  grpLayout->addWidget(m_bookmarkView);
  //  connect(m_bookmarkView,
  //          &QTableView::clicked,
  //          this,
  //          &GoToBookmarkDialog::handleClicked);

  m_buttonBox =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_buttonBox);

  connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

int
GoToBookmarkDialog::bookmark()
{
  return m_bookmarkIndex;
}

QString
GoToBookmarkDialog::text()
{
  return m_text;
}

void
GoToBookmarkDialog::handleClicked(const QModelIndex& index)
{
  QModelIndex i0 = m_bookmarkView->model()->index(index.row(), 0);
  QModelIndex i1 = m_bookmarkView->model()->index(index.row(), 1);

  if (i0.isValid()) {
    m_bookmarkIndex = m_bookmarkView->model()->data(i0).toInt();
  }

  if (i1.isValid()) {
    m_text = m_bookmarkView->model()->data(i1).toString();
  }
}

//====================================================================
//=== BookmarkEditDialog
//====================================================================
BookmarkEditDialog::BookmarkEditDialog(QWidget* parent)
  : QDialog(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);

  m_textEdit = new LabelledLineEdit(tr("Note Text:"), this);
  layout->addWidget(m_textEdit);
  m_linenumberEdit = new LabelledSpinBox(tr("Line Number"), this);
  layout->addWidget(m_linenumberEdit);

  m_buttonBox =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_buttonBox);

  connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void
BookmarkEditDialog::setText(const QString& text)
{
  m_textEdit->setText(text);
}

QString
BookmarkEditDialog::text()
{
  return m_textEdit->text();
}

void
BookmarkEditDialog::setLineNumber(int linenumber)
{
  m_linenumberEdit->setValue(linenumber);
}

int
BookmarkEditDialog::lineNumber()
{
  return m_linenumberEdit->value();
}

//====================================================================
//=== LNPlainTextEdit
//====================================================================
int LNPlainTextEdit::m_customPixmap = -1;
QMap<int, QPixmap> LNPlainTextEdit::m_pixmaps = QMap<int, QPixmap>();

LNPlainTextEdit::LNPlainTextEdit(QWidget* parent)
  : QPlainTextEdit(parent)
  , m_lnAreaBackColor(QColorConstants::X11::grey90)
  , m_lnAreaTextColor(QColorConstants::X11::grey40)
  , m_lnAreaSelectedBackColor(QColorConstants::X11::grey80)
  , m_lnAreaSelectedTextColor(QColorConstants::X11::yellowgreen)
{
  setMouseTracking(true);
  setFontToSourceCodePro();
  loadStandardPixmaps();

  connect(this,
          &LNPlainTextEdit::blockCountChanged,
          this,
          &LNPlainTextEdit::updateViewableAreaWidth);
  connect(this,
          &LNPlainTextEdit::updateRequest,
          this,
          &LNPlainTextEdit::updateBookmarkArea);
  connect(this,
          &LNPlainTextEdit::updateRequest,
          this,
          &LNPlainTextEdit::updateLineNumberArea);
  connect(this,
          &LNPlainTextEdit::updateRequest,
          this,
          &LNPlainTextEdit::updateFoldArea);
  connect(this,
          &LNPlainTextEdit::cursorPositionChanged,
          this,
          &LNPlainTextEdit::highlightCurrentLine);

  enableBookmarks(true);
  enableLineNumbers(true);
  enableFolds(true);
}

void
LNPlainTextEdit::setFontToSourceCodePro()
{
  auto sysFamilies = QFontDatabase::families();
  if (!sysFamilies.contains("Source Code Pro")) {
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont("://fonts/SourceCodePro-Black.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-BlackItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont("://fonts/SourceCodePro-Bold.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-BoldItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-ExtraBold.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-ExtraBoldItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-ExtraLight.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-ExtraLightItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-Italic-VariableFont_wght.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont("://fonts/SourceCodePro-Light.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-LightItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(
        QFontDatabase::addApplicationFont("://fonts/SourceCodePro-Medium.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-MediumItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-Regular.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-SemiBold.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-SemiBoldItalic.ttf")),
      m_fontFamilies);
    appendIfNotInList(
      QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(
        "://fonts/SourceCodePro-VariableFont_wght.ttf")),
      m_fontFamilies);
  }
  for (auto& family : sysFamilies) {
    auto fixed = QFontDatabase::isFixedPitch(family);
    if (fixed) {
      appendIfNotInList(QStringList() << family, m_fontFamilies);
    }
  }

  QFont font = QFont("Source Code Pro Medium", 10, 2);
  setFont(font);
}

void
LNPlainTextEdit::loadStandardPixmaps()
{
  m_pixmaps.insert(Bookmark, QPixmap(":/icon/BookmarkFlag"));
  m_pixmaps.insert(Cross, QPixmap(":/icon/Cross"));
  m_pixmaps.insert(Bar, QPixmap(":/icon/Dash"));
  m_pixmaps.insert(FoldedCross, QPixmap(":/icon/FoldedCross"));
  m_pixmaps.insert(UnfoldedCross, QPixmap(":/icon/UnfoldedCross"));
  m_pixmaps.insert(FoldedArrow, QPixmap(":/icon/FoldedArrow"));
  m_pixmaps.insert(UnfoldedArrow, QPixmap(":/icon/UnfoldedArrow"));
  m_pixmaps.insert(Warning, QPixmap(":/icon/Warning"));
  m_pixmaps.insert(BlackCircle, QPixmap(":/icon/BlackDot"));
  m_pixmaps.insert(WhiteCircle, QPixmap(":/icon/WhiteDot"));
  m_pixmaps.insert(BlueCircle, QPixmap(":/icon/BlueDot"));
  m_pixmaps.insert(DarkBlueCircle, QPixmap(":/icon/DarkBlueDot"));
  m_pixmaps.insert(RedCircle, QPixmap(":/icon/RedDot"));
  m_pixmaps.insert(DarkRedCircle, QPixmap(":/icon/DarkRedDot"));
  m_pixmaps.insert(GreenCircle, QPixmap(":/icon/GreenDot"));
  m_pixmaps.insert(DarkGreenCircle, QPixmap(":/icon/DarkGreenDot"));
  m_pixmaps.insert(YellowCircle, QPixmap(":/icon/YellowDot"));
  m_pixmaps.insert(OrangeCircle, QPixmap(":/icon/OrangeDot"));
  m_pixmaps.insert(PinkCircle, QPixmap(":/icon/PinkDot"));
  m_pixmaps.insert(CyanCircle, QPixmap(":/icon/CyanDot"));
}

const QStringList&
LNPlainTextEdit::getFontFamilies() const
{
  return m_fontFamilies;
}

const QPixmap
LNPlainTextEdit::pixmap(int id)
{
  return m_pixmaps.value(id);
}

QPixmap
LNPlainTextEdit::unfoldedPixmap(Style style, int pixmap)
{
  switch (style) {
    case ArrowStyle:
      return m_pixmaps.value(UnfoldedArrow);
    case CrossStyle:
      return m_pixmaps.value(UnfoldedCross);
    case CustomStyle:
      if (pixmap >= 0 && pixmap >= CustomPixmap && pixmap < m_pixmaps.size())
        return m_pixmaps.value(pixmap);
      else
        break;
  }
  return QPixmap();
}

QPixmap
LNPlainTextEdit::foldedPixmap(Style style, int pixmap)
{
  switch (style) {
    case ArrowStyle:
      return m_pixmaps.value(FoldedArrow);
    case CrossStyle:
      return m_pixmaps.value(FoldedCross);
    case CustomStyle:
      if (pixmap >= 0 && pixmap >= CustomPixmap && pixmap < m_pixmaps.size())
        return m_pixmaps.value(pixmap);
      else
        break;
  }
  return QPixmap();
}

const QList<int>
LNPlainTextEdit::folds()
{
  return m_foldArea->folds();
}

void
LNPlainTextEdit::setFolds(QList<int> folds)
{
  m_foldArea->setFolds(folds);
}

bool
LNPlainTextEdit::setFolds(QList<int> folds, QList<bool> folded)
{
  return m_foldArea->setFolds(folds, folded);
}

void
LNPlainTextEdit::insertFold(int lineNumber)
{
  m_foldArea->insertFold(lineNumber);
}

void
LNPlainTextEdit::insertFold(int lineNumber, bool folded)
{
  m_foldArea->insertFold(lineNumber, folded);
}

void
LNPlainTextEdit::clearFolds()
{
  m_foldArea->clearFolds();
}

void
LNPlainTextEdit::insertHoverItem(int lineNumber,
                                 int icon,
                                 const QString& title,
                                 const QString& text)
{
  m_bookmarkArea->insertHover(lineNumber, icon, title, text);
}

bool
LNPlainTextEdit::hasHoverItem(int lineNumber)
{
  return m_bookmarkArea->hasHover(lineNumber);
}

void
LNPlainTextEdit::clearHoverItems()
{
  m_bookmarkArea->clearHovers();
}

bool
LNPlainTextEdit::hasFold(int lineNumber)
{
  return m_foldArea->hasFold(lineNumber);
}

bool
LNPlainTextEdit::isFolded(int lineNumber)
{
  return m_foldArea->isFolded(lineNumber);
}

void
LNPlainTextEdit::setFold(int lineNumber, bool folded)
{
  m_foldArea->set(lineNumber, folded);
}

int
LNPlainTextEdit::addCustomPixmap(const QPixmap pixmap)
{
  if (m_customPixmap == -1) {
    m_customPixmap = int(CustomPixmap);
  }

  auto id = m_customPixmap++;
  m_pixmaps.insert(id, pixmap);
  return id;
}

void
LNPlainTextEdit::paintBookmarkArea(QPaintEvent* event)
{
  auto rect = m_bookmarkArea->rect();
  QPainter painter(m_bookmarkArea);
  painter.fillRect(rect, m_lnAreaBackColor);

  auto block = firstVisibleBlock();
  auto blockNumber = block.blockNumber();
  auto top =
    qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  auto bottom = top + qRound(blockBoundingRect(block).height());
  auto height = fontMetrics().height();
  auto blockHeight = blockBoundingRect(block).height();

  while (block.isValid() && top <= rect.bottom()) {
    if (block.isVisible() && bottom >= rect.top()) {
      int lineNumber = blockNumber + 1;

      if (lineNumber == m_lineNumber)
        painter.fillRect(
          rect.x(), top, rect.width(), blockHeight, m_lnAreaSelectedBackColor);

      if (m_bookmarkArea->hasHover(lineNumber)) {
        int pxId = m_bookmarkArea->hoverIcon(lineNumber);
        auto px = pixmap(pxId);
        painter.drawPixmap(
          Math::halfDifference(m_bookmarkArea->width(), px.width()),
          top + Math::halfDifference(height, px.height()),
          px);
      }

      if (hasBookmark(lineNumber)) {
        auto data = bookmark(lineNumber);
        auto pxId = data.pixmap;
        auto px = pixmap(pxId);
        painter.drawPixmap(
          Math::halfDifference(m_bookmarkArea->width(), px.width()),
          top + Math::halfDifference(height, px.height()),
          px);
        data.rect = QRect(0, top, width(), height);
      }
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockHeight);
    ++blockNumber;
  }
}

void
LNPlainTextEdit::paintFoldArea(QPaintEvent* event)
{
  auto rect = event->rect();
  QPainter painter(m_foldArea);
  painter.fillRect(rect, m_lnAreaBackColor);

  auto block = firstVisibleBlock();
  auto blockNumber = block.blockNumber();
  auto top =
    qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  auto bottom = top + qRound(blockBoundingRect(block).height());
  auto blockHeight = qRound(blockBoundingRect(block).height());

  while (block.isValid() && top <= rect.bottom()) {
    if (block.isVisible() && bottom >= rect.top()) {
      auto lineNumber = blockNumber + 1;

      if (lineNumber == m_lineNumber)
        painter.fillRect(
          rect.x(), top, rect.width(), blockHeight, m_lnAreaSelectedBackColor);

      if (m_foldArea->hasFold(lineNumber)) {
        QPixmap px;
        if (m_foldArea->isFolded(lineNumber))
          px = foldedPixmap(m_foldStyle);
        else
          px = unfoldedPixmap(m_foldStyle);
        painter.drawPixmap(
          Math::halfDifference(m_foldArea->width(), px.width()),
          top + Math::halfDifference(blockHeight, px.height()),
          px);
        m_foldArea->set(lineNumber,
                        QRect(0, top, m_foldArea->width(), blockHeight));
      }
    }

    block = block.next();
    top = bottom;
    bottom = top + blockHeight;
    ++blockNumber;
  }
}

void
LNPlainTextEdit::paintLNArea(QPaintEvent* event)
{
  auto rect = event->rect();
  auto fm = fontMetrics();

  QPainter painter(m_lineNumberArea);
  painter.fillRect(rect, m_lnAreaBackColor);

  QTextBlock block = firstVisibleBlock();
  auto blockNumber = block.blockNumber();
  auto top =
    qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  auto blockHeight = qRound(blockBoundingRect(block).height());
  auto bottom = top + blockHeight;

  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      auto lineNumber = blockNumber + 1;
      QString number = QString::number(lineNumber);

      if (lineNumber == m_lineNumber)
        painter.fillRect(
          rect.x(), top, rect.width(), blockHeight, m_lnAreaSelectedBackColor);

      painter.setPen((textCursor().blockNumber() == blockNumber)
                       ? m_lnAreaSelectedTextColor
                       : m_lnAreaTextColor);
      auto width = m_lineNumberArea->width();
      painter.drawText((width - fm.horizontalAdvance(number)) - 3,
                       bottom - fm.descent(),
                       number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

int
LNPlainTextEdit::lineNumberAreaWidth()
{
  auto digits = 1;
  auto max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  auto space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

void
LNPlainTextEdit::handleBookmarkContextMenu(QContextMenuEvent* event)
{
  QMenu menu;
  auto tc = cursorForPosition(event->pos());
  auto data = calculateLineNumber(tc);
  bool hasBmk = hasBookmark(data.first);
  m_lineNumber = data.first;

  auto action = actionForKey(tr("Add Bookmark"), KeyEventMapper::AddBookmark);
  action->setEnabled(!hasBmk);
  connect(
    action, &QAction::triggered, this, &LNPlainTextEdit::handleAddBookmark);
  menu.addAction(action);

  action = actionForKey(tr("Edit Bookmark"), KeyEventMapper::EditBookmark);
  action->setEnabled(hasBmk);
  connect(
    action, &QAction::triggered, this, &LNPlainTextEdit::handleEditBookmark);
  menu.addAction(action);

  action = actionForKey(tr("Remove Bookmark"), KeyEventMapper::RemoveBookmark);
  action->setEnabled(hasBmk);
  connect(
    action, &QAction::triggered, this, &LNPlainTextEdit::handleRemoveBookmark);
  menu.addAction(action);

  menu.addSeparator();

  auto submenu = new QMenu(tr("Goto Bookmark"), this);
  for (auto [key, value] : asKeyValueRange(m_bookmarks)) {
    action = submenu->addAction(tr("Line %1").arg(key));
    action->setData(key);
    connect(action,
            &QAction::triggered,
            this,
            qOverload<bool>(&LNPlainTextEdit::goToBookmark));
  }
  menu.addMenu(submenu);

  menu.addSeparator();

  action = actionForKey(tr("Clear bookmarks"), KeyEventMapper::ClearBookmarks);
  action->setEnabled(!m_bookmarks.isEmpty());
  connect(action, &QAction::triggered, this, &LNPlainTextEdit::clearBookmarks);
  menu.addAction(action);

  menu.exec(event->globalPos());
}

void
LNPlainTextEdit::resizeEvent(QResizeEvent* event)
{
  QPlainTextEdit::resizeEvent(event);

  auto cr = contentsRect();
  auto left = cr.left();
  if (m_bookmarkArea) {
    m_bookmarkArea->setGeometry(
      QRect(left, cr.top(), m_bookmarkArea->width(), cr.height()));
    left += m_bookmarkArea->width();
  }
  if (m_lineNumberArea) {
    m_lineNumberArea->setGeometry(
      QRect(left, cr.top(), lineNumberAreaWidth(), cr.height()));
    left += m_lineNumberArea->width();
  }
  if (m_foldArea) {
    m_foldArea->setGeometry(
      QRect(left, cr.top(), m_foldArea->width(), cr.height()));
  }
}

bool
LNPlainTextEdit::highlightLine() const
{
  return m_highlightLine;
}

void
LNPlainTextEdit::setHighlightLine(bool highlightLine)
{
  m_highlightLine = highlightLine;
}

const QColor&
LNPlainTextEdit::lnAreaTextColor() const
{
  return m_lnAreaTextColor;
}

void
LNPlainTextEdit::setLNAreaTextColor(const QColor& foreColor)
{
  m_lnAreaTextColor = foreColor;
}

const QColor&
LNPlainTextEdit::lineNumberBackColor() const
{
  return m_lnAreaBackColor;
}

const QColor&
LNPlainTextEdit::lnAreaSelectedTextColor() const
{
  return m_lnAreaSelectedTextColor;
}

void
LNPlainTextEdit::setLNAreaSelectedTextColor(const QColor& textColor)
{
  m_lnAreaSelectedTextColor = textColor;
}

const QColor&
LNPlainTextEdit::lnAreaSelectedBackColor() const
{
  return m_lnAreaSelectedBackColor;
}

void
LNPlainTextEdit::setLNAreaSelectedBackColor(const QColor& backColor)
{
  m_lnAreaSelectedBackColor = backColor;
}

void
LNPlainTextEdit::setLineNumberBackColor(const QColor& backColor)
{
  m_lnAreaBackColor = backColor;
}

void
LNPlainTextEdit::updateViewableAreaWidth(int /*blockCount*/)
{
  setViewportMargins(m_bookmarkArea->width() + lineNumberAreaWidth() +
                       m_foldArea->width(),
                     0,
                     0,
                     0);
}

void
LNPlainTextEdit::highlightCurrentLine()
{
  if (m_highlightLine) {
    QVector<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
      QTextEdit::ExtraSelection selection;

      selection.format.setBackground(m_lnAreaSelectedBackColor);
      selection.format.setProperty(QTextFormat::FullWidthSelection, true);
      selection.cursor = textCursor();
      selection.cursor.clearSelection();
      extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
  }
}

void
LNPlainTextEdit::updateBookmarkArea(const QRect& rect, int dy)
{
  if (dy) {
    m_bookmarkArea->scroll(0, dy);
  } else {
    m_bookmarkArea->update(0, rect.y(), m_bookmarkArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect())) {
    updateViewableAreaWidth(0);
  }
}

void
LNPlainTextEdit::updateFoldArea(const QRect& rect, int dy)
{
  if (dy) {
    m_foldArea->scroll(0, dy);
  } else {
    m_foldArea->update(0, rect.y(), m_foldArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect())) {
    updateViewableAreaWidth(0);
  }
}

void
LNPlainTextEdit::updateLineNumberArea(const QRect& rect, int dy)
{
  if (dy) {
    m_lineNumberArea->scroll(0, dy);
  } else {
    m_lineNumberArea->update(
      rect.x(), rect.y(), m_lineNumberArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect())) {
    updateViewableAreaWidth(0);
  }
}

/*
   Calculates the current text column.
*/
int LNPlainTextEdit::calculateColumn(/*QTextCursor textCursor*/)
{
  QTextCursor currentCursor = textCursor();
  QTextCursor cursor(currentCursor);
  cursor.movePosition(QTextCursor::StartOfLine);
  return currentCursor.anchor() - cursor.anchor();
}

QPair<int, int>
LNPlainTextEdit::calculateLineNumber(QTextCursor textCursor)
{
  QTextCursor cursor(textCursor);
  cursor.movePosition(QTextCursor::StartOfLine);

  int lines = 1;

  while (cursor.positionInBlock() > 0) {
    cursor.movePosition(QTextCursor::Up);
    lines++;
  }

  QTextBlock block = cursor.block().previous();

  while (block.isValid()) {
    lines += block.lineCount();
    block = block.previous();
  }

  int count = lines;

  block = cursor.block().next();

  while (block.isValid()) {
    count += block.lineCount();
    block = block.next();
  }

  return qMakePair(lines, count);
}

void
LNPlainTextEdit::goToLine(int lineNumber)
{
  m_lineNumber = lineNumber;
  gotoLineNumber();
}

void
LNPlainTextEdit::goToBookmark(bool /*triggered*/)
{
  auto a = qobject_cast<QAction*>(sender());
  if (a) {
    bool ok;
    auto lineNumber = a->data().toInt(&ok);
    if (ok) {
      goToLine(lineNumber);
    }
  }
}

void
LNPlainTextEdit::goToBookmark(int bookmark)
{
  if (hasBookmarks()) {
    auto data = this->bookmark(bookmark);
    if (data.isValid()) {
      goToLine(bookmark);
    }
  }
}

bool
LNPlainTextEdit::goToPreviousBookmark()
{
  auto lineNumberData = calculateLineNumber(textCursor());
  m_lineNumber = lineNumberData.first;
  m_lineCount = lineNumberData.second;
  int i;

  if (hasBookmarks()) {
    for (i = m_lineNumber - 1; i >= 0; i--) {
      auto data = bookmark(i);
      if (data.isValid()) {
        goToLine(i);
        return true;
      }
    }

    for (i = m_lineCount - 1; i >= m_lineNumber + 1; i--) {
      auto data = bookmark(i);
      if (data.isValid()) {
        goToLine(i);
        return true;
      }
    }
  }
  return false;
}

bool
LNPlainTextEdit::goToNextBookmark()
{
  auto lineNumberData = calculateLineNumber(textCursor());
  m_lineNumber = lineNumberData.first;
  m_lineCount = lineNumberData.second;
  int i;

  if (hasBookmarks()) {
    for (i = m_lineNumber + 1; i < m_lineCount; i++) {
      auto data = bookmark(i);
      if (data.isValid()) {
        goToLine(i);
        return true;
      }
    }

    for (i = 0; i < m_lineNumber; i++) {
      auto data = bookmark(i);
      if (data.isValid()) {
        goToLine(i);
        return true;
      }
    }
  }
  return false;
}

void
LNPlainTextEdit::handleAddBookmark(bool)
{
  if (!m_bookmarks.contains(m_lineNumber)) {
    insertBookmark(m_lineNumber);
    update();
  }
}

void
LNPlainTextEdit::handleRemoveBookmark(bool)
{
  if (m_bookmarks.contains(m_lineNumber)) {
    removeBookmark(m_lineNumber);
    update();
  }
}

void
LNPlainTextEdit::handleEditBookmark(bool)
{
  if (m_bookmarks.contains(m_lineNumber)) {
    editBookmark(m_lineNumber);
    update();
  }
}

void
LNPlainTextEdit::handleClearBookmarks(bool)
{
  clearBookmarks();
  update();
}

LNPlainTextEdit::BookmarkData
LNPlainTextEdit::bookmark(int number)
{
  if (hasBookmark(number))
    return m_bookmarks.value(number);
  return BookmarkData();
}

void
LNPlainTextEdit::bookmarkHoverFinished()
{
  if (m_hoverWidget) {
    m_hoverWidget->deleteLater();
    m_hoverWidget = nullptr;
  }
}

void
LNPlainTextEdit::setBookmarks(QMap<int, BookmarkData>* bookmarks)
{
  if (!m_bookmarks.isEmpty()) {
    m_bookmarks.clear();
  }

  auto keys = bookmarks->keys();
  for (auto& key : keys) {
    if (key <= blockCount()) {
      m_bookmarks.insert(key, bookmarks->value(key));
    }
  }
  update();
}

void
LNPlainTextEdit::insertBookmark(int bookmark, const QString& text)
{
  if (m_bookmarks.contains(bookmark) && hasBookmarkText(bookmark)) {
    m_oldBookmarks.insert(bookmark, bookmarkText(bookmark));

  } else {
    if (bookmark > 0 && bookmark <= blockCount()) {
      BookmarkData data(text);
      data.text = text;
      data.pixmap = int(Bookmark);
      m_bookmarks.insert(bookmark, data);
    }
  }

  update();
  saveBookmarks();
}

void
LNPlainTextEdit::toggleBookmark(int bookmark)
{
  if (m_bookmarks.contains(bookmark)) {
    removeBookmark(bookmark);

  } else {
    if (m_oldBookmarks.contains(bookmark)) {
      insertBookmark(bookmark, m_oldBookmarks.value(bookmark));

    } else {
      insertBookmark(bookmark);
    }
  }
  saveBookmarks();
}

void
LNPlainTextEdit::removeBookmark(int bookmark)
{
  if (m_bookmarks.contains(bookmark)) {
    m_oldBookmarks.insert(bookmark, m_bookmarks.value(bookmark).text);
    m_bookmarks.remove(bookmark);
    saveBookmarks();
    update();
  }
}

void
LNPlainTextEdit::editBookmark(int lineNumber)
{
  auto lineCount = blockCount();
  if (lineNumber > 0 && lineNumber < lineCount) {
    QString text = bookmarkText(lineNumber);

    BookmarkEditDialog dlg(this);
    dlg.setText(text);
    dlg.setLineNumber(lineNumber);
    auto result = dlg.exec();

    if (result == QDialog::Accepted) {
      auto ln = dlg.lineNumber();
      removeBookmark(lineNumber);
      insertBookmark(ln, dlg.text());
    }
    saveBookmarks();
  }
}

void
LNPlainTextEdit::clearBookmarks()
{
  m_bookmarks.clear();
  update();
  saveBookmarks();
}

bool
LNPlainTextEdit::hasBookmark(int bookmark)
{
  return m_bookmarks.contains(bookmark);
}

bool
LNPlainTextEdit::hasBookmarks()
{
  return !m_bookmarks.isEmpty();
}

bool
LNPlainTextEdit::hasBookmarkText(int bookmark)
{
  auto bm = m_bookmarks.value(bookmark);
  if (bm.isValid())
    return !bm.text.isEmpty();
  return false;
}

QString
LNPlainTextEdit::bookmarkText(int bookmark)
{
  QString text = m_bookmarks.value(bookmark).text;
  text = (text.isEmpty() ? tr("Bookmark") : text);
  return text;
}

int
LNPlainTextEdit::bookmarkCount()
{
  return m_bookmarks.size();
}

void
LNPlainTextEdit::loadBookmarks()
{
  auto filename = Paths::join(
    QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation),
    "bookmarks.yaml");
  QFile file(filename);

  if (file.open(QFile::ReadOnly)) {
    auto bookmarks = YAML::LoadFile(filename);

    if (bookmarks && bookmarks.IsMap()) {
      for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
        BookmarkData bookmark;
        auto id = it->first.as<int>();
        auto value = it->second;
        if (value.IsMap()) {
          auto text = value["text"].as<QString>();
          auto rect = value["rect"].as<QRect>();
          auto pixmap = value["pixmap"].as<int>();
          bookmark.text = text;
          bookmark.rect = rect;
          bookmark.pixmap = pixmap;
          bookmark.valid = true;
        }
        m_bookmarks.insert(id, bookmark);
      }
    }
  }
}

void
LNPlainTextEdit::saveBookmarks()
{
  auto filename = Paths::join(
    QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation),
    "bookmarks.yaml");
  QFile file(filename);
  if (file.open((QFile::WriteOnly | QFile::Truncate))) {
    YAML::Emitter emitter;

    emitter << YAML::Comment(
      tr("Bookmarks storage.\n\n"
         "Care should be taken editing this file manually\n"
         "as the wrong key-value pair could cause problems.\n"));

    emitter << YAML::Anchor("bookmarks");
    emitter << YAML::BeginMap;
    for (auto [key, value] : asKeyValueRange(m_bookmarks)) {
      emitter << YAML::Key << key << YAML::Value;
      emitter << YAML::BeginMap;
      emitter << YAML::Key << "text" << YAML::Value << value.text;
      emitter << YAML::Key << "rect" << YAML::Value << value.rect;
      emitter << YAML::Key << "pixmap" << YAML::Value << value.pixmap;
      emitter << YAML::EndMap;
    }
    emitter << YAML::EndMap;

    emitter << YAML::EndSeq;

    QTextStream out(&file);
    out << emitter.c_str();
    file.close();
  }
}

void
LNPlainTextEdit::mousePressEvent(QMouseEvent* event)
{
  QPlainTextEdit::mousePressEvent(event);
  if (event->button() == Qt::LeftButton) {
    auto data = calculateLineNumber(textCursor());
    m_lineNumber = data.first;
    m_lineCount = data.second;
    gotoLineNumber();
  }
}

// void
// LNPlainTextEdit::mouseMoveEvent(QMouseEvent* event)
//{
//   auto bookmarkRect = m_bookmarkArea->rect();
//   auto pos = event->position().toPoint();
//   if (bookmarkRect.contains(pos)) {
//     m_bookmarkArea->mouseMoveEvent(event);
//     return;
//   }
//   QPlainTextEdit::mouseMoveEvent(event);
// }

bool
LNPlainTextEdit::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress) {
    auto keyEvent = static_cast<QKeyEvent*>(event);
    auto key = keyEvent->key();
    auto modifiers = keyEvent->modifiers();

    auto mapper = m_keyMap.value(KeyEventMapper::PreviousBookmark);
    if (mapper.isValid() && key == mapper.key &&
        modifiers == mapper.modifiers) {
      goToPreviousBookmark();
      return true;
    }

    mapper = m_keyMap.value(KeyEventMapper::NextBookmark);
    if (mapper.isValid() && key == mapper.key &&
        modifiers == mapper.modifiers) {
      goToNextBookmark();
      return true;
    }

    if (key == Qt::Key_Up && m_lineNumber > 0) {
      m_lineNumber--;
    }

    if (key == Qt::Key_Down && m_lineNumber < m_lineCount - 1) {
      m_lineNumber++;
    }
  }
  return QPlainTextEdit::eventFilter(obj, event);
}

bool
LNPlainTextEdit::event(QEvent* event)
{
  switch (event->type()) {
      //    case QEvent::HoverEnter:
      //      hoverEnter(static_cast<QHoverEvent*>(event));
      //      return true;
    case QEvent::HoverLeave:
      hoverLeave(static_cast<QHoverEvent*>(event));
      return true;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(event));
      return true;
    default:
      break;
  }
  return QPlainTextEdit::event(event);
}

// void
// LNPlainTextEdit::hoverEnter(QHoverEvent* event)
//{
// }

void
LNPlainTextEdit::hoverLeave(QHoverEvent* /*event*/)
{
  // should already have been deleted but to be safe
  if (m_hoverWidget) {
    m_hoverWidget->deleteLater();
    m_hoverWidget = nullptr;
  }
}

void
LNPlainTextEdit::hoverMove(QHoverEvent* event)
{
  // TODO
  auto bookmarkRect = m_bookmarkArea->rect();
  auto pos = event->position().toPoint();
  if (bookmarkRect.contains(pos)) {
    auto data = calculateLineNumber(cursorForPosition(pos));
    auto lineNumber = data.first;
    if (m_bookmarkArea->hasHover(lineNumber)) {
      if (!m_hoverWidget) {
        m_hoverWidget = new HoverWidget(this);
        connect(m_hoverWidget,
                &HoverWidget::finished,
                this,
                &LNPlainTextEdit::bookmarkHoverFinished);
      }
      if (!m_hoverWidget->isVisible()) {
        m_hoverWidget->setText(m_bookmarkArea->hoverText(lineNumber));
        m_hoverWidget->setTitle(m_bookmarkArea->hoverTitle(lineNumber));
        auto size = m_hoverWidget->sizeHint();
        m_hoverWidget->setGeometry(
          m_bookmarkArea->x(), pos.y(), 500, 200);
        m_hoverWidget->show(10000);
      }
    } else {
      // TODO not certain what needs to go here
    }
  }
}

void
LNPlainTextEdit::gotoLineNumber()
{
  auto oldColumn = calculateColumn();

  QTextCursor cursorStart(document());
  cursorStart.movePosition(QTextCursor::Start);
  cursorStart.movePosition(
    QTextCursor::Down, QTextCursor::MoveAnchor, m_lineNumber - 1);
  auto start = cursorStart.position();

  QTextCursor cursorEnd(cursorStart);
  cursorEnd.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
  auto end = cursorEnd.position();

  auto length = end - start;
  if (length > oldColumn) {
    cursorStart.movePosition(
      QTextCursor::Right, QTextCursor::MoveAnchor, oldColumn);
    setTextCursor(cursorStart);
    auto data = calculateLineNumber(cursorStart);
    emit locationChanged(data.first, data.second, oldColumn);
  } else {
    setTextCursor(cursorEnd);
    auto data = calculateLineNumber(cursorEnd);
    emit locationChanged(data.first, data.second, length);
  }

  setFocus(Qt::OtherFocusReason);

  //  emit locationChanged(data.first, data.second, offset);
}

void
LNPlainTextEdit::setKeyMap(const QMap<KeyEventMapper, KeyMapper>& keyMap)
{
  m_keyMap = keyMap;
}

QAction*
LNPlainTextEdit::actionForKey(const QString& text, KeyEventMapper mapper)
{
  auto action = new QAction(text, this);
  if (m_keyMap.contains(mapper)) {
    auto k = m_keyMap.value(mapper).keySequence();
    action->setShortcut(k);
  }
  return action;
}

void
LNPlainTextEdit::showNewline()
{
  m_showNewline = true;
  auto text = toPlainText();
  QList<int> positions;
  for (int i = 0; i < text.length(); i++) {
    auto c = text.at(i);
    if (c == Characters::NEWLINE) {
      positions.append(i);
    }
  }
  QList<int>::const_reverse_iterator it;
  it = positions.crbegin();
  while (it != positions.crend()) {
    text.insert(*it, Characters::NEWLINE_LEFT_SYMBOL);
    ++it;
  }
  setPlainText(text);
}

void
LNPlainTextEdit::clearNewline()
{
  m_showNewline = false;
  auto text = toPlainText();
  text.replace(Characters::NEWLINE_LEFT_SYMBOL, "");
  setPlainText(text);
}

void
LNPlainTextEdit::showTabs()
{
  m_showTabs = true;
  auto text = toPlainText();
  QList<int> positions;
  for (int i = 0; i < text.length(); i++) {
    auto c = text.at(i);
    if (c == Characters::TAB) {
      positions.append(i);
    }
  }
  QList<int>::const_reverse_iterator it;
  it = positions.crbegin();
  while (it != positions.crend()) {
    text.insert(*it, Characters::HORIZONTAL_TAB_SYMBOL);
    ++it;
  }
  setPlainText(text);
}

void
LNPlainTextEdit::clearTabs()
{
  m_showNewline = false;
  auto text = toPlainText();
  text.replace(Characters::HORIZONTAL_TAB_SYMBOL, "");
  setPlainText(text);
}

void
LNPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
  auto menu = createStandardContextMenu();
  if (m_bookmarkArea->isEnabled()) {
    menu->addSeparator();
    auto action = actionForKey(tr("Goto Previous Bookmark"),
                               KeyEventMapper::PreviousBookmark);
    connect(action,
            &QAction::triggered,
            this,
            &LNPlainTextEdit::goToPreviousBookmark);
    menu->addAction(action);

    action =
      actionForKey(tr("Goto Next Bookmark"), KeyEventMapper::NextBookmark);
    connect(
      action, &QAction::triggered, this, &LNPlainTextEdit::goToNextBookmark);
    menu->addAction(action);

    auto submenu = new QMenu(tr("Goto Bookmark"), this);
    for (auto [key, value] : asKeyValueRange(m_bookmarks)) {
      auto action = submenu->addAction(tr("Line %1").arg(key));
      action->setData(key);
      connect(action,
              &QAction::triggered,
              this,
              qOverload<bool>(&LNPlainTextEdit::goToBookmark));
    }
    menu->addMenu(submenu);
  }
  menu->exec(event->globalPos());
  menu->deleteLater();
}

bool
LNPlainTextEdit::isShowNewline()
{
  return m_showNewline;
}

void
LNPlainTextEdit::setShowNewline(bool showNewline)
{
  m_showNewline = showNewline;
  if (m_showNewline) {

  } else {
  }
}

bool
LNPlainTextEdit::isShowTabs()
{
  return m_showTabs;
}

void
LNPlainTextEdit::setShowTabs(bool showTabs)
{
  m_showTabs = showTabs;
}

void
LNPlainTextEdit::setFoldStyle(Style foldStyle)
{
  m_foldStyle = foldStyle;
}

void
LNPlainTextEdit::enableBookmarks(bool enable)
{
  m_bookmarkAreaEnabled = enable;
  if (enable) {
    if (!m_bookmarkArea) {
      m_bookmarkArea = new BookmarkArea(this);
      loadBookmarks();
    }
    m_bookmarkArea->setEnabled(enable);
  } else {
    saveBookmarks();
    m_bookmarkArea->deleteLater();
    m_bookmarkArea = nullptr;
  }
  repaint();
}

bool
LNPlainTextEdit::isBookmarksEnabled()
{
  return m_bookmarkAreaEnabled;
}

void
LNPlainTextEdit::enableFolds(bool enable)
{
  m_foldAreaEnabled = enable;
  if (enable) {
    if (!m_foldArea) {
      m_foldArea = new FoldArea(this);
    }
    m_foldArea->setEnabled(enable);
  } else {
    m_foldArea->deleteLater();
    m_foldArea = nullptr;
  }
  repaint();
}

bool
LNPlainTextEdit::isFoldsEnabled()
{
  return m_foldAreaEnabled;
}

void
LNPlainTextEdit::enableLineNumbers(bool enable)
{
  m_lineNumberAreaEnabled = enable;
  if (enable) {
    if (!m_lineNumberArea) {
      m_lineNumberArea = new LineNumberArea(this);
    }
    m_lineNumberArea->setEnabled(enable);
  } else {
    m_lineNumberArea->deleteLater();
    m_lineNumberArea = nullptr;
  }
  repaint();
}

bool
LNPlainTextEdit::isLineNumbersEnabled()
{
  return m_lineNumberAreaEnabled;
}

//====================================================================
//=== LNPlainTextEdit::CommonArea
//====================================================================

LNPlainTextEdit::CommonArea::CommonArea(LNPlainTextEdit* editor)
  : QWidget(editor)
  , m_editor(editor)
{
  setMouseTracking(true);
}

void
LNPlainTextEdit::CommonArea::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    auto pos = event->position().toPoint();
    auto cursor = m_editor->cursorForPosition(pos);
    auto data = m_editor->calculateLineNumber(cursor);
    m_editor->goToLine(data.first);
  } else
    QWidget::mousePressEvent(event);
}

bool
LNPlainTextEdit::CommonArea::isEnabled() const
{
  return m_enabled;
}

void
LNPlainTextEdit::CommonArea::setEnabled(bool enabled)
{
  m_enabled = enabled;
}

void
LNPlainTextEdit::CommonArea::contextMenuEvent(QContextMenuEvent* event)
{
  m_editor->handleBookmarkContextMenu(event);
}

//====================================================================
//=== LNPlainTextEdit::LineNumberArea
//====================================================================
LNPlainTextEdit::LineNumberArea::LineNumberArea(LNPlainTextEdit* editor)
  : CommonArea(editor)
{
}

QSize
LNPlainTextEdit::LineNumberArea::sizeHint() const
{
  if (m_enabled)
    return QSize(m_editor->lineNumberAreaWidth(), 0);
  else
    return QSize(0, 0);
}

void
LNPlainTextEdit::LineNumberArea::paintEvent(QPaintEvent* event)
{
  m_editor->paintLNArea(event);
}

//====================================================================
//=== LNPlainTextEdit::FoldArea
//====================================================================
LNPlainTextEdit::FoldArea::FoldArea(LNPlainTextEdit* editor)
  : CommonArea(editor)
  , m_width(WIDTH)
{
  setMouseTracking(true);

  // TODO set default to off
}

void
LNPlainTextEdit::FoldArea::paintEvent(QPaintEvent* event)
{
  if (m_enabled) {
    m_rect.setLeft(0);
    m_rect.setRight(0 + event->rect().width());
    m_rect.setTop(event->rect().top());
    m_rect.setBottom(event->rect().bottom());

    m_editor->paintFoldArea(event);
  }
}

// void
// LNPlainTextEdit::FoldArea::load()
//{
//   auto filename = Paths::join(
//     QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation),
//     "folds.yaml");
//   QFile file(filename);

//  if (file.open(QFile::ReadOnly)) {
//    auto node = YAML::LoadFile(filename);

//    if (node) {
//      if (node.IsSequence()) {
//        auto folds = node[0];
//        for (auto it = folds.begin(); it != folds.end(); ++it) {
//          auto id = it->first.as<int>();
//          auto folded = it->second.as<bool>();
//          m_folds.insert(id, qMakePair(folded, QRect()));
//        }
//      }
//    }
//  }
//}

// void
// LNPlainTextEdit::FoldArea::save()
//{
//   auto filename = Paths::join(
//     QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation),
//     "folds.yaml");
//   QFile file(filename);
//   if (file.open((QFile::WriteOnly | QFile::Truncate))) {
//     YAML::Emitter emitter;

//    emitter << YAML::Comment(
//      tr("Custom pixmaps storage.\n\n"
//         "Don't edit this manually as it is an automatic\n"
//         "load/unload file.\n"
//         "Load pixmaps using LNPlainTextEdit::addCustomPixmap()"));

//    emitter << YAML::BeginSeq;
//    if (m_customPixmap > -1) {
//      emitter << YAML::Anchor("folds");
//      emitter << YAML::BeginMap;
//      for (auto [key, value] : asKeyValueRange(m_folds)) {
//        emitter << YAML::Key << key << YAML::Value << value.first;
//      }
//      emitter << YAML::EndMap;
//    }
//    emitter << YAML::EndSeq;

//    QTextStream out(&file);
//    out << emitter.c_str();
//    file.close();
//  }
//}

const QList<int>
LNPlainTextEdit::FoldArea::folds()
{
  return m_folds.keys();
}

void
LNPlainTextEdit::FoldArea::setFolds(QList<int> folds)
{
  for (auto fold : folds) {
    m_folds.insert(fold, qMakePair(false, QRect()));
    repaint();
  }
}

bool
LNPlainTextEdit::FoldArea::setFolds(QList<int> folds, QList<bool> folded)
{
  if (folds.size() != folded.size())
    return false;
  for (int i = 0; i < folds.size(); i++) {
    m_folds.insert(folds.at(i), qMakePair(folded.at(i), QRect()));
  }
  repaint();
  return true;
}

bool
LNPlainTextEdit::FoldArea::setFolds(QList<int> folds,
                                    QList<bool> folded,
                                    QList<QRect> rects)
{
  if (folds.size() != folded.size() != rects.size())
    return false;
  for (int i = 0; i < folds.size(); i++) {
    m_folds.insert(folds.at(i), qMakePair(folded.at(i), rects.at(i)));
  }
  repaint();
  return true;
}

void
LNPlainTextEdit::FoldArea::insertFold(int lineNumber)
{
  if (!m_folds.contains(lineNumber)) {
    m_folds.insert(lineNumber, qMakePair(false, QRect()));
    repaint();
  }
}

void
LNPlainTextEdit::FoldArea::insertFold(int lineNumber, bool folded)
{
  if (!m_folds.contains(lineNumber)) {
    m_folds.insert(lineNumber, qMakePair(folded, QRect()));
    repaint();
  }
}

void
LNPlainTextEdit::FoldArea::insertFold(int lineNumber, bool folded, QRect rect)
{
  if (!m_folds.contains(lineNumber)) {
    m_folds.insert(lineNumber, qMakePair(folded, rect));
    repaint();
  }
}

void
LNPlainTextEdit::FoldArea::clearFolds()
{
  m_folds.clear();
  repaint();
}

bool
LNPlainTextEdit::FoldArea::hasFold(int lineNumber)
{
  return m_folds.contains(lineNumber);
}

bool
LNPlainTextEdit::FoldArea::isFolded(int lineNumber)
{
  auto value = m_folds.value(lineNumber);
  // TODO value should be (false, QRect()) if no fold. Check this.
  return value.first;
}

QRect
LNPlainTextEdit::FoldArea::rect(int lineNumber)
{
  return m_folds.value(lineNumber).second;
}

void
LNPlainTextEdit::FoldArea::set(int lineNumber, bool folded)
{
  if (hasFold(lineNumber)) {
    auto pair = m_folds.value(lineNumber);
    m_folds.insert(lineNumber, qMakePair(folded, pair.second));
  } else {
    m_folds.insert(lineNumber, qMakePair(folded, QRect()));
  }
  repaint();
}

void
LNPlainTextEdit::FoldArea::set(int lineNumber, QRect rect)
{
  auto pair = m_folds.value(lineNumber);
  m_folds.insert(lineNumber, qMakePair(pair.first, rect));
}

void
LNPlainTextEdit::FoldArea::set(int lineNumber, bool folded, QRect rect)
{
  m_folds.insert(lineNumber, qMakePair(folded, rect));
  repaint();
}

int
LNPlainTextEdit::FoldArea::width() const
{
  return m_width;
}

void
LNPlainTextEdit::FoldArea::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    for (auto [lineNumber, value] : asKeyValueRange(m_folds)) {
      auto rect = value.second;
      if (rect.contains(event->position().toPoint())) {
        if (value.first) { // TODO are these the right way around?
          emit m_editor->unfold(lineNumber);
        } else {
          emit m_editor->fold(lineNumber);
        }
        set(lineNumber, !value.first, value.second);
        repaint();
      }
    }
  }
  CommonArea::mousePressEvent(event);
}

QSize
LNPlainTextEdit::FoldArea::sizeHint() const
{
  if (!m_enabled) {
    return QSize(0, 0);
  }
  return QSize(WIDTH, 0);
}

//====================================================================
//=== LNPlainTextEdit::BookmarkArea
//====================================================================
LNPlainTextEdit::BookmarkArea::BookmarkArea(LNPlainTextEdit* editor)
  : CommonArea(editor)
  , m_width(WIDTH)
{
  setMouseTracking(true);
}

LNPlainTextEdit::BookmarkArea::~BookmarkArea()
{
  clearHovers();
}

QSize
LNPlainTextEdit::BookmarkArea::sizeHint() const
{
  if (!m_enabled) {
    return QSize(0, 0);
  }
  return QSize(WIDTH, 0);
}

int
LNPlainTextEdit::BookmarkArea::width() const
{
  if (!m_enabled) {
    return 0;
  }
  return m_width;
}

void
LNPlainTextEdit::BookmarkArea::paintEvent(QPaintEvent* event)
{
  if (m_enabled) {
    m_editor->paintBookmarkArea(event);
  }
}

void
LNPlainTextEdit::BookmarkArea::resizeEvent(QResizeEvent* event)
{
  m_rect.setSize(event->size());
}

const QRect&
LNPlainTextEdit::BookmarkArea::rect() const
{
  return m_rect;
}

void
LNPlainTextEdit::BookmarkArea::insertHover(int lineNumber,
                                           int icon,
                                           const QString& title,
                                           const QString& text)
{
  if (!m_hovers.contains(lineNumber)) {
    auto item = new HoverItem;
    item->iconId = icon;
    item->title = title;
    item->text = text;
    m_hovers.insert(lineNumber, item);
    repaint();
  }
}

bool
LNPlainTextEdit::BookmarkArea::hasHover(int lineNumber)
{
  return m_hovers.contains(lineNumber);
}

void
LNPlainTextEdit::BookmarkArea::clearHovers()
{
  qDeleteAll(m_hovers);
  m_hovers.clear();
}

QString
LNPlainTextEdit::BookmarkArea::hoverTitle(int lineNumber)
{
  if (m_hovers.contains(lineNumber)) {
    auto item = m_hovers.value(lineNumber);
    if (item)
      return item->title;
  }
  return QString();
}

QString
LNPlainTextEdit::BookmarkArea::hoverText(int lineNumber)
{
  if (m_hovers.contains(lineNumber)) {
    auto item = m_hovers.value(lineNumber);
    if (item)
      return item->text;
  }
  return QString();
}

int
LNPlainTextEdit::BookmarkArea::hoverIcon(int lineNumber)
{
  if (m_hovers.contains(lineNumber)) {
    auto item = m_hovers.value(lineNumber);
    if (item)
      return item->iconId;
  }
  return -1;
}

//====================================================================
//=== LNPlainTextEdit::BookmarkModel
//====================================================================
LNPlainTextEdit::BookmarkModel::BookmarkModel(
  QMap<int, BookmarkData*>* bookmarks)
  : QAbstractTableModel()
  , m_bookmarks(bookmarks)
{
}

int
LNPlainTextEdit::BookmarkModel::columnCount(const QModelIndex&) const
{
  return 2;
}

int
LNPlainTextEdit::BookmarkModel::rowCount(const QModelIndex&) const
{
  return m_bookmarks->size();
}

QVariant
LNPlainTextEdit::BookmarkModel::data(const QModelIndex& index, int role) const
{
  if (index.isValid() && role == Qt::DisplayRole) {
    switch (index.column()) {
      case 0: {
        auto keys = m_bookmarks->keys();
        return keys.at(index.row());
      }

      case 1: {
        auto keys = m_bookmarks->keys();
        int i = keys.at(index.row());
        return m_bookmarks->value(i)->text;
      }
    }
  }

  return QVariant();
}

Qt::ItemFlags
LNPlainTextEdit::BookmarkModel::flags(const QModelIndex& index) const
{
  if (index.isValid()) {
    return (QAbstractTableModel::flags(index));
  }

  return Qt::NoItemFlags;
}

QVariant
LNPlainTextEdit::BookmarkModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int /*role*/) const
{
  if (orientation == Qt::Horizontal) {
    if (section == 0) {
      return tr("Bookmark");

    } else {
      return tr("Message");
    }
  }

  return QVariant();
}
