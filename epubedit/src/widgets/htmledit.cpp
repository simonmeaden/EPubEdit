#include "widgets/htmledit.h"
#include "config.h"
#include "utilities/characters.h"
#include "widgets/htmlhighlighter.h"
#include "widgets/htmlscanner.h"
#include "widgets/isettingspage.h"
#include "widgets/settingsdialog.h"

#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"
#include "utilities/x11colors.h"
//#include "widgets/colordialog.h"

//====================================================================
//=== HtmlEdit
//====================================================================
HtmlEdit::HtmlEdit(PConfig config, HtmlEditSettings* settings, QWidget* parent)
  : LNPlainTextEdit(settings, parent)
  , m_document(new QTextDocument(this))
  , m_config(config)
  , m_scanner(new HtmlScanner(document()))
  , m_highlighter(
      new HtmlHighlighter(m_scanner, this)) // TODO document() in two places
  , m_settings(settings)
{

  setDocument(m_document);
  setAcceptDrops(true);

  m_highlighter->setBackground(QColorConstants::White);
  m_highlighter->setMatchColor(QColorConstants::X11::chartreuse,
                               QColorConstants::X11::grey50);
  m_highlighter->setNameColor(QColorConstants::X11::mediumblue);
  m_highlighter->setAttrColor(QColorConstants::X11::darkkhaki);
  m_highlighter->setValueColor(QColorConstants::X11::midnightblue);
  m_highlighter->setSQuoteColor(QColorConstants::Svg::olive);
  m_highlighter->setDQuoteColor(QColorConstants::X11::darkgreen);

  setLNAreaSelectedBackColor(QColorConstants::X11::grey80);
  m_highlighter->setCommentColor(QColorConstants::X11::darkturquoise);
  m_highlighter->setErrorColor(QColorConstants::X11::orangered);

  connect(this,
          &QPlainTextEdit::cursorPositionChanged,
          m_highlighter,
          &HtmlHighlighter::cursorPosHasChanged);

  setKeyMap(m_config->keyMap());
}

QTextCursor
HtmlEdit::currentCursor()
{
  return QPlainTextEdit::textCursor();
}

void
HtmlEdit::setCurrentCursor(const QTextCursor& cursor)
{
  QPlainTextEdit::setTextCursor(cursor);
}

const QString
HtmlEdit::href() const
{
  return m_href;
}

void
HtmlEdit::loadHref(const QString& href)
{
  m_href = href;
  // TODO replace later
  auto zipfile = m_config->currentFilename();
  auto fileName = JlCompress::extractFile(zipfile, href);
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    auto text = file.readAll();
    setText(text);
  }
}

void
HtmlEdit::setText(const QString& text)
{
  disconnect(LNPlainTextEdit::document(),
             &QTextDocument::contentsChange,
             this,
             &HtmlEdit::textHasChanged);
  QPlainTextEdit::setPlainText(text);
  m_scanner->load(text);
  auto cursor = textCursor();
  auto tag = m_scanner->tagAt(cursor.position());
  m_highlighter->setCurrentTag(tag);
  m_highlighter->rehighlight();
  connect(LNPlainTextEdit::document(),
          &QTextDocument::contentsChange,
          this,
          &HtmlEdit::textHasChanged);
}

void
HtmlEdit::focusInEvent(QFocusEvent* event)
{
  if (event->gotFocus())
    emit gotFocus(this);

  QPlainTextEdit::focusInEvent(event);
}

void
HtmlEdit::focusOutEvent(QFocusEvent* event)
{
  if (event->lostFocus())
    emit lostFocus(this);

  QPlainTextEdit::focusOutEvent(event);
}

