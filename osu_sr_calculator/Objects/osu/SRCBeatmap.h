#ifndef OSU_SR_CALCULATOR_SRCBEATMAP_H
#define OSU_SR_CALCULATOR_SRCBEATMAP_H

#include "HitObjects/SRCHitObject.h"
#include "Objects/osu/SRCTimingPoint.h"
#include "vector"

/**
 * Class for beatmaps
 */
class SRCBeatmap {
public:
    /**
     * Difficulty settings of the beatmap
     */
    struct Difficulty {
        float HPDrainRate = 0;
        float CircleSize = 0;
        float OverallDifficulty = 0;
        float ApproachRate = 0;
        float SliderMultiplier = 0;
        float SliderTickRate = 0;
    };
    Difficulty difficulty;

    /**
     * Hit objects in this beatmap
     */
    std::vector<SRCHitObject*> hitObjects;

    /**
     * Timing points in this beatmap, focussed on timing settings
     */
    std::vector<SRCTimingPoint> timingPoints;

    /**
     * Timing points in this beatmap, focussed on difficulty
     */
    std::vector<SRCDifficultyTimingPoint> difficultyTimingPoints = {};

    /**
    * .osu file version
    */
    int version = 0;

    /**
     * Stack leniency used in the beatmap
     */
    float stackLeniency = 0;
};

#endif //OSU_SR_CALCULATOR_SRCBEATMAP_H
