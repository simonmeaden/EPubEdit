#include "forms/languagetagbuilderdialog.h"
#include "helpdialogs.h"
#include "paths.h"
#include "x11colors.h"

//====================================================================
//=== LanguageTagBuilderDialog
//====================================================================
LanguageTagBuilderDialog::LanguageTagBuilderDialog(Config* config,
                                                   QWidget* parent)
  : QDialog(parent)
  , m_languages(config->languages())
  , m_configDir(QDir(config->configDir()))
  , m_configFile(config->configFile())
  , m_languageTag(tr("MISSING-PRIMARY-LANGUAGE"))
{
  Q_INIT_RESOURCE(languagetagbuilderdialogresource);

  setWindowTitle(tr("Language tag Builder"));

  if (m_languages->languageDescriptions().isEmpty()) {
    m_languages->readFromLocalFile(m_configFile);
  }

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

  // PRIMARY LANGUAGE BOX
  auto box =
    new QGroupBox(tr("Primary Language", "Primary Language box title"), this);
  auto boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);
  auto list = m_languages->languageDescriptions();
  m_primaryFilterEdit = new Private__::FilterEdit(list, false, this);
  boxLayout->addWidget(m_primaryFilterEdit, 0, 0);
  auto btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_primaryFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    this->updateTag(BCP47Language::NO_VALUE);
    this->showPrivateLanguageBox(BCP47Language::NO_VALUE);
  });
  boxLayout->addWidget(btn, 0, 1);
  boxLayout->addWidget(m_primaryFilterEdit->comboBox(), 0, 2);
  connect(m_primaryFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::languageChanged);

  // HIDDEN PRIVATE LANGUAGE
  m_privateLangBox =
    new Private__::PrivateLanguageFrame("(q[a-t][a-z])|x|i", "qaa", this);
  m_privateLangBox->setHidden(true);
  connect(m_privateLangBox,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateLanguageChanged);
  boxLayout->addWidget(m_privateLangBox, 1, 0, 1, 3);

  // EXTENSION BOX
  box = new QGroupBox(tr("Language Extensions", "Language extension box title"),
                      this);
  boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);

  // EXTLANG
  int row = 0;
  m_extlangFilterEdit =
    new Private__::FilterEdit(m_languages->extlangDescriptions(), true, this);
  boxLayout->addWidget(
    new QLabel(tr("Ext Lang", "EXTLANG label title"), this), row, 0);
  boxLayout->addWidget(m_extlangFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_extlangFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    this->updateTag(BCP47Language::NO_VALUE);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_extlangFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_extlangFilterEdit->checkBox(), row, 4);
  connect(m_extlangFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  connect(m_extlangFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::extlangChanged);

  row++;
  // SCRIPT
  m_privateScriptBox = new Private__::PrivateScriptFrame(
    "Qa(([a][a-z])|([b][a-x]))", "Qaaa", this);
  m_privateScriptBox->setHidden(true);
  connect(m_privateScriptBox,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateScriptChanged);
  boxLayout->addWidget(m_privateScriptBox, row, 0, 1, 3);

  row++;
  m_scriptFilterEdit =
    new Private__::FilterEdit(m_languages->scriptDescriptions(), true, this);
  boxLayout->addWidget(
    new QLabel(tr("Script", "Script label title"), this), row, 0);
  boxLayout->addWidget(m_scriptFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_scriptFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    this->updateTag(BCP47Language::NO_VALUE);
    this->hidePrivateScriptBox(BCP47Language::NO_VALUE);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_scriptFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_scriptFilterEdit->checkBox(), row, 4);
  connect(m_scriptFilterEdit->comboBox(),
          &QComboBox::activated,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  connect(m_scriptFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
          this,
          &LanguageTagBuilderDialog::scriptChanged);

  row++;
  // REGION
  m_privateRegionBox =
    new Private__::PrivateRegionFrame("AA|ZZ|Q[M-Z]|X[A-Z])", "AA", this);
  m_privateRegionBox->setHidden(true);
  connect(m_privateRegionBox,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateRegionChanged);
  boxLayout->addWidget(m_privateRegionBox, row, 0, 1, 3);

  m_regionFilterEdit =
    new Private__::FilterEdit(m_languages->regionDescriptions(), true, this);
  boxLayout->addWidget(
    new QLabel(tr("Region", "Region label title"), this), row, 0);
  boxLayout->addWidget(m_regionFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_regionFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    this->updateTag(BCP47Language::NO_VALUE);
    this->hidePrivateRegionBox(BCP47Language::NO_VALUE);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_regionFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_regionFilterEdit->checkBox(), row, 4);
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
  m_resultLbl = new Private__::FilterLabel(this);
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
LanguageTagBuilderDialog::privateLanguageChanged(const QString& value)
{
  m_languageTag = value;
  updateTag(BCP47Language::PRIVATE_LANGUAGE);
}

void
LanguageTagBuilderDialog::privateScriptChanged(const QString& value)
{
  m_scriptTag = value;
  updateTag(BCP47Language::PRIVATE_SCRIPT);
}

void
LanguageTagBuilderDialog::privateRegionChanged(const QString& value)
{
  m_regionTag = value;
  updateTag(BCP47Language::PRIVATE_REGION);
}

void
LanguageTagBuilderDialog::showPrivateLanguageBox(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_LANGUAGE) {
    m_privateLangBox->setHidden(false);
  } else {
    m_privateLangBox->setHidden(true);
  }
}

void
LanguageTagBuilderDialog::hidePrivateScriptBox(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_SCRIPT) {
    m_privateScriptBox->setHidden(false);
  } else {
    m_privateScriptBox->setHidden(true);
  }
}

void
LanguageTagBuilderDialog::hidePrivateRegionBox(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_REGION) {
    m_privateRegionBox->setHidden(false);
  } else {
    m_privateRegionBox->setHidden(true);
  }
}

void
LanguageTagBuilderDialog::languageChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_PRIMARY_LANGUAGE;
  if (m_primaryFilterEdit->hasCurrentText()) {
    auto editText = m_primaryFilterEdit->currentText();
    m_language = m_languages->languageFromDescription(editText);
    if (m_language) {
      // find matching extlang if any.
      auto extlangs = m_languages->extlangsWithPrefix(m_language->subtag());
      // set extlang to only acceptable values.
      m_extlangFilterEdit->setValues(extlangs);
      m_languageTag = m_language->subtag();
      if (m_language->description() == "Private use")
        languageType = BCP47Language::PRIVATE_LANGUAGE;
      else
        languageType = BCP47Language::PRIMARY_LANGUAGE;
    } else {
      m_extlangFilterEdit->clear();
    }
  } else {
    m_languageTag = tr("MISSING-PRIMARY-LANGUAGE",
                       "Missing primary language warning message.");
  }
  showPrivateLanguageBox(languageType);
  updateTag(languageType);
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
  updateTag(BCP47Language::NO_VALUE);
}

void
LanguageTagBuilderDialog::usePreferredValue()
{
  // TODO action this
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
  auto languageType = m_language->description() == "Private use"
                        ? BCP47Language::PRIVATE_SCRIPT
                        : BCP47Language::SCRIPT_LANGUAGE;
  showPrivateLanguageBox(languageType);
  updateTag(languageType);
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
  updateTag(BCP47Language::NO_VALUE);
}