void
HtmlEdit::handleMatching(QTextCursor cursor)
{
  auto doc = document();
  auto charAt = doc->characterAt(cursor.position());
  auto preCharAt = doc->characterAt(cursor.position() - 1);
  if (charAt == Characters::HTML_OPEN_TAG ||
      charAt == Characters::OPEN_ROUND_BRACKET ||
      charAt == Characters::OPEN_SQUARE_BRACKET ||
      charAt == Characters::OPEN_CURLY_BRACKET) {
    QChar searchChar;
    if (charAt == Characters::HTML_OPEN_TAG) {
      searchChar = Characters::HTML_CLOSE_TAG;
    } else if (charAt == Characters::OPEN_ROUND_BRACKET) {
      searchChar = Characters::CLOSE_ROUND_BRACKET;
    } else if (charAt == Characters::OPEN_SQUARE_BRACKET) {
      searchChar = Characters::CLOSE_SQUARE_BRACKET;
    } else if (charAt == Characters::OPEN_CURLY_BRACKET) {
      searchChar = Characters::CLOSE_CURLY_BRACKET;
    }
    auto found = doc->find(searchChar, cursor);
    if (!found.isNull()) {
      m_highlighter->setBracketMatchStart(cursor.position());
      m_highlighter->setBracketMatchEnd(found.position() - 1);
    } else {
      m_highlighter->clearBracketMatch();
    }
  } else if (charAt == Characters::QUOTATION ||
             charAt == Characters::SINGLEQUOTE) {
    QChar searchChar;
    searchChar = charAt;
    auto found = doc->find(searchChar, cursor.position() + 1);
    if (!found.isNull()) {
      m_highlighter->setBracketMatchStart(cursor.position());
      m_highlighter->setBracketMatchEnd(found.position() - 1);
    } else {
      m_highlighter->clearBracketMatch();
    }
  } else {
    if (preCharAt == Characters::HTML_CLOSE_TAG ||
        preCharAt == Characters::CLOSE_ROUND_BRACKET ||
        preCharAt == Characters::CLOSE_SQUARE_BRACKET ||
        preCharAt == Characters::CLOSE_CURLY_BRACKET) {
      QChar searchChar;
      if (preCharAt == Characters::HTML_CLOSE_TAG) {
        searchChar = Characters::HTML_OPEN_TAG;
      } else if (preCharAt == Characters::CLOSE_ROUND_BRACKET) {
        searchChar = Characters::OPEN_ROUND_BRACKET;
      } else if (preCharAt == Characters::CLOSE_SQUARE_BRACKET) {
        searchChar = Characters::OPEN_SQUARE_BRACKET;
      } else if (preCharAt == Characters::CLOSE_CURLY_BRACKET) {
        searchChar = Characters::OPEN_CURLY_BRACKET;
      }
      auto found = doc->find(searchChar, cursor, QTextDocument::FindBackward);
      if (!found.isNull()) {
        m_highlighter->setBracketMatchStart(found.position() - 1);
        m_highlighter->setBracketMatchEnd(cursor.position() - 1);
      } else {
        m_highlighter->clearBracketMatch();
      }
    } else {
      m_highlighter->clearBracketMatch();
    }
  }
}

void
HtmlEdit::mousePressEvent(QMouseEvent* event)
{
  //  auto cursor = textCursor();
  auto cursor = cursorForPosition(event->position().toPoint());
  auto block = cursor.block();
  m_highlighter->rehighlightBlock(block);

  handleMatching(cursor);

  auto tag = m_scanner->tagAt(cursor.position());
  if (tag) {
    auto match = m_scanner->matchedTag(tag);
    if (match) {
      if (tag->start() < match->start()) {
        m_highlighter->setStartTagMatch(tag);
        m_highlighter->setEndTagMatch(match);
      } else {
        m_highlighter->setStartTagMatch(match);
        m_highlighter->setEndTagMatch(tag);
      }
    } else {
      m_highlighter->setStartTagMatch(tag);
      m_highlighter->setEndTagMatch(nullptr);
    }
  }

  LNPlainTextEdit::mousePressEvent(event);
}

void
HtmlEdit::mouseReleaseEvent(QMouseEvent* event)
{
  setFocus();
  QPlainTextEdit::mouseReleaseEvent(event);
}

bool
HtmlEdit::eventFilter(QObject* obj, QEvent* event)
{
  switch (event->type()) {
    case QEvent::HoverEnter:
      hoverEnter(static_cast<QHoverEvent*>(event)->position().toPoint());
      return true;
    case QEvent::HoverLeave:
      hoverLeave();
      return true;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(event)->position().toPoint());
      return true;
    default:
      break;
  }

  return LNPlainTextEdit::eventFilter(obj, event);
}

// void
// HtmlEdit::cursorPositionHasChanged()
//{
//   qWarning();
// }

HtmlScanner*
HtmlEdit::scanner() const
{
  return m_scanner;
}

void
HtmlEdit::rehighlight()
{
  m_scanner->load(toPlainText());
  m_highlighter->rehighlight();
}

