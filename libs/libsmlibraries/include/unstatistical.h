#ifndef UNSTATISTICAL_H
#define UNSTATISTICAL_H

#include <QMap>
#include <QString>
#include <QStringLiteral>

namespace UNStatisticalCodes {

namespace { // anonymous namespace

struct UNStatisticalCode
{
  UNStatisticalCode() {}
  UNStatisticalCode(int n, const QString& a)
    : numerical(n)
    , alpha(a)
  {}

  int numerical;
  QString alpha;
};

QMap<QString, UNStatisticalCode> CODES;

UNStatisticalCode
code(const QString& name);

//! \brief Returns a QStringList containing all of the UN statistical named
//! areas.
static QStringList
names();

//! \brief returns the numerical code for the UN area as an integer value.
//!
//! Normally these codes are required as a three character code. For this
//! use the static numericalCodeString(const QString&) method.
static int
numericalCode(const QString& name);

//! \brief Returns the numerical code for the UN area an a three character
//! numerical code.
//!
//! If you need the code a s a numerical int value use numericalCode(const
//! QString&).
static QString
numericalCodeString(const QString& name);

//! \brief Returns the three character alpha code for the UN area as a
//! QString.
static QString
alphaCode(const QString& name);

} // end anonymous namespace

} // end UNStatisticalCodes namespace

#endif // UNSTATISTICAL_H
