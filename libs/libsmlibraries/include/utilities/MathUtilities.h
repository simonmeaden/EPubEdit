#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

#include "qglobal.h"

namespace Math {

/*!
 * \brief Calculates half the difference between the external ind internal values.
 *
 * Used primarily to contralise text or widgets within a larger area.
 */
inline int halfDifference(int external, int internal) { return int((external - internal) / 2); }

/*!
 * \brief Calculates half the difference between the external ind internal values.
 *
 * Used primarily to contralise text or widgets within a larger area.
 */
inline qreal halfDifference(qreal external, qreal internal) { return (external - internal) / 2; }

}
#endif // MATHUTILITIES_H
