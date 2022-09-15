#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>

/*!
 * \ingroup utilities
 * \file stringutil.h
 * \brief A holding place for some reusable QString functions.
 * \author Simon Meaden
 * \date 2018-06-15
 */
namespace StringUtil
{

/*!
 *  \brief Removes excess whitespace from a QString.
 *
 *  Internal whitespace is limited to a single space serarator
 *  character per space.
 */
QString removeWhitespace(const QString& text);

} // end of namespace Strings

#endif // STRINGUTIL_H
