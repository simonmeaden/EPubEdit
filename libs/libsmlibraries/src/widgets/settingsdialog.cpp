#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent)
  , m_tabs(new SettingsTabWidget(this))
{
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_tabs, 0, 0);

  auto btnBox = new QDialogButtonBox(QDialogButtonBox::Apply|QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel| QDialogButtonBox::Help);
  btnBox->button(QDialogButtonBox::Apply)->setText("Apply Changes");
  btnBox->button(QDialogButtonBox::Ok)->setText("Accept Changes");
  btnBox->button(QDialogButtonBox::Cancel)->setText("Cancel Changes");

  connect(btnBox, &QDialogButtonBox::accepted, this, &SettingsDialog::acceptChanges);
  connect(btnBox, &QDialogButtonBox::rejected, this, &SettingsDialog::cancelChanges);
  connect(btnBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &SettingsDialog::applyChanges);
  connect(btnBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, &SettingsDialog::help);

  layout->addWidget(btnBox, 1, 0);

}

int
SettingsDialog::addTab(ISettingsWidget* parentWidget, const QString& label)
{
  if (parentWidget) {
    auto settingsWidget =
      qobject_cast<SettingsWidget*>(parentWidget->settingsPage());
    if (settingsWidget) {
      return m_tabs->addTab(parentWidget->settingsPage(), label);
    }
  }
  return -1;
}

int
SettingsDialog::addTab(ISettingsWidget *parentWidget,
                       const QIcon& icon,
                       const QString& label)
{
  if (parentWidget) {
    auto settingsWidget =
      qobject_cast<SettingsWidget*>(parentWidget->settingsPage());
    if (settingsWidget) {
      return m_tabs->addTab(settingsWidget, icon, label);
    }
  }
  return -1;
}

int
SettingsDialog::insertTab(int index,
                          ISettingsWidget* parentWidget,
                          const QString& label)
{
  if (parentWidget) {
    auto settingsWidget =
      qobject_cast<SettingsWidget*>(parentWidget->settingsPage());
    if (settingsWidget) {
      return m_tabs->insertTab(index, parentWidget->settingsPage(), label);
    }
  }
  return -1;
}

int
SettingsDialog::insertTab(int index,
                          ISettingsWidget *parentWidget,
                          const QIcon& icon,
                          const QString& label)
{
  if (parentWidget) {
    auto settingsWidget =
      qobject_cast<SettingsWidget*>(parentWidget->settingsPage());
    if (settingsWidget) {
      return m_tabs->insertTab(
        index, parentWidget->settingsPage(), icon, label);
    }
  }
  return -1;
}

bool
SettingsDialog::isTabEnabled(int index) const
{
  return m_tabs->isTabEnabled(index);
}

bool
SettingsDialog::isTabVisible(int index) const
{
  return m_tabs->isTabVisible(index);
}

void
SettingsDialog::removeTab(int index)
{
  m_tabs->removeTab(index);
}

void SettingsDialog::acceptChanges() {
  accept();
}

void SettingsDialog::applyChanges() {
  emit apply();
}

void SettingsDialog::cancelChanges() {
  reject();
}

void SettingsDialog::help() {
  // TODO help dialog.
}

int SettingsDialog::count() {
  return m_tabs->count();
}

SettingsWidget *SettingsDialog::widget(int index) {
  return m_tabs->widget(index);
}

QSize SettingsDialog::sizeHint() const
{
  return QSize(650,900);
}
