#ifndef OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H
#define OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H

#include "Objects/osu/HitObjects/DifficultyHitObject.h"
#include "Skills/Aim.h"
#include "Skills/Speed.h"

#include "types.h"
#include "vector"
#include "cmath"

class StarRatingCalculator {
private:
    const int section_length = 400;
    const float difficlty_multiplier = 0.0675;
public:
    Output calculate(const std::vector<DifficultyHitObject*>& hitObjects, float timeRate) const;
};

#endif //OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H