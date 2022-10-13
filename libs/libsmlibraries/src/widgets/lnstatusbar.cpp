#include "widgets/lnstatusbar.h"

LNStatusBar::LNStatusBar(QWidget* parent)
  : QStatusBar{ parent }
{
  auto px = QPixmap(QPixmap(":/icon/SettingsGear"));
  auto scaled = px.scaled(QSize(16, 16));
  auto btn = new QToolButton(this);
  btn->setIcon(scaled);
  addPermanentWidget(btn);
  connect(btn, &QToolButton::clicked, this, &LNStatusBar::settingsClicked);

  m_posLbl =
    new QLabel(tr("Line: %1 of %2: Col %3").arg(0).arg(0).arg(0), this);
  addPermanentWidget(m_posLbl);
}

void
LNStatusBar::setLocation(int line, int lineCount, int column)
{
  m_posLbl->setText(
    tr("Line: %1 of %2: Col %3").arg(line).arg(lineCount).arg(column));
}

void
LNStatusBar::addPermanentWidget(QWidget* widget, int stretch)
{
  insertPermanentWidget(0, widget, stretch);
}

void
LNStatusBar::settingsClicked()
{
  emit settingsRequested();
}
