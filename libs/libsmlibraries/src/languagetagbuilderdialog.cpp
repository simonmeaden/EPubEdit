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

void
LanguageTagBuilderDialog::clearTag()
{
  m_resultLbl->clear();
}

void
LanguageTagBuilderDialog::setReport(const QString& report)
{
  m_reportLbl->setText(report);
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
  auto list = m_languages->languageDescriptions();
  m_primaryFilterEdit = new FilterEdit(list, false, this);
  boxLayout->addWidget(m_primaryFilterEdit, 0, 0);
  auto btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn, &QPushButton::clicked, m_primaryFilterEdit, &FilterEdit::clear);
  boxLayout->addWidget(btn, 0, 1);
  boxLayout->addWidget(m_primaryFilterEdit->comboBox(), 0, 2);
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
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn, &QPushButton::clicked, m_extlangFilterEdit, &FilterEdit::clear);
  boxLayout->addWidget(btn, 0, 2);
  boxLayout->addWidget(m_extlangFilterEdit->comboBox(), 0, 3);
  boxLayout->addWidget(m_extlangFilterEdit->checkBox(), 0, 4);
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
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn, &QPushButton::clicked, m_scriptFilterEdit, &FilterEdit::clear);
  boxLayout->addWidget(btn, 1, 2);
  boxLayout->addWidget(m_scriptFilterEdit->comboBox(), 1, 3);
  boxLayout->addWidget(m_scriptFilterEdit->checkBox(), 1, 4);
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
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn, &QPushButton::clicked, m_regionFilterEdit, &FilterEdit::clear);
  boxLayout->addWidget(btn, 2, 2);
  boxLayout->addWidget(m_regionFilterEdit->comboBox(), 2, 3);
  boxLayout->addWidget(m_regionFilterEdit->checkBox(), 2, 4);
  connect(m_regionFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::regionChanged);
  connect(m_regionFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::regionChanged);

  box = new QGroupBox(tr("Language Tag", "Tag display box title"), this);
  auto resultLayout = new QGridLayout;
  resultLayout->setColumnStretch(0, 2);
  box->setLayout(resultLayout);
  layout->addWidget(box);

  m_reportLbl = new QLabel(this);
  resultLayout->addWidget(m_reportLbl, 0, 0, 1, 2);
  m_resultLbl = new FilterLabel(this);
  resultLayout->addWidget(m_resultLbl, 1, 0);
  m_usePreferredBtn = new QPushButton(
    tr("Use Preferred", "Use preferred option button text."), this);
  m_usePreferredBtn->setEnabled(false);
  connect(m_usePreferredBtn,
          &QPushButton::clicked,
          this,
          &LanguageTagBuilderDialog::usePreferredValue);
  resultLayout->addWidget(m_usePreferredBtn, 0, 2);

  auto testBtn = new QPushButton(tr("Test Tag", "Test tag button label"), this);
  resultLayout->addWidget(testBtn, 1, 1);
  connect(
    testBtn, &QPushButton::clicked, this, &LanguageTagBuilderDialog::testTag);
  auto clearBtn = new QPushButton(tr("Clear", "Clear tag button label"), this);
  resultLayout->addWidget(clearBtn, 1, 2);
  connect(
    testBtn, &QPushButton::clicked, this, &LanguageTagBuilderDialog::clearTag);

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
    auto editText = m_primaryFilterEdit->currentText();
    m_language = m_languages->languageFromDescription(editText);
    // find matching extlang if any.
    auto extlangs = m_languages->extlangsWithPrefix(m_language->subtag());
    // set extlang to only acceptable values.
    m_extlangFilterEdit->setValues(extlangs);
    if (m_language) {
      m_languageTag = m_language->subtag();
    }
  } else {
    m_languageTag = tr("MISSING-PRIMARY-LANGUAGE",
                       "Missing primary language warning message.");
  }
  updateTag(m_language->description() == "Private use"
              ? BCP47Language::PRIVATE_LANGUAGE
              : BCP47Language::PRIMARY_LANGUAGE);
}

