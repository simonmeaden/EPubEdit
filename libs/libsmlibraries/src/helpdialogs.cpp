#include "helpdialogs.h"

//====================================================================
//=== SimpleHelpDialog
//====================================================================
SimpleHelpDialog::SimpleHelpDialog(const QString& title, QWidget* parent)
  : QDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QLabel(this);
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  auto closeBtn = new QPushButton(tr("Close", "Close button text"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &SimpleHelpDialog::reject);
}

void
SimpleHelpDialog::setHelpText(const QString& text)
{
  m_primaryLbl->setText(text);
}

//====================================================================
//=== ExtendedHelpDialog
//====================================================================
ExtendedHelpDialog::ExtendedHelpDialog(const QString& title, QWidget* parent)
  : QDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QLabel(this);
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  m_moreBtn = new QPushButton(tr("More"), this);
  connect(m_moreBtn, &QPushButton::clicked, this, &ExtendedHelpDialog::more);
  layout->addWidget(m_moreBtn, 0, 1);

  m_extension = new QWidget(this);
  auto extensionLayout = new QHBoxLayout;
  m_extendedLbl = new QLabel(this);
  extensionLayout->setContentsMargins(QMargins());
  m_extension->setLayout(extensionLayout);
  extensionLayout->addWidget(m_extendedLbl);
  m_extension->hide();
  layout->addWidget(m_extension, 1, 0);

  auto closeBtn = new QPushButton(tr("Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &SimpleHelpDialog::reject);
}

void
ExtendedHelpDialog::more()
{
  if (m_extension->isVisible()) {
    m_moreBtn->setText(tr("More"));
    m_extension->setVisible(false);
  } else {
    m_moreBtn->setText(tr("Less"));
    m_extension->setVisible(true);
  }
}

void
ExtendedHelpDialog::setPrimaryHelpText(const QString& text)
{
  m_primaryLbl->setText(text);
}

void
ExtendedHelpDialog::setAdditionalHelpText(const QString& text)
{
  m_extendedLbl->setText(text);
}

//====================================================================
//=== ExtendedHelpWithTabsDialog
//====================================================================
ExtendedHelpWithTabsDialog::ExtendedHelpWithTabsDialog(const QString& title,
                                                       QWidget* parent)
  : QDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QLabel(this);
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  m_moreBtn = new QPushButton(tr("More"), this);
  connect(
    m_moreBtn, &QPushButton::clicked, this, &ExtendedHelpWithTabsDialog::more);
  layout->addWidget(m_moreBtn, 0, 1);

  m_tabs = new QTabWidget(this);
  m_tabs->hide();
  layout->addWidget(m_tabs, 1, 0);

  auto closeBtn = new QPushButton(tr("Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &SimpleHelpDialog::reject);
}

void ExtendedHelpWithTabsDialog::setPrimaryHelpText(const QString &text)
{
  m_primaryLbl->setText(text);
}

void
ExtendedHelpWithTabsDialog::addAdditionalHelpPage(const QString& title,
                                                  const QString& text)
{
  auto lbl = new QLabel(text, this);
  m_tabs->addTab(lbl, title);
}

void
ExtendedHelpWithTabsDialog::more()
{
  if (m_tabs->isVisible()) {
    m_moreBtn->setText(tr("More"));
    m_tabs->setVisible(false);
  } else {
    m_moreBtn->setText(tr("Less"));
    m_tabs->setVisible(true);
  }
}
