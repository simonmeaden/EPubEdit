#include "testbcp47languages.h"

namespace {

BCP47LanguagesTest::BCP47LanguagesTest()
  : configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , libraryDir(QDir(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      "library")))
  , configFilename(
      Paths::join(configDir.path(), "epubedit", "languages.yaml"))
{}

void
BCP47LanguagesTest::SetUp()
{
  languages.readFromLocalFile(configFilename);
}

void
BCP47LanguagesTest::TearDown()
{}

TEST_F(BCP47LanguagesTest, FileLoaded)
{
  auto date = languages.fileDate();
  ASSERT_TRUE(date.toString(Qt::ISODate) == "2021-08-06");
}

TEST_F(BCP47LanguagesTest, Languages)
{
  // Nonexistant language
  auto language = languages.languageFromDescription("Arrrrgh");
  ASSERT_TRUE(language.isNull());

  language = languages.languageFromDescription("Abkhazian");
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "ab");
  ASSERT_TRUE(language->description() == "Abkhazian");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  ASSERT_TRUE(language->suppressScriptLang() == "Cyrl");
  ASSERT_TRUE(language->macrolanguageName() == "");

  language = languages.languageFromDescription("Chuanqiandian Cluster Miao");
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "cqd");
  ASSERT_TRUE(language->description() == "Chuanqiandian Cluster Miao");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2009-07-29");
  ASSERT_TRUE(language->suppressScriptLang() == "");
  ASSERT_TRUE(language->macrolanguageName() == "hmn");

  // Multi description languages. Both descriptions should point to
  // the same BCP47Language node.
  language = languages.languageFromDescription("Bengali");
  auto language2 = languages.languageFromDescription("Bangla");
  ASSERT_TRUE(language == language2);
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "bn");
  ASSERT_TRUE(language->descriptions().at(0) == "Bengali");
  ASSERT_TRUE(language->descriptions().at(1) == "Bangla");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  ASSERT_TRUE(language->suppressScriptLang() == "Beng");
  ASSERT_TRUE(language->macrolanguageName() == "");

  // // Multi value script
  language = languages.languageFromDescription("Church Slavic");
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "cu");
  ASSERT_TRUE(language->description() == "Church Slavic");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  // No suppress script lang for this item
  ASSERT_TRUE(language->suppressScriptLang() == "");
  ASSERT_TRUE(language->macrolanguageName() == "");
  for (auto& description : language->descriptions()) {
    if (description == "Naxi Geba")
      continue;
    language2 = languages.languageFromDescription(description);
    ASSERT_TRUE(language == language2);
  }
}

TEST_F(BCP47LanguagesTest, Scripts)
{
  // // Multi value script
  auto language = languages.scriptFromDescription("Naxi Geba");
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::SCRIPT);
  ASSERT_TRUE(language->subtag() == "Nkgb");
  ASSERT_TRUE(language->description() == "Naxi Geba");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2009-03-13");
  // No suppress script lang for this item
  ASSERT_TRUE(language->suppressScriptLang() == "");
  for (auto& description : language->descriptions()) {
    if (description == "Naxi Geba")
      continue;
    auto language2 = languages.scriptFromDescription(description);
    ASSERT_TRUE(language == language2);
  }
}

TEST_F(BCP47LanguagesTest, Variants)
{
  // // Multi value script
  auto language =
    languages.variantFromDescription("ALA-LC Romanization, 1997 edition");
  ASSERT_FALSE(language.isNull()) << "Language not empty";
  ASSERT_TRUE(language->type() == BCP47Language::VARIANT);
  ASSERT_TRUE(language->subtag() == "alalc97");
  ASSERT_TRUE(language->description() == "ALA-LC Romanization, 1997 edition");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2009-12-09");
  // No suppress script lang for this item
  ASSERT_TRUE(language->suppressScriptLang() == "");
  ASSERT_TRUE(language->macrolanguageName() == "");
  ASSERT_TRUE(
    language->comments() ==
    "Romanizations recommended by the American Library Association\n  and the "
    "Library of Congress, in \"ALA-LC Romanization Tables:\n  Transliteration "
    "Schemes for Non-Roman Scripts\" (1997), ISBN\n  978-0-8444-0940-5.");
}

TEST_F(BCP47LanguagesTest, PrimaryLanguage)
{
  auto values = languages.languageDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated language.
    auto language = languages.languageFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE)
      << QObject::tr("Language: %1, %2")
           .arg(language->typeString(), language->description())
           .toStdString();
  }

  values = languages.languageSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.languageFromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE)
      << QObject::tr("Language: %1, %2")
           .arg(language->typeString(), language->description())
           .toStdString();
  }

  auto language = languages.languageFromSubtag("alu");
  ASSERT_FALSE(language.isNull());
}

TEST_F(BCP47LanguagesTest, Region)
{
  auto values = languages.regionDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated regions.
    auto language = languages.regionFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REGION);
  }

  values = languages.regionSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.regionFromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REGION);
  }
}

TEST_F(BCP47LanguagesTest, Variant)
{
  auto values = languages.variantDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated variant.
    auto language = languages.variantFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::VARIANT);
  }

  values = languages.variantSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.variantFromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::VARIANT);
  }
}

TEST_F(BCP47LanguagesTest, ExtLang)
{
  auto values = languages.extlangDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated extlang.
    auto language = languages.extlangFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::EXTLANG);
  }

  values = languages.extlangSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.extlangFromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::EXTLANG);
  }
}

TEST_F(BCP47LanguagesTest, Script)
{
  auto values = languages.scriptDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated extlang.
    auto language = languages.scriptFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::SCRIPT);
  }

  values = languages.scriptSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.scriptFromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::SCRIPT);
  }
}

TEST_F(BCP47LanguagesTest, Grandfathered)
{
  auto values = languages.grandfatheredDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated extlang.
    auto language = languages.grandfatheredFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::GRANDFATHERED);
  }

  values = languages.grandfatheredTags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.grandfatheredFromTag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::GRANDFATHERED);
  }
}

TEST_F(BCP47LanguagesTest, Redundant)
{
  auto values = languages.redundantDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated extlang.
    auto language = languages.redundantFromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REDUNDANT);
  }

  values = languages.redundantTags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.redundantFromTag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REDUNDANT);
  }
}

TEST_F(BCP47LanguagesTest, TestPrimaryLanguage)
{
  auto types = languages.isPrimaryLanguage("en");
  types = languages.isPrimaryLanguage("EN");
}

} // end of anonymous namespace
