#include "forms/languagetagbuilderdialog.h"
#include "widgets/helpdialogs.h"
#include "utilities/paths.h"
#include "utilities/stringutil.h"
#include "utilities/x11colors.h"

//====================================================================
//=== LanguageTagBuilderDialog
//====================================================================
// QString LanguageTagBuilderDialog::MISSING_PRIMARY_LANGUAGE =
//  tr("[PRIMARY LANGUAGE REQUIRED]",
//     "Missing primary language warning message.");

LanguageTagBuilderDialog::LanguageTagBuilderDialog(Config* config,
                                                   QWidget* parent)
  : QDialog(parent)
  , m_languages(config->languages())
  , m_configDir(QDir(config->configDir()))
  , m_configFilename(config->configFilename())
//  , m_languageTag(MISSING_PRIMARY_LANGUAGE)
{
  Q_INIT_RESOURCE(languagetagbuilderdialogresource);

  setWindowTitle(tr("Language Tag Builder"));

  if (m_languages->languageDescriptions().isEmpty()) {
    m_languages->readFromLocalFile(m_configFilename);
  }

  initGui();
}

void
LanguageTagBuilderDialog::setConfigFile(const QString& filename)
{
  m_configFilename=filename;
  m_languages->readFromLocalFile(m_configFilename);
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
LanguageTagBuilderDialog::clearTag()
{
  //  clearAllTagvalues();
  updateTag(BCP47Language::NO_VALUE);
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
  m_primaryBox = new QGroupBox(
    tr("Primary Language (Required)", "Primary Language box title"), this);
  auto primaryLayout = new QGridLayout;
  m_primaryBox->setLayout(primaryLayout);
  layout->addWidget(m_primaryBox);

  // EXTENSION BOX
  m_extensionBox = new QGroupBox(
    tr("Language Extensions", "Language extension box title"), this);
  auto extensionLayout = new QGridLayout;
  m_extensionBox->setLayout(extensionLayout);
  m_extensionBox->setEnabled(false);
  layout->addWidget(m_extensionBox);

  auto list = m_languages->languageDescriptions();
  m_primaryFilterEdit = new Private__::FilterEdit(list, this);
  // line edit cleared manually
  connect(m_primaryFilterEdit,
          &QLineEdit::textChanged,
          this,
          [=](const QString& text) {
            if (text.isEmpty()) {
              this->updateTag(BCP47Language::NO_VALUE);
              this->showPrivateLanguageFrame(BCP47Language::NO_VALUE);
              m_extensionBox->setEnabled(false);
            } else {
              m_extensionBox->setEnabled(true);
            }
          });
  // return pressed
  connect(m_primaryFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::languageChanged);
  primaryLayout->addWidget(m_primaryFilterEdit, 0, 0);
  auto btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  // combo box cleared with clear button
  connect(btn,
          &QPushButton::clicked,
          m_primaryFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    if (!m_primaryFilterEdit->hasCurrentSelection()) {
      m_extlangFilterEdit->setValues(m_languages->extlangDescriptions());
      m_variantFilterEdit->setValues(m_languages->variantDescriptions());
      this->updateTag(BCP47Language::NO_PRIMARY_LANGUAGE);
      this->showPrivateLanguageFrame(BCP47Language::NO_PRIMARY_LANGUAGE);
    }
  });
  primaryLayout->addWidget(btn, 0, 1);
  primaryLayout->addWidget(m_primaryFilterEdit->comboBox(), 0, 2);

  // HIDDEN PRIVATE LANGUAGE
  m_privateLangFrame =
    new Private__::PrivateLanguageFrame("(q[a-t][a-z])|x|i", "qaa", this);
  m_privateLangFrame->setHidden(true);
  connect(m_privateLangFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          [=]() { updateTag(BCP47Language::PRIVATE_LANGUAGE); });
  primaryLayout->addWidget(m_privateLangFrame, 1, 0, 1, 3);

  // EXTLANG
  int row = 0;
  m_extlangFilterEdit =
    new Private__::FilterEdit(m_languages->extlangDescriptions(), this);
  connect(m_extlangFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  connect(m_extlangFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  extensionLayout->addWidget(
    new QLabel(tr("Ext Lang", "EXTLANG label title"), this), row, 0);
  extensionLayout->addWidget(m_extlangFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_extlangFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    updateTag(BCP47Language::NO_EXTENDED_LANGUAGE);
  });
  extensionLayout->addWidget(btn, row, 2);
  extensionLayout->addWidget(m_extlangFilterEdit->comboBox(), row, 3);
  m_usePreferredBtn = new QPushButton(
    tr("Use Preferred", "Use preferred option button text."), this);
  m_usePreferredBtn->setCheckable(true);
  m_usePreferredBtn->setEnabled(false);
  //  connect(m_usePreferredBtn, &QPushButton::clicked, this, [=](bool checked)
  //  {
  //    // TODO
  //    auto editText = m_extlangFilterEdit->currentText();
  //    auto extLanguage = m_languages->extlangFromDescription(editText);
  //    if (extLanguage) {
  //      if (checked) {

  //      }
  //    }
  //  });
  extensionLayout->addWidget(m_usePreferredBtn, row, 4);

  row++;
  // clang-format off
  auto lbl = new QLabel(
        tr("Please note: The IANA language file does NOT limit either Scripts OR Regions to any particular language.\n"
           "This means that you can select nonsense regions or scripts to a primary language.\n"
           "So choose these with care as for instance:\n"
           "'en-Tglg-Kuman' English(language) Tagalog(script) Papua New Guinea(region)\n"
           "is technically valid but makes little or no real sense.",
           "Script/Region validity note text"), this);
  // clang-format on
  extensionLayout->addWidget(lbl, row, 0, 1, 4);

  row++;
  m_scriptFilterEdit =
    new Private__::FilterEdit(m_languages->scriptDescriptions(), this);
  connect(m_scriptFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  connect(m_scriptFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  extensionLayout->addWidget(
    new QLabel(tr("Script", "Script label title"), this), row, 0);
  extensionLayout->addWidget(m_scriptFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_scriptFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    updateTag(BCP47Language::NO_SCRIPT);
    showPrivateScriptFrame(BCP47Language::NO_SCRIPT);
  });
  extensionLayout->addWidget(btn, row, 2);
  extensionLayout->addWidget(m_scriptFilterEdit->comboBox(), row, 3);

  row++;
  // SCRIPT
  m_privateScriptFrame = new Private__::PrivateScriptFrame(
    "Qa(([a][a-z])|([b][a-x]))", "Qaaa", this);
  m_privateScriptFrame->setHidden(true);
  connect(m_privateScriptFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          [=]() { updateTag(BCP47Language::PRIVATE_SCRIPT); });
  extensionLayout->addWidget(m_privateScriptFrame, row, 0, 1, 3);

  row++;
  m_regionFilterEdit =
    new Private__::FilterEdit(m_languages->regionDescriptions(), this);
  connect(m_regionFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::regionChanged);
  // line edit cleared manually
  connect(m_regionFilterEdit,
          &QLineEdit::textChanged,
          this,
          [=](const QString& text) {
            if (text.isEmpty()) {
              updateTag(BCP47Language::NO_REGION);
              showPrivateRegionFrame(BCP47Language::NO_REGION);
            }
          });
  extensionLayout->addWidget(
    new QLabel(tr("Region", "Region label title"), this), row, 0);
  extensionLayout->addWidget(m_regionFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_regionFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    updateTag(BCP47Language::NO_REGION);
    showPrivateRegionFrame(BCP47Language::NO_REGION);
  });
  extensionLayout->addWidget(btn, row, 2);
  extensionLayout->addWidget(m_regionFilterEdit->comboBox(), row, 3);

  row++;
  // HIDDEN PRIVATE REGION
  m_privateRegionFrame =
    new Private__::PrivateRegionFrame("AA|ZZ|Q[M-Z]|X[A-Z])", "AA", this);
  m_privateRegionFrame->setHidden(true);
  connect(m_privateRegionFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          [=]() { updateTag(BCP47Language::PRIVATE_REGION); });
  extensionLayout->addWidget(m_privateRegionFrame, row, 0, 1, 3);

  auto resultBox =
    new QGroupBox(tr("Language Tag", "Tag display box title"), this);
  auto resultLayout = new QGridLayout;
  resultLayout->setColumnStretch(0, 2);
  resultLayout->setRowStretch(0, 2);
  resultBox->setLayout(resultLayout);
  layout->addWidget(resultBox);

  // VARIANT
  row++;
  m_variantFilterEdit =
    new Private__::FilterEdit(m_languages->variantDescriptions(), this);
  connect(m_variantFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::variantChanged);
  connect(m_variantFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::variantChanged);
  extensionLayout->addWidget(
    new QLabel(tr("Variant", "VARIANT label title"), this), row, 0);
  extensionLayout->addWidget(m_variantFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_variantFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    updateTag(BCP47Language::NO_VARIANT_LANGUAGE);
  });
  extensionLayout->addWidget(btn, row, 2);
  extensionLayout->addWidget(m_variantFilterEdit->comboBox(), row, 3);

  m_reportLbl = new QLabel(this);
  resultLayout->addWidget(m_reportLbl, 0, 0, 1, 2);
  m_resultLbl = new Private__::LanguageLabel(m_languages, this);
  connect(m_resultLbl,
          &Private__::LanguageLabel::textChanged,
          this,
          [=](const QString& text) { m_resultLbl->setCurrentTag(text); });
  resultLayout->addWidget(m_resultLbl, 1, 0);

  auto clearBtn = new QPushButton(tr("Clear", "Clear tag button label"), this);
  clearBtn->setToolTip(tr("Clears all values, you will need to start again.",
                          "Tag clear button tooltip"));
  resultLayout->addWidget(clearBtn, 1, 2);
  connect(
    clearBtn, &QPushButton::clicked, this, &LanguageTagBuilderDialog::clearTag);

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
LanguageTagBuilderDialog::showPrivateLanguageFrame(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_LANGUAGE) {
    m_privateLangFrame->show();
  } else {
    m_privateLangFrame->hide();
  }
}

