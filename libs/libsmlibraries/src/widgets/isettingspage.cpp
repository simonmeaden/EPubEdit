#include "widgets/isettingspage.h"


//====================================================================
//=== SettingsWidget
//====================================================================
SettingsWidget::SettingsWidget(QWidget *parent)
  : QWidget(parent)
  ,m_layout(new QGridLayout)
{
  setLayout(m_layout);
}

//====================================================================
//=== SettingsTabWidget
//====================================================================
SettingsTabWidget::SettingsTabWidget(QWidget *parent)
  : QTabWidget(parent)
{
}

SettingsWidget *SettingsTabWidget::widget(int index) const
{
  return qobject_cast<SettingsWidget*>(QTabWidget::widget(index));
}
