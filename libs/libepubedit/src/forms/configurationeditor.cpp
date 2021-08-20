#include "forms/configurationeditor.h"

ConfigurationEditor::ConfigurationEditor(Config* config, QWidget* parent)
  : QDialog(parent)
  , m_config(config)
  , m_modified(false)
{
  initGui();
}

Config*
ConfigurationEditor::config() const
{
  return m_config;
}

bool
ConfigurationEditor::modified() const
{
  return m_modified;
}

QWidget*
ConfigurationEditor::initPage1()
{
  auto page = new QWidget(this);
  auto layout = new QFormLayout;
  page->setLayout(layout);

  auto statusTimeoutEdit = new QSpinBox(this);
  layout->addRow(tr("Status bar message timeout (secs)"), statusTimeoutEdit);
  statusTimeoutEdit->setValue(m_config->statusTimeout());
  connect(statusTimeoutEdit,
          &QSpinBox::valueChanged,
          this,
          &ConfigurationEditor::statusTimeoutChanged);

  return page;
}

void
ConfigurationEditor::statusTimeoutChanged(int value)
{
  m_config->setStatusTimeout(value);
  m_modified = true;
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
