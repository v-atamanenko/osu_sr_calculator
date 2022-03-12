#ifndef OSU_SR_CALCULATOR_SRCHITOBJECT_H
#define OSU_SR_CALCULATOR_SRCHITOBJECT_H

#include "Objects/osu/SRCHitType.h"
#include "Objects/SRCVector2.h"

/**
 * Base class for all osu! hit objects
 */
class SRCHitObject {
public:
    SRCVector2* position = nullptr;
    SRCVector2* stackedPosition = nullptr;
    float startTime;
    float radius;
    int stackHeight = 0;

    SRCHitObject(const SRCVector2& pos, float start_time, float m_radius = 0) {
        radius = m_radius;
        position = new SRCVector2(pos);
        startTime = start_time;
    }

    virtual ~SRCHitObject() {
        delete position;
        delete stackedPosition;
    }

    void calculateStackedPosition(float scale) {
        float coordinate = (float)stackHeight * scale * -6.4f;
        SRCVector2 stackOffset = SRCVector2(coordinate, coordinate);
        if (position != nullptr) {
            delete stackedPosition;
            stackedPosition = new SRCVector2(position->add(stackOffset));
        }
    }

    virtual SRCHitType getType() = 0;
};

#endif //OSU_SR_CALCULATOR_SRCHITOBJECT_H