void
LanguageTagBuilderDialog::extlangChanged()
{
  if (m_extlangFilterEdit->hasCurrentText() &&
      m_extlangFilterEdit->isRequired()) {
    auto language =
      m_languages->extlangFromDescription(m_extlangFilterEdit->currentText());
    if (language->hasPreferredValue()) {
      QString reportText;
      if (language->isMacrolanguage()) {
        reportText =
          tr("The language dialect has a preferred LANGUAGE tag "
             "(%1), this should be used\n"
             "in preference to the 'LANGUAGE-EXTENDED' format (%2-%3).");
        setReport(reportText.arg(language->preferredValue(),
                                 language->prefix().at(0),
                                 language->subtag()));
        m_usePreferredBtn->setEnabled(true);
      } else {
        reportText =
          tr("The extended language has a preferred LANGUAGE tag "
             "(%1), this should be used\n"
             "in preference to the 'LANGUAGE-EXTENDED' format (%2-%3).");
        setReport(reportText.arg(language->preferredValue(),
                                 language->macrolanguageName(),
                                 language->subtag()));
        m_usePreferredBtn->setEnabled(true);
      }
    } else {
      m_usePreferredBtn->setEnabled(false);
    }
    if (language) {
      m_extlangTag = language->subtag();
      m_extlangFilterEdit->setRequired(true);
    }
  }
  updateTag();
}

