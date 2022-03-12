#ifndef OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H
#define OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H

#include "Objects/osu/HitObjects/SRCDifficultyHitObject.h"
#include "Skills/SRCAim.h"
#include "Skills/SRCSpeed.h"

#include "vector"
#include "cmath"

#ifndef TYPE_OUTPUT
#define TYPE_OUTPUT
struct Output {
    float aim;
    float speed;
    float total;
};
#endif

class StarRatingCalculator {
private:
    const int section_length = 400;
    const float difficlty_multiplier = 0.0675;
public:
    Output calculate(const std::vector<SRCDifficultyHitObject*>& hitObjects, float timeRate) const;
};

#endif //OSU_SR_CALCULATOR_STARRATINGCALCULATOR_H