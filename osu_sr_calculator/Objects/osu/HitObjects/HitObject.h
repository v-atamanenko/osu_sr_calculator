#ifndef OSU_SR_CALCULATOR_HITOBJECT_H
#define OSU_SR_CALCULATOR_HITOBJECT_H

#include "Objects/osu/HitType.h"
#include "Objects/Vector2.h"

/**
 * Base class for all osu! hit objects
 */
class HitObject {
public:
    Vector2* position = nullptr;
    Vector2* stackedPosition = nullptr;
    float startTime;
    float radius;
    int stackHeight = 0;

    HitObject(const Vector2& pos, float start_time, float m_radius = 0) {
        radius = m_radius;
        position = new Vector2(pos);
        startTime = start_time;
    }

    virtual ~HitObject() {
        delete position;
        delete stackedPosition;
    }

    void calculateStackedPosition(float scale) {
        float coordinate = (float)stackHeight * scale * -6.4f;
        Vector2 stackOffset = Vector2(coordinate, coordinate);
        if (position != nullptr) {
            delete stackedPosition;
            stackedPosition = new Vector2(position->add(stackOffset));
        }
    }

    virtual HitType getType() = 0;
};

#endif //OSU_SR_CALCULATOR_HITOBJECT_H
