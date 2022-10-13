#ifndef HTMLEDIT_H
#define HTMLEDIT_H

#include <QColorDialog>
#include <QGroupBox>
#include <QKeyEvent>
#include <QMimeData>
#include <QTableWidget>
#include <QToolTip>
#include <QPair>

#include "document/bookpointers.h"
#include "widgets/iepubeditor.h"
#include "widgets/isettingspage.h"
#include "widgets/lnplaintextedit.h"

class HtmlHighlighter;
class HtmlScanner;
class Tag;
class Attribute;
class HtmlEdit;

class HtmlEditSettings : public LNPlainTextEditSettings
{
public:
  HtmlEditSettings(QObject* parent)
    : LNPlainTextEditSettings(parent)
  {
  }
};

class HtmlEditSettingsWidget : public LNPlainTextEditSettingsWidget
{
  Q_OBJECT

  enum Colors
  {
    NoType,
    Text,
    Background,
    Selection,
    SelectionBackground,
    Parenthesis,
    CurrentLineNumberAreaText,
    CurrentLineNumberAreaBackground,
    LineNumberAreaText,
    LineNumberAreaBackground,
    QuotedString,
    ApostrophiedString,
    TagName,
    AttributeName,
    AttributeValue,
    Comment,
    SpecialChars,
    SpecialCharsBackground,
  };

public:
  HtmlEditSettingsWidget(HtmlEditSettings* settings,
                         HtmlHighlighter* highlighter,
                         HtmlEdit* parent);

  // SettingsWidget interface
  bool isModified() const override;

protected:
  void initGui(int& row);

private:
  bool m_modified = false;
  HtmlEdit* m_editor;
  HtmlHighlighter* m_highlighter;
  HtmlEdit* m_display = nullptr;
  HtmlEditSettings* m_settings;
  QMap<Colors, QColor> m_colorMap;
  QMap<Colors, QList<QTableWidgetItem*>> m_itemMap;

  void colorChanged(QTableWidgetItem* item);
  void textChanged();
  void backChanged();
  void currLNAreaBackChanged();
  void currLNAreaTextChanged();
  void lnAreaTextChanged();
  void lnAreaBackChanged();
  void specTextChanged();
  void specBackChanged();
  void resetDisplaySize(int size);
};
// Q_DECLARE_METATYPE(HtmlEditSettings::Colors);

enum HtmlButtonType
{
  NoButton = -1,
  TextType,
  CommentType,
  HtmlType,
};
Q_DECLARE_METATYPE(HtmlButtonType)

class HtmlDragData : public QMimeData
{
public:
  HtmlDragData() {}

  inline void setData(HtmlButtonType data) { m_data = data; }

  inline HtmlButtonType data() { return m_data; }

private:
  HtmlButtonType m_data;
};

class HtmlEdit
  : public LNPlainTextEdit
  , public IEPubEditor
{
  Q_OBJECT
public:
  HtmlEdit(PConfig config, HtmlEditSettings* settings, QWidget* parent);

  // IEPubEditor interface
  QTextCursor currentCursor() override;
  void setCurrentCursor(const QTextCursor& cursor) override;
  const QString href() const override;
  void loadHref(const QString& href) override;
  void setText(const QString& text) override;

  HtmlScanner* scanner() const;

  QTextDocument* document() override { return LNPlainTextEdit::document(); }

  void rehighlight();

  // ISettings interface
  bool isModified() const override;

  // ISettingsWidget interface
  SettingsWidget* settingsPage() override;
  void setSettingsPage(SettingsWidget* widget) override;

  void optionsDialog();

signals:
  void lostFocus(QWidget*);
  void gotFocus(QWidget*);
  void mouseClicked(QPoint pos);

protected:
  void focusInEvent(QFocusEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  //  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;
  void hoverEnter(QPoint pos);
  void hoverLeave();
  void hoverMove(QPoint pos);
  //! \reimplements{lNPlainTextEdit::contextMenuEvent(QContextMenuEvent*)
  void contextMenuEvent(QContextMenuEvent* event) override;

  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

  void setFontSize(int size);
  void setFontFamily(const QString& family);

private:
  QTextDocument* m_document = nullptr;
  QWidget* m_parent;
  PConfig m_config;
  QString m_href;
  HtmlScanner* m_scanner;
  HtmlHighlighter* m_highlighter;
  Tag* m_hoverTag = nullptr;
  Attribute* m_hoverAttribute = nullptr;
  HtmlEditSettings* m_settings;

  //  void cursorPositionHasChanged();
  void textHasChanged(int position, int charsRemoved, int charsAdded);
  //  void textModified(bool modified);
  void hoverCheckTags(QPoint pos, int cursorPosition);

  friend class HtmlEditSettingsWidget;
  void handleMatching(QTextCursor cursor);

  QPair<Tag*, Tag*> createTagPairAt(int position);
};

#endif // HTMLEDIT_H
