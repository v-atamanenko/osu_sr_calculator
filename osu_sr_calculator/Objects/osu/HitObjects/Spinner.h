#ifndef OSU_SR_CALCULATOR_SPINNER_H
#define OSU_SR_CALCULATOR_SPINNER_H

#include "HitObject.h"
#include "Objects/Vector2.h"

/**
 * Class for spinners
 */
class Spinner : public HitObject {
public:
    float EndTime;

    /**
     *
     * @param start_time The start time of the spinner
     * @param end_time The end time of the spinner
     */
    Spinner(const Vector2& pos, float start_time, float end_time) : HitObject(pos, start_time) {
        EndTime = end_time;
    }

    HitType getType() override { return HitType::HTSpinner; }
};

#endif //OSU_SR_CALCULATOR_SPINNER_H
