/*
 * extra.h
 *
 *  Created on: Jan 24, 2014
 *      Author: segmentedbit
 */

#ifndef EXTRA_H_
#define EXTRA_H_

namespace im {
inline int round(double r) {
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

inline int round(float r) {
	return round(static_cast<double>(r));
}

}

#endif /* EXTRA_H_ */
