#ifndef LANGUAGETAGBUILDERDIALOG_H
#define LANGUAGETAGBUILDERDIALOG_H

#include <QAbstractTextDocumentLayout>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSharedPointer>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QToolButton>
#include <QWidget>

#include "config.h"
#include "languages.h"
#include "stringutil.h"
#include "x11colors.h"

class LanguageTagBuilderDialog;

namespace Private__ {

class FilterLabel : public QLabel
{
public:
  FilterLabel(QWidget* parent);

  QString currentTag() const;
  void setCurrentTag(const QString& tagValue);
  void clear();

protected:
  QString m_tagValue;
  QString m_initialText;
  QTextCharFormat m_badFormat;
  QTextCharFormat m_goodFormat;

  void paintEvent(QPaintEvent* event) override;
};

class FilterEdit : public QLineEdit
{
  Q_OBJECT
  class FilterComboBox : public QComboBox
  {
  public:
    FilterComboBox(QWidget* parent);

    void setUnavailableText(const QString& text) { m_unavailableText = text; }

  protected:
    QString m_unavailableText;
    void paintEvent(QPaintEvent* event) override;
  };

public:
  FilterEdit(QStringList items, LanguageTagBuilderDialog* parent);

  QComboBox* comboBox();
  QSortFilterProxyModel* model();

  void setEnabled(bool enable);

  void clearValues();
  void setValue(const QString& value);
  void setValues(const QStringList& values);

  //! True if the combo box has been selected
  bool hasCurrentSelection();
  //! A QString holding the value of the combo box if selected, otherwise an empty QString.
  QString currentSelection();
  bool isEmpty();

  void setFilterText(const QString& text);
  void setUnavailableText(const QString& text);

  bool hasIndexChanged();
  void clearIndexChanged();

signals:
  void activated(int index);
  void stateChanged(int);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QString m_filterText;
  QString m_tagValue;
  FilterComboBox* m_selectionBox;
  LanguageTagBuilderDialog* m_parent;
  int m_currentIndex = -1;
  bool m_currentIndexChanged = false;

  void primaryFilterChanged(const QString& text);
  void setIndexChanged(int index);
};

class PrivateEdit : public QLineEdit
{
  Q_OBJECT
public:
  PrivateEdit(const QString& regex,
              const QString& initialValue,
              QWidget* parent);

  QString value() const;

signals:
  void valueChanged(const QString& text);

protected:
  QString m_value;
};

class PrivateLanguageEdit : public PrivateEdit
{
  Q_OBJECT
public:
  enum Type
  {
    i,
    x,
    qaa,
  };
  PrivateLanguageEdit(const QString& regex,
                      const QString& initialValue,
                      QWidget* parent);

  void up2();
  void up3();
  void down2();
  void down3();
  void setType(Type type);

private:
  char col1 = 'q';
  char col2 = 'a';
  char col3 = 'a';
  Type m_type;
  QString m_storedValue; // used to save 'q**' value if i or x clicked

  void build();
  void manualChange(const QString& value);
};

class PrivateScriptEdit : public PrivateEdit
{
  Q_OBJECT
public:
  PrivateScriptEdit(const QString& regex,
                    const QString& initialValue,
                    QWidget* parent);

  void up3();
  void up4();
  void down3();
  void down4();

private:
  char col1 = 'Q';
  char col2 = 'a';
  char col3 = 'a';
  char col4 = 'a';

  void build();
};

class PrivateRegionEdit : public PrivateEdit
{
  Q_OBJECT
public:
  enum Type
  {
    AA,
    QM,
    XA,
    ZZ,
  };
  PrivateRegionEdit(const QString& regex,
                    const QString& initialValue,
                    QWidget* parent);

  void up();
  void down();
  void setType(Type type);

private:
  char col1;
  char col2;
  Type m_type;
  QString m_storedValueQ, m_storedValueX;

  void build();
  void manualChange(const QString& value);
};

class PrivateFrame : public QFrame
{
  Q_OBJECT
public:
  PrivateFrame(QWidget* parent);

  virtual QString value() const;
  void setValue(const QString& value);

signals:
  void privateValueChanged(const QString& value);

protected:
  PrivateEdit* m_edit = nullptr;
  QString m_value;

  virtual void enableEdit(bool enable) = 0;
};

class PrivateLanguageFrame : public PrivateFrame
{
  Q_OBJECT
public:
  PrivateLanguageFrame(const QString& regex,
                       const QString& initialValue,
                       QWidget* parent);

  void enableEdit(bool enable) override;
  void show();

  QString value() const override;

protected:
  QToolButton *m_up1Btn, *m_up2Btn, *m_down1Btn, *m_down2Btn;
  QRadioButton *m_iBtn, *m_xBtn, *m_qBtn;
  QString m_extension;

  void setValue(const QString& value);
  void setExtension(const QString& extension);
  QString buildString() const;
};

class PrivateScriptFrame : public PrivateFrame
{
  Q_OBJECT
public:
  PrivateScriptFrame(const QString& regex,
                     const QString& initialValue,
                     QWidget* parent);

