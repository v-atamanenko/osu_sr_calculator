#ifndef OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H
#define OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H

#include "Objects/osu/HitObjects/SRCHitObject.h"
#include "Objects/osu/HitObjects/SRCDifficultyHitObject.h"
#include "Objects/osu/HitObjects/SRCSlider.h"
#include "Objects/osu/HitObjects/SRCSpinner.h"
#include "Objects/SRCVector2.h"
#include <cmath>
#include "vector"

class DifficultyHitObjectCreator {
public:
    std::vector<SRCDifficultyHitObject*> convertToDifficultyHitObjects(std::vector<SRCHitObject*> hitObjects, float time_rate);
    SRCDifficultyHitObject* createDifficultyHitObject(SRCHitObject* lastLast, SRCHitObject* last, SRCHitObject* current, float time_rate);
    void setDistances();
    void setTimingValues();
    void computeSliderCursorPosition(SRCSlider* slider);
    SRCVector2 getEndCursorPosition(SRCHitObject* obj);

private:
    std::vector<SRCDifficultyHitObject*> difficultyHitObjects = {};

    SRCHitObject* lastLastObject = nullptr;
    SRCHitObject* lastObject = nullptr;
    SRCHitObject* currentObject = nullptr;
    float timeRate;

    float normalized_radius = 52;
    float TravelDistance = 0;
    float JumpDistance = 0;
    float Angle = 0;
    float DeltaTime = 0;
    float StrainTime = 0;
};

#endif //OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H