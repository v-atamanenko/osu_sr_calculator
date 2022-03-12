#ifndef OSU_SR_CALCULATOR_BEATMAPPARSER_H
#define OSU_SR_CALCULATOR_BEATMAPPARSER_H

#include "string"
#include "vector"
#include "Objects/osu/SRCTimingPoint.h"
#include "Objects/osu/SRCBeatmap.h"
#include "Logger.h"

inline bool sortTimingPoints(const SRCTimingPointContainer &a, const SRCTimingPointContainer &b) {
    return a.Time < b.Time;
}

class BeatmapParser {
public:
    BeatmapParser() = default;
    SRCBeatmap* parseBeatmap(const std::string& data, const std::vector<std::string>& mods, bool verbose);
private:
    SRCBeatmap* beatmap = nullptr;
    Logger* logr = nullptr;

    float getCircleSize(const std::vector<std::string>& mods);
    float getSpeedMultiplier(float startTime);
    float getBeatLength(float startTime);
    void applyStacking(int startIndex, int endIndex);

    SRCTimingPoint getTimingPoints(float startTime, std::vector<SRCTimingPoint> timingPoints);
    SRCDifficultyTimingPoint getTimingPoints(float startTime, std::vector<SRCDifficultyTimingPoint> timingPoints);



};


#endif //OSU_SR_CALCULATOR_BEATMAPPARSER_H
