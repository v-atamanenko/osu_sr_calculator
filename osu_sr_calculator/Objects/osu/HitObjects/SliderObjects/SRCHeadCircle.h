#ifndef OSU_SR_CALCULATOR_SRCHEADCIRCLE_H
#define OSU_SR_CALCULATOR_SRCHEADCIRCLE_H

#include "Objects/osu/HitObjects/SRCHitCircle.h"
#include "Objects/SRCVector2.h"

class SRCHeadCircle : public SRCHitCircle {
public:
    SRCHeadCircle(const SRCVector2& pos, float start_time, float m_radius = 0) : SRCHitCircle(pos, start_time, m_radius) {}
};

#endif //OSU_SR_CALCULATOR_SRCHEADCIRCLE_H
