#ifndef OSU_SR_CALCULATOR_HITCIRCLE_H
#define OSU_SR_CALCULATOR_HITCIRCLE_H

#include "HitObject.h"
#include "Objects/Vector2.h"

/**
 * Class for hit circles
 */
class HitCircle : public HitObject {
public:
    /**
     *
     * @param pos The raw position of the hit circle (as listed in the .osu file)
     * @param start_time The hit time of the hit circle in ms from start
     * @param m_radius The radius of the hit circle
     */
    HitCircle(const Vector2& pos, float start_time, float m_radius = 0) : HitObject(pos, start_time, m_radius) {}
    HitType getType() override { return HitType::HTNormal; }
};

#endif //OSU_SR_CALCULATOR_HITCIRCLE_H
