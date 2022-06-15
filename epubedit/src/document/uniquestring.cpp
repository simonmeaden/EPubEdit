#include "document/uniquestring.h"

//============================================================================
//==== UniqueString
//============================================================================
QStringList UniqueString::m_uniqueList = QStringList();

UniqueString::UniqueString()
  : m_valid(false) // empty string is invalid.
{}

UniqueString::UniqueString(const QString& data)
{
  if (!m_uniqueList.contains(m_data)) {
    m_uniqueList.append(data);
    m_data = data;
    m_valid = true;
  } else
    m_valid = false;
}

UniqueString::UniqueString(const UniqueString& other)
  : m_data(other.m_data)
  , m_valid(other.m_valid)
{}

UniqueString::~UniqueString() {}

QString UniqueString::toString() const { return m_data; }

bool UniqueString::isEmpty() const { return m_data.isEmpty(); }

bool UniqueString::exists(const QString &value) {
  return m_uniqueList.contains(value);
}

bool UniqueString::exists(const UniqueString &value) {
  return m_uniqueList.contains(value.toString());
}

void UniqueString::operator=(const QString &rhs) { m_data = rhs; }

bool UniqueString::operator==(const char *rhs) const { return (m_data == rhs); }

bool UniqueString::operator<(const char *rhs) { return (m_data < rhs); }

bool UniqueString::operator<(const QString &rhs) { return (m_data < rhs); }

bool UniqueString::operator==(const QString &rhs) { return (m_data == rhs); }

bool UniqueString::operator<(const UniqueString &rhs)
{
  return (m_data < rhs.m_data);
}

bool UniqueString::operator==(const UniqueString &rhs)
{
  return (m_data == rhs.m_data);
}

bool
UniqueString::isValid() const
{
  return m_valid;
}

bool UniqueString::isPossible(const QString &data) {
  return !m_uniqueList.contains(data);
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
