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

  moreBtn = new QPushButton(tr("More"), this);
  connect(moreBtn, &QPushButton::clicked, this, &ExtendedHelpDialog::more);
  layout->addWidget(moreBtn, 0, 1);

  extension = new QWidget(this);
  auto extensionLayout = new QHBoxLayout;
  m_extendedLbl = new QLabel(tr("Extended help to be done"), this);
  extensionLayout->setContentsMargins(QMargins());
  extension->setLayout(extensionLayout);
  extensionLayout->addWidget(m_extendedLbl);
  extension->hide();
  layout->addWidget(extension, 1, 0);

  auto closeBtn = new QPushButton(tr("Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &SimpleHelpDialog::reject);
}

void
ExtendedHelpDialog::more()
{
  if (extension->isVisible()) {
    moreBtn->setText(tr("More"));
    extension->setVisible(false);
  } else {
    moreBtn->setText(tr("Less"));
    extension->setVisible(true);
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
