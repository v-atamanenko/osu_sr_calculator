#ifndef OSU_SR_CALCULATOR_SRCREPEATPOINT_H
#define OSU_SR_CALCULATOR_SRCREPEATPOINT_H

#include "Objects/osu/HitObjects/SRCHitObject.h"
#include "Objects/SRCVector2.h"

class SRCRepeatPoint : public SRCHitObject {
public:
    int RepeatIndex;
    float SpanDuration;

    SRCRepeatPoint(const SRCVector2& pos, float start_time, int repeatIndex, float spanDuration, float m_radius = 0)
    : SRCHitObject(pos, start_time, m_radius) {
        RepeatIndex = repeatIndex;
        SpanDuration = spanDuration;
    }

    SRCHitType getType() override { return SRCHitType::HTRepeatPoint; }
};

#endif //OSU_SR_CALCULATOR_SRCREPEATPOINT_H
