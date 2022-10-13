#ifndef CONTAINERUTIL_H
#define CONTAINERUTIL_H

/*!
 * \brief Allows Qt C++17 key-value mapping on QMap & QHash
 *
 * Based on the code in the KDAB blog by
 * [Ivan Čukić](https://www.kdab.com/author/ivan/) on C++17 structured bindings
 * [Qt, range-based for loops and structured bindings](https://www.kdab.com/qt-range-based-for-loops-and-structured-bindings/)
 * int the section 'QHash, QMap and STL compatibility' which allows the following code
 * \code
 * for (auto [key, value]: asKeyValueRange(map)) {
 *  // ...
 * }
 * \endcode
 * to allow key/value iteration in Qt code.
 *
 * This is easily remedied by using a simple wrapper over QHash and QMap
 * that will rename .keyValueBegin() and .keyValueEnd() to .begin() and .end()
 * as required by C++17 stl.
 */
template<typename T>
class asKeyValueRange
{
public:
  //! Constructor
  asKeyValueRange(T& data)
    : m_data{ data }
  {
  }

  //! Renames keyValueBegin() to begin()
  auto begin() { return m_data.keyValueBegin(); }

  //! Renames keyValueEnd() to end()
  auto end() { return m_data.keyValueEnd(); }

private:
  T& m_data;
};

#endif // CONTAINERUTIL_H
