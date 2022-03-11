#ifndef OSU_SR_CALCULATOR_BEATMAP_H
#define OSU_SR_CALCULATOR_BEATMAP_H

#include "HitObjects/HitObject.h"
#include "Objects/osu/TimingPoint.h"
#include "vector"

/**
 * Class for beatmaps
 */
class Beatmap {
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
    std::vector<HitObject*> hitObjects;

    /**
     * Timing points in this beatmap, focussed on timing settings
     */
    std::vector<TimingPoint> timingPoints;

    /**
     * Timing points in this beatmap, focussed on difficulty
     */
    std::vector<DifficultyTimingPoint> difficultyTimingPoints = {};

    /**
    * .osu file version
    */
    int version = 0;

    /**
     * Stack leniency used in the beatmap
     */
    float stackLeniency = 0;
};

#endif //OSU_SR_CALCULATOR_BEATMAP_H
