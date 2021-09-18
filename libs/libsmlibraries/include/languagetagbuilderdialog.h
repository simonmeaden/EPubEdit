#ifndef LANGUAGETAGBUILDERDIALOG_H
#define LANGUAGETAGBUILDERDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QCompleter>
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
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QStringListModel>
#include <QStyledItemDelegate>

#include "languages.h"
#include "paths.h"
#include "x11colors.h"

class LanguageTagBuilderDialog : public QDialog
{
  Q_OBJECT

  class HelpDialog : public QDialog
  {
  public:
    HelpDialog(QWidget* parent);

  private:
    QWidget* extension;
    QPushButton* moreBtn;

    void more();
  };

  class FilterEdit : public QLineEdit
  {
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

  protected:
    void paintEvent(QPaintEvent* event) override;

  private:
    bool m_showRequired;
    QComboBox* m_selection;
    QCheckBox* m_required;
    LanguageTagBuilderDialog* m_parent;

    void primaryFilterChanged(const QString& text);
  };

public:
  explicit LanguageTagBuilderDialog(QWidget* parent = nullptr);

  void setConfigFile(const QString& filename);

  void languageChanged();
  void extlangChanged();
  void scriptChanged();
  void regionChanged();
  void updateTag();

  QString tag() { return m_resultLbl->text(); }

signals:

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QFile m_configFile;
  QLabel* m_resultLbl;
  FilterEdit* m_primaryFilterEdit;
  FilterEdit* m_extlangFilterEdit;
  FilterEdit* m_scriptFilterEdit;
  FilterEdit* m_regionFilterEdit;
  QString m_language, m_script, m_region, m_extlang;

  void initGui();
  QSortFilterProxyModel* createProxyModel();

  void help();
};

#endif // LANGUAGETAGBUILDERDIALOG_H
