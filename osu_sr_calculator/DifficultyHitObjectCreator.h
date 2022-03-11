#ifndef OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H
#define OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H

#include "Objects/osu/HitObjects/HitObject.h"
#include "Objects/osu/HitObjects/DifficultyHitObject.h"
#include "Objects/osu/HitObjects/Slider.h"
#include "Objects/osu/HitObjects/Spinner.h"
#include "Objects/Vector2.h"
#include <cmath>
#include "vector"

class DifficultyHitObjectCreator {
public:
    std::vector<DifficultyHitObject*> convertToDifficultyHitObjects(std::vector<HitObject*> hitObjects, float time_rate);
    DifficultyHitObject* createDifficultyHitObject(HitObject* lastLast, HitObject* last, HitObject* current, float time_rate);
    void setDistances();
    void setTimingValues();
    void computeSliderCursorPosition(Slider* slider);
    Vector2 getEndCursorPosition(HitObject* obj);

private:
    std::vector<DifficultyHitObject*> difficultyHitObjects = {};

    HitObject* lastLastObject = nullptr;
    HitObject* lastObject = nullptr;
    HitObject* currentObject = nullptr;
    float timeRate;

    float normalized_radius = 52;
    float TravelDistance = 0;
    float JumpDistance = 0;
    float Angle = 0;
    float DeltaTime = 0;
    float StrainTime = 0;
};

#endif //OSU_SR_CALCULATOR_DIFFICULTYHITOBJECTCREATOR_H