#ifndef CSVSPLITTER_H
#define CSVSPLITTER_H

#include <QChar>
#include <QString>
#include <QStringList>

/*!
  \ingroup utilities
  \file csvsplitter.h
  \brief some useful functions for splitting CSV files.
  \author Simon Meaden
  \date 2022-08-10
  \namespace CSVSplitter
 */
namespace CSVSplitter {
//! /brief Splits a text string on the delimiting character.
static QStringList
split(const QString data, const QChar delimiter);

//! /brief Splits a text string on the delimiting character.
//!
//! This is an overloaded function.
//! Splits the data string on commas.
static QStringList
splitOnComma(const QString data);

//! /brief Splits a text string on the delimiting character.
//!
//! This is an overloaded function.
//! Splits the data string on colons.
static QStringList
splitOnColon(const QString data);

//! /brief Splits a text string on the delimiting character.
//!
//! This is an overloaded function.
//! Splits the data string on spaces.
static QStringList
splitOnSpace(const QString data);

//! /brief Splits a text string on the delimiting character.
//!
//! This is an overloaded function.
//! Splits the data string on semicolons.
static QStringList
splitOnSemicolon(const QString data);

};

#endif // CSVSPLITTER_H
