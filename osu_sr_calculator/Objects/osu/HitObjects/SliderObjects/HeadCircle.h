#ifndef OSU_SR_CALCULATOR_HEADCIRCLE_H
#define OSU_SR_CALCULATOR_HEADCIRCLE_H

#include "Objects/osu/HitObjects/HitCircle.h"
#include "Objects/Vector2.h"

class HeadCircle : public HitCircle {
public:
    HeadCircle(const Vector2& pos, float start_time, float m_radius = 0) : HitCircle(pos, start_time, m_radius) {}
};

#endif //OSU_SR_CALCULATOR_HEADCIRCLE_H
