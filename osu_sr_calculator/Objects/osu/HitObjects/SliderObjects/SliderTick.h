#ifndef OSU_SR_CALCULATOR_SLIDERTICK_H
#define OSU_SR_CALCULATOR_SLIDERTICK_H

#include "Objects/osu/HitObjects/HitObject.h"
#include "Objects/Vector2.h"

class SliderTick : public HitObject {
public:
    int SpanIndex;
    float SpanStartTime;

    SliderTick(const Vector2& pos, float start_time, int spanIndex, float spanStartTime, float m_radius = 0)
            : HitObject(pos, start_time, m_radius) {
        SpanIndex = spanIndex;
        SpanStartTime = spanStartTime;
    }

    HitType getType() override { return HitType::HTSliderTick; }
};

#endif //OSU_SR_CALCULATOR_SLIDERTICK_H
