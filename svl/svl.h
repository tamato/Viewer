#ifndef SVL_H
#define SVL_H

// Main include for the sony vector library that also gives a more convient namespace then Vectormath::Aos::

#include "vectormath_aos.h"

namespace svl {
    using namespace Vectormath::Aos;

    const float PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;
    #define DegToRadains(x) float((x) * (PI / 180.0f));
    #define RadToDegrees(x) float((x) * (180.0f / PI));
}

#endif // SVL_H
