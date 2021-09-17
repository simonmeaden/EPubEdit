#ifndef LANGUAGETAGBUILDERDIALOG_H
#define LANGUAGETAGBUILDERDIALOG_H

#include <QComboBox>
#include <QCompleter>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpression>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QStringListModel>

#include "languages.h"
#include "paths.h"

class FilterEdit;
class LanguageTagBuilderDialog : public QDialog
{
  Q_OBJECT
public:
  explicit LanguageTagBuilderDialog(QWidget* parent = nullptr);

  void setConfigFile(const QString& filename);

signals:

private:
  BCP47Languages* m_languages;
  QDir m_configDir;
  QFile m_configFile;
  QLabel* resultLbl;
  FilterEdit*m_primaryFilterEdit;
  FilterEdit *m_extlangFilterEdit;
  FilterEdit *m_scriptFilterEdit1;
  FilterEdit *m_scriptFilterEdit2;
  FilterEdit *m_regionFilterEdit1;
  FilterEdit *m_regionFilterEdit2;
  FilterEdit *m_regionFilterEdit3;

  void initGui();
  QSortFilterProxyModel* createProxyModel();
//  void primaryFilterChanged(const QString& text);
  void valueChanged();

  friend class FilterEdit;
};

class FilterEdit : public QLineEdit
{
  Q_OBJECT
public:
  FilterEdit(QStringList items, LanguageTagBuilderDialog* parent)
    : QLineEdit(parent)
    , m_box(new QComboBox(parent))
    , m_items(items)
    , m_parent(parent)
  {
    connect(
      this, &QLineEdit::textChanged, this, &FilterEdit::primaryFilterChanged);
    connect(m_box,
            &QComboBox::currentTextChanged,
            m_parent,
            &LanguageTagBuilderDialog::valueChanged);
  }

  QComboBox* box() { return m_box; }
  QSortFilterProxyModel* model()
  {
    return qobject_cast<QSortFilterProxyModel*>(m_box->model());
  }

private:
  QComboBox* m_box;
  QStringList m_items;
  LanguageTagBuilderDialog* m_parent;

  void createProxyModel()
  {
    auto model = new QStringListModel();
    model->setStringList(m_items);
    auto proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  }

  void primaryFilterChanged(const QString& text)
  {
    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    model()->setFilterRegularExpression(regex);
  }

};

#endif // LANGUAGETAGBUILDERDIALOG_H