void
LanguageTagBuilderDialog::showPrivateScriptFrame(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_SCRIPT) {
    m_privateScriptFrame->show();
  } else {
    m_privateScriptFrame->hide();
  }
}

void
LanguageTagBuilderDialog::showPrivateRegionFrame(
  BCP47Language::TagType languageType)
{
  if (languageType == BCP47Language::PRIVATE_REGION) {
    m_privateRegionFrame->show();
  } else {
    m_privateRegionFrame->hide();
  }
}

void
LanguageTagBuilderDialog::languageChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_PRIMARY_LANGUAGE;
  if (m_primaryFilterEdit->hasCurrentSelection()) {
    m_extensionBox->setEnabled(true);
    auto editText = m_primaryFilterEdit->currentSelection();
    m_language = m_languages->languageFromDescription(editText);
    if (m_language) {

      // find matching extlang's and variant's if any for language and update
      // their values.
      auto prefixesForLanguage =
        m_languages->extlangsWithPrefix(m_language->subtag());
      m_extlangFilterEdit->setValues(prefixesForLanguage);

      auto variantsForLanguage =
        m_languages->variantsWithPrefix(m_language->subtag());
      m_variantFilterEdit->setValues(variantsForLanguage);

      if (m_language->description() == "Private use") {
        languageType = BCP47Language::PRIVATE_LANGUAGE;
      } else {
        languageType = BCP47Language::PRIMARY_LANGUAGE;
      }
    } else {
      m_extlangFilterEdit->clear();
      m_variantFilterEdit->clear();
    }
  } /*else {
//    m_languageTag = MISSING_PRIMARY_LANGUAGE;
  }*/
  showPrivateLanguageFrame(languageType);
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::extlangChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_VARIANT_LANGUAGE;
  if (m_extlangFilterEdit->hasCurrentSelection()) {
    if (m_extlangFilterEdit->hasIndexChanged()) {
      m_extlangFilterEdit->clearIndexChanged();
    }
    auto editText = m_extlangFilterEdit->currentSelection();
    auto extLanguage = m_languages->extlangFromDescription(editText);
    if (extLanguage->hasPreferredValue()) {
      m_usePreferredBtn->setEnabled(true);
      auto reportText =
        tr("The language dialect has a preferred LANGUAGE tag "
           "(%1), this should be used\n"
           "in preference to the 'LANGUAGE-EXTENDED' format (%2-%3).");
      setReport(reportText.arg(extLanguage->preferredValue(),
                               extLanguage->prefix().at(0),
                               extLanguage->subtag()));
    } else {
      m_usePreferredBtn->setEnabled(false);
      auto reportText =
        tr("The extended language has a preferred LANGUAGE tag "
           "(%1), this should be used\n"
           "in preference to the 'LANGUAGE-EXTENDED' format (%2-%3).");
      setReport(reportText.arg(extLanguage->preferredValue(),
                               extLanguage->macrolanguageName(),
                               extLanguage->subtag()));
    }
    if (extLanguage) {
      languageType = BCP47Language::EXTENDED_LANGUAGE;
    }
  }
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::scriptChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_SCRIPT;
  if (m_scriptFilterEdit->hasCurrentSelection()) {
    if (m_scriptFilterEdit->hasIndexChanged()) {
      m_scriptFilterEdit->clearIndexChanged();
    }
    auto editText = m_scriptFilterEdit->currentSelection();
    auto script = m_languages->scriptFromDescription(editText);
    if (script->hasSuppressScriptLang()) {
      m_scriptFilterEdit->setUnavailableText(
        tr("Script is suppressed for language %1",
           "Requested script tag has been suppressed by ISO639")
          .arg(script->description()));
    } else {
      m_scriptFilterEdit->setUnavailableText(
        tr(" == No script available == ",
           "Displayed when no possible value exists"));
      if (script) {
        if (script->description() == "Private use") {
          languageType = BCP47Language::PRIVATE_SCRIPT;
        } else {
          languageType = BCP47Language::SCRIPT_LANGUAGE;
        }
      }
      showPrivateScriptFrame(languageType);
      updateTag(languageType);
    }
  }
}

