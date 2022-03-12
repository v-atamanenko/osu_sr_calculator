#ifndef OSU_SR_CALCULATOR_SRCSPINNER_H
#define OSU_SR_CALCULATOR_SRCSPINNER_H

#include "SRCHitObject.h"
#include "Objects/SRCVector2.h"

/**
 * Class for spinners
 */
class SRCSpinner : public SRCHitObject {
public:
    float EndTime;

    /**
     *
     * @param start_time The start time of the spinner
     * @param end_time The end time of the spinner
     */
    SRCSpinner(const SRCVector2& pos, float start_time, float end_time) : SRCHitObject(pos, start_time) {
        EndTime = end_time;
    }

    SRCHitType getType() override { return SRCHitType::HTSpinner; }
};

#endif //OSU_SR_CALCULATOR_SRCSPINNER_H
