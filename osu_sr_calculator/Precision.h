#ifndef OSU_SR_CALCULATOR_PRECISION_H
#define OSU_SR_CALCULATOR_PRECISION_H

#include "Objects/SRCVector2.h"
#include <vector>

#define FLOAT_EPSILON 1e-3

class Precision {
public:
    static bool almostEqualsNumber(float value1, float value2, float acceptableDifference = FLOAT_EPSILON) {
        return abs(value1 - value2) <= acceptableDifference;
    }

    static bool almostEqualsVector(const SRCVector2& vec1, const SRCVector2& vec2, float acceptableDifference = FLOAT_EPSILON) {
        return almostEqualsNumber(vec1.x, vec2.x, acceptableDifference) && almostEqualsNumber(vec1.y, vec2.y, acceptableDifference);
    }

    static bool isLinear(std::vector<SRCVector2> p) {
        return almostEqualsNumber(0, (p[1].y - p[0].y) * (p[2].x - p[0].x) - (p[1].x - p[0].x) * (p[2].y - p[0].y));
    }
};

#endif //OSU_SR_CALCULATOR_PRECISION_H