void
LanguageTagBuilderDialog::regionChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_REGION;
  if (m_regionFilterEdit->hasCurrentSelection()) {
    if (m_regionFilterEdit->hasIndexChanged()) {
      m_regionFilterEdit->clearIndexChanged();
    }
    auto editText = m_regionFilterEdit->currentSelection();
    auto region = m_languages->regionFromDescription(editText);
    if (region) {
      if (region->description() == "Private use") {
        languageType = BCP47Language::PRIVATE_REGION;
      } else {
        languageType = BCP47Language::REGIONAL_LANGUAGE;
      }
    }
  }
  showPrivateRegionFrame(languageType);
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::variantChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_VARIANT_LANGUAGE;
  if (m_variantFilterEdit->hasCurrentSelection()) {
    if (m_variantFilterEdit->hasIndexChanged()) {
      m_variantFilterEdit->clearIndexChanged();
    }
    auto language = m_languages->variantFromDescription(
      m_variantFilterEdit->currentSelection());
    m_usePreferredBtn->setEnabled(false);
    if (language) {
      languageType = BCP47Language::VARIANT_LANGUAGE;
    }
  }
  updateTag(languageType);
}

// void
// LanguageTagBuilderDialog::clearAllTagvalues()
//{}

QString
LanguageTagBuilderDialog::calcExtlangResult()
{
  if (m_extlangFilterEdit->hasCurrentSelection()) {
    auto text = m_extlangFilterEdit->currentSelection();
    auto extlang = m_languages->extlangFromDescription(text);
    if (extlang) {
      if (m_usePreferredBtn->isChecked()) {
        return QString("-%1").arg(extlang->preferredValue());
      } else {
        return QString("-%1").arg(extlang->subtag());
      }
    }
  }
  return QString();
}

QString
LanguageTagBuilderDialog::calcScriptResult(BCP47Language::TagType type)
{
  if (m_scriptFilterEdit->hasCurrentSelection()) {
    auto text = m_scriptFilterEdit->currentSelection();
    auto script = m_languages->scriptFromDescription(text);
    if (type == BCP47Language::PRIVATE_SCRIPT) {
      return QString("-%1").arg(m_privateScriptFrame->value());
    } else {
      if (script) {
        text = script->subtag();
        if (script->description() == "Private use" &&
            text == "Qaaa..Qabx") // just in case
          text = "Qaaa";
        return QString("-%1").arg(text);
      }
    }
  }
  return QString();
}

QString
LanguageTagBuilderDialog::calcRegionResult(BCP47Language::TagType type)
{
  if (m_regionFilterEdit->hasCurrentSelection()) {
    auto text = m_regionFilterEdit->currentSelection();
    auto region = m_languages->regionFromDescription(text);
    if (type == BCP47Language::PRIVATE_REGION) {
      return QString("-%1").arg(m_privateRegionFrame->value());
    } else {
      if (region) {
        text = region->subtag();
        if (region->description() == "Private use") { // just in case
          if (text == "XA..XZ") {
            text = "XA";
          } else if (text == "QM..QZ") {
            text = "QM";
          } // the others are single case AA and ZZ
          return QString("-%1").arg(text);
        }
      }
      return QString("-%1").arg(m_regionFilterEdit->currentSelection());
    }
  }
  return QString();
}

QString LanguageTagBuilderDialog::calcVariantResult(
  /*BCP47Language::TagType type*/)
{
  if (m_variantFilterEdit->hasCurrentSelection()) {
    return QString("-%1").arg(m_variantFilterEdit->currentSelection());
  }
  return QString();
}

QString
LanguageTagBuilderDialog::calcLanguageResult(BCP47Language::TagType type)
{
  if (m_primaryFilterEdit->hasCurrentSelection()) {
    auto text = m_primaryFilterEdit->currentSelection();
    auto language = m_languages->languageFromDescription(text);
    if (type == BCP47Language::PRIVATE_LANGUAGE) {
      return m_privateLangFrame->value();
    } else {
      if (language) {
        text = language->subtag();
        if (language->description() == "Private use") {
          if (text == "qaa..qtz")
            text = "qaa";
        }
        return text;
      }
      return m_primaryFilterEdit->currentSelection();
    }
  }
  return QString();
}

void
LanguageTagBuilderDialog::updateTag(BCP47Language::TagType type)
{
  auto result = calcLanguageResult(type);

  if (m_extensionBox->isEnabled()) {
    result += calcExtlangResult();
    result += calcScriptResult(type);
    result += calcRegionResult(type);
    result += calcVariantResult();
  }

  m_resultLbl->setCurrentTag(result);
}

