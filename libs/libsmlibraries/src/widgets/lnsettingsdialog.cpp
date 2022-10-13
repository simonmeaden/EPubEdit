#include "widgets/lnsettingsdialog.h"

LNSettingsDialog::LNSettingsDialog(QWidget* parent)
{
  auto layout = new QHBoxLayout;
  setLayout(layout);
  m_tabs = new QTabWidget(this);
  layout->addWidget(m_tabs);

  m_tabs->addTab(initMainSettings(), "Line NumberSettings");
}

QWidget* LNSettingsDialog::initMainSettings()
{
  auto mainSettings = new QWidget(this);
  auto mainLayout = new QGridLayout;
  mainSettings->setLayout(mainLayout);
  return mainSettings;
}


