#include "StarRatingCalculator.h"

Output StarRatingCalculator::calculate(const std::vector<SRCDifficultyHitObject*>& hitObjects, float timeRate) const {
    SRCAim aimSkill = SRCAim();
    SRCSpeed speedSkill = SRCSpeed();

    float sectionLength = (float)section_length * timeRate;

    float currentSectionEnd = ceil(((hitObjects.at(0) != nullptr) ? hitObjects.at(0)->startTime : 0) / sectionLength) * sectionLength;

    for (auto &h : hitObjects) {
        while (h->currentObject->startTime > currentSectionEnd) {
            aimSkill.saveCurrentPeak();
            aimSkill.startNewSectionFrom(currentSectionEnd);

            speedSkill.saveCurrentPeak();
            speedSkill.startNewSectionFrom(currentSectionEnd);

            currentSectionEnd += sectionLength;
        }

        aimSkill.process(h);
        speedSkill.process(h);
    }

    aimSkill.saveCurrentPeak();
    speedSkill.saveCurrentPeak();

    float aimRating = sqrt(aimSkill.difficultyValue()) * difficlty_multiplier;
    float speedRating = sqrt(speedSkill.difficultyValue()) * difficlty_multiplier;
    float starRating = aimRating + speedRating + abs(aimRating - speedRating) / 2;

    return Output({aimRating, speedRating, starRating});
}