QString
LanguageTagBuilderDialog::tag()
{
  return m_resultLbl->currentTag();
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
    tr("Scripts", "Help text for scripts."),
    tr("&Unfortunately BCP47 does not limit the various scripts to specific\n"
    "languages, therefore such tags as 'un-Hanja' (English with Korean characters),\n"
    "whilst not really valid, are still viable. Therefore it falls to the user "
    "to choose a script with care."));
  dlg->addAdditionalHelpPage(
    tr("Private use Languages :", "Private primary language help page title"),
    tr(
      "Private use language tags area allowable when used in agreement\n"
      "between agreed parties for internal use. Available values are in the\n"
      "range 'qaa' to 'qtz.\n\n"
      "Alternatively you can use the characters 'x' followed by private\n"
      "subtag values. Subtags following the 'x' will be ignored by\n"
      "non-private users.\n\n"
      "You can also use 'i' in the same way as the 'x', however this value\n"
      "has been grandfathered and should no longer be used in new sites.\n\n"
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
FilterEdit::FilterEdit(QStringList items, LanguageTagBuilderDialog* parent)
  : QLineEdit(parent)
  , m_filterText(tr("== Filter ==", "FilterEdit initial empty value text"))
  , m_selectionBox(new FilterComboBox(parent))
  , m_parent(parent)
{
  auto model = new QStringListModel();
  model->setStringList(items);
  auto proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(model);
  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  m_selectionBox->setModel(proxyModel);
  connect(
    this, &QLineEdit::textChanged, this, &FilterEdit::primaryFilterChanged);
  connect(m_selectionBox,
          qOverload<int>(&QComboBox::activated),
          this,
          &FilterEdit::activated);
  connect(m_selectionBox,
          qOverload<int>(&QComboBox::currentIndexChanged),
          this,
          &FilterEdit::setIndexChanged);
  //  if (m_required) {
  //    connect(
  //      m_required, &QCheckBox::clicked, this, [=](bool checked) {
  //      if (checked)
  //        emit FilterEdit::stateChanged(Qt::Checked);
  //      else
  //        emit FilterEdit::stateChanged(Qt::Unchecked);
  //    });
  //  }
}

QComboBox*
FilterEdit::comboBox()
{
  return m_selectionBox;
}

// QCheckBox*
// FilterEdit::checkBox()
//{
//  return m_required;
//}

QSortFilterProxyModel*
FilterEdit::model()
{
  return qobject_cast<QSortFilterProxyModel*>(m_selectionBox->model());
}

void
FilterEdit::setEnabled(bool enable)
{
  QLineEdit::setEnabled(enable);
  m_selectionBox->setEnabled(enable);
}

void
FilterEdit::clearValues()
{
  clear();
  primaryFilterChanged(QString());
  m_selectionBox->setCurrentIndex(-1);
}

void
FilterEdit::setValue(const QString& value)
{
  m_selectionBox->clear();
  m_selectionBox->addItem(value);
}

void
FilterEdit::setValues(const QStringList& values)
{
  m_selectionBox->clear();
  m_selectionBox->addItems(values);
}

bool
FilterEdit::hasCurrentSelection()
{
  auto index = m_selectionBox->currentIndex();
  return (index != -1);
}

QString
FilterEdit::currentSelection()
{
  return m_selectionBox->currentText();
}

bool
FilterEdit::isEmpty()
{
  return text().isEmpty();
}

void
FilterEdit::setFilterText(const QString& text)
{
  m_filterText = text;
}

void
FilterEdit::setUnavailableText(const QString& text)
{
  m_selectionBox->setUnavailableText(text);
}

bool
FilterEdit::hasIndexChanged()
{
  return m_currentIndexChanged;
}

void
FilterEdit::clearIndexChanged()
{
  m_currentIndexChanged = false;
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
//  m_selectionBox->setCurrentIndex(0);
  if (m_selectionBox->count() == 1) {
    emit m_selectionBox->activated(0);
  }
}

void
FilterEdit::setIndexChanged(int index)
{
  if (index != -1 && index != m_currentIndex) {
    m_currentIndex = index;
    m_currentIndexChanged = true;
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
  setType(qaa);
  connect(
    this, &QLineEdit::textChanged, this, &PrivateLanguageEdit::manualChange);
}

void
PrivateLanguageEdit::up2()
{
  if (col2 < 't')
    col2++;
  build();
}

void
PrivateLanguageEdit::up3()
{
  if (col3 < 'z')
    col3++;
  build();
}

void
PrivateLanguageEdit::down2()
{
  if (col2 > 'a')
    col2--;
  build();
}

void
PrivateLanguageEdit::down3()
{
  if (col3 > 'a')
    col3--;
  build();
}

void
PrivateLanguageEdit::setType(PrivateLanguageEdit::Type type)
{
  m_type = type;
  switch (type) {
    case i:
      col1 = 'i';
      build();
      break;
    case x:
      col1 = 'x';
      build();
      break;
    case qaa:
      if (m_storedValue.isEmpty()) {
        col1 = 'q';
        col2 = 'a';
        col3 = 'a';
      } else {
        col1 = m_storedValue.at(0).toLatin1();
        col2 = m_storedValue.at(1).toLatin1();
        col3 = m_storedValue.at(2).toLatin1();
      }
      build();
      break;
  }
}

void
PrivateLanguageEdit::build()
{
  switch (m_type) {
    case i:
    case x:
      m_value = col1;
      break;
    case qaa:
      m_value = col1;
      m_value.append(col2).append(col3);
      m_storedValue = m_value;
      break;
  }
  setText(m_value);
  emit valueChanged(m_value);
}

void
PrivateLanguageEdit::manualChange(const QString& value)
{
  if (value == "i" || value == "x") {
    emit valueChanged(value);
  } else {
    m_storedValue = value.left(3);
    if (m_storedValue.length() == 3) {
      emit valueChanged(m_storedValue);
    }
  }
}

//====================================================================
//=== PrivateScriptEdit
//====================================================================
PrivateScriptEdit::PrivateScriptEdit(const QString& regex,
                                     const QString& initialValue,
                                     QWidget* parent)
  : PrivateEdit(regex, initialValue, parent)
{}

void
PrivateScriptEdit::up3()
{
  if (col3 < 'b')
    col3++;
  if (col3 == 'b' && col4 > 'x')
    col4 = 'x';
  build();
}

void
PrivateScriptEdit::up4()
{
  if (col3 == 'b') {
    if (col4 < 'x')
      col4++;
  } else {
    if (col4 < 'z')
      col4++;
  }
  build();
}

void
PrivateScriptEdit::down3()
{
  if (col3 > 'a')
    col3--;
  build();
}

void
PrivateScriptEdit::down4()
{
  if (col4 > 'a')
    col4--;
  build();
}

void
PrivateScriptEdit::build()
{
  m_value = col1;
  m_value.append(col2).append(col3).append(col4);
  setText(m_value);
  emit valueChanged(m_value);
}

//====================================================================
//=== PrivateRegionEdit
//====================================================================
PrivateRegionEdit::PrivateRegionEdit(const QString& regex,
                                     const QString& initialValue,
                                     QWidget* parent)
  : PrivateEdit(regex, initialValue, parent)
{
  setType(AA);
  connect(
    this, &QLineEdit::textChanged, this, &PrivateRegionEdit::manualChange);
}

void
PrivateRegionEdit::up()
{
  switch (m_type) {
    case QM:
    case XA:
      if (col2 < 'Z') {
        col2++;
      }
      build();
      break;
    default:
      break;
  }
}

void
PrivateRegionEdit::down()
{
  switch (m_type) {
    case QM:
      if (col2 > 'M') {
        col2--;
      }
      build();
      break;
    case XA:
      if (col2 > 'A') {
        col2--;
      }
      build();
      break;
    default:
      break;
  }
}

void
PrivateRegionEdit::setType(PrivateRegionEdit::Type type)
{
  m_type = type;
  switch (type) {
    case AA:
    case ZZ:
      build();
      break;
    case QM:
      if (m_storedValueQ.isEmpty()) {
        col1 = 'Q';
        col2 = 'M';
      } else {
        col1 = m_storedValueQ.at(0).toLatin1();
        col2 = m_storedValueQ.at(1).toLatin1();
      }
      build();
      break;
    case XA:
      if (m_storedValueX.isEmpty()) {
        col1 = 'X';
        col2 = 'A';
      } else {
        col1 = m_storedValueX.at(0).toLatin1();
        col2 = m_storedValueX.at(1).toLatin1();
      }
      build();
      break;
  }
}

void
PrivateRegionEdit::build()
{
  m_value.clear();
  switch (m_type) {
    case AA:
      m_value = "AA";
      break;
    case QM:
      m_value.append(col1).append(col2);
      m_storedValueQ = m_value;
      break;
    case XA:
      m_value.append(col1).append(col2);
      m_storedValueX = m_value;
      break;
    case ZZ:
      m_value = "ZZ";
      break;
  }
  setText(m_value);
  emit valueChanged(m_value);
}

void
PrivateRegionEdit::manualChange(const QString& value)
{
  if (value == "AA" || value == "ZZ") {
    emit valueChanged(value);
  } else {
    if (m_type == PrivateRegionEdit::QM) {
      m_storedValueQ = value.left(2);
      if (m_storedValueQ.length() == 2) {
        emit valueChanged(m_storedValueQ);
      }
    } else if (m_type == PrivateRegionEdit::XA) {
      m_storedValueX = value.left(2);
      if (m_storedValueX.length() == 2) {
        emit valueChanged(m_storedValueX);
      }
    }
  }
}

//====================================================================
//=== PrivateFrame
//====================================================================
PrivateFrame::PrivateFrame(QWidget* parent)
  : QFrame(parent)
{
  auto layout = new QGridLayout;
  setLayout(layout);
}

QString
PrivateFrame::value() const
{
  return m_value;
}

void
PrivateFrame::setValue(const QString& value)
{
  m_value = value;
  emit privateValueChanged(value);
}

//====================================================================
//=== PrivateLanguageFrame
//====================================================================
PrivateLanguageFrame::PrivateLanguageFrame(const QString& regex,
                                           const QString& initialValue,
                                           QWidget* parent)
  : PrivateFrame(parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());
  layout->setVerticalSpacing(0);
  auto lbl =
    new QLabel(tr("Private language tags should be one of :", ""), this);
  layout->addWidget(lbl, 0, 0);

  // This needs to be created BEFORE radio buttons.
  auto edit = new PrivateLanguageEdit(regex, initialValue, this);
  m_edit = edit;
  m_edit->setToolTip(tr("Private language tag. Use the up and down buttons\n"
                        "to change values or enter manually for 'x' or 'i'.",
                        "Private language edit tooltip text"));
  connect(
    edit, &PrivateEdit::valueChanged, this, &PrivateLanguageFrame::setValue);
  m_value = edit->value();

  m_iBtn = new QRadioButton(
    tr("'i' grandfathered (only use if you are ALREADY using it)."), this);
  connect(m_iBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateLanguageEdit::i);
  });
  layout->addWidget(m_iBtn, 1, 0);

  m_xBtn = new QRadioButton(tr("'x' valid but not preferred."), this);
  connect(m_xBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateLanguageEdit::x);
  });
  layout->addWidget(m_xBtn, 2, 0);

  m_qBtn = new QRadioButton(tr("The range 'qaa' to 'qtz' preferred"), this);
  m_qBtn->setChecked(true);
  connect(m_qBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateLanguageEdit::qaa);
  });
  layout->addWidget(m_qBtn, 3, 0);

  auto downArrow = QPixmap(":/icons/down-arrow");
  auto upArrow = QPixmap(":/icons/up-arrow");

  m_up1Btn = new QToolButton(this);
  m_up1Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up1Btn->setIcon(upArrow);
  m_up1Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up1Btn->setToolTip(
    tr("Second character up",
       "Private language second character up button tooltip text"));
  connect(m_up1Btn, &QToolButton::clicked, edit, &PrivateLanguageEdit::up2);

  m_up2Btn = new QToolButton(this);
  m_up2Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up2Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up2Btn->setIcon(upArrow);
  m_up2Btn->setToolTip(
    tr("Third character up",
       "Private language third character up button tooltip text"));
  connect(m_up2Btn, &QToolButton::clicked, edit, &PrivateLanguageEdit::up3);

  m_down1Btn = new QToolButton(this);
  m_down1Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down1Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down1Btn->setIcon(downArrow);
  m_down1Btn->setToolTip(
    tr("Third character up",
       "Private language third character up button tooltip text"));
  connect(m_down1Btn, &QToolButton::clicked, edit, &PrivateLanguageEdit::down2);

  m_down2Btn = new QToolButton(this);
  m_down2Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down2Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down2Btn->setIcon(downArrow);
  m_down2Btn->setToolTip(
    tr("Third character down",
       "Private language third character down button tooltip text"));
  connect(m_down2Btn, &QToolButton::clicked, edit, &PrivateLanguageEdit::down3);

  layout->addWidget(edit, 0, 1, 2, 1, Qt::AlignVCenter);
  // shouldn't need this but for some reason if I don't m_down1Btn doesn't work.
  layout->addWidget(new QWidget(this), 0, 2, 4, 1);
  layout->addWidget(m_up1Btn, 0, 3);
  layout->addWidget(m_up2Btn, 0, 4);
  layout->addWidget(m_down1Btn, 1, 3);
  layout->addWidget(m_down2Btn, 1, 4);

  auto pLayout = new QGridLayout;
  lbl = new QLabel(
    tr("Enter private tags : ", "Private non-standard tags label"), this);
  pLayout->addWidget(lbl, 0, 0);
  auto nonStandardEdit = new QLineEdit(this);
  nonStandardEdit->setToolTip(
    tr("Private non-standard tags should be entered\n"
       "here. For example klingon WITHOUT initial '-'\n"
       "as this is added automatically."));
  pLayout->addWidget(nonStandardEdit, 0, 1);
  connect(nonStandardEdit,
          &QLineEdit::textChanged,
          this,
          &PrivateLanguageFrame::setExtension);
  auto clearBtn = new QPushButton(tr("Clear", "Clear edit text"), this);
  // lineedit cleared with clear button
  connect(clearBtn, &QPushButton::clicked, nonStandardEdit, &QLineEdit::clear);
  pLayout->addWidget(clearBtn, 0, 2);
  layout->addLayout(pLayout, 2, 1, 1, 2);
}

