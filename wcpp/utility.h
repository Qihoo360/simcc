#include <string>
#include <sstream>

#include "wcpp/inner_pre.h"

namespace wcpp {

class Util {
public:
    static bool Equals(float64 x, float64 y, float64 tolerance = 0.000001) {
        if (x - y > -tolerance && x - y < tolerance) {
            return true;
        }

        return false;
    }

    enum EqualElememt {
        kLessEqual,
        kEqual,
        kLargeEqual
    };

    // @brief compare tow double
    // @param kLessEqual if x only less than y a little, this function will return true
    // @param E_LARGER_EQUAL if x only larger than y a little, this function will return true
    // @param E_EQUAL the same as Equals( f64 x, f64 y, f64 tolerance = 0.000001 )
    static bool Equals(float64 x, float64 y, EqualElememt ee, float64 tolerance = 0.000001) {
        assert(tolerance > 0);
        switch (ee) {
        case kLessEqual:
            if (x - y > -tolerance && x - y <= 0) {
                return true;
            }
            break;
        case kLargeEqual:
            if (x - y < tolerance && x - y >= 0) {
                return true;
            }
            break;
        case kEqual:
            return Equals(x, y, tolerance);
            break;
        default:
            assert(false);
            break;
        }

        return false;
    }

    // @brief This function calculate the next power of two higher than or equal to a given number. 
    //  i.e. round a number up to the next power of two
    //  i.e. : 8 -> 8 , 7 -> 8, 31 -> 32, 33 -> 64
    // @param originalvalue
    // @return uint32
    static uint32 RoundUpToNextPow2(uint32 originalvalue) {
        originalvalue -= 1;
        originalvalue |= (originalvalue >> 1);
        originalvalue |= (originalvalue >> 2);
        originalvalue |= (originalvalue >> 4);
        originalvalue |= (originalvalue >> 8);
        originalvalue |= (originalvalue >> 16);

        return originalvalue + 1;
    }

    // @brief Limit a number in a specified range
    // @param tmin
    // @param tmax
    // @param value
    // @return T
    template<typename T>
    static T GetValueInRange(T tmin, T tmax, T value) {
        if (tmax > tmin) {
            if (value < tmin) {
                return tmin;
            } else if (value > tmax) {
                return tmax;
            } else {
                return value;
            }
        } else {
            if (value < tmax) {
                return tmax;
            } else if (value > tmin) {
                return tmin;
            } else {
                return value;
            }
        }
    }
};
}