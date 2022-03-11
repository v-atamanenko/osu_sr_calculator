#ifndef OSU_SR_CALCULATOR_SPEED_H
#define OSU_SR_CALCULATOR_SPEED_H

#include "cmath"

#include "OsuStrainSkill.h"
#include "Objects/osu/HitObjects/DifficultyHitObject.h"

class Speed : public OsuStrainSkill {
public:
    Speed() {
        SkillMultiplier = 1400;
        StrainDecayBase = 0.3;
    }

    float strainValueOf(DifficultyHitObject* currentObject) override {
        const float distance = fminf(SINGLE_SPACING_THRESHOLD, currentObject->travelDistance + currentObject->jumpDistance);
        const float deltaTime = fmaxf(max_speed_bonus, currentObject->deltaTime);

        float speedBonus = 1.0f;
        if (deltaTime < min_speed_bonus) {
            speedBonus = 1 + powf((min_speed_bonus - deltaTime) / speed_balancing_factor, 2);
        }

        float angleBonus = 1.0f;
        if (currentObject->angle > 0 && currentObject->angle < angle_bonus_begin) {
            angleBonus = 1.f + powf(sinf(1.5f * (angle_bonus_begin - currentObject->angle)), 2) / 3.57f;
            if (currentObject->angle < pi_over_2) {
                angleBonus = 1.28;
                if (distance < 90 && currentObject->angle < pi_over_4) {
                    angleBonus += (1 - angleBonus) * fminf((90 - distance) / 10, 1);
                } else if (distance < 90) {
                    angleBonus += (1 - angleBonus) * fminf((90 - distance) / 10, 1) * sinf((pi_over_2 - currentObject->angle) / pi_over_4);
                }
            }
        }

        return (1 + (speedBonus - 1) * 0.75f) * angleBonus * (0.95f + speedBonus * powf(distance / SINGLE_SPACING_THRESHOLD, 3.5f)) / currentObject->strainTime;
    }

private:
    const float angle_bonus_begin = 5 * (float)M_PI / 6;
    const float pi_over_4 = (float)M_PI / 4;
    const float pi_over_2 = (float)M_PI / 2;

    const float min_speed_bonus = 75;
    const float max_speed_bonus = 45;
    const float speed_balancing_factor = 40;
};

#endif //OSU_SR_CALCULATOR_SPEED_H
