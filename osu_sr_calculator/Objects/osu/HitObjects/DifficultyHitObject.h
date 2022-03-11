#ifndef OSU_SR_CALCULATOR_DIFFICULTYHITOBJECT_H
#define OSU_SR_CALCULATOR_DIFFICULTYHITOBJECT_H

#include "HitObject.h"

class DifficultyHitObject : public HitObject {
public:
    float travelDistance;
    float jumpDistance;
    float angle;
    float deltaTime;
    float strainTime;

    HitObject* currentObject;
    HitObject* lastObject;
    HitObject* lastLastObject;

    /**
     *
     * @param CurrentObject HitObject to create a DifficultyHitObject from
     * @param LastObject HitObject before the current HitObject
     * @param LastLastObject HitObject before the lastObject
     * @param TravelDistance The calculated Travel Distance for this HitObject
     * @param JumpDistance The calculated Jump Distance for this HitObject
     * @param Angle The calculated Angle for this HitObject
     * @param DeltaTime The calculated Deltatime for this HitObject
     * @param StrainTime The calculated Straintime for this hitObject
     */
    DifficultyHitObject(HitObject* CurrentObject, HitObject* LastObject, HitObject* LastLastObject,
                        float TravelDistance, float JumpDistance, float Angle, float DeltaTime, float StrainTime)
                        : HitObject(*CurrentObject->position, CurrentObject->startTime, CurrentObject->radius)
    {
        travelDistance = TravelDistance;
        jumpDistance = JumpDistance;
        angle = Angle;
        deltaTime = DeltaTime;
        strainTime = StrainTime;

        currentObject = CurrentObject;
        lastObject = LastObject;
        lastLastObject = LastLastObject;
    }

    HitType getType() override { return HitType::HTDifficulty; }
};

#endif //OSU_SR_CALCULATOR_DIFFICULTYHITOBJECT_H
