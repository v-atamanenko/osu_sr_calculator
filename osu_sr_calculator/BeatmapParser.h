#ifndef OSU_SR_CALCULATOR_BEATMAPPARSER_H
#define OSU_SR_CALCULATOR_BEATMAPPARSER_H

#include "string"
#include "vector"
#include "Objects/osu/TimingPoint.h"
#include "Objects/osu/Beatmap.h"
#include "Logger.h"

inline bool sortTimingPoints(const TimingPointContainer &a, const TimingPointContainer &b) {
    return a.Time < b.Time;
}

class BeatmapParser {
public:
    BeatmapParser() = default;
    Beatmap* parseBeatmap(const std::string& data, const std::vector<std::string>& mods, bool verbose);
private:
    Beatmap* beatmap = nullptr;
    Logger* logr = nullptr;

    float getCircleSize(const std::vector<std::string>& mods);
    float getSpeedMultiplier(float startTime);
    float getBeatLength(float startTime);
    void applyStacking(int startIndex, int endIndex);

    TimingPoint getTimingPoints(float startTime, std::vector<TimingPoint> timingPoints);
    DifficultyTimingPoint getTimingPoints(float startTime, std::vector<DifficultyTimingPoint> timingPoints);



};


#endif //OSU_SR_CALCULATOR_BEATMAPPARSER_H
