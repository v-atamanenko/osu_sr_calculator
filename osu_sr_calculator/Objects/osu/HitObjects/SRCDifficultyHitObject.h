#ifndef OSU_SR_CALCULATOR_SRCDIFFICULTYHITOBJECT_H
#define OSU_SR_CALCULATOR_SRCDIFFICULTYHITOBJECT_H

#include "SRCHitObject.h"

class SRCDifficultyHitObject : public SRCHitObject {
public:
    float travelDistance;
    float jumpDistance;
    float angle;
    float deltaTime;
    float strainTime;

    SRCHitObject* currentObject;
    SRCHitObject* lastObject;
    SRCHitObject* lastLastObject;

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
    SRCDifficultyHitObject(SRCHitObject* CurrentObject, SRCHitObject* LastObject, SRCHitObject* LastLastObject,
                           float TravelDistance, float JumpDistance, float Angle, float DeltaTime, float StrainTime)
                        : SRCHitObject(*CurrentObject->position, CurrentObject->startTime, CurrentObject->radius)
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

    SRCHitType getType() override { return SRCHitType::HTDifficulty; }
};

#endif //OSU_SR_CALCULATOR_SRCDIFFICULTYHITOBJECT_H
