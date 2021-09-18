#include "languagetagbuilderdialog.h"

//====================================================================
//=== LanguageTagBuilderDialog
//====================================================================
LanguageTagBuilderDialog::LanguageTagBuilderDialog(QWidget* parent)
  : QDialog(parent)
  , m_languages(new BCP47Languages(this))
  , m_configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , m_configFile(
      (Paths::join(m_configDir.path(), "epubedit", "languages.yaml")))
{
  setWindowTitle(tr("Language tag Builder"));
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
  auto layout = new QVBoxLayout;
  setLayout(layout);

  auto box = new QGroupBox(tr("Primary Language"), this);
  auto boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);
  m_primaryFilterEdit =
    new FilterEdit(m_languages->languageDescriptions(), false, this);
  boxLayout->addWidget(m_primaryFilterEdit, 0, 0);
  boxLayout->addWidget(m_primaryFilterEdit->comboBox(), 0, 1);
  connect(m_primaryFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::languageChanged);

  box = new QGroupBox(tr("Second Layer"), this);
  boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);
  m_extlangFilterEdit =
    new FilterEdit(m_languages->extlangDescriptions(), true, this);
  boxLayout->addWidget(new QLabel(tr("Ext Lang"), this), 0, 0);
  boxLayout->addWidget(m_extlangFilterEdit, 0, 1);
  boxLayout->addWidget(m_extlangFilterEdit->comboBox(), 0, 2);
  boxLayout->addWidget(m_extlangFilterEdit->checkBox(), 0, 3);
  connect(m_extlangFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  connect(m_extlangFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  m_scriptFilterEdit =
    new FilterEdit(m_languages->scriptDescriptions(), true, this);
  boxLayout->addWidget(new QLabel(tr("Script"), this), 1, 0);
  boxLayout->addWidget(m_scriptFilterEdit, 1, 1);
  boxLayout->addWidget(m_scriptFilterEdit->comboBox(), 1, 2);
  boxLayout->addWidget(m_scriptFilterEdit->checkBox(), 1, 3);
  connect(m_scriptFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  connect(m_scriptFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  m_regionFilterEdit =
    new FilterEdit(m_languages->regionDescriptions(), true, this);
  boxLayout->addWidget(new QLabel(tr("Region"), this), 2, 0);
  boxLayout->addWidget(m_regionFilterEdit, 2, 1);
  boxLayout->addWidget(m_regionFilterEdit->comboBox(), 2, 2);
  boxLayout->addWidget(m_regionFilterEdit->checkBox(), 2, 3);
  connect(m_regionFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::regionChanged);
  connect(m_regionFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::regionChanged);

  box = new QGroupBox(tr("Language Tag"), this);
  auto resultLayout = new QVBoxLayout;
  box->setLayout(resultLayout);
  layout->addWidget(box);
  m_resultLbl = new QLabel(
    tr("A primary language is required by ISO639-1 and later!"), this);
  resultLayout->addWidget(m_resultLbl);

  auto buttonBox = new QDialogButtonBox(
    QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Help,
    this);
  layout->addWidget(buttonBox);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(buttonBox,
          &QDialogButtonBox::helpRequested,
          this,
          &LanguageTagBuilderDialog::help);

  auto filler = new QWidget(this);
  filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(filler);
}

void
LanguageTagBuilderDialog::languageChanged()
{
  if (m_primaryFilterEdit->hasCurrentText()) {
    auto language =
      m_languages->languageFromDescription(m_primaryFilterEdit->currentText());
    // find matching extlang if any.
    auto extlangs = m_languages->extlangsWithPrefix(language->subtag());
    if (!extlangs.isEmpty()) {
      // set extlang to only acceptable values.
      m_extlangFilterEdit->setValues(extlangs);
    } else {
      m_extlangFilterEdit->clearValues();
    }
    if (language) {
      m_language = language->subtag();
    }
  } else {
    m_language = tr("MISSING-PRIMARY-LANGUAGE");
  }
  updateTag();
}

void
LanguageTagBuilderDialog::extlangChanged()
{
  if (m_extlangFilterEdit->hasCurrentText() &&
      m_extlangFilterEdit->isRequired()) {
    m_extlang = "-";
    auto language =
      m_languages->extlangFromDescription(m_extlangFilterEdit->currentText());
    if (language) {
      m_extlang += language->subtag();
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::scriptChanged()
{
  if (m_scriptFilterEdit->hasCurrentText() &&
      m_scriptFilterEdit->isRequired()) {
    m_script = "-";
    auto language =
      m_languages->scriptFromDescription(m_scriptFilterEdit->currentText());
    if (language) {
      m_script += language->subtag();
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::regionChanged()
{
  if (m_regionFilterEdit->hasCurrentText() &&
      m_regionFilterEdit->isRequired()) {
    m_region = "-";
    auto language =
      m_languages->regionFromDescription(m_extlangFilterEdit->currentText());
    if (language) {
      m_region += language->subtag();
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::updateTag()
{
  QString result;
  if (!m_language.isEmpty())
    result = m_language;

  if (!m_extlang.isEmpty() && m_extlangFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_extlang);
  }

  if (!m_script.isEmpty() && m_scriptFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_script);
  }

  if (!m_region.isEmpty() && m_regionFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_region);
  }

  m_resultLbl->setText(result);
  //  if (m_languages->testTag(result) == 0) {
  //    m_resultLbl->setStyleSheet("QLabel {color: green; }");
  //  } else {
  //    m_resultLbl->setStyleSheet("QLabel {color: green; }");
  //  }
}

void
LanguageTagBuilderDialog::help()
{
  // TODO help shit
  auto dlg = new HelpDialog(this);
  dlg->show();
}

//====================================================================
//=== HelpDialog
//====================================================================
LanguageTagBuilderDialog::HelpDialog::HelpDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Help about Language Tags"));

  auto lbl = new QLabel(this);
  lbl->setText(
    tr("ISO639-1 and later require that the language tags be\n"
       "built in a certain order. There MUST be a primary language\n"
       "tag in the first position.This can be followed by one of\n"
       "  1. An extended language tag\n"
       "     This can be followed by further optional tags, a script tag or\n"
       "     a regional tag, or a script tag followed by a regional tag.\n"
       "  2. A language script tag\n"
       "     This can be followed by an optional regional tag\n"
       "  3. A regional tag.\n"
       ""));
  auto layout = new QGridLayout;
  setLayout(layout);
  layout->addWidget(lbl, 0, 0);

  moreBtn = new QPushButton(tr("More"), this);
  connect(moreBtn, &QPushButton::clicked, this, &HelpDialog::more);
  layout->addWidget(moreBtn, 0, 1);

  extension = new QWidget(this);
  auto extensionLayout = new QHBoxLayout;
  auto lbl2 = new QLabel(tr("Extended help to be done"), this);
  extensionLayout->setContentsMargins(QMargins());
  extension->setLayout(extensionLayout);
  extensionLayout->addWidget(lbl2);
  extension->hide();
  layout->addWidget(extension, 1, 0);

  auto closeBtn = new QPushButton(tr("Close"), this);
  layout->addWidget(closeBtn, 2, 0, 1, 2);
  connect(closeBtn, &QPushButton::clicked, this, &HelpDialog::reject);
}

void
LanguageTagBuilderDialog::HelpDialog::more()
{
  if (extension->isVisible()) {
    moreBtn->setText(tr("More"));
    extension->setVisible(false);
  } else {
    moreBtn->setText(tr("Less"));
    extension->setVisible(true);
  }
}

//====================================================================
//=== FilterEdit
//====================================================================
LanguageTagBuilderDialog::FilterEdit::FilterEdit(
  QStringList items,
  bool showRequired,
  LanguageTagBuilderDialog* parent)
  : QLineEdit(parent)
  , m_showRequired(showRequired)
  , m_selection(new QComboBox(parent))
  , m_required(showRequired ? new QCheckBox(tr("Use Tag"), parent) : nullptr)
  , m_parent(parent)
{
  items.prepend(tr("Nothing selected"));
  auto model = new QStringListModel();
  model->setStringList(items);
  auto proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_selection->setModel(proxyModel);
  connect(
    this, &QLineEdit::textChanged, this, &FilterEdit::primaryFilterChanged);
}

QComboBox*
LanguageTagBuilderDialog::FilterEdit::comboBox()
{
  return m_selection;
}

QCheckBox*
LanguageTagBuilderDialog::FilterEdit::checkBox()
{
  return m_required;
}

QSortFilterProxyModel*
LanguageTagBuilderDialog::FilterEdit::model()
{
  return qobject_cast<QSortFilterProxyModel*>(m_selection->model());
}

void
LanguageTagBuilderDialog::FilterEdit::setEnabled(bool enable)
{
  QLineEdit::setEnabled(enable);
  m_selection->setEnabled(enable);
}

void
LanguageTagBuilderDialog::FilterEdit::clearValues()
{
  m_selection->clear();
}

void
LanguageTagBuilderDialog::FilterEdit::setValue(const QString& value)
{
  m_selection->clear();
  m_selection->addItem(value);
}

void
LanguageTagBuilderDialog::FilterEdit::setValues(const QStringList& values)
{
  m_selection->clear();
  m_selection->addItems(values);
}

bool
LanguageTagBuilderDialog::FilterEdit::hasCurrentText()
{
  return !m_selection->currentText().isEmpty();
}

QString
LanguageTagBuilderDialog::FilterEdit::currentText()
{
  return m_selection->currentText();
}

bool
LanguageTagBuilderDialog::FilterEdit::isRequired()
{
  if (m_required)
    return m_required->isChecked();
  return false;
}

void
LanguageTagBuilderDialog::FilterEdit::paintEvent(QPaintEvent* event)
{
  if (text().isEmpty()) {
    QLineEdit::paintEvent(event);
    QString filterText = tr(" == Filter == ");
    QPainter painter(this);
    painter.setPen(QColorConstants::Svg::lightgrey);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, filterText);
  } else {
    QLineEdit::paintEvent(event);
  }
}

void
LanguageTagBuilderDialog::FilterEdit::primaryFilterChanged(const QString& text)
{
  QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
  model()->setFilterRegularExpression(regex);
}