void
PrivateLanguageFrame::setValue(const QString& value)
{
  PrivateFrame::setValue(
    value.toLower()); // primary language tags are generally in lower case.
  if (value.compare("i", Qt::CaseInsensitive) == 0) {
    m_iBtn->setChecked(true);
  } else if (value.compare("x", Qt::CaseInsensitive) == 0) {
    m_xBtn->setChecked(true);
  } else {
    m_qBtn->setChecked(true);
  }
  emit privateValueChanged(buildString());
}

void
PrivateLanguageFrame::setExtension(const QString& extension)
{
  m_extension = extension;
  emit privateValueChanged(buildString());
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

void
PrivateLanguageFrame::show()
{
  emit privateValueChanged(buildString());
  QWidget::show();
}

QString
PrivateLanguageFrame::value() const
{
  return buildString();
}

QString
PrivateLanguageFrame::buildString() const
{
  QString value;
  if (!m_extension.isEmpty())
    value += QString("%1-%2").arg(m_value, m_extension);
  else
    value = m_value;
  return value;
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
  : PrivateFrame(/*regex, initialValue, */ parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());
  layout->setVerticalSpacing(0);
  auto lbl = new QLabel(
    tr("Private script tags should be in the range 'Qaaa' to 'Qabx'"), this);
  layout->addWidget(lbl, 0, 0, 1, 3);

  auto edit = new PrivateScriptEdit(regex, initialValue, this);
  m_edit = edit;
  edit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  m_edit->setToolTip(tr("Private section tag. Use the up and down buttons\n"
                        "to change values or enter manually for 'Q***'.",
                        "Private region edit tooltip text"));
  connect(m_edit, &PrivateEdit::valueChanged, this, &PrivateFrame::setValue);

  auto downArrow = QPixmap(":/icons/down-arrow");
  auto upArrow = QPixmap(":/icons/up-arrow");

  m_up3Btn = new QToolButton(this);
  m_up3Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up3Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up3Btn->setIcon(upArrow);
  m_up3Btn->setToolTip(
    tr("Third character up",
       "Private language third character up button tooltip text"));
  connect(m_up3Btn, &QToolButton::clicked, edit, &PrivateScriptEdit::up3);

  m_up4Btn = new QToolButton(this);
  m_up4Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_up4Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_up4Btn->setIcon(upArrow);
  m_up4Btn->setToolTip(
    tr("Fourth character up",
       "Private language fourth character up button tooltip text"));
  connect(m_up4Btn, &QToolButton::clicked, edit, &PrivateScriptEdit::up4);

  m_down3Btn = new QToolButton(this);
  m_down3Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down3Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down3Btn->setIcon(downArrow);
  m_down3Btn->setToolTip(
    tr("Third character down",
       "Private language third character down button tooltip text"));
  connect(m_down3Btn, &QToolButton::clicked, edit, &PrivateScriptEdit::down3);

  m_down4Btn = new QToolButton(this);
  m_down4Btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_down4Btn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_down4Btn->setIcon(downArrow);
  m_down4Btn->setToolTip(
    tr("Fourth character down",
       "Private language fourth character down button tooltip text"));
  connect(m_down4Btn, &QToolButton::clicked, edit, &PrivateScriptEdit::down4);

  layout->addWidget(edit, 1, 0, 2, 1, Qt::AlignVCenter);
  // shouldn't need this but for some reason if I don't m_up4Btn and m_down4Btn
  // don't work.
  layout->addWidget(new QWidget(this), 1, 2);
  layout->addWidget(new QWidget(this), 2, 2);
  layout->addWidget(m_up3Btn, 1, 1);
  layout->addWidget(m_up4Btn, 1, 3);
  layout->addWidget(m_down3Btn, 2, 1);
  layout->addWidget(m_down4Btn, 2, 3);

  //  auto spacer = new QWidget();
  //  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  layout->addWidget(spacer, 3, 1, 2, 3);
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
  : PrivateFrame(parent)
{
  auto layout = qobject_cast<QGridLayout*>(this->layout());
  layout->setVerticalSpacing(0);
  auto lbl = new QLabel(tr("Private region tags should be one of :"), this);
  layout->addWidget(lbl, 0, 0);

  // This needs to be created BEFORE radio buttons.
  auto edit = new PrivateRegionEdit(regex, initialValue, this);
  m_edit = edit;
  edit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  edit->setToolTip(tr("Private region tag. Use the up and down buttons\n"
                      "to change values or enter manually for 'Q*' or 'X*'.",
                      "Private region edit tooltip text"));
  connect(
    m_edit, &PrivateEdit::valueChanged, this, &PrivateRegionFrame::setValue);

  m_aaBtn = new QRadioButton(tr("AA"), this);
  m_aaBtn->setChecked(true);
  connect(m_aaBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateRegionEdit::AA);
  });
  layout->addWidget(m_aaBtn, 1, 0);

  m_qmBtn = new QRadioButton(tr("The range QM - QZ"), this);
  connect(m_qmBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateRegionEdit::QM);
  });
  layout->addWidget(m_qmBtn, 2, 0);

  m_xaBtn = new QRadioButton(tr("The range XA - XZ"), this);
  connect(m_xaBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateRegionEdit::XA);
  });
  layout->addWidget(m_xaBtn, 3, 0);

  m_zzBtn = new QRadioButton(tr("ZZ"), this);
  connect(m_zzBtn, &QRadioButton::clicked, this, [=]() {
    edit->setType(PrivateRegionEdit::ZZ);
  });
  layout->addWidget(m_zzBtn, 4, 0);

  layout->addWidget(m_edit, 1, 1, 2, 1, Qt::AlignVCenter);

  auto downArrow = QPixmap(":/icons/down-arrow");
  auto upArrow = QPixmap(":/icons/up-arrow");

  m_upBtn = new QToolButton(this);
  m_upBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_upBtn->setIcon(upArrow);
  m_upBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_upBtn->setToolTip(
    tr("Second character up",
       "Private language second character up button tooltip text"));
  connect(m_upBtn, &QToolButton::clicked, edit, &PrivateRegionEdit::up);
  m_downBtn = new QToolButton(this);
  m_downBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_downBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
  m_downBtn->setIcon(downArrow);
  m_downBtn->setToolTip(
    tr("Second character down",
       "Private language second character down button tooltip text"));
  connect(m_downBtn, &QToolButton::clicked, edit, &PrivateRegionEdit::down);
  layout->addWidget(m_upBtn, 1, 2);
  layout->addWidget(m_downBtn, 2, 2);

  auto spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(spacer, 3, 1, 2, 2);
}