void
LanguageTagBuilderDialog::updateTag(BCP47Language::TagType type)
{
  QString result;
  if (!m_languageTag.isEmpty()) {
    if (type == BCP47Language::PRIVATE_LANGUAGE) {
      result = m_privateLangBox->value();
    } else {
      result = m_languageTag;
    }
  }

  if (!m_extlangTag.isEmpty() && m_extlangFilterEdit->isRequired()) {
    result += QString("-%1").arg(m_extlangTag);
  }

  if (!m_scriptTag.isEmpty() && m_scriptFilterEdit->isRequired()) {
    if (type == BCP47Language::PRIVATE_SCRIPT) {
      result += QString("-%1").arg(m_privateScriptBox->value());
    } else {
      result += QString("-%1").arg(m_scriptTag);
    }
  }

  if (!m_regionTag.isEmpty() && m_regionFilterEdit->isRequired()) {
    if (type == BCP47Language::PRIVATE_REGION) {
      result += QString("-%1").arg(m_privateRegionBox->value());
    } else {
      result += QString("-%1").arg(m_regionTag);
    }
  }

  m_resultLbl->setCurrentTag(result);
  // TODO highlight label value.
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
       "- A regional tag.\n\n"
       "Filter edit boxes can be used to limit choices. For instance, entering\n"
       "'arabic' into the primary languages filter will limit choices to\n"
       "various Arabic languages.",
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

namespace Private__ {

//====================================================================
//=== FilterEdit
//====================================================================
FilterEdit::FilterEdit(QStringList items,
                       bool showRequired,
                       LanguageTagBuilderDialog* parent)
  : QLineEdit(parent)
  , m_filterText(tr("== Filter ==", "FilterEdit initial empty value text"))
  , m_showRequired(showRequired)
  , m_selection(new FilterComboBox(parent))
  , m_required(
      showRequired
        ? new QCheckBox(tr("Use Tag", "Use tag required checkbox text"), parent)
        : nullptr)
  , m_parent(parent)
{
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
FilterEdit::comboBox()
{
  return m_selection;
}

QCheckBox*
FilterEdit::checkBox()
{
  return m_required;
}

QSortFilterProxyModel*
FilterEdit::model()
{
  return qobject_cast<QSortFilterProxyModel*>(m_selection->model());
}

void
FilterEdit::setEnabled(bool enable)
{
  QLineEdit::setEnabled(enable);
  m_selection->setEnabled(enable);
}

void
FilterEdit::clearValues()
{
  m_selection->clear();
}

void
FilterEdit::setValue(const QString& value)
{
  m_selection->clear();
  m_selection->addItem(value);
}

void
FilterEdit::setValues(const QStringList& values)
{
  m_selection->clear();
  m_selection->addItems(values);
}

bool
FilterEdit::hasCurrentText()
{
  return !m_selection->currentText().isEmpty();
}

QString
FilterEdit::currentText()
{
  return m_selection->currentText();
}

bool
FilterEdit::isRequired()
{
  if (m_required)
    return m_required->isChecked();
  return false;
}

void
FilterEdit::setRequired(bool value)
{
  if (m_required)
    m_required->setChecked(value);
}

void
FilterEdit::setFilterText(const QString& text)
{
  m_filterText = text;
}

void
FilterEdit::setUnavailableText(const QString& text)
{
  m_selection->setUnavailableText(text);
}

void
FilterEdit::paintEvent(QPaintEvent* event)
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
FilterEdit::primaryFilterChanged(const QString& text)
{
  QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
  model()->setFilterRegularExpression(regex);
  m_selection->setCurrentIndex(0);
  if (m_selection->count() == 1) {
    emit m_selection->activated(0);
  }
}

//====================================================================
//=== FilterComboBox
//====================================================================
FilterEdit::FilterComboBox::FilterComboBox(QWidget* parent)
  : QComboBox(parent)
{
  m_unavailableText =
    tr(" == None Available == ", "Displayed when no values are available");
}

void
FilterEdit::FilterComboBox::paintEvent(QPaintEvent* event)
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
FilterLabel::FilterLabel(QWidget* parent)
  : QLabel(parent)
  , m_initialText(tr("A primary language is required by ISO639-1 and later!",
                     "Inital text in tag display"))
{
  setText(m_initialText);
}

QString
FilterLabel::currentTag() const
{
  return m_tagValue;
}

void
FilterLabel::setCurrentTag(const QString& tagValue)
{
  m_tagValue = tagValue;
  auto displayText =
    tr("Current Tag : %1", "Tag display text, value replaces %1")
      .arg(m_tagValue);
  setText(displayText);
}

void
FilterLabel::clear()
{
  m_tagValue.clear();
  QLabel::clear();
}

void
FilterLabel::paintEvent(QPaintEvent* event)
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
PrivateEdit::PrivateEdit(const QString& regex,
                         const QString& initialValue,
                         QWidget* parent)
  : QLineEdit(initialValue, parent)
{
  QRegularExpression re(regex); // x & i not required here
  auto validator = new QRegularExpressionValidator(re, parent);
  setValidator(validator);
}

QString
PrivateEdit::value() const
{
  return m_value;
}

//====================================================================
//=== PrivateLanguageEdit
//====================================================================
PrivateLanguageEdit::PrivateLanguageEdit(const QString& regex,
                                         const QString& initialValue,
                                         QWidget* parent)
  : PrivateEdit(regex, initialValue, parent)
{
  connect(
    this, &QLineEdit::textChanged, this, &PrivateLanguageEdit::manualChange);
}

void
PrivateLanguageEdit::up1()
{
  if (col2 < 't')
    col2++;
  build();
}

void
PrivateLanguageEdit::up2()
{
  if (col3 < 'z')
    col3++;
  build();
}

void
PrivateLanguageEdit::down1()
{
  if (col2 > 'a')
    col2--;
  build();
}

void
PrivateLanguageEdit::down2()
{
  if (col3 > 'a')
    col3--;
  build();
}

void
PrivateLanguageEdit::build()
{
  m_value.clear();
  m_value.append(col1).append(col2).append(col3);
  setText(m_value);
  emit valueChanged(m_value);
}

//====================================================================
//=== PrivateFrame
//====================================================================
PrivateFrame::PrivateFrame(const QString& regex,
                           const QString& initialValue,
                           QWidget* parent)
  : QFrame(parent)
{
  auto layout = new QGridLayout;
  setLayout(layout);
}

QString
PrivateFrame::value() const
{
  return m_edit->text();
}

//====================================================================
//=== PrivateLanguageFrame
//====================================================================
PrivateLanguageFrame::PrivateLanguageFrame(const QString& regex,
                                           const QString& initialValue,
                                           QWidget* parent)
  : PrivateFrame(regex, initialValue, parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());
  auto lbl = new QLabel(
    tr("Private language tags should be one of :<ul>"
       "<li>'i' grandfathered (only use if you are ALREADY using it).</li>"
       "<li>'x' valid but not preferred.</li>"
       "<li>The range 'qaa' to 'qtz' preferred</li></ul>"),
    this);
  layout->addWidget(lbl, 0, 0, 3, 1);

  m_edit = new PrivateLanguageEdit(regex, initialValue, this);
  m_edit->setToolTip(tr("Private language tag. Use the up and down buttons\n"
                        "to change values or enter manually for 'x' or 'i'.",
                        "Private language edit tooltip text"));
  layout->addWidget(m_edit, 0, 1, 2, 1, Qt::AlignVCenter);
  connect(m_edit,
          &PrivateEdit::valueChanged,
          this,
          &PrivateFrame::privateValueChanged);

  QString resourcePath = ":/icons/down-arrow";
  auto downArrow = QPixmap(":/icons/down-arrow");
  auto upArrow = QPixmap(":/icons/up-arrow");

  m_up1Btn = new QToolButton(this);
  m_up1Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up1Btn->setIcon(upArrow);
  m_up1Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up1Btn->setToolTip(
    tr("Second character up",
       "Private language second character up button tooltip text"));
  connect(m_up1Btn,
          &QToolButton::clicked,
          qobject_cast<PrivateLanguageEdit*>(m_edit),
          &PrivateLanguageEdit::up1);
  m_up2Btn = new QToolButton(this);
  m_up2Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up2Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up2Btn->setIcon(upArrow);
  m_up2Btn->setToolTip(
    tr("Third character up",
       "Private language third character up button tooltip text"));
  connect(m_up2Btn,
          &QToolButton::clicked,
          qobject_cast<PrivateLanguageEdit*>(m_edit),
          &PrivateLanguageEdit::up2);
  m_down1Btn = new QToolButton(this);
  m_down1Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down1Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down1Btn->setIcon(downArrow);
  m_down1Btn->setToolTip(
    tr("Second character down",
       "Private language second character down button tooltip text"));
  connect(m_down1Btn,
          &QToolButton::clicked,
          qobject_cast<PrivateLanguageEdit*>(m_edit),
          &PrivateLanguageEdit::down1);
  m_down2Btn = new QToolButton(this);
  m_down2Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down2Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down2Btn->setIcon(downArrow);
  m_down2Btn->setToolTip(
    tr("Third character down",
       "Private language third character down button tooltip text"));
  connect(m_down2Btn,
          &QToolButton::clicked,
          qobject_cast<PrivateLanguageEdit*>(m_edit),
          &PrivateLanguageEdit::down2);
  layout->addWidget(m_up1Btn, 0, 2);
  layout->addWidget(m_up2Btn, 0, 3);
  layout->addWidget(m_down1Btn, 1, 2);
  layout->addWidget(m_down2Btn, 1, 3);
  auto spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer, 2, 1, 1, 2);
}

void
PrivateLanguageFrame::enableEdit(bool enable)
{
  m_edit->setEnabled(enable);
  m_up1Btn->setEnabled(enable);
  m_up2Btn->setEnabled(enable);
  m_down1Btn->setEnabled(enable);
  m_down2Btn->setEnabled(enable);
}

//////====================================================================
//////=== PrivateExtLangFrame
//////====================================================================
// PrivateExtLangFrame::PrivateExtLangFrame(const QString &regex, const QString
// &initialValue, QWidget *parent)
//  : PrivateFrame(regex, initialValue, parent)
//{
//  auto layout = qobject_cast<QGridLayout*>(this->layout());

//  m_edit = new PrivateEdit(regex, initialValue, this);
//  layout->addWidget(m_edit, 1, 0);
//  connect(m_edit, &PrivateEdit::valueChanged, this,
//  &PrivateFrame::privateValueChanged);

//}

// void PrivateExtLangFrame::enableEdit(bool enable)
//{
//  m_edit->setEnabled(enable);
//}

////====================================================================
////=== PrivateScriptFrame
////====================================================================
PrivateScriptFrame::PrivateScriptFrame(const QString& regex,
                                       const QString& initialValue,
                                       QWidget* parent)
  : PrivateFrame(regex, initialValue, parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());

  m_edit = new PrivateEdit(regex, initialValue, this);
  layout->addWidget(m_edit, 1, 0);
  connect(m_edit,
          &PrivateEdit::valueChanged,
          this,
          &PrivateFrame::privateValueChanged);
}

void
PrivateScriptFrame::enableEdit(bool enable)
{
  m_edit->setEnabled(enable);
}

////====================================================================
////=== PrivateRegionFrame
////====================================================================
PrivateRegionFrame::PrivateRegionFrame(const QString& regex,
                                       const QString& initialValue,
                                       QWidget* parent)
  : PrivateFrame(regex, initialValue, parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());

  m_edit = new PrivateEdit(regex, initialValue, this);
  layout->addWidget(m_edit, 1, 0);
  connect(m_edit,
          &PrivateEdit::valueChanged,
          this,
          &PrivateFrame::privateValueChanged);
}

void
PrivateRegionFrame::enableEdit(bool enable)
{
  m_edit->setEnabled(enable);
}

} // end of namespace Private__
