#ifndef OSU_SR_CALCULATOR_REPEATPOINT_H
#define OSU_SR_CALCULATOR_REPEATPOINT_H

#include "Objects/osu/HitObjects/HitObject.h"
#include "Objects/Vector2.h"

class RepeatPoint : public HitObject {
public:
    int RepeatIndex;
    float SpanDuration;

    RepeatPoint(const Vector2& pos, float start_time, int repeatIndex, float spanDuration, float m_radius = 0)
    : HitObject(pos, start_time, m_radius) {
        RepeatIndex = repeatIndex;
        SpanDuration = spanDuration;
    }

    HitType getType() override { return HitType::HTRepeatPoint; }
};

#endif //OSU_SR_CALCULATOR_REPEATPOINT_H
