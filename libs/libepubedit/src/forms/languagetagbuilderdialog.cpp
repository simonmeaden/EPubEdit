#include "forms/languagetagbuilderdialog.h"
#include "helpdialogs.h"
#include "paths.h"
#include "stringutil.h"
#include "x11colors.h"

//====================================================================
//=== LanguageTagBuilderDialog
//====================================================================
QString LanguageTagBuilderDialog::MISSING_PRIMARY_LANGUAGE =
  tr("[MISSING-PRIMARY-LANGUAGE]", "Missing primary language warning message.");

LanguageTagBuilderDialog::LanguageTagBuilderDialog(Config* config,
                                                   QWidget* parent)
  : QDialog(parent)
  , m_languages(config->languages())
  , m_configDir(QDir(config->configDir()))
  , m_configFile(config->configFile())
  , m_languageTag(MISSING_PRIMARY_LANGUAGE)
{
  Q_INIT_RESOURCE(languagetagbuilderdialogresource);

  setWindowTitle(tr("Language Tag Builder"));

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

// void
// LanguageTagBuilderDialog::testTag()
//{
//  auto tag = m_resultLbl->currentTag();
//  QList<QSharedPointer<BCP47Language::TagTestResult>> results;
//  QSharedPointer<BCP47Language::TagTestResult> result;

//  // remove all spaces.
//  auto testValue = StringUtil::removeWhitespace(tag);

//  QString subvalue;

//  for (int i = 0; i < testValue.length(); i++) {
//    auto c = testValue.at(i);
//    auto pos = i + 1;
//    if (c != '-' && pos <= testValue.length()) {
//      subvalue += c;
//      if (!result) {
//        result = QSharedPointer<BCP47Language::TagTestResult>(
//          new BCP47Language::TagTestResult());
//        result->start = i;
//      }
//    }

//    if (c == '-' || pos == testValue.length()) {
//      auto type = m_languages->checkPrimaryLanguage(subvalue);
//      if (type != BCP47Language::NO_PRIMARY_LANGUAGE) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        //        if (results.isEmpty()) {
//        //          result->position = BCP47Language::FIRST;
//        //        }
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkExtendedlanguage(subvalue);
//      if (type != BCP47Language::NO_EXTENDED_LANGUAGE) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        //        if (results.size() == 1) {
//        //          result->position = BCP47Language::SECOND;
//        //        } else {
//        //          result->position = BCP47Language::BAD_POSITION;
//        //        }
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkScript(subvalue);
//      if (type != BCP47Language::NO_SCRIPT) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        //        if (results.size() == 1) {
//        //          result->position = BCP47Language::SECOND;
//        //        } else if (results.size() == 2) {
//        //          result->position = BCP47Language::THIRD;
//        //        } else {
//        //          result->position = BCP47Language::BAD_POSITION;
//        //        }
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkRegion(subvalue);
//      if (type != BCP47Language::NO_REGION) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        //        if (results.size() == 1) {
//        //          result->position = BCP47Language::SECOND;
//        //        } else if (results.size() == 2) {
//        //          result->position = BCP47Language::THIRD;
//        //        } else if (results.size() == 3) {
//        //          result->position = BCP47Language::FOURTH;
//        //        } else {
//        //          result->position = BCP47Language::BAD_POSITION;
//        //        }
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkVariant(subvalue);
//      if (type != BCP47Language::NO_VARIANT_LANGUAGE) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkGrandfathered(subvalue);
//      if (type != BCP47Language::NO_GRANDFATHERED_LANGUAGE) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      type = m_languages->checkRedundant(subvalue);
//      if (type != BCP47Language::NO_REDUNDANT_LANGUAGE) {
//        result->type = type;
//        result->text = subvalue;
//        result->length = subvalue.length();
//        results.append(result);
//        result.clear();
//        continue;
//      }

//      result->length = subvalue.length();
//      result->type = BCP47Language::BAD_SUBTAG;
//      //      result->position = BCP47Language::BAD_POSITION;
//      results.append(result);
//      result.clear();
//      continue;
//    }
//  }

//  if (result) {
//    results.append(result);
//  }
//  qWarning();
//}

void
LanguageTagBuilderDialog::clearTag()
{
  clearAllTagvalues();
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
  auto box =
    new QGroupBox(tr("Primary Language", "Primary Language box title"), this);
  auto boxLayout = new QGridLayout;
  box->setLayout(boxLayout);
  layout->addWidget(box);
  auto list = m_languages->languageDescriptions();
  m_primaryFilterEdit = new Private__::FilterEdit(list, false, this);
  // line edit cleared manually
  connect(m_primaryFilterEdit,
          &QLineEdit::textChanged,
          this,
          [=](const QString& text) {
            if (text.isEmpty()) {
              this->updateTag(BCP47Language::NO_VALUE);
              this->showPrivateLanguageFrame(BCP47Language::NO_VALUE);
            }
          });
  // return pressed
  connect(m_primaryFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::languageChanged);
  boxLayout->addWidget(m_primaryFilterEdit, 0, 0);
  auto btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  // combo box cleared with clear button
  connect(btn,
          &QPushButton::clicked,
          m_primaryFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    m_languageTag.clear();
    m_extlangFilterEdit->setValues(m_languages->extlangDescriptions());
    m_variantFilterEdit->setValues(m_languages->variantDescriptions());
    this->updateTag(BCP47Language::NO_PRIMARY_LANGUAGE);
    this->showPrivateLanguageFrame(BCP47Language::NO_PRIMARY_LANGUAGE);
  });
  boxLayout->addWidget(btn, 0, 1);
  boxLayout->addWidget(m_primaryFilterEdit->comboBox(), 0, 2);

  // HIDDEN PRIVATE LANGUAGE
  m_privateLangFrame =
    new Private__::PrivateLanguageFrame("(q[a-t][a-z])|x|i", "qaa", this);
  m_privateLangFrame->setHidden(true);
  connect(m_privateLangFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateLanguageChanged);
  boxLayout->addWidget(m_privateLangFrame, 1, 0, 1, 3);

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
  connect(m_extlangFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  connect(m_extlangFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::extlangChanged);
  boxLayout->addWidget(
    new QLabel(tr("Ext Lang", "EXTLANG label title"), this), row, 0);
  boxLayout->addWidget(m_extlangFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_extlangFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    m_extlangTag.clear();
    updateTag(BCP47Language::NO_EXTENDED_LANGUAGE);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_extlangFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_extlangFilterEdit->checkBox(), row, 4);

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
  boxLayout->addWidget(lbl, row, 0, 1, 5);

  row++;
  m_scriptFilterEdit =
    new Private__::FilterEdit(m_languages->scriptDescriptions(), true, this);
  connect(m_scriptFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  connect(m_scriptFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::scriptChanged);
  boxLayout->addWidget(
    new QLabel(tr("Script", "Script label title"), this), row, 0);
  boxLayout->addWidget(m_scriptFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_scriptFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    m_scriptTag.clear();
    updateTag(BCP47Language::NO_SCRIPT);
    showPrivateScriptFrame(BCP47Language::NO_SCRIPT);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_scriptFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_scriptFilterEdit->checkBox(), row, 4);

  row++;
  // SCRIPT
  m_privateScriptFrame = new Private__::PrivateScriptFrame(
    "Qa(([a][a-z])|([b][a-x]))", "Qaaa", this);
  m_privateScriptFrame->setHidden(true);
  connect(m_privateScriptFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateScriptChanged);
  boxLayout->addWidget(m_privateScriptFrame, row, 0, 1, 3);

  row++;
  m_regionFilterEdit =
    new Private__::FilterEdit(m_languages->regionDescriptions(), true, this);
  connect(m_regionFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::regionChanged);
  connect(m_regionFilterEdit->checkBox(),
          &QCheckBox::stateChanged,
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
  boxLayout->addWidget(
    new QLabel(tr("Region", "Region label title"), this), row, 0);
  boxLayout->addWidget(m_regionFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_regionFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    m_regionTag.clear();
    updateTag(BCP47Language::NO_REGION);
    showPrivateRegionFrame(BCP47Language::NO_REGION);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_regionFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_regionFilterEdit->checkBox(), row, 4);

  row++;
  // HIDDEN PRIVATE REGION
  m_privateRegionFrame =
    new Private__::PrivateRegionFrame("AA|ZZ|Q[M-Z]|X[A-Z])", "AA", this);
  m_privateRegionFrame->setHidden(true);
  connect(m_privateRegionFrame,
          &Private__::PrivateFrame::privateValueChanged,
          this,
          &LanguageTagBuilderDialog::privateRegionChanged);
  boxLayout->addWidget(m_privateRegionFrame, row, 0, 1, 3);

  box = new QGroupBox(tr("Language Tag", "Tag display box title"), this);
  auto resultLayout = new QGridLayout;
  resultLayout->setColumnStretch(0, 2);
  resultLayout->setRowStretch(0, 2);
  box->setLayout(resultLayout);
  layout->addWidget(box);

  // VARIANT
  row++;
  m_variantFilterEdit =
    new Private__::FilterEdit(m_languages->extlangDescriptions(), true, this);
  connect(m_variantFilterEdit,
          &Private__::FilterEdit::activated,
          this,
          &LanguageTagBuilderDialog::variantChanged);
  connect(m_variantFilterEdit,
          &Private__::FilterEdit::stateChanged,
          this,
          &LanguageTagBuilderDialog::variantChanged);
  boxLayout->addWidget(
    new QLabel(tr("Variant", "VARIANT label title"), this), row, 0);
  boxLayout->addWidget(m_variantFilterEdit, row, 1);
  btn = new QPushButton(tr("Clear", "Clear edit text"), this);
  connect(btn,
          &QPushButton::clicked,
          m_variantFilterEdit,
          &Private__::FilterEdit::clear);
  connect(btn, &QPushButton::clicked, this, [=]() {
    m_variantTag.clear();
    updateTag(BCP47Language::NO_VARIANT_LANGUAGE);
  });
  boxLayout->addWidget(btn, row, 2);
  boxLayout->addWidget(m_variantFilterEdit->comboBox(), row, 3);
  boxLayout->addWidget(m_variantFilterEdit->checkBox(), row, 4);

  m_reportLbl = new QLabel(this);
  resultLayout->addWidget(m_reportLbl, 0, 0, 1, 2);
  m_resultLbl = new Private__::LanguageLabel(m_languages, this);
  connect(m_resultLbl,
          &Private__::LanguageLabel::textChanged,
          this,
          [=](const QString& text) { m_resultLbl->setCurrentTag(text); });
  resultLayout->addWidget(m_resultLbl, 1, 0);
  m_usePreferredBtn = new QPushButton(
    tr("Use Preferred", "Use preferred option button text."), this);
  m_usePreferredBtn->setEnabled(false);
  connect(m_usePreferredBtn,
          &QPushButton::clicked,
          this,
          &LanguageTagBuilderDialog::usePreferredValue);
  resultLayout->addWidget(m_usePreferredBtn, 0, 2);

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
  if (m_primaryFilterEdit->hasCurrentText()) {
    m_primaryFilterEdit->setRequired(true);
    auto editText = m_primaryFilterEdit->currentText();
    m_language = m_languages->languageFromDescription(editText);
    if (m_language) {

      // find matching extlang's and variant's if any for language and update
      // their values.
      auto prefixesForLanguage =
        m_languages->extlangsWithPrefix(m_language->subtag());
      m_extlangFilterEdit->setValues(prefixesForLanguage);
      prefixesForLanguage =
        m_languages->variantsWithPrefix(m_language->subtag());
      (m_language->subtag());
      m_variantFilterEdit->setValues(prefixesForLanguage);

      if (m_language->description() == "Private use") {
        languageType = BCP47Language::PRIVATE_LANGUAGE;
        m_languageTag = "qaa"; // actual tag is 'qaa..qtz'
      } else {
        languageType = BCP47Language::PRIMARY_LANGUAGE;
        m_languageTag = m_language->subtag();
      }
    } else {
      m_extlangFilterEdit->clear();
      m_variantFilterEdit->clear();
    }
  } else {
    m_languageTag = MISSING_PRIMARY_LANGUAGE;
  }
  showPrivateLanguageFrame(languageType);
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::extlangChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_VARIANT_LANGUAGE;
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
      languageType = BCP47Language::EXTENDED_LANGUAGE;
    }
  }
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::usePreferredValue()
{
  // TODO action this
}

void
LanguageTagBuilderDialog::scriptChanged()
{
  BCP47Language::TagType languageType = BCP47Language::NO_SCRIPT;
  if (m_scriptFilterEdit->hasCurrentText()) {
    m_scriptFilterEdit->setRequired(true);
    auto editText = m_scriptFilterEdit->currentText();
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
        m_scriptTag = script->subtag();
        //        m_scriptFilterEdit->setRequired(true);
        if (script->description() == "Private use") {
          languageType = BCP47Language::PRIVATE_SCRIPT;
          m_scriptTag = "Qaaa"; // actual tag is 'Qaaa..Qabx'
        } else {
          languageType = BCP47Language::SCRIPT_LANGUAGE;
          m_scriptTag = script->subtag();
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
  if (m_regionFilterEdit->hasCurrentText()) {
    m_regionFilterEdit->setRequired(true);
    auto editText = m_regionFilterEdit->currentText();
    auto region = m_languages->regionFromDescription(editText);
    if (region) {
      m_regionTag = region->subtag();
      m_regionFilterEdit->setRequired(true);
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
  if (m_variantFilterEdit->hasCurrentText()) {
    m_variantFilterEdit->setRequired(true);
    auto language =
      m_languages->variantFromDescription(m_variantFilterEdit->currentText());
    m_usePreferredBtn->setEnabled(false);
    if (language) {
      m_variantTag = language->subtag();
      m_variantFilterEdit->setRequired(true);
      languageType = BCP47Language::VARIANT_LANGUAGE;
    }
  }
  updateTag(languageType);
}

void
LanguageTagBuilderDialog::clearAllTagvalues()
{
  m_languageTag.clear();
  m_extlangTag.clear();
  m_scriptTag.clear();
  m_regionTag.clear();
  m_variantTag.clear();
}

void
LanguageTagBuilderDialog::updateTag(BCP47Language::TagType type)
{
  QString result;
  if (type == BCP47Language::NO_VALUE) {
    clearAllTagvalues();
  } else {
    if (!m_languageTag.isEmpty()) {
      if (type == BCP47Language::PRIVATE_LANGUAGE) {
        result = m_privateLangFrame->value();
      } else {
        result = m_languageTag;
      }
    }

    if (!m_extlangTag.isEmpty() && m_extlangFilterEdit->isRequired()) {
      result += QString("-%1").arg(m_extlangTag);
    }

    if (!m_scriptTag.isEmpty() && m_scriptFilterEdit->isRequired()) {
      if (type == BCP47Language::PRIVATE_SCRIPT) {
        result += QString("-%1").arg(m_privateScriptFrame->value());
      } else {
        result += QString("-%1").arg(m_scriptTag);
      }
    }

    if (!m_regionTag.isEmpty() && m_regionFilterEdit->isRequired()) {
      if (type == BCP47Language::PRIVATE_REGION) {
        result += QString("-%1").arg(m_privateRegionFrame->value());
      } else {
        result += QString("-%1").arg(m_regionTag);
      }
    }

    if (!m_variantTag.isEmpty() && m_variantFilterEdit->isRequired()) {
      result += QString("-%1").arg(m_variantTag);
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
  connect(m_selection,
          qOverload<int>(&QComboBox::activated),
          this,
          &FilterEdit::activated);
  if (m_required) {
    connect(
      m_required, &QCheckBox::stateChanged, this, &FilterEdit::stateChanged);
  }
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
  clear();
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
  m_extlangColor.setForeground(QColorConstants::X11::cornflowerblue);
  m_scriptColor.setForeground(QColorConstants::X11::darkorchid);
  m_regionColor.setForeground(QColorConstants::X11::gold);
  m_variantColor.setForeground(QColorConstants::X11::magenta);
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

//void
//LanguageLabel::mousePressEvent(QMouseEvent* event)
//{
//  QWidget::mousePressEvent(event);
//}

//void
//LanguageLabel::mouseMoveEvent(QMouseEvent* event)
//{
//  QWidget::mouseMoveEvent(event);
//}

//void
//LanguageLabel::mouseReleaseEvent(QMouseEvent* event)
//{
//  QWidget::mouseReleaseEvent(event);
//}

//void
//LanguageLabel::hoverEnter(QHoverEvent* event)
//{
//}

//void
//LanguageLabel::hoverLeave(QHoverEvent* event)
//{
////  m_hoverPos = -1;
//}

void
LanguageLabel::hoverMove(QHoverEvent* event)
{
  if (m_document->toPlainText().size() > 0) {
    auto layout = m_document->documentLayout();
    auto pos = event->pos();
    auto cursorPos = layout->hitTest(pos, Qt::FuzzyHit);
    if (cursorPos > -1) {
      for (auto block : m_hoverData) {
        if (cursorPos >= block->start && cursorPos<block->end) {
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
      subtag = results.takeFirst();
      block = new HoverBlock;
      block->start = cursor.anchor();
      if (subtag->type.testFlag(BCP47Language::EXTENDED_LANGUAGE)) {
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

} // end of Private__ namespace
