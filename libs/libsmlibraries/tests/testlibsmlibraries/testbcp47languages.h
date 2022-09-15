#ifndef TESTBCP47LANGUAGES_H
#define TESTBCP47LANGUAGES_H

#include "language/languages.h"
#include "utilities/paths.h"
#include "gtest/gtest.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QDate>

namespace{ // anonymous namespace


class BCP47LanguagesTest : public testing::Test
{
public:
  BCP47LanguagesTest();

protected: // You should make the members protected s.t. they can be
  // accessed from sub-classes.
  BCP47Languages languages;
  QDir configDir;
  QDir libraryDir;
  QString configFilename;

  void SetUp() override;

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  void TearDown() override;

}; // end of BCP47LanguagesTest

} // end of anonymous namespace

#endif // TESTBCP47LANGUAGES_H
