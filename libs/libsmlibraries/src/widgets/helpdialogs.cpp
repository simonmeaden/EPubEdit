#include "widgets/helpdialogs.h"

//====================================================================
//=== BaseHelpDialog
//====================================================================
void
BaseHelpDialog::setHelpText(const QString& text, Type type)
{
  switch (type) {
    case PLAIN:
      m_primaryLbl->setPlainText(text);
      break;
    case HTML:
      m_primaryLbl->setHtml(text);
      break;
    case MARKDOWN:
      m_primaryLbl->setMarkdown(text);
      break;
  }
  auto w = calculateMinimumWidth(text);
  auto margins = m_primaryLbl->contentsMargins();
  w = w + margins.left() + margins.right();
  m_primaryLbl->setMinimumWidth(w);
}

int BaseHelpDialog::calculateMinimumWidth(const QString &text) {
  auto lines = text.split("\n");
  auto fm = QFontMetrics(font());
  int minWidth=0;
  for (auto &line : lines) {
    auto w = fm.horizontalAdvance(line);
    minWidth = w > minWidth ? w : minWidth;
  }
  return minWidth;
}

//====================================================================
//=== SimpleHelpDialog
//====================================================================
SimpleHelpDialog::SimpleHelpDialog(const QString& title, QWidget* parent)
  : BaseHelpDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QTextEdit(this);
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  auto closeBtn = new QPushButton(tr("&Close", "Close button text"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
}

//====================================================================
//=== ExtendedHelpDialog
//====================================================================
ExtendedHelpDialog::ExtendedHelpDialog(const QString& title, QWidget* parent)
  : BaseHelpDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QTextEdit(this);
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  m_moreBtn = new QPushButton(tr("&More"), this);
  connect(m_moreBtn, &QPushButton::clicked, this, &ExtendedHelpDialog::more);
  layout->addWidget(m_moreBtn, 0, 1);

  m_extended = new QTextEdit(this);
  layout->addWidget(m_extended, 1, 0, 1, 2);

  auto closeBtn = new QPushButton(tr("&Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void
ExtendedHelpDialog::more()
{
  if (m_extended->isVisible()) {
    m_moreBtn->setText(tr("&More"));
    m_extended->setVisible(false);
  } else {
    m_moreBtn->setText(tr("&Less"));
    m_extended->setVisible(true);
  }
}

void
ExtendedHelpDialog::setAdditionalHelpText(const QString& text, Type type)
{
  auto w = calculateMinimumWidth(text);
  auto margins = m_extended->contentsMargins();
  w = w + margins.left() + margins.right();
  m_extended->setMinimumWidth(w);

  switch (type) {
    case PLAIN:
      m_extended->setPlainText(text);
    break;
    case HTML:
      m_extended->setHtml(text);
      break;
    case MARKDOWN:
      m_extended->setMarkdown(text);
      break;
  }
}

//====================================================================
//=== ExtendedHelpWithTabsDialog
//====================================================================
ExtendedHelpWithTabsDialog::ExtendedHelpWithTabsDialog(const QString& title,
                                                       QWidget* parent)
  : BaseHelpDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  m_moreBtn = new QPushButton(tr("&More"), this);
  connect(
    m_moreBtn, &QPushButton::clicked, this, &ExtendedHelpWithTabsDialog::more);
  layout->addWidget(m_moreBtn, 0, 1);

  m_tabs = new QTabWidget(this);
  m_tabs->hide();
  layout->addWidget(m_tabs, 1, 0, 1, 2);

  auto closeBtn = new QPushButton(tr("&Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void
ExtendedHelpWithTabsDialog::addAdditionalHelpPage(const QString& title,
                                                  const QString& text,
                                                  Type type)
{
  auto lbl = new QTextEdit(text, this);
  m_tabs->addTab(lbl, title);

  auto w = calculateMinimumWidth(text);
  auto margins = lbl->contentsMargins();
  w = w + margins.left() + margins.right();
  lbl->setMinimumWidth(w);

  switch (type) {
    case PLAIN:
      lbl->setPlainText(text);
    break;
    case HTML:
      lbl->setHtml(text);
      break;
    case MARKDOWN:
      lbl->setMarkdown(text);
      break;
  }
}

void
ExtendedHelpWithTabsDialog::more()
{
  if (m_tabs->isVisible()) {
    m_moreBtn->setText(tr("&More"));
    m_tabs->setVisible(false);
  } else {
    m_moreBtn->setText(tr("&Less"));
    m_tabs->setVisible(true);
  }
}

//====================================================================
//=== ExtendedHelpWithComboDialog
//====================================================================
ExtendedHelpWithComboDialog::ExtendedHelpWithComboDialog(const QString& title,
                                                         QWidget* parent)
  : BaseHelpDialog(parent)
{
  if (!title.isEmpty())
    setWindowTitle(title);
  else
    setWindowTitle(tr("Help Dialog", "Default window Title"));

  m_primaryLbl = new QTextEdit(this);
  auto layout = new QGridLayout;
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);
  layout->addWidget(m_primaryLbl, 0, 0);

  m_moreBtn = new QPushButton(tr("&More"), this);
  connect(
    m_moreBtn, &QPushButton::clicked, this, &ExtendedHelpWithComboDialog::more);
  layout->addWidget(m_moreBtn, 0, 1);

  m_extension = new QFrame(this);
  auto extLayout = new QGridLayout;
  m_extension->setLayout(extLayout);

  m_box = new QComboBox(this);
  extLayout->addWidget(m_box, 0, 0);
  connect(m_box,
          qOverload<int>(&QComboBox::currentIndexChanged),
          this,
          &ExtendedHelpWithComboDialog::changePage);

  auto extFrame = new QWidget(this);
  extFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_stack = new QStackedLayout;
  extFrame->setLayout(m_stack);
  extLayout->addWidget(extFrame, 1, 0);

  m_extension->hide();
  layout->addWidget(m_extension, 1, 0, 1, 2);

  auto closeBtn = new QPushButton(tr("&Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void
ExtendedHelpWithComboDialog::addAdditionalHelpPage(const QString& title,
                                                   const QString& text,
                                                   Type type)
{
  m_box->addItem(title);
  auto lbl = new QTextEdit(this);

  auto w = calculateMinimumWidth(text);
  auto margins = lbl->contentsMargins();
  w = w + margins.left() + margins.right();
  lbl->setMinimumWidth(w);

  switch (type) {
    case PLAIN:
      lbl->setPlainText(text);
    break;
    case HTML:
      lbl->setHtml(text);
      break;
    case MARKDOWN:
      lbl->setMarkdown(text);
      break;
  }
  m_stack->addWidget(lbl);
}

void
ExtendedHelpWithComboDialog::more()
{
  if (m_extension->isVisible()) {
    m_moreBtn->setText(tr("&More"));
    m_extension->setVisible(false);
  } else {
    m_moreBtn->setText(tr("&Less"));
    m_extension->setVisible(true);
  }
}

void
ExtendedHelpWithComboDialog::changePage(int page)
{
  m_stack->setCurrentIndex(page);
}
