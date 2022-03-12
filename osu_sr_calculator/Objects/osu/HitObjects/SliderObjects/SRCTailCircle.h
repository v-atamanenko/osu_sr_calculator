#ifndef OSU_SR_CALCULATOR_SRCTAILCIRCLE_H
#define OSU_SR_CALCULATOR_SRCTAILCIRCLE_H

#include "Objects/osu/HitObjects/SRCHitCircle.h"
#include "Objects/SRCVector2.h"

class SRCTailCircle : public SRCHitCircle {
public:
    SRCTailCircle(const SRCVector2& pos, float start_time, float m_radius = 0) : SRCHitCircle(pos, start_time, m_radius) {}
};

#endif //OSU_SR_CALCULATOR_SRCTAILCIRCLE_H