void
LanguageTagBuilderDialog::usePreferredValue()
{}

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
LanguageTagBuilderDialog::updateTag(BCP47Language::TagType type)
{
  QString result;
  if (!m_languageTag.isEmpty()) {
    if (type == BCP47Language::PRIVATE_LANGUAGE) {
      auto dlg = new PrivateLanguageDialog(this);
      if (dlg->exec() == QDialog::Accepted) {
        qWarning();
      }
    } else {
      result = m_languageTag;
    }
  }

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
  auto dlg = new ExtendedHelpWithComboDialog(
    tr("Language Tag Builder Help", "Help dialog title"), this);
  dlg->setMinimumWidth(1500);
  // clang-format off
  dlg->setHelpText(
    tr("ISO639-1 and later require that the language tags be\n"
       "built in a certain order. There MUST be a primary language\n"
       "tag in the first position.This can be followed by one of :\n"
       "- An extended language tag\n"
       "  This can be followed by further optional tags, a script tag or\n"
       "  a regional tag, or a script tag followed by a regional tag.\n"
       "- A language script tag\n"
       "  This can be followed by an optional regional tag\n"
       "- A regional tag.\n",
       "Primary help text"));
  dlg->addAdditionalHelpPage(
    tr("Extended Languages :", "Extended help page title"),
    tr(
      "- No extended language support.\n"
      "  Primary languages with NO extended languages cause the extended language\n"
      "  code field to be blank.\n"
      "- Extended language support\n"
      "   Some languages have both a three character primary language code and  a\n"
      "   two character primary language code plus a three character extended\n"
      "   language code.\n"
      "   In this case the three character primary language code is  preferred.\n"
      "   Examples are 'Gan Chinese' which has a three character code 'gan'\n"
      "   and a two character code 'zh' plus a three character code 'gan'\n"
      "   and 'Gulf Arabic' which has a three character code 'afb' ant two\n"
      "   character 'ar' plus three character 'afb'.\n"
      "   In these cases 'gan' and 'afb' three character codes are preferred.\n"
      "\n"
      "The dialog will remove the extended codes in this case.\n\n"
      "*Please note that as far as I can see there are **NO** cases in which\n"
      "there is no three character code, so the extended language option\n"
      "will probably <b>NEVER</b> appear.\n"
      "On the other hand there are a huge number of primary languages so\n"
      "I might have missed one.*",
      "Extended languages help page"));
  dlg->addAdditionalHelpPage(
    tr("Private use Languages :", "Private primary language help page title"),
    tr(
      "Private use language tags area allowable when used in agreement\n"
      "between agreed parties for internal use. Available values are in the\n"
      "range 'qaa' to 'qtz.\n\n"
      "Alternatively you can use the characters 'x' followed by private\n"
      "subtag values. Subtags following the 'x' will be ignored by\n"
      "non-private users.\n\n"
      "The 'qXX' values are preferred over the 'x' primary tags and the 'i'\n"
      "tag should only be used if used in an existing system.\n"
      "The 'i' private tag is included only for grandfathered use for example\n"
      "'i-klingon' and 'i-bnn'.",
      "Private languages help page"));
  // clang-format on
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

void
LanguageTagBuilderDialog::FilterLabel::clear()
{
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

//====================================================================
//=== PrivateEdit
//====================================================================
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::PrivateEdit(
  QWidget* parent)
  : QLineEdit("qaa", parent)
{
  QRegularExpression re("(q[a-t][a-z])|x|i");
  auto validator = new QRegularExpressionValidator(re, parent);
  setValidator(validator);
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::up1()
{
  if (col2 < 't')
    col2++;
  build();
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::up2()
{
  if (col3 < 'z')
    col3++;
  build();
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::down1()
{
  if (col2 > 'a')
    col2--;
  build();
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::down2()
{
  if (col3 > 'a')
    col3--;
  build();
}

QString
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::value() const
{
  return m_value;
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateEdit::build()
{
  m_value.clear();
  m_value.append(col1).append(col2).append(col3);
  setText(m_value);
}

//====================================================================
//=== PrivateLanguageDialog
//====================================================================
LanguageTagBuilderDialog::PrivateLanguageDialog::PrivateLanguageDialog(
  QWidget* parent)
  : QDialog(parent)
{
  auto layout = new QGridLayout;
  setLayout(layout);

  m_xBox = new QRadioButton(tr("x - not preferred"), this);
  layout->addWidget(m_xBox, 0, 0, 1, 3);
  connect(
    m_xBox, &QRadioButton::clicked, this, &PrivateLanguageDialog::xClicked);

  m_iBox = new QRadioButton(tr("i - grandfathered, not preferred"), this);
  layout->addWidget(m_iBox, 1, 0, 1, 3);
  connect(
    m_iBox, &QRadioButton::clicked, this, &PrivateLanguageDialog::iClicked);

  m_pBox = new QRadioButton("qaa-qtz, preferred", this);
  m_pBox->setChecked(true);
  layout->addWidget(m_pBox, 2, 0, 1, 3);
  connect(
    m_pBox, &QRadioButton::clicked, this, &PrivateLanguageDialog::pClicked);

  m_edit = new PrivateEdit(this);
  m_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout->addWidget(m_edit, 3, 0);

  m_up1Btn = new QPushButton(this);
  m_up1Btn->setIcon(QIcon(QStringLiteral(":/images/go-up.svg")));
  m_up1Btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  connect(m_up1Btn, &QPushButton::clicked, m_edit, &PrivateEdit::up1);
  m_up2Btn = new QPushButton(this);
  m_up2Btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_up2Btn->setIcon(QIcon(":/images/up"));
  connect(m_up2Btn, &QPushButton::clicked, m_edit, &PrivateEdit::up2);
  m_down1Btn = new QPushButton(this);
  m_down1Btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_down1Btn->setIcon(QIcon(":/images/down"));
  connect(m_down1Btn, &QPushButton::clicked, m_edit, &PrivateEdit::down1);
  m_down2Btn = new QPushButton(this);
  m_down2Btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_down2Btn->setIcon(QIcon(":/images/down"));
  connect(m_down2Btn, &QPushButton::clicked, m_edit, &PrivateEdit::down2);
  layout->addWidget(m_up1Btn, 3, 1);
  layout->addWidget(m_up2Btn, 3, 2);
  layout->addWidget(m_down1Btn, 4, 1);
  layout->addWidget(m_down2Btn, 4, 2);

  auto box = new QDialogButtonBox(
    QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Help,
    this);
  layout->addWidget(box, 5, 0, 1, 3);
  connect(
    box, &QDialogButtonBox::helpRequested, this, &PrivateLanguageDialog::help);
  connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void
LanguageTagBuilderDialog::PrivateLanguageDialog::help()
{
  // TODO help for
}

void LanguageTagBuilderDialog::PrivateLanguageDialog::iClicked(bool checked)
{
  if (checked) {
    m_edit->setEnabled(false);
    m_up1Btn->setEnabled(false);
    m_up2Btn->setEnabled(false);
    m_down1Btn->setEnabled(false);
    m_down2Btn->setEnabled(false);
  }
}

void LanguageTagBuilderDialog::PrivateLanguageDialog::xClicked(bool checked)
{
  if (checked) {
    m_edit->setEnabled(false);
    m_up1Btn->setEnabled(false);
    m_up2Btn->setEnabled(false);
    m_down1Btn->setEnabled(false);
    m_down2Btn->setEnabled(false);
  }
}

void LanguageTagBuilderDialog::PrivateLanguageDialog::pClicked(bool checked)
{
  if (checked) {
    m_edit->setEnabled(true);
    m_up1Btn->setEnabled(true);
    m_up2Btn->setEnabled(true);
    m_down1Btn->setEnabled(true);
    m_down2Btn->setEnabled(true);
  }
}
