#ifndef OSU_SR_CALCULATOR_SRCHITCIRCLE_H
#define OSU_SR_CALCULATOR_SRCHITCIRCLE_H

#include "SRCHitObject.h"
#include "Objects/SRCVector2.h"

/**
 * Class for hit circles
 */
class SRCHitCircle : public SRCHitObject {
public:
    /**
     *
     * @param pos The raw position of the hit circle (as listed in the .osu file)
     * @param start_time The hit time of the hit circle in ms from start
     * @param m_radius The radius of the hit circle
     */
    SRCHitCircle(const SRCVector2& pos, float start_time, float m_radius = 0) : SRCHitObject(pos, start_time, m_radius) {}
    SRCHitType getType() override { return SRCHitType::HTNormal; }
};

#endif //OSU_SR_CALCULATOR_SRCHITCIRCLE_H
