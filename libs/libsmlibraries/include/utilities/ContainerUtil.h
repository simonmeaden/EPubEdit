#ifndef CONTAINERUTIL_H
#define CONTAINERUTIL_H

template<typename T>
class asKeyValueRange
{
public:
  asKeyValueRange(T& data)
    : m_data{ data }
  {
  }

  auto begin() { return m_data.keyValueBegin(); }

  auto end() { return m_data.keyValueEnd(); }

private:
  T& m_data;
};

#endif // CONTAINERUTIL_H
