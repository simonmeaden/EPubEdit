#include "document/uniquestring.h"

//============================================================================
//==== UniqueString
//============================================================================
UniqueString::UniqueString()
  : m_unique(true)
{}

UniqueString::UniqueString(const QString& other, bool unique)
  : m_data(other)
  , m_unique(unique)
{}

UniqueString::~UniqueString() {}

bool
UniqueString::isUnique() const
{
  return m_unique;
}

void
UniqueString::setUnique(bool unique)
{
  m_unique = unique;
}

bool
operator==(const UniqueString& lhs, const UniqueString& rhs)
{
  return (lhs.toString() < rhs.toString());
}

bool
operator==(const QString& lhs, const UniqueString& rhs)
{
  return (lhs < rhs.toString());
}

bool
operator==(const UniqueString& lhs, const QString& rhs)
{
  return (lhs.toString() < rhs);
}

bool
operator<(const UniqueString& lhs, const UniqueString& rhs)
{
  return (lhs.toString() < rhs.toString());
}

//============================================================================
//==== UniqueStringList
//============================================================================
UniqueStringList::UniqueStringList() {}

UniqueStringList::~UniqueStringList() {}

UniqueString
UniqueStringList::append(const QString& value, int lineNumber)
{
  UniqueString uniqueValue(value, true);
  if (m_allValues.contains(value)) {
    m_unique.removeAll(value);
    uniqueValue.setUnique(false);
    if (!m_nonUnique.contains(uniqueValue)) {
      m_nonUnique.append(uniqueValue.toString());
    }
  } else {
    m_unique.append(uniqueValue.toString());
  }
  m_allValues.append(uniqueValue.toString());
  m_lineNumbers.append(lineNumber);
  return uniqueValue;
}

//! Returns a list of all non-unique values.
//!
//! Only one copy of each is returned in this list.
QStringList
UniqueStringList::nonUniqueStringList()
{
  return m_nonUnique;
}

//! Returns a list of all unique values.
//!
//! Only one copy of each is returned in this list.
QStringList
UniqueStringList::uniqueList() const
{
  return m_unique;
}

//! Returns a list of all values.
//!
//! Multiple values of non-unique strings will be
//! stored in this list.
QStringList
UniqueStringList::allValues() const
{
  return m_allValues;
}

QList<int>
UniqueStringList::lineNumbers(QString value)
{
  QList<int> lineNumbers;
  int last = 0;
  for (auto i = 0; i < m_allValues.size(); i++) {
    if (m_allValues.indexOf(value, last) > -1) {
      last = i;
      lineNumbers.append(m_lineNumbers.at(i));
    }
  }
  return lineNumbers;
}

bool
UniqueStringList::contains(const QString& value)
{
  return m_allValues.contains(value);
}

bool
UniqueStringList::contains(const UniqueString& value)
{
  return m_allValues.contains(value.toString());
}
