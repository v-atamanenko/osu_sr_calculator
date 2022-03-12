#ifndef OSU_SR_CALCULATOR_SRCSLIDERTICK_H
#define OSU_SR_CALCULATOR_SRCSLIDERTICK_H

#include "Objects/osu/HitObjects/SRCHitObject.h"
#include "Objects/SRCVector2.h"

class SRCSliderTick : public SRCHitObject {
public:
    int SpanIndex;
    float SpanStartTime;

    SRCSliderTick(const SRCVector2& pos, float start_time, int spanIndex, float spanStartTime, float m_radius = 0)
            : SRCHitObject(pos, start_time, m_radius) {
        SpanIndex = spanIndex;
        SpanStartTime = spanStartTime;
    }

    SRCHitType getType() override { return SRCHitType::HTSliderTick; }
};

#endif //OSU_SR_CALCULATOR_SRCSLIDERTICK_H