bool
HtmlEdit::isModified() const
{
  return m_settings->isModified();
}

SettingsWidget*
HtmlEdit::settingsPage()
{
  auto widget = new HtmlEditSettingsWidget(m_settings, m_highlighter, this);
  return widget;
}

void
HtmlEdit::setSettingsPage(SettingsWidget* widget)
{
  auto settingsWidget = qobject_cast<HtmlEditSettingsWidget*>(widget);
  if (settingsWidget) {
    LNPlainTextEdit::setSettingsPage(settingsWidget);
    // TODO HtmlEdit settings.
  }
}

void
HtmlEdit::textHasChanged(int position, int charsRemoved, int charsAdded)
{
  if (position == 0 || (charsRemoved == 0 && charsAdded == 0))
    return;

  auto currentTag = m_highlighter->currentTag();
  if (currentTag) {
    auto start = currentTag->start();
    auto length = currentTag->length() + charsAdded - charsRemoved;
    auto text = toPlainText();
    if (currentTag->type == TagText) {
      auto texttag = dynamic_cast<TextTag*>(currentTag);
      auto currentTagText = text.mid(start, length);
      texttag->text = currentTagText;
    } else {
      auto modifiedTagText = text.mid(start, length);
      auto modifiedTag = m_scanner->rescanTag(modifiedTagText, 0, length);
      modifiedTag->startCursor = currentTag->startCursor;
      modifiedTag->endCursor = currentTag->endCursor;
      m_highlighter->setCurrentTag(modifiedTag);
      m_scanner->replaceTag(currentTag, modifiedTag);
      delete currentTag;
    }
    m_highlighter->rehighlight();
  }
}

void
HtmlEdit::hoverCheckTags(QPoint pos, int cursorPosition)
{
  for (auto tag : m_scanner->tagList()) {
    auto result = tag->isIn(cursorPosition);
    if (result == NotIn) {
      continue;
    } else {
      if (result == IsInAttributeName || result == IsInAttrubuteValue) {
        auto htmltag = dynamic_cast<HtmlTag*>(tag);
        if (htmltag && htmltag->hasAttributeIndex()) {
          auto attribute = htmltag->attributes.at(htmltag->attributeIndex);
          if (!attribute->hoverText.isEmpty()) {
            m_hoverTag = tag;
            m_hoverAttribute = attribute;
            QToolTip::showText(pos, attribute->hoverText, this);
          } else {
            QToolTip::hideText();
          }
        }
      } else {
        if (!tag->hoverText.isEmpty()) {
          m_hoverTag = tag;
          m_hoverAttribute = nullptr;
          QToolTip::showText(pos, tag->hoverText, this);
        } else {
          QToolTip::hideText();
        }
      }
    }
  }
}

void
HtmlEdit::hoverEnter(QPoint pos)
{
  // TODO hover
  //  auto cursor = cursorForPosition(pos);
  //  hoverCheckTags(pos, cursor.position());
}

void
HtmlEdit::hoverLeave()
{
  // TODO hover
  //  m_hoverTag = nullptr;
  //  m_hoverAttribute = nullptr;
}

void
HtmlEdit::hoverMove(QPoint pos)
{
  // TODO hover
  //  auto cursor = cursorForPosition(pos);
  //  auto position = cursor.position();
  //  if (m_hoverTag) {
  //    if (m_hoverTag->isIn(position) != NotIn) {
  //      QToolTip::showText(pos, m_hoverTag->hoverText, this);
  //    } else if (m_hoverAttribute) {
  //      if (m_hoverAttribute->isIn(position) != NotIn) {
  //        QToolTip::showText(pos, m_hoverAttribute->hoverText, this);
  //      }
  //    } else {
  //      hoverCheckTags(pos, cursor.position());
  //    }
  //  }
}

void
HtmlEdit::contextMenuEvent(QContextMenuEvent* event)
{
  auto menu = LNPlainTextEdit::createContextMenu();

  menu->addSeparator();
  auto action = actionForKey(tr("Options"), KeyEventMapper::Options);
  connect(action, &QAction::triggered, this, &HtmlEdit::optionsDialog);
  menu->addAction(action);

  menu->exec(event->globalPos());
  menu->deleteLater();
}

void
HtmlEdit::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasFormat(
        QStringLiteral("application/x-dnd_htmleditbtn")) &&
      event->source() != this->m_parent) {
    // we don't accept button drags from other htmledit's
    event->ignore();
  }
}