  void enableEdit(bool enable) override;

protected:
  QToolButton *m_up3Btn, *m_up4Btn, *m_down3Btn, *m_down4Btn;
};

class PrivateRegionFrame : public PrivateFrame
{
  Q_OBJECT
public:
  PrivateRegionFrame(const QString& regex,
                     const QString& initialValue,
                     QWidget* parent);

  void enableEdit(bool enable) override;
  void show();

protected:
  QToolButton *m_upBtn, *m_downBtn;
  QRadioButton *m_aaBtn, *m_zzBtn, *m_qmBtn, *m_xaBtn;

  void setValue(const QString& value);
};

class LanguageLabel : public QLineEdit
{
  Q_OBJECT

  struct HoverBlock
  {
    int start = 0;
    int end = 0;
    QString hoverText;
  };

  class LanguageTextHighlighter : public QSyntaxHighlighter
  {
  public:
    LanguageTextHighlighter(QTextDocument* document);

    void highlightBlock(const QString& text) {}

  private:
  };

public:
  LanguageLabel(BCP47Languages* languages, QWidget* parent);

  void setPrimaryLanguageColor(const QColor& primaryLanguageColor);
  void setExtlangColor(const QColor& extlangColor);
  void setScriptColor(const QColor& scriptColor);
  void setRegionColor(const QColor& regionColor);
  void setVariantColor(const QColor& variantColor);
  void setBadColor(const QColor& badColor);
  void setPrivateLanguageColor(const QTextCharFormat& privateLanguageColor);
  void setTextColor(const QTextCharFormat& textColor);

  QString currentTag() const;
  void setCurrentTag(const QString& text);

protected:
  void paintEvent(QPaintEvent* event) override;
  //  void mousePressEvent(QMouseEvent* event) override;
  //  void mouseMoveEvent(QMouseEvent* event) override;
  //  void mouseReleaseEvent(QMouseEvent* event) override;
  //  void hoverEnter(QHoverEvent* event);
  //  void hoverLeave(QHoverEvent* event);
  void hoverMove(QHoverEvent* event);
  bool event(QEvent* event) override;

private:
  QString m_currentTag;
  BCP47Languages* m_languages;
  QTextDocument* m_document;
  LanguageTextHighlighter* m_highlighter;
  QTextCharFormat m_textColor;
  QTextCharFormat m_primaryLanguageColor;
  QTextCharFormat m_privateLanguageColor;
  QTextCharFormat m_extlangColor;
  QTextCharFormat m_scriptColor;
  QTextCharFormat m_regionColor;
  QTextCharFormat m_variantColor;
  QTextCharFormat m_badColor;
  QTextCharFormat m_badPositionColor;
  QList<HoverBlock*> m_hoverData;

  void parseAndHighlightTag();
  void highlightTag(
    QList<QSharedPointer<BCP47Language::TagTestResult>> results);
};

} // end of namespace Private__

class LanguageTagBuilderDialog : public QDialog
{
  Q_OBJECT

public:
  explicit LanguageTagBuilderDialog(Config* config, QWidget* parent = nullptr);

  void setConfigFile(const QString& filename);

  void languageChanged();
  void extlangChanged();
  void scriptChanged();
  void regionChanged();
  void variantChanged();
  void updateTag(BCP47Language::TagType type = BCP47Language::NO_VALUE);

  QString tag();

private:
  QGroupBox *m_primaryBox, *m_extensionBox;
  BCP47Languages* m_languages;
  QDir m_configDir;
  QString m_configFilename;
  QLabel* m_reportLbl;
  Private__::LanguageLabel* m_resultLbl;
  Private__::FilterEdit* m_primaryFilterEdit;
  Private__::FilterEdit* m_extlangFilterEdit;
  Private__::FilterEdit* m_scriptFilterEdit;
  Private__::FilterEdit* m_regionFilterEdit;
  Private__::FilterEdit* m_variantFilterEdit;
  Private__::PrivateLanguageFrame* m_privateLangFrame;
  Private__::PrivateScriptFrame* m_privateScriptFrame;
  Private__::PrivateRegionFrame* m_privateRegionFrame;
  QPushButton* m_usePreferredBtn;
//  QString m_languageTag;
  QSharedPointer<BCP47Language> m_language;

  void initGui();
  QSortFilterProxyModel* createProxyModel();
  //  void testTag();
  void clearTag();
  void setReport(const QString& report);

  void help();
  void showPrivateLanguageFrame(BCP47Language::TagType languageType);
  void showPrivateScriptFrame(BCP47Language::TagType languageType);
  void showPrivateRegionFrame(BCP47Language::TagType languageType);

//  static QString MISSING_PRIMARY_LANGUAGE;
//  void clearAllTagvalues();
  QString calcExtlangResult();
  QString calcScriptResult(BCP47Language::TagType type);
  QString calcRegionResult(BCP47Language::TagType type);
  QString calcVariantResult();
  QString calcLanguageResult(BCP47Language::TagType type);
};

#endif // LANGUAGETAGBUILDERDIALOG_H