void
PrivateRegionFrame::enableEdit(bool enable)
{
  m_edit->setEnabled(enable);
}

void
PrivateRegionFrame::show()
{
  emit privateValueChanged(value());
  QWidget::show();
}

void
PrivateRegionFrame::setValue(const QString& value)
{
  PrivateFrame::setValue(
    value.toUpper()); // in this case values are generally in upper case
  if (value.compare("AA", Qt::CaseInsensitive) == 0) {
    m_aaBtn->setChecked(true);
  } else if (value.compare("ZZ", Qt::CaseInsensitive) == 0) {
    m_zzBtn->setChecked(true);
  } else if (value.startsWith("Q", Qt::CaseInsensitive)) {
    m_qmBtn->setChecked(true);
  } else if (value.startsWith("X", Qt::CaseInsensitive)) {
    m_xaBtn->setChecked(true);
  }
  emit privateValueChanged(m_value);
}

////====================================================================
////=== LanguageLabel
////====================================================================
LanguageLabel::LanguageLabel(BCP47Languages* languages, QWidget* parent)
  : QLineEdit(parent)
  , m_languages(languages)
  , m_document(new QTextDocument(this))
  , m_highlighter(new LanguageTextHighlighter(m_document))
{
  m_textColor.setForeground(QColorConstants::Black);
  m_primaryLanguageColor.setForeground(QColorConstants::X11::limegreen);
  m_privateLanguageColor.setForeground(QColorConstants::X11::darkgreen);
  m_extlangColor.setForeground(QColorConstants::X11::limegreen);
  m_scriptColor.setForeground(QColorConstants::X11::limegreen);
  m_regionColor.setForeground(QColorConstants::X11::limegreen);
  m_variantColor.setForeground(QColorConstants::X11::limegreen);
  m_badColor.setForeground(QColorConstants::Red);
  m_badColor.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  m_badPositionColor.setForeground(QColorConstants::X11::orange);
  m_badPositionColor.setUnderlineStyle(QTextCharFormat::WaveUnderline);

  setMouseTracking(true);
  setAttribute(Qt::WA_Hover);
}

