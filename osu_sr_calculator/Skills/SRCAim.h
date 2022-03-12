#ifndef OSU_SR_CALCULATOR_SRCAIM_H
#define OSU_SR_CALCULATOR_SRCAIM_H

#include "cmath"

#include "SRCOsuStrainSkill.h"
#include "Objects/osu/HitObjects/SRCDifficultyHitObject.h"

inline float applyDimishingExp(float val) {
    return powf(val, 0.99);
}

class SRCAim : public SRCOsuStrainSkill {
public:
    SRCAim() {
        SkillMultiplier = 26.25;
        StrainDecayBase = 0.15;
    }

    float strainValueOf(SRCDifficultyHitObject* currentObject) override {
        float result = 0;
        const float scale = 90;

        if (!Previous.empty()) {
            if (currentObject->angle > 0 && currentObject->angle > angle_bonus_begin) {
                float angleBonus = sqrtf(
                    fmaxf(Previous.at(0)->jumpDistance - scale, 0) *
                    powf(sinf(currentObject->angle - angle_bonus_begin), 2) *
                    fmaxf(currentObject->jumpDistance - scale, 0)
                );
                result = 1.5f * applyDimishingExp(fmaxf(0, angleBonus)) / fmaxf((float)timing_threshold, Previous.at(0)->strainTime);
            }
        }

        const float jumpDistanceExp = applyDimishingExp(currentObject->jumpDistance);
        const float travelDistanceExp = applyDimishingExp(currentObject->travelDistance);

        return fmaxf(
                result + (jumpDistanceExp + travelDistanceExp + sqrtf(travelDistanceExp * jumpDistanceExp)) / fmaxf(currentObject->strainTime, (float)timing_threshold),
                (sqrtf(travelDistanceExp * jumpDistanceExp) + jumpDistanceExp + travelDistanceExp) / currentObject->strainTime);
    }
private:
    const float angle_bonus_begin = (float)M_PI / 3;
    const int timing_threshold = 107;
};

#endif //OSU_SR_CALCULATOR_SRCAIM_H
