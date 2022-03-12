#include "DifficultyHitObjectCreator.h"
#include "Objects/osu/SRCHitType.h"

std::vector<SRCDifficultyHitObject*> DifficultyHitObjectCreator::convertToDifficultyHitObjects(std::vector<SRCHitObject*> hitObjects, float time_rate) {
    for (int i = 1; i < hitObjects.size(); ++i) {
        SRCHitObject* lastLast = (i > 1) ? hitObjects.at(i - 2) : nullptr;
        SRCHitObject* last = hitObjects.at(i - 1);
        SRCHitObject* current = hitObjects.at(i);

        SRCDifficultyHitObject* difficultyHitObject = createDifficultyHitObject(lastLast, last, current, time_rate);
        difficultyHitObjects.push_back(difficultyHitObject);
    }

    return difficultyHitObjects;
}

SRCDifficultyHitObject* DifficultyHitObjectCreator::createDifficultyHitObject(SRCHitObject* lastLast, SRCHitObject* last, SRCHitObject* current, float time_rate) {
    lastLastObject = lastLast;
    lastObject = last;
    currentObject = current;
    timeRate = time_rate;

    setDistances();
    setTimingValues();

    return new SRCDifficultyHitObject(currentObject, lastObject, lastLastObject, TravelDistance, JumpDistance, Angle, DeltaTime, StrainTime);
}

void DifficultyHitObjectCreator::setDistances() {
    TravelDistance = 0;
    JumpDistance = 0;
    Angle = 0;
    DeltaTime = 0;
    StrainTime = 0;

    float scalingFactor = normalized_radius / currentObject->radius;
    if (currentObject->radius < 30) {
        float smallCircleBonus = fminf(30 - currentObject->radius, 5) / 50;
        scalingFactor *= 1 + smallCircleBonus;
    }

    if (lastObject->getType() == SRCHitType::HTSlider) {
        auto lastSlider = (SRCSlider*)lastObject;
        computeSliderCursorPosition(lastSlider);
        TravelDistance = lastSlider->LazyTravelDistance * scalingFactor;
    }

    SRCVector2 lastCursorPosition = getEndCursorPosition(lastObject);

    if (currentObject->getType() != SRCHitType::HTSpinner) {
        JumpDistance = currentObject->stackedPosition->scale(scalingFactor).substract(lastCursorPosition.scale(scalingFactor)).length();
    }

    if (lastLastObject != nullptr) {
        SRCVector2 lastLastCursorPosition = getEndCursorPosition(lastLastObject);

        SRCVector2 v1 = lastLastCursorPosition.substract(lastObject->stackedPosition);
        SRCVector2 v2 = currentObject->stackedPosition->substract(lastCursorPosition);

        float dot = v1.dot(v2);
        float det = v1.x * v2.y - v1.y * v2.x;

        Angle = abs(atan2f(det, dot));
    }
}

void DifficultyHitObjectCreator::setTimingValues() {
    DeltaTime = (currentObject->startTime - lastObject->startTime) / timeRate;
    StrainTime = fmaxf(50, DeltaTime);
}

void DifficultyHitObjectCreator::computeSliderCursorPosition(SRCSlider* slider) {
    if (slider->LazyEndPosition != nullptr) {
        return;
    }

    slider->LazyEndPosition = new SRCVector2(slider->stackedPosition);
    slider->LazyTravelDistance = 0;

    float approxFollowCircleRadius = slider->radius * 3;
    for (int i = 1; i < slider->NestedHitObjects.size(); ++i) {
        float t = slider->NestedHitObjects.at(i)->startTime;
        int progress = floor((t - slider->startTime) / slider->SpanDuration);
        if (progress % 2 >= 1) {
            progress = 1 - progress % 1;
        } else {
            progress = progress % 1;
        }

        SRCVector2 diff = slider->stackedPosition->add(slider->Path->PositionAt((float)progress)).substract(slider->LazyEndPosition);
        float dist = diff.length();

        if (dist > approxFollowCircleRadius) {
            diff.normalize();
            dist -= approxFollowCircleRadius;
            auto* NewLazyEndPosition = new SRCVector2(slider->LazyEndPosition->add(diff.scale(dist)));
            delete slider->LazyEndPosition;
            slider->LazyEndPosition = NewLazyEndPosition;
            slider->LazyTravelDistance = (slider->LazyTravelDistance == -999.f) ? dist : slider->LazyTravelDistance + dist;
        }
    }
}

SRCVector2 DifficultyHitObjectCreator::getEndCursorPosition(SRCHitObject* obj) {
    SRCVector2 pos = SRCVector2(obj->stackedPosition);
    if (obj->getType() == SRCHitType::HTSlider) {
        computeSliderCursorPosition((SRCSlider *)obj);
        pos = (((SRCSlider *)obj)->LazyEndPosition != nullptr) ? SRCVector2(((SRCSlider *)obj)->LazyEndPosition) : pos;
    }

    return pos;
}
