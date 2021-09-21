#include "languagetagbuilderdialog.h"
#include "helpdialogs.h"
#include "paths.h"
#include "x11colors.h"

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
  , m_languageTag(tr("MISSING-PRIMARY-LANGUAGE"))
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
LanguageTagBuilderDialog::testTag()
{
  auto tag = m_resultLbl->currentTag();
  auto tags = m_languages->testTag(tag);
  qWarning();
}

void LanguageTagBuilderDialog::clearTag()
{
  m_resultLbl->clear();
}

void
LanguageTagBuilderDialog::initGui()
{
  auto layout = new QVBoxLayout;
  setLayout(layout);

  auto box =
    new QGroupBox(tr("Primary Language", "Primary Language box title"), this);
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

  box = new QGroupBox(tr("Language Extensions", "Language extension box title"),
                      this);
  boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);
  m_extlangFilterEdit =
    new FilterEdit(m_languages->extlangDescriptions(), true, this);
  boxLayout->addWidget(
    new QLabel(tr("Ext Lang", "EXTLANG label title"), this), 0, 0);
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
  boxLayout->addWidget(
    new QLabel(tr("Script", "Script label title"), this), 1, 0);
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
  boxLayout->addWidget(
    new QLabel(tr("Region", "Region label title"), this), 2, 0);
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

  box = new QGroupBox(tr("Language Tag", "Tag dislay box title"), this);
  auto resultLayout = new QHBoxLayout;
  box->setLayout(resultLayout);
  layout->addWidget(box);
  m_resultLbl = new FilterLabel(this);
  resultLayout->addWidget(m_resultLbl, 2);
  auto TESTBTN = new QPushButton(tr("Test Tag", "Test tag button label"), this);
  resultLayout->addWidget(TESTBTN, 1);
  connect(
    TESTBTN, &QPushButton::clicked, this, &LanguageTagBuilderDialog::testTag);
  auto clearBtn = new QPushButton(tr("Clear tag", "Clear tag button label"), this);
  resultLayout->addWidget(clearBtn, 1);
  connect(
    TESTBTN, &QPushButton::clicked, this, &LanguageTagBuilderDialog::clearTag);

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
    m_language =
      m_languages->languageFromDescription(m_primaryFilterEdit->currentText());
    // find matching extlang if any.
    auto extlangs = m_languages->extlangsWithPrefix(m_language->subtag());
    if (!extlangs.isEmpty()) {
      // set extlang to only acceptable values.
      m_extlangFilterEdit->setValues(extlangs);
    } else {
      m_extlangFilterEdit->clearValues();
    }
    if (m_language) {
      m_languageTag = m_language->subtag();
    }
  } else {
    m_languageTag = tr("MISSING-PRIMARY-LANGUAGE",
                       "Missing primary language warning message.");
  }
  updateTag();
}

void
LanguageTagBuilderDialog::extlangChanged()
{
  if (m_extlangFilterEdit->hasCurrentText() &&
      m_extlangFilterEdit->isRequired()) {
    auto language =
      m_languages->extlangFromDescription(m_extlangFilterEdit->currentText());
    if (language) {
      m_extlangTag = language->subtag();
      m_extlangFilterEdit->setRequired(true);
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::scriptChanged()
{
  if (!m_language->suppressScriptLang().isEmpty()) {
    m_scriptFilterEdit->setUnavailableText(
      tr("Script is suppressed for language %1",
         "Requested script tag has been suppressed by ISO639")
        .arg(m_language->description()));
  } else {
    m_scriptFilterEdit->setUnavailableText(
      tr(" == None Available == ", "Displayed when no possible value exists"));
    if (m_scriptFilterEdit->hasCurrentText() &&
        m_scriptFilterEdit->isRequired()) {
      auto language =
        m_languages->scriptFromDescription(m_scriptFilterEdit->currentText());
      if (language) {
        m_scriptTag = language->subtag();
        m_scriptFilterEdit->setRequired(true);
      }
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::regionChanged()
{
  if (m_regionFilterEdit->hasCurrentText() &&
      m_regionFilterEdit->isRequired()) {
    auto language =
      m_languages->regionFromDescription(m_regionFilterEdit->currentText());
    if (language) {
      m_regionTag = language->subtag();
      m_regionFilterEdit->setRequired(true);
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::updateTag()
{
  QString result;
  if (!m_languageTag.isEmpty())
    result = m_languageTag;

  if (!m_extlangTag.isEmpty() && m_extlangFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_extlangTag);
  }

  if (!m_scriptTag.isEmpty() && m_scriptFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_scriptTag);
  }

  if (!m_regionTag.isEmpty() && m_regionFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_regionTag);
  }

  m_resultLbl->setCurrentTag(result);
  //  auto tags = m_languages->testTag(result);
  //  qWarning();
  //  if (tags == 0) {
  //    m_resultLbl->setStyleSheet("QLabel {color: green; }");
  //  } else {
  //    m_resultLbl->setStyleSheet("QLabel {color: green; }");
  //  }
}

QString
LanguageTagBuilderDialog::tag()
{
  return m_resultLbl->text();
}

void
LanguageTagBuilderDialog::help()
{
  auto dlg = new SimpleHelpDialog(tr("Language Tag Builder Help"), this);
  dlg
    ->setHelpText(
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
  dlg->show();
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
  , m_selection(new FilterComboBox(parent))
  , m_required(showRequired ? new QCheckBox(tr("Use Tag"), parent) : nullptr)
  , m_parent(parent)
{
  m_filterText = tr(" == Filter == ");

  items.prepend(
    tr("Nothing selected", "Displayed when no filter text is entered."));
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
LanguageTagBuilderDialog::FilterEdit::setRequired(bool value)
{
  if (m_required)
    m_required->setChecked(value);
}

void
LanguageTagBuilderDialog::FilterEdit::setFilterText(const QString& text)
{
  m_filterText = text;
}

void
LanguageTagBuilderDialog::FilterEdit::setUnavailableText(const QString& text)
{
  m_selection->setUnavailableText(text);
}

void
LanguageTagBuilderDialog::FilterEdit::paintEvent(QPaintEvent* event)
{
  if (text().isEmpty()) {
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QColorConstants::X11::grey70);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, m_filterText);
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

//====================================================================
//=== FilterComboBox
//====================================================================
LanguageTagBuilderDialog::FilterEdit::FilterComboBox::FilterComboBox(
  QWidget* parent)
  : QComboBox(parent)
{
  m_unavailableText =
    tr(" == None Available == ", "Displayed whenno values are available");
}

void
LanguageTagBuilderDialog::FilterEdit::FilterComboBox::paintEvent(
  QPaintEvent* event)
{
  if (currentText().isEmpty()) {
    QComboBox::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QColorConstants::X11::grey50);
    painter.drawText(
      rect(), Qt::AlignLeft | Qt::AlignVCenter, m_unavailableText);
  } else {
    QComboBox::paintEvent(event);
  }
}

//====================================================================
//=== FilterLabel
//====================================================================
LanguageTagBuilderDialog::FilterLabel::FilterLabel(QWidget* parent)
  : QLabel(parent)
  , m_initialText(tr("A primary language is required by ISO639-1 and later!",
                     "Inital text in tag display"))
{
  setText(m_initialText);
}

QString
LanguageTagBuilderDialog::FilterLabel::currentTag() const
{
  return m_tagValue;
}

void
LanguageTagBuilderDialog::FilterLabel::setCurrentTag(const QString& tagValue)
{
  m_tagValue = tagValue;
  auto displayText =
    tr("Current Tag : %1", "Tag display text, value replaces %1")
      .arg(m_tagValue);
  setText(displayText);
}

void LanguageTagBuilderDialog::FilterLabel::clear() {
  m_tagValue.clear();
  QLabel::clear();
}

void
LanguageTagBuilderDialog::FilterLabel::paintEvent(QPaintEvent* event)
{
  if (currentTag().isEmpty()) {
    QLabel::paintEvent(event);
    QPainter painter(this);
    //    painter.setPen(QColorConstants::X11::grey50);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, m_tagValue);
  } else {
    QLabel::paintEvent(event);
  }
}
