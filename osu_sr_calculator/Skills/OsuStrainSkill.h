#ifndef OSU_SR_CALCULATOR_OSUSTRAINSKILL_H
#define OSU_SR_CALCULATOR_OSUSTRAINSKILL_H

#include "Objects/osu/HitObjects/DifficultyHitObject.h"
#include "Objects/osu/HitObjects/Spinner.h"

#include "vector"

#define SINGLE_SPACING_THRESHOLD 125
#define STREAM_SPACING_THRESHOLD 110

class OsuStrainSkill {
public:
    std::vector<DifficultyHitObject *> Previous = {};

    float currentStrain = 1;
    float currentSectionPeak = 1;
    std::vector<float> strainPeaks;

    float SkillMultiplier;
    float StrainDecayBase;

    void saveCurrentPeak() {
        if (!Previous.empty()) {
            strainPeaks.push_back(currentSectionPeak);
        }
    }

    void startNewSectionFrom(float offset) {
        if (!Previous.empty()) {
            currentSectionPeak = currentStrain * strainDecay(offset - Previous.at(0)->currentObject->startTime);
        }
    }

    void process(DifficultyHitObject* currentObject) {
        currentStrain *= strainDecay(currentObject->deltaTime);
        if (currentObject->currentObject->getType() != HitType::HTSpinner) {
            currentStrain += strainValueOf(currentObject) * SkillMultiplier;
        }

        currentSectionPeak = fmax(currentStrain, currentSectionPeak);

        addToHistory(currentObject);
    }

    float difficultyValue() {
        std::sort(strainPeaks.begin(), strainPeaks.end(), std::greater<>());

        float difficulty = 0;
        float weight = 1;

        for (auto &strain : strainPeaks) {
            difficulty += strain * weight;
            weight *= 0.9;
        }

        return difficulty;
    }

    virtual float strainValueOf(DifficultyHitObject* currentObject) = 0;

    float strainDecay(float ms) const {
        return powf(StrainDecayBase, ms / 1000);
    }

    void addToHistory(DifficultyHitObject* currentObject) {
        Previous.insert(Previous.begin(), currentObject);
        if (Previous.size() > 2) {
            Previous.pop_back();
        }
    }
};

#endif //OSU_SR_CALCULATOR_OSUSTRAINSKILL_H
