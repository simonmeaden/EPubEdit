#include "testbcp47languages.h"

namespace {

BCP47LanguagesTest::BCP47LanguagesTest()
  : configDir(
      QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)))
  , libraryDir(QDir(Paths::join(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
      "library")))
  , configFile(
      QFile(Paths::join(configDir.path(), "epubedit", "languages.yaml")))
{}

void
BCP47LanguagesTest::SetUp()
{
  languages.readFromLocalFile(configFile);
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
  auto language = languages.fromDescription("Arrrrgh");
  ASSERT_EQ(language, nullptr);

  language = languages.fromDescription("Abkhazian");
  ASSERT_NE(language, nullptr);
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "ab");
  ASSERT_TRUE(language->description() == "Abkhazian");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  ASSERT_TRUE(language->suppressScriptLang() == "Cyrl");
  ASSERT_TRUE(language->macrolanguageName() == "");

  language = languages.fromDescription("Chuanqiandian Cluster Miao");
  ASSERT_NE(language, nullptr);
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "cqd");
  ASSERT_TRUE(language->description() == "Chuanqiandian Cluster Miao");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2009-07-29");
  ASSERT_TRUE(language->suppressScriptLang() == "");
  ASSERT_TRUE(language->macrolanguageName() == "hmn");

  // Multi description languages. Both descriptions should point to
  // the same BCP47Language node.
  language = languages.fromDescription("Bengali");
  auto language2 = languages.fromDescription("Bangla");
  ASSERT_TRUE(language == language2);
  ASSERT_NE(language, nullptr) << "Language empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "bn");
  ASSERT_TRUE(language->descriptions().at(0) == "Bengali");
  ASSERT_TRUE(language->descriptions().at(1) == "Bangla");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  ASSERT_TRUE(language->suppressScriptLang() == "Beng");
  ASSERT_TRUE(language->macrolanguageName() == "");

  // // Multi value script
  language = languages.fromDescription("Church Slavic");
  ASSERT_NE(language, nullptr) << "Language empty";
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
    language2 = languages.fromDescription(description);
    ASSERT_TRUE(language == language2);
  }
}

TEST_F(BCP47LanguagesTest, Scripts)
{
  // // Multi value script
  auto language = languages.fromDescription("Naxi Geba");
  ASSERT_NE(language, nullptr) << "Languge not empty";
  ASSERT_TRUE(language->type() == BCP47Language::SCRIPT);
  ASSERT_TRUE(language->subtag() == "Nkgb");
  ASSERT_TRUE(language->description() == "Naxi Geba");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2009-03-13");
  // No suppress script lang for this item
  ASSERT_TRUE(language->suppressScriptLang() == "");
  for (auto& description : language->descriptions()) {
    if (description == "Naxi Geba")
      continue;
    auto language2 = languages.fromDescription(description);
    ASSERT_TRUE(language == language2);
  }
}

TEST_F(BCP47LanguagesTest, Variants)
{
  // // Multi value script
  auto language =
    languages.fromDescription("ALA-LC Romanization, 1997 edition");
  ASSERT_NE(language, nullptr) << "Languge not empty";
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  }

  values = languages.languageSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  }

  auto language = languages.fromSubtag("alu");
  ASSERT_FALSE(language.isNull());
}

TEST_F(BCP47LanguagesTest, Region)
{
  auto values = languages.regionDescriptions();
  ASSERT_FALSE(values.isEmpty());
  for (auto& name : values) {
    // First check that all of the names have an associated regions.
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REGION);
  }

  values = languages.regionSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::VARIANT);
  }

  values = languages.variantSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::EXTLANG);
  }

  values = languages.extlangSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::SCRIPT);
  }

  values = languages.scriptSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::GRANDFATHERED);
  }

  values = languages.grandfatheredSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
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
    auto language = languages.fromDescription(name);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REDUNDANT);
  }

  values = languages.redundantSubtags();
  ASSERT_FALSE(values.isEmpty());
  for (auto& subtag : values) {
    // First check that all of the names have an associated language.
    auto language = languages.fromSubtag(subtag);
    ASSERT_FALSE(language.isNull());
    ASSERT_TRUE(language->type() == BCP47Language::REDUNDANT);
  }
}

} // end of anonymous namespace
