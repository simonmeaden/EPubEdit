#ifndef LANGUAGETAGBUILDERDIALOG_H
#define LANGUAGETAGBUILDERDIALOG_H

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
#include <QSharedPointer>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpressionValidator>

#include "languages.h"

class LanguageTagBuilderDialog : public QDialog
{
  Q_OBJECT

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

  class PrivateLanguageDialog : public QDialog
  {
    class PrivateEdit : public QLineEdit
    {
    public:
      PrivateEdit(QWidget* parent)
        : QLineEdit("qaa", parent)
      {
        QRegularExpression re("q[a-t][a-z]");
        auto validator = new QRegularExpressionValidator(re, this);
        setValidator(validator);
      }

      void up(int column)
      {
        switch (column) {
          case 1:
            if (col2 < 't')
              col2++;
            break;
          case 2:
            if (col3 < 'z')
              col3++;
            break;
        }
        build();
      }
      void down(int column)
      {
        switch (column) {
          case 1:
            if (col2 > 'a')
              col2--;
            break;
          case 2:
            if (col3 < 'a')
              col3--;
            break;
        }
        build();
      }

    signals:
      void newValue(const QString&);

    private:
      char col1 = 'q';
      char col2 = 'a';
      char col3 = 'a';

      void build()
      {
        QString value;
        value.append(col1).append(col2).append(col3);
        emit newValue(value);
      }
    };

  public:
    PrivateLanguageDialog(QWidget* parent)
      : QDialog(parent)
    {
      auto layout = new QGridLayout;
      setLayout(layout);

      auto xBox = new QRadioButton(tr("x - not preferred"), this);
      layout->addWidget(xBox,0, 0);

      auto iBox =
        new QRadioButton(tr("i - grandfathered, not preferred"), this);
      layout->addWidget(iBox,1, 0);

      auto pBox = new QRadioButton("qaa-qtz, preferred", this);
      pBox->setChecked(true);
      layout->addWidget(pBox,2, 0);

      auto valueEdit = new PrivateEdit(this);
      valueEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      layout->addWidget(valueEdit, 3, 0);

      auto upCol1Btn = new QPushButton(this);
    }
  };

public:
  explicit LanguageTagBuilderDialog(QWidget* parent = nullptr);

  void setConfigFile(const QString& filename);

  void languageChanged();
  void extlangChanged();
  void scriptChanged();
  void regionChanged();
  void updateTag(BCP47Language::TagType type = BCP47Language::NO_VALUE);
  void usePreferredValue();

  QString tag();

signals:

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QFile m_configFile;
  QLabel* m_reportLbl;
  FilterLabel* m_resultLbl;
  FilterEdit* m_primaryFilterEdit;
  FilterEdit* m_extlangFilterEdit;
  FilterEdit* m_scriptFilterEdit;
  FilterEdit* m_regionFilterEdit;
  QPushButton* m_usePreferredBtn;
  QString m_languageTag, m_scriptTag, m_regionTag, m_extlangTag;
  QSharedPointer<BCP47Language> m_language;

  void initGui();
  QSortFilterProxyModel* createProxyModel();
  void testTag();
  void clearTag();
  void setReport(const QString& report);

  void help();
};

#endif // LANGUAGETAGBUILDERDIALOG_H
