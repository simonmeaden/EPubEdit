#ifndef LANGUAGETAGBUILDERDIALOG_H
#define LANGUAGETAGBUILDERDIALOG_H

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
#include <QToolButton>

#include "config.h"
#include "languages.h"

class LanguageTagBuilderDialog;

namespace Private__ {

class FilterLabel : public QLabel
{
public:
  FilterLabel(QWidget* parent);

  QString currentTag() const;
  void setCurrentTag(const QString& tagValue);
  void clear();

  void setBadFormat(const QTextCharFormat& badFormat);
  void setGoodFormat(const QTextCharFormat& goodFormat);

protected:
  QString m_tagValue;
  QString m_initialText;
  QTextCharFormat m_badFormat;
  QTextCharFormat m_goodFormat;

  void paintEvent(QPaintEvent* event) override;
};

class FilterEdit : public QLineEdit
{
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
  FilterEdit(QStringList items,
             bool showRequired,
             LanguageTagBuilderDialog* parent);

  QComboBox* comboBox();
  QCheckBox* checkBox();
  QSortFilterProxyModel* model();

  void setEnabled(bool enable);

  void clearValues();
  void setValue(const QString& value);
  void setValues(const QStringList& values);

  bool hasCurrentText();
  QString currentText();
  bool isRequired();
  void setRequired(bool value);

  void setFilterText(const QString& text);
  void setUnavailableText(const QString& text);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QString m_filterText;
  QString m_tagValue;
  bool m_showRequired;
  FilterComboBox* m_selection;
  QCheckBox* m_required;
  LanguageTagBuilderDialog* m_parent;

  void primaryFilterChanged(const QString& text);
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

  virtual void build() {}
};

class PrivateLanguageEdit : public PrivateEdit
{
  Q_OBJECT
public:
  PrivateLanguageEdit(const QString& regex,
                      const QString& initialValue,
                      QWidget* parent);

  void up1();
  void up2();
  void down1();
  void down2();

private:
  char col1 = 'q';
  char col2 = 'a';
  char col3 = 'a';

  void build() override;
  void manualChange(const QString& value) {
    if (value=="i" || value=="x") {
      emit valueChanged(value);
    }
  }
};

class PrivateFrame : public QFrame
{
  Q_OBJECT
public:
  PrivateFrame(const QString& regex,
               const QString& initialValue,
               QWidget* parent);

  QString value() const;

signals:
  void privateValueChanged(const QString& value);

protected:
  PrivateEdit* m_edit = nullptr;

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

protected:
  QToolButton *m_up1Btn, *m_up2Btn, *m_down1Btn, *m_down2Btn;
};

//class PrivateExtLangFrame : public PrivateFrame
//{
//  Q_OBJECT
//public:
//  PrivateExtLangFrame(const QString& regex,
//                      const QString& initialValue,
//                      QWidget* parent);

//  void enableEdit(bool enable) override;

//protected:
//  QToolButton *m_up1Btn, *m_up2Btn, *m_down1Btn, *m_down2Btn;
//};

class PrivateScriptFrame : public PrivateFrame
{
  Q_OBJECT
public:
  PrivateScriptFrame(const QString& regex,
                     const QString& initialValue,
                     QWidget* parent);

  void enableEdit(bool enable) override;

protected:
  QToolButton *m_up1Btn, *m_up2Btn, *m_down1Btn, *m_down2Btn;
};

class PrivateRegionFrame : public PrivateFrame
{
  Q_OBJECT
public:
  PrivateRegionFrame(const QString& regex,
                     const QString& initialValue,
                     QWidget* parent);

  void enableEdit(bool enable) override;

protected:
  QToolButton *m_up1Btn, *m_up2Btn, *m_down1Btn, *m_down2Btn;
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
  void updateTag(BCP47Language::TagType type = BCP47Language::NO_VALUE);
  void usePreferredValue();

  QString tag();

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QFile m_configFile;
  QLabel* m_reportLbl;
  Private__::FilterLabel* m_resultLbl;
  Private__::FilterEdit* m_primaryFilterEdit;
  Private__::FilterEdit* m_extlangFilterEdit;
  Private__::FilterEdit* m_scriptFilterEdit;
  Private__::FilterEdit* m_regionFilterEdit;
  Private__::PrivateLanguageFrame* m_privateLangBox;
  Private__::PrivateScriptFrame* m_privateScriptBox;
  Private__::PrivateRegionFrame* m_privateRegionBox;
  //  Private__::PrivateLanguageFrame* m_privateLangBox;
  //  Private__::PrivateScriptFrame *m_privateScriptBox;
  QPushButton* m_usePreferredBtn;
  QString m_languageTag, m_scriptTag, m_regionTag, m_extlangTag;
  QSharedPointer<BCP47Language> m_language;

  void initGui();
  QSortFilterProxyModel* createProxyModel();
  void testTag();
  void clearTag();
  void setReport(const QString& report);
  void privateLanguageChanged(const QString& value);
  void privateScriptChanged(const QString& value);
  void privateRegionChanged(const QString& value);

  void help();
  void showPrivateLanguageBox(BCP47Language::TagType languageType);
  void hidePrivateScriptBox(BCP47Language::TagType languageType);
  void hidePrivateRegionBox(BCP47Language::TagType languageType);
};

#endif // LANGUAGETAGBUILDERDIALOG_H
