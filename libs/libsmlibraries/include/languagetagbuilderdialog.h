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
#include <QCoreApplication>
#include <QSharedPointer>

#include "languages.h"

class LanguageTagBuilderDialog : public QDialog
{
  Q_OBJECT

  class FilterLabel: public QLabel {
  public:
    FilterLabel(QWidget*parent);

    QString currentTag() const;
    void setCurrentTag(const QString &tagValue);
    void clear();

  protected:
    QString m_tagValue;
    QString m_initialText;
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

public:
  explicit LanguageTagBuilderDialog(QWidget* parent = nullptr);

  void setConfigFile(const QString& filename);

  void languageChanged();
  void extlangChanged();
  void scriptChanged();
  void regionChanged();
  void updateTag();

  QString tag();

signals:

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QFile m_configFile;
  FilterLabel* m_resultLbl;
  FilterEdit* m_primaryFilterEdit;
  FilterEdit* m_extlangFilterEdit;
  FilterEdit* m_scriptFilterEdit;
  FilterEdit* m_regionFilterEdit;
  QString m_languageTag, m_scriptTag, m_regionTag, m_extlangTag;
  QSharedPointer<BCP47Language> m_language;

  void initGui();
  QSortFilterProxyModel* createProxyModel();
  void testTag();
  void clearTag();

  void help();
};

#endif // LANGUAGETAGBUILDERDIALOG_H
