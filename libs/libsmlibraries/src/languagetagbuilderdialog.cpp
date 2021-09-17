#include "languagetagbuilderdialog.h"

LanguageTagBuilderDialog::LanguageTagBuilderDialog(QWidget* parent)
  : QDialog(parent)
  , m_languages(new BCP47Languages(this))
  , m_configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , m_configFile(
      (Paths::join(m_configDir.path(), "epubedit", "languages.yaml")))
{
  m_languages->readFromLocalFile(m_configFile);
  initGui();
}

void
LanguageTagBuilderDialog::setConfigFile(const QString& filename)
{
  m_configFile.setFileName(filename);
  m_languages->readFromLocalFile(m_configFile);
}

QSortFilterProxyModel*
LanguageTagBuilderDialog::createProxyModel()
{
  auto model = new QStringListModel();
  model->setStringList(m_languages->languageDescriptions());
  auto proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  return proxyModel;
}

void
LanguageTagBuilderDialog::initGui()
{
  auto layout = new QGridLayout;
  setLayout(layout);

  auto lbl = new QLabel(tr("Primary Language"), this);
  layout->addWidget(lbl, 0, 0);

  m_primaryFilterEdit = new FilterEdit(m_languages->languageDescriptions(), this);
  layout->addWidget(m_primaryFilterEdit, 1, 0);
  layout->addWidget(m_primaryFilterEdit->box(), 1, 1);

  m_extlangFilterEdit = new FilterEdit(m_languages->extlangDescriptions(), this);
  layout->addWidget(m_extlangFilterEdit, 1, 3);
  layout->addWidget(m_extlangFilterEdit->box(), 1, 2);

  m_scriptFilterEdit1 = new FilterEdit(m_languages->scriptDescriptions(), this);
  layout->addWidget(m_scriptFilterEdit1, 2, 3);
  layout->addWidget(m_scriptFilterEdit1->box(), 2, 2);

  m_regionFilterEdit1 = new FilterEdit(m_languages->regionDescriptions(), this);
  layout->addWidget(m_regionFilterEdit1, 3, 3);
  layout->addWidget(m_regionFilterEdit1->box(), 3, 2);

  m_scriptFilterEdit2 = new FilterEdit(m_languages->scriptDescriptions(), this);
  layout->addWidget(m_scriptFilterEdit2, 1, 5);
  layout->addWidget(m_scriptFilterEdit2->box(), 1, 4);

  m_regionFilterEdit2 = new FilterEdit(m_languages->regionDescriptions(), this);
  layout->addWidget(m_regionFilterEdit2, 2, 5);
  layout->addWidget(m_regionFilterEdit2->box(), 2, 4);

  m_regionFilterEdit3 = new FilterEdit(m_languages->regionDescriptions(), this);
  layout->addWidget(m_regionFilterEdit3, 1, 7);
  layout->addWidget(m_regionFilterEdit3->box(), 1, 6);

  resultLbl = new QLabel(this);
  layout->addWidget(resultLbl, 8, 0);

  auto filler = new QWidget(this);
  filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(filler, 10, 0);
}

void
LanguageTagBuilderDialog::valueChanged()
{
  QString result;
  auto primary = m_primaryFilterEdit->box()->currentText();
  if (primary.isEmpty()) {
    result = tr("A primary language is required by ISO639-1 and later!");
  } else {
    result = m_languages->languageFromDescription(primary)->subtag();
  }

  // TODO the rest of the edits

  resultLbl->setText(result);
}
