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

TEST_F(BCP47LanguagesTest, Language)
{
  auto language = languages.language("Abkhazian");
  ASSERT_NE(language, nullptr) << "Languge not empty";
  ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
  ASSERT_TRUE(language->subtag() == "ab");
  ASSERT_TRUE(language->description() == "Abkhazian");
  ASSERT_TRUE(language->subtag() == "ab");
  ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
  ASSERT_TRUE(language->suppressScriptLang() == "Cyrl");
  //
  language = languages.language("Bengali");
    ASSERT_NE(language, nullptr) << "Languge not empty";
    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
    ASSERT_TRUE(language->subtag() == "ab");
    ASSERT_TRUE(language->descriptions().at(0) == "Bengali");
    ASSERT_TRUE(language->descriptions().at(1) == "Bangla");
    ASSERT_TRUE(language->subtag() == "bn");
    ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
    ASSERT_TRUE(language->suppressScriptLang() == "Beng");
//    auto language2 = languages.language("Bangla");
//    ASSERT_TRUE(language->type() == BCP47Language::LANGUAGE);
//    ASSERT_TRUE(language->subtag() == "ab");
//    ASSERT_TRUE(language->descriptions().at(0) == "Bangla");
//    ASSERT_TRUE(language->descriptions().at(1) == "Bengali");
//    ASSERT_TRUE(language->subtag() == "bn");
//    ASSERT_TRUE(language->dateAdded().toString(Qt::ISODate) == "2005-10-16");
//    ASSERT_TRUE(language->suppressScriptLang() == "Beng");
}

} // end of anonymous namespace