void
HtmlEdit::dragMoveEvent(QDragMoveEvent* event)
{
  if (event->mimeData()->hasFormat(
        QStringLiteral("application/x-dnd_htmleditbtn"))) {
    if (event->source() == this->m_parent) {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    } else {
      event->acceptProposedAction();
    }
  } else {
    event->ignore();
  }
}

QPair<Tag*, Tag*>
HtmlEdit::createTagPairAt(int position)
{
  auto openTag = new HtmlTag();
  auto openStart = QTextCursor(m_document);
  auto openEnd = QTextCursor(m_document);
  openStart.setPosition(position);
  openTag->startCursor = openStart;
  openTag->endCursor = openEnd;
  openEnd.setPosition(position + openTag->length());

  auto closeTag = new CloserTag();
  auto closeStart = QTextCursor(openTag->endCursor);
  auto closeEnd = QTextCursor(m_document);
  closeStart.setPosition(position);
  closeTag->startCursor = closeStart;
  closeTag->endCursor = closeEnd;
  closeEnd.setPosition(position + closeTag->length());

  return qMakePair(openTag, closeTag);
}

void
HtmlEdit::dropEvent(QDropEvent* event)
{
  auto mimeData = event->mimeData();
  if (mimeData->hasFormat(QStringLiteral("application/x-dnd_htmleditbtn"))) {
    auto type = mimeData->data("application/x-dnd_htmleditbtn");
    if (event->source() == this->m_parent) {
      event->setDropAction(Qt::MoveAction);
      event->accept();

      auto pos = event->position().toPoint();
      auto cursor = cursorForPosition(pos);
      auto position = cursor.position();
      auto tag = m_scanner->tagAt(position);
      if (tag) {
        if (tag->type == TagText) {
          // TODO shouldn't insert a new text tag inside a text tag ??
        } else {
          if (tag->isIn(position) == IsInTag) {
            // if inside a tag then move to the end
            position = tag->end();
          }
          auto tags = createTagPairAt(position);
          tags.first->startCursor.insertText(tags.first->toString());
          tags.second->startCursor.insertText(tags.second->toString());
          m_scanner->insertAfter(tag, tags.first);
          m_scanner->insertAfter(tags.first, tags.second);
        }
      }
    } else {
      event->acceptProposedAction();
    }
  } else {
    event->ignore();
  }
}

void
HtmlEdit::optionsDialog()
{
  auto m_settingsDlg = new SettingsDialog(this);
  m_settingsDlg->addTab(this, "Code Editor");

  if (m_settingsDlg->exec() == QDialog::Accepted) {
    for (auto i = 0; i < m_settingsDlg->count(); i++) {
      auto page = m_settingsDlg->widget(i);
      setSettingsPage(page);
    }
  }
}

void
HtmlEdit::setFontSize(int size)
{
  auto font = this->font();
  font.setPointSize(size);
  setFont(font);
}

void
HtmlEdit::setFontFamily(const QString& family)
{
  auto font = this->font();
  font.setFamily(family);
  setFont(font);
}

//====================================================================
//=== HtmlEditSettings
//====================================================================
HtmlEditSettingsWidget::HtmlEditSettingsWidget(HtmlEditSettings* settings,
                                               HtmlHighlighter* highlighter,
                                               HtmlEdit* parent)
  : LNPlainTextEditSettingsWidget(settings, parent)
  , m_editor(parent)
  , m_highlighter(highlighter)
{
  int row = 0;
  m_colorMap.insert(Text, m_highlighter->textColor());
  m_colorMap.insert(Background, m_highlighter->background());
  m_colorMap.insert(Parenthesis, m_highlighter->matchColor());
  m_colorMap.insert(CurrentLineNumberAreaText,
                    m_editor->lnAreaSelectedTextColor());
  m_colorMap.insert(CurrentLineNumberAreaBackground,
                    m_editor->lnAreaSelectedBackColor());
  m_colorMap.insert(LineNumberAreaText, m_editor->lnAreaTextColor());
  m_colorMap.insert(LineNumberAreaBackground, m_editor->lnAreaBackColor());
  m_colorMap.insert(QuotedString, m_highlighter->dQuoteColor());
  m_colorMap.insert(ApostrophiedString, m_highlighter->sQuoteColor());
  m_colorMap.insert(TagName, m_highlighter->nameColor());
  m_colorMap.insert(AttributeName, m_highlighter->attrColor());
  m_colorMap.insert(AttributeValue, m_highlighter->valueColor());
  m_colorMap.insert(Comment, m_highlighter->commentColor());
  m_colorMap.insert(SpecialChars, m_highlighter->charColor());
  m_colorMap.insert(SpecialCharsBackground,
                    m_highlighter->charBackgroundColor());
  LNPlainTextEditSettingsWidget::initGui(row);
  initGui(row);
}

