#include "forms/configurationeditor.h"
#include "config.h"

ConfigurationEditor::ConfigurationEditor(Config* config, QWidget* parent)
  : QDialog(parent)
  , m_config(config)
{
  initGui();
}

Config*
ConfigurationEditor::config() const
{
  return m_config;
}

QWidget*
ConfigurationEditor::initPage1()
{
  auto page = new QWidget(this);
  auto layout = new QFormLayout;
  page->setLayout(layout);

  auto preferredSaveVersion = new QComboBox(this);
  preferredSaveVersion->addItems(Config::versions());
  preferredSaveVersion->setCurrentText(m_config->versionToString());
  layout->addRow(tr("Preferred Save Version"), preferredSaveVersion);
  connect(preferredSaveVersion,
          &QComboBox::currentTextChanged,
          m_config,
          qOverload<QString>(&Config::setSaveVersion));

  auto statusTimeoutEdit = new QSpinBox(this);
  layout->addRow(tr("Status bar message timeout (secs)"), statusTimeoutEdit);
  statusTimeoutEdit->setValue(m_config->statusTimeout());
  connect(statusTimeoutEdit,
          qOverload<int>(&QSpinBox::valueChanged),
          m_config,
          &Config::setStatusTimeout);

  return page;
}

void
ConfigurationEditor::initGui()
{
  auto layout = new QGridLayout;
  setLayout(layout);

  auto tabs = new QTabWidget(this);
  layout->addWidget(tabs, 0, 0);
  tabs->addTab(initPage1(), tr("Main Values"));

  auto box =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(box, 1, 0);
  connect(box, &QDialogButtonBox::accepted, this, &ConfigurationEditor::accept);
  connect(box, &QDialogButtonBox::rejected, this, &ConfigurationEditor::reject);
}