void
LanguageLabel::setPrimaryLanguageColor(const QColor& primaryLanguageColor)
{
  m_primaryLanguageColor.setForeground(primaryLanguageColor);
}

void
LanguageLabel::setPrivateLanguageColor(
  const QTextCharFormat& privateLanguageColor)
{
  m_privateLanguageColor = privateLanguageColor;
}

void
LanguageLabel::setTextColor(const QTextCharFormat& textColor)
{
  m_textColor = textColor;
}

QString
LanguageLabel::currentTag() const
{
  return m_currentTag;
}

void
LanguageLabel::setCurrentTag(const QString& text)
{
  m_currentTag = StringUtil::removeWhitespace(text);
  parseAndHighlightTag();
}

void
LanguageLabel::paintEvent(QPaintEvent* event)
{
  QLineEdit::paintEvent(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  auto text = m_document->toPlainText();
  m_document->drawContents(&painter, rect());
}

// void
// LanguageLabel::mousePressEvent(QMouseEvent* event)
//{
//  QWidget::mousePressEvent(event);
//}

// void
// LanguageLabel::mouseMoveEvent(QMouseEvent* event)
//{
//  QWidget::mouseMoveEvent(event);
//}

// void
// LanguageLabel::mouseReleaseEvent(QMouseEvent* event)
//{
//  QWidget::mouseReleaseEvent(event);
//}

// void
// LanguageLabel::hoverEnter(QHoverEvent* event)
//{
//}

// void
// LanguageLabel::hoverLeave(QHoverEvent* event)
//{
////  m_hoverPos = -1;
//}

void
LanguageLabel::hoverMove(QHoverEvent* event)
{
  if (m_document->toPlainText().size() > 0) {
    auto layout = m_document->documentLayout();
    auto pos = event->position().toPoint();
    auto cursorPos = layout->hitTest(pos, Qt::FuzzyHit);
    if (cursorPos > -1) {
      for (auto block : m_hoverData) {
        if (cursorPos >= block->start && cursorPos < block->end) {
          setToolTip(block->hoverText);
        }
      }
    }
  }
}

bool
LanguageLabel::event(QEvent* event)
{
  switch (event->type()) {
      //    case QEvent::HoverEnter:
      //      hoverEnter(static_cast<QHoverEvent*>(event));
      //      return true;
      //      break;
      //    case QEvent::HoverLeave:
      //      hoverLeave(static_cast<QHoverEvent*>(event));
      //      return true;
      //      break;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(event));
      return true;
      break;
    default:
      break;
  }
  return QWidget::event(event);
}

void
LanguageLabel::parseAndHighlightTag()
{
  QList<QSharedPointer<BCP47Language::TagTestResult>> results;
  QSharedPointer<BCP47Language::TagTestResult> result;

  // remove all spaces.
  auto testValue = StringUtil::removeWhitespace(m_currentTag);

  QString subvalue;

  for (int i = 0; i < testValue.length(); i++) {
    auto c = testValue.at(i);
    auto pos = i + 1;

    if (c != '-' && pos <= testValue.length()) {
      subvalue += c;
      if (result.isNull()) {
        // create new tag result if empty or start of subtag
        result = QSharedPointer<BCP47Language::TagTestResult>(
          new BCP47Language::TagTestResult());
        results.append(result);
        result->start = i;
      }
    }

    if (c == '-' || pos == testValue.length()) {
      auto type = m_languages->checkPrimaryLanguage(subvalue);
      if (type != BCP47Language::NO_PRIMARY_LANGUAGE) {
        if (results.size() > 1)
          result->type =
            BCP47Language::BAD_SUBTAG | BCP47Language::SUBTAG_OUT_OF_POSITION;
        else
          result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkExtendedlanguage(subvalue);
      if (type != BCP47Language::NO_EXTENDED_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkScript(subvalue);
      if (type != BCP47Language::NO_SCRIPT) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkRegion(subvalue);
      if (type != BCP47Language::NO_REGION) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkVariant(subvalue);
      if (type != BCP47Language::NO_VARIANT_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkGrandfathered(subvalue);
      if (type != BCP47Language::NO_GRANDFATHERED_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      type = m_languages->checkRedundant(subvalue);
      if (type != BCP47Language::NO_REDUNDANT_LANGUAGE) {
        result->type = type;
        result->text = subvalue;
        result->length = subvalue.length();
        result.clear();
        subvalue.clear();
        continue;
      }

      if (subvalue != "-") {
        result->length = subvalue.length();
        result->type = BCP47Language::BAD_SUBTAG;
        result->text = subvalue;
        result.clear();
        subvalue.clear();
      } else {
        subvalue.clear();
      }
      continue;
    }
  }

  if (!results.isEmpty()) {
    highlightTag(results);
    update();
  }
}

void
LanguageLabel::highlightTag(
  QList<QSharedPointer<BCP47Language::TagTestResult>> results)
{
  if (!results.isEmpty()) {
    m_document->clear();
    m_hoverData.clear();
    auto subtag = results.takeFirst();
    QTextCursor cursor(m_document);
    auto block = new HoverBlock;
    // first should be a primary language.
    block->start = cursor.anchor();
    if (subtag->type == BCP47Language::PRIMARY_LANGUAGE) {
      cursor.setCharFormat(m_primaryLanguageColor);
      cursor.insertText(subtag->text);
      block->hoverText = tr("Good primary language.");
    } else if (subtag->type == BCP47Language::PRIVATE_LANGUAGE) {
      cursor.setCharFormat(m_privateLanguageColor);
      cursor.insertText(subtag->text);
      block->hoverText = tr("Private primary language.");
    } else {
      cursor.setCharFormat(m_badColor);
      cursor.insertText(subtag->text);
      block->hoverText = tr("Bad language tag.");
    }
    block->end = cursor.anchor();
    m_hoverData.append(block);

    while (!results.isEmpty()) {
      auto prevSubtag = subtag;
      subtag = results.takeFirst();
      block = new HoverBlock;
      block->start = cursor.anchor();
      if (prevSubtag->type == BCP47Language::PRIVATE_LANGUAGE) {
        cursor.setCharFormat(m_textColor);
        cursor.insertText("-");
        cursor.setCharFormat(m_privateLanguageColor);
        cursor.insertText(subtag->text);
        block->hoverText = tr("Private language tag.");
      } else if (subtag->type.testFlag(BCP47Language::EXTENDED_LANGUAGE)) {
        cursor.setCharFormat(m_textColor);
        cursor.insertText("-");
        cursor.setCharFormat(m_extlangColor);
        cursor.insertText(subtag->text);
        block->hoverText = tr("Good extended language tag.");
      } else if (subtag->type.testFlag(BCP47Language::SCRIPT_LANGUAGE)) {
        cursor.setCharFormat(m_textColor);
        cursor.insertText("-");
        cursor.setCharFormat(m_scriptColor);
        cursor.insertText(subtag->text);
        block->hoverText = tr("Good script tag.");
      } else if (subtag->type.testFlag(BCP47Language::REGIONAL_LANGUAGE)) {
        cursor.setCharFormat(m_textColor);
        cursor.insertText("-");
        cursor.setCharFormat(m_regionColor);
        cursor.insertText(subtag->text);
        block->hoverText = tr("Good region tag.");
      } else if (subtag->type.testFlag(BCP47Language::VARIANT_LANGUAGE)) {
        cursor.setCharFormat(m_textColor);
        cursor.insertText("-");
        cursor.setCharFormat(m_variantColor);
        cursor.insertText(subtag->text);
        block->hoverText = tr("Good variant tag.");
      } else if (subtag->type.testFlag(BCP47Language::BAD_SUBTAG)) {
        if (m_document->toPlainText().length() > 0) {
          cursor.setCharFormat(m_textColor);
          cursor.insertText("-");
        }
        if (subtag->type.testFlag(BCP47Language::SUBTAG_OUT_OF_POSITION)) {
          cursor.setCharFormat(m_badPositionColor);
          block->hoverText = tr("Out of position language tag.");
        } else {
          cursor.setCharFormat(m_badColor);
          cursor.insertText(subtag->text);
          block->hoverText = tr("Bad language tag.");
        }
      }
      block->end = cursor.anchor();
      m_hoverData.append(block);
    }
  }
}

void
LanguageLabel::setExtlangColor(const QColor& extlangColor)
{
  m_extlangColor.setForeground(extlangColor);
}

void
LanguageLabel::setScriptColor(const QColor& scriptColor)
{
  m_scriptColor.setForeground(scriptColor);
}

void
LanguageLabel::setRegionColor(const QColor& regionColor)
{
  m_regionColor.setForeground(regionColor);
}

void
LanguageLabel::setVariantColor(const QColor& variantColor)
{
  m_variantColor.setForeground(variantColor);
}

void
LanguageLabel::setBadColor(const QColor& badColor)
{
  m_badColor.setForeground(badColor);
}

////====================================================================
////=== LanguageTextHighlighter
////====================================================================
LanguageLabel::LanguageTextHighlighter::LanguageTextHighlighter(
  QTextDocument* document)
  : QSyntaxHighlighter(document)
{}

} // end of Private__ namespace