bool
HtmlEditSettingsWidget::isModified() const
{
  return LNPlainTextEditSettingsWidget::isModified() || m_modified;
}

void
HtmlEditSettingsWidget::initGui(int& row)
{
  auto grp = new QGroupBox(tr("Colour scheme"), this);
  auto grpLayout = new QGridLayout;
  grp->setLayout(grpLayout);
  m_layout->addWidget(grp, row++, 0, 2, 1);

  auto settings = new HtmlEditSettings(this);
  m_display = new HtmlEdit(m_editor->m_config, settings, this);
  m_display->setReadOnly(true);
  grpLayout->addWidget(m_display, 0, 0);
  QString text =
    "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang='en-us'>\n"
    "<head>\n"
    "  <title>On Basilisk Station</title>\n"
    "</head>\n"
    "<body class=\"calibre\">\n"
    "  <!-- This is a comment -->\n"
    "  <h1 class=\"calibre8\" id=\"calibre_pb_11'>\n"
    "</body>\n"
    "</html>";
  m_display->setText(text);
  // a bit untidy but gives reasonable size at smaller
  // < 16 font sizes.
  m_display->setFixedHeight(18 * font().pointSize());
  connect(this,
          &LNPlainTextEditSettingsWidget::fontSizeChanged,
          this,
          &HtmlEditSettingsWidget::resetDisplaySize);
  connect(this,
          &LNPlainTextEditSettingsWidget::fontFamilyChanged,
          m_display,
          &HtmlEdit::setFontFamily);

  int tRow = 0;
  auto tbl = new QTableWidget(8, 2, this);
  grpLayout->addWidget(tbl, 1, 0);
  tbl->verticalHeader()->setVisible(false);
  tbl->horizontalHeader()->setVisible(false);
  tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  tbl->horizontalHeader()->setStretchLastSection(true);
  connect(tbl,
          &QTableWidget::itemClicked,
          this,
          &HtmlEditSettingsWidget::colorChanged);
  tbl->setRowHeight(tRow, fontMetrics().height());

  QList<QTableWidgetItem*> textItems;
  QList<QTableWidgetItem*> lineAreaText;
  QList<QTableWidgetItem*> lineAreaBack;
  QList<QTableWidgetItem*> specChars;
  QList<QTableWidgetItem*> specCharsBack;
  QList<QTableWidgetItem*> backgroundItems;
  QList<QTableWidgetItem*> currLineAreaText;
  QList<QTableWidgetItem*> currLineAreaBack;

  int height = 0;

  auto item = new QTableWidgetItem(tr("Text"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(Text));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(Text));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  textItems.append(item);
  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("Background"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(Text));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(Background));
  tbl->setItem(tRow, 1, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  textItems.append(item);
  backgroundItems.append(item);
  height += tbl->rowHeight(tRow++);

  //  item = new QTableWidgetItem(tr("Selection")); // TODO
  //  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  //  item->setForeground(m_highlighter->matchColor());
  //  item->setBackground(Qt::white);
  //  item->setData(Qt::UserRole,QVariant::fromValue(ColorType(int(Text),
  //  int(Background)))); tbl->setItem(tRow++, 0, item); tbl->setRowHeight(tRow,
  //  fontMetrics().height());
  //  textItems.append(item);
  //  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("Parentheses"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(Parenthesis));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(Parenthesis));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("Current line number area text"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(CurrentLineNumberAreaText));
  item->setBackground(m_colorMap.value(CurrentLineNumberAreaBackground));
  item->setData(Qt::UserRole, QVariant::fromValue(CurrentLineNumberAreaText));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  currLineAreaText.append(item);
  currLineAreaBack.append(item);

  item = new QTableWidgetItem(tr("Current line number area background"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(CurrentLineNumberAreaText));
  item->setBackground(m_colorMap.value(CurrentLineNumberAreaBackground));
  item->setData(Qt::UserRole,
                QVariant::fromValue(CurrentLineNumberAreaBackground));
  tbl->setItem(tRow, 1, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  currLineAreaText.append(item);
  currLineAreaBack.append(item);
  height += tbl->rowHeight(tRow++);

  item = new QTableWidgetItem(tr("Line number area text"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(LineNumberAreaText));
  item->setBackground(m_colorMap.value(LineNumberAreaBackground));
  item->setData(Qt::UserRole, QVariant::fromValue(LineNumberAreaText));
  tbl->setItem(tRow, 0, item);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  tbl->setRowHeight(tRow, fontMetrics().height());
  lineAreaText.append(item);
  lineAreaBack.append(item);

  item = new QTableWidgetItem(tr("Line number area background"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(LineNumberAreaText));
  item->setBackground(m_colorMap.value(LineNumberAreaBackground));
  item->setData(Qt::UserRole, QVariant::fromValue(LineNumberAreaBackground));
  tbl->setItem(tRow, 1, item);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  tbl->setRowHeight(tRow, fontMetrics().height());
  lineAreaText.append(item);
  lineAreaBack.append(item);
  height += tbl->rowHeight(tRow++);

  item = new QTableWidgetItem(tr("\"Quoted string\""));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(QuotedString));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(QuotedString));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("'Apostrophed string'"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(ApostrophiedString));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(ApostrophiedString));
  tbl->setItem(tRow, 1, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);
  height += tbl->rowHeight(tRow++);

  item = new QTableWidgetItem(tr("HTML 5 Tag name"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(TagName));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(TagName));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("HTML 5 Attribute name"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(AttributeName));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(AttributeName));
  tbl->setItem(tRow, 1, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);
  height += tbl->rowHeight(tRow++);

  item = new QTableWidgetItem(tr("HTML 5 Attribute = value"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(AttributeValue));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(AttributeValue));
  tbl->setItem(tRow, 0, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);

  item = new QTableWidgetItem(tr("HTML 5 comment"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(Comment));
  item->setBackground(m_colorMap.value(Background));
  item->setData(Qt::UserRole, QVariant::fromValue(Comment));
  tbl->setItem(tRow, 1, item);
  tbl->setRowHeight(tRow, fontMetrics().height());
  backgroundItems.append(item);
  height += tbl->rowHeight(tRow++);

  item = new QTableWidgetItem(tr("Special characters"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(SpecialChars));
  item->setBackground(m_colorMap.value(SpecialCharsBackground));
  item->setData(Qt::UserRole, QVariant::fromValue(SpecialChars));
  tbl->setItem(tRow, 0, item);
  specChars.append(item);
  specCharsBack.append(item);

  item = new QTableWidgetItem(tr("Special characters background"));
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setForeground(m_colorMap.value(SpecialChars));
  item->setBackground(m_colorMap.value(SpecialCharsBackground));
  item->setData(Qt::UserRole, QVariant::fromValue(SpecialCharsBackground));
  tbl->setItem(tRow, 1, item);
  specChars.append(item);
  specCharsBack.append(item);
  height += tbl->rowHeight(tRow++);

  tbl->setFixedHeight(height + 4);

  m_itemMap.insert(Text, textItems);
  m_itemMap.insert(Background, backgroundItems);
  m_itemMap.insert(CurrentLineNumberAreaText, currLineAreaText);
  m_itemMap.insert(CurrentLineNumberAreaBackground, currLineAreaBack);
  m_itemMap.insert(LineNumberAreaText, lineAreaText);
  m_itemMap.insert(LineNumberAreaBackground, lineAreaBack);
  m_itemMap.insert(SpecialChars, specChars);
  m_itemMap.insert(SpecialCharsBackground, specCharsBack);
}

void
HtmlEditSettingsWidget::colorChanged(QTableWidgetItem* item)
{
  auto colors = item->data(Qt::UserRole);
  auto data = colors.value<Colors>();
  auto dlg = new QColorDialog(item->foreground().color(), this);
  if (dlg->exec() == QDialog::Accepted) {
    m_colorMap.insert(data, dlg->currentColor());
    switch (data) {
      case Text:
        textChanged();
        m_display->m_highlighter->setTextColor(m_colorMap.value(Text));
        break;
      case Background:
        backChanged();
        m_display->m_highlighter->setBackground(m_colorMap.value(Background));
        break;
      case CurrentLineNumberAreaText:
        currLNAreaTextChanged();
        m_display->setLNAreaSelectedTextColor(
          m_colorMap.value(CurrentLineNumberAreaText));
        break;
      case CurrentLineNumberAreaBackground:
        currLNAreaBackChanged();
        m_display->setLNAreaSelectedBackColor(
          m_colorMap.value(CurrentLineNumberAreaBackground));
        break;
      case LineNumberAreaText:
        lnAreaTextChanged();
        m_display->setLNAreaTextColor(m_colorMap.value(LineNumberAreaText));
        break;
      case LineNumberAreaBackground:
        lnAreaBackChanged();
        m_display->setLineNumberBackColor(
          m_colorMap.value(LineNumberAreaBackground));
        break;
      case SpecialChars:
        specTextChanged();
        m_display->m_highlighter->setCharForegroundColor(
          m_colorMap.value(SpecialChars));
        break;
      case SpecialCharsBackground:
        specBackChanged();
        m_display->m_highlighter->setCharBackgroundColor(
          m_colorMap.value(SpecialCharsBackground));
        break;
      case Parenthesis:
        m_display->m_highlighter->setMatchColor(m_colorMap.value(Parenthesis));
        break;
      case QuotedString:
        m_display->m_highlighter->setDQuoteColor(
          m_colorMap.value(QuotedString));
        break;
      case ApostrophiedString:
        m_display->m_highlighter->setSQuoteColor(
          m_colorMap.value(ApostrophiedString));
        break;
      case TagName:
        m_display->m_highlighter->setNameColor(m_colorMap.value(TagName));
        break;
      case AttributeName:
        m_display->m_highlighter->setAttrColor(m_colorMap.value(AttributeName));
        break;
      case AttributeValue:
        m_display->m_highlighter->setValueColor(
          m_colorMap.value(AttributeValue));
        break;
      case Comment:
        m_display->m_highlighter->setCommentColor(m_colorMap.value(Comment));
        break;
      case Selection:
        // TODO
        // m_display->m_highlighter->setCommentColor(m_colorMap.value(Selection));
        break;
      case SelectionBackground:
        // TODO
        // m_display->m_highlighter->setCommentColor(m_colorMap.value(SelectionBackground));
        break;
      case NoType:
        break;
    }
  }
}

void
HtmlEditSettingsWidget::textChanged()
{
  for (auto item : m_itemMap.value(Text)) {
    item->setForeground(m_colorMap.value(Text));
  }
}

void
HtmlEditSettingsWidget::backChanged()
{
  for (auto item : m_itemMap.value(Background)) {
    item->setBackground(m_colorMap.value(Background));
  }
}

void
HtmlEditSettingsWidget::currLNAreaBackChanged()
{
  for (auto item : m_itemMap.value(CurrentLineNumberAreaBackground)) {
    item->setBackground(m_colorMap.value(CurrentLineNumberAreaBackground));
  }
}

void
HtmlEditSettingsWidget::currLNAreaTextChanged()
{
  for (auto item : m_itemMap.value(CurrentLineNumberAreaText)) {
    item->setForeground(m_colorMap.value(CurrentLineNumberAreaText));
  }
}

void
HtmlEditSettingsWidget::lnAreaTextChanged()
{
  for (auto item : m_itemMap.value(LineNumberAreaText)) {
    item->setForeground(m_colorMap.value(LineNumberAreaText));
  }
}

void
HtmlEditSettingsWidget::lnAreaBackChanged()
{
  for (auto item : m_itemMap.value(LineNumberAreaBackground)) {
    item->setBackground(m_colorMap.value(LineNumberAreaBackground));
  }
}

void
HtmlEditSettingsWidget::specTextChanged()
{
  for (auto item : m_itemMap.value(SpecialChars)) {
    item->setForeground(m_colorMap.value(SpecialChars));
  }
}

void
HtmlEditSettingsWidget::specBackChanged()
{
  for (auto item : m_itemMap.value(SpecialCharsBackground)) {
    item->setBackground(m_colorMap.value(SpecialCharsBackground));
  }
}

void
HtmlEditSettingsWidget::resetDisplaySize(int size)
{
  m_display->setFixedHeight(18 * size);
  m_display->setFontSize(size);
}
