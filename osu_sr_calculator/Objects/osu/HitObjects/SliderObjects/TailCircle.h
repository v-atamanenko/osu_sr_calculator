#ifndef OSU_SR_CALCULATOR_TAILCIRCLE_H
#define OSU_SR_CALCULATOR_TAILCIRCLE_H

#include "Objects/osu/HitObjects/HitCircle.h"
#include "Objects/Vector2.h"

class TailCircle : public HitCircle {
public:
    TailCircle(const Vector2& pos, float start_time, float m_radius = 0) : HitCircle(pos, start_time, m_radius) {}
};

#endif //OSU_SR_CALCULATOR_TAILCIRCLE_H
