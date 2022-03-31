#include <sstream>
#include "algorithm"
#include "Objects/osu/SRCHitType.h"
#include "Objects/osu/SRCPathType.h"
#include "Objects/SRCVector2.h"
#include "Objects/osu/SRCBeatmap.h"
#include "Objects/osu/HitObjects/SRCHitObject.h"
#include "Objects/osu/HitObjects/SRCHitCircle.h"
#include "Objects/osu/HitObjects/SRCSlider.h"
#include "Objects/osu/HitObjects/SRCSpinner.h"
#include "SliderPath.h"
#include "Precision.h"
#include "Logger.h"
#include "Objects/osu/SRCTimingPoint.h"

#include "BeatmapParser.h"

SRCBeatmap* BeatmapParser::parseBeatmap(const std::string& data, const std::vector<std::string>& mods, bool verbose) {
    logr = new Logger(verbose, "BeatmapParser");

    if (data.empty()) {
        logr->log("No beatmap data", "err");
        delete logr;
        return nullptr;
    }

    beatmap = new SRCBeatmap();
    std::string section;

    std::stringstream lines(data);
    std::string line;

    logr->log("Parsing beatmap");
    while(std::getline(lines,line,'\n')){
        line = Utils::trim(line);
        if (line.rfind("//", 0) == 0 || line.empty() || line == "\r") {
            continue;
        }

        if (section.empty() && line.rfind("osu file format v") != std::string::npos) {
            beatmap->version = std::stoi(Utils::str_split(line, "osu file format v", 2).at(1));
            continue;
        }

        if (line.at(0) == '[') {
            section = Utils::str_split(Utils::str_split(line, "[", 2).at(1), "]", 2).at(0);
            continue;
        }

        if (section == "General") {
            std::vector<std::string> section_val = Utils::str_split_and_strip(line, ":", 2);
            if (section_val.at(0) == "StackLeniency") {
                beatmap->stackLeniency = std::stof(section_val.at(1));
            }
        }

        else if (section == "Difficulty") {
            std::vector<std::string> section_val = Utils::str_split_and_strip(line, ":", 2);
            float f = std::stof(section_val.at(1));

            if (section_val.at(0) == "HPDrainRate") {
                beatmap->difficulty.HPDrainRate = f;
            } else if (section_val.at(0) == "CircleSize") {
                beatmap->difficulty.CircleSize = f;
            } else if (section_val.at(0) == "OverallDifficulty") {
                beatmap->difficulty.OverallDifficulty = f;
            } else if (section_val.at(0) == "ApproachRate") {
                beatmap->difficulty.ApproachRate = f;
            } else if (section_val.at(0) == "SliderMultiplier") {
                beatmap->difficulty.SliderMultiplier = f;
            } else if (section_val.at(0) == "SliderTickRate") {
                beatmap->difficulty.SliderTickRate = f;
            }
        }

        else if (section == "TimingPoints") {
            std::vector<std::string> section_val = Utils::str_split(line, ",", 8); // TODO: why 8?
            float time = std::stof(section_val.at(0)) + ((beatmap->version < 5) ? 24.f : 0.f);
            float beatLength = std::stof(section_val.at(1));
            float speedMultiplier = (beatLength < 0) ? 100 / (beatLength*-1) : 1;
            int timeSignature = 4;
            if (section_val.size() >= 3) {
                timeSignature = (section_val.at(2).at(0) == '0') ? 4 : std::stoi(section_val.at(2));
            }

            bool timingChange = true;
            if (section_val.size() >= 7) {
                timingChange = (section_val.at(6).at(0) == '1');
            }

            if (timingChange) {
                beatmap->timingPoints.emplace_back(SRCTimingPoint({time, beatLength, timeSignature}));
            }

            beatmap->difficultyTimingPoints.emplace_back(SRCDifficultyTimingPoint({time, speedMultiplier}));
        }

        else if (section == "HitObjects") {
            std::vector<std::string> split = Utils::str_split(line, ","); // TODO: why 16?
            SRCVector2 pos = SRCVector2(std::stof(split.at(0)), std::stof(split.at(1)));
            float startTime = std::stof(split.at(2));
            int hitType = std::stoi(split.at(3));

            SRCHitObject* result = nullptr;

            float scale = (1 - 0.7f * (getCircleSize(mods) - 5) / 5) / 2;
            float radius = 64 * scale;

            if (hitType & SRCHitType::HTNormal) {
                result = new SRCHitCircle(pos, startTime, radius);
            }

            if (hitType & SRCHitType::HTSlider) {
                SRCPathType pathType;
                float length = 0;
                std::vector<std::string> pointSplit = Utils::str_split(split.at(5), "|"); // TODO: why 16?

                std::vector<SRCVector2> points = {SRCVector2(0, 0)};
                for (auto &point : pointSplit) {
                    if (point.length() == 1) {
                        if (point.at(0) == 'C') {
                            pathType = SRCPathType::Catmull;
                        } else if (point.at(0) == 'B') {
                            pathType = SRCPathType::Bezier;
                        } else if (point.at(0) == 'L') {
                            pathType = SRCPathType::Linear;
                        } else if (point.at(0) == 'P') {
                            pathType = SRCPathType::PerfectCurve;
                        } else {
                            pathType = SRCPathType::Catmull;
                        }
                        continue;
                    }
                    std::vector<std::string> temp = Utils::str_split(point, ":"); // TODO: why 16?
                    points.push_back(SRCVector2(std::stof(temp.at(0)), std::stof(temp.at(1))).substract(pos));
                }

                if (points.size() == 3 && pathType == SRCPathType::PerfectCurve && Precision::isLinear(points)) {
                    pathType = SRCPathType::Linear;
                }

                int repeatCount = std::stoi(split.at(6));
                repeatCount = (int)fmax(0, repeatCount - 1);

                if (split.size() > 7) {
                    length = std::stof(split.at(7));
                }
                length = fmaxf(0, length);

                auto slider_path = new SliderPath(pathType, points, length);
                result = new SRCSlider(pos, startTime, slider_path,
                                       repeatCount, getSpeedMultiplier(startTime), getBeatLength(startTime),
                                       beatmap->difficulty, radius);
            }

            if (hitType & SRCHitType::HTSpinner) {
                float endTime = std::stof(split.at(5));
                result = new SRCSpinner(pos, startTime, endTime);
            }

            beatmap->hitObjects.emplace_back(result);
        }
    }

    logr->log("File parsed");

    applyStacking(0, (int)beatmap->hitObjects.size() - 1);
    logr->log("Stacking applied");

    float scale = (1 - 0.7f * (getCircleSize(mods) - 5) / 5) / 2;
    for (auto &h : beatmap->hitObjects) {
        h->calculateStackedPosition(scale);
    }
    logr->log("Calculated stacked position of HitObjects");

    delete logr;
    return beatmap;
}

float BeatmapParser::getSpeedMultiplier(float startTime) {
    SRCDifficultyTimingPoint currentTimingPoint = getTimingPoints(startTime, beatmap->difficultyTimingPoints);
    return currentTimingPoint.SpeedMultiplier;
}

float BeatmapParser::getBeatLength(float startTime) {
    SRCTimingPoint currentTimingPoint = getTimingPoints(startTime, beatmap->timingPoints);
    return currentTimingPoint.BeatLength;
}

SRCTimingPoint BeatmapParser::getTimingPoints(float startTime, std::vector<SRCTimingPoint> timingPoints) {
    std::sort(timingPoints.begin(), timingPoints.end(), sortTimingPoints);
    int currentTimingPoint = -999;
    for (int i = 0; i < timingPoints.size(); i++) {
        if (timingPoints.at(i).Time > startTime) {
            currentTimingPoint = i - 1;
            break;
        }
    }

    if (currentTimingPoint == -999) {
        currentTimingPoint = (int)timingPoints.size() - 1;
    }

    if (currentTimingPoint < 0) {
        currentTimingPoint = 0;
        logr->log("first timing point after current hit object ("+ std::to_string(startTime) +"). Defaulting to first timing point of the map.", "warn");
    }

    return timingPoints.at(currentTimingPoint);
}

SRCDifficultyTimingPoint BeatmapParser::getTimingPoints(float startTime, std::vector<SRCDifficultyTimingPoint> timingPoints) {
    std::sort(timingPoints.begin(), timingPoints.end(), sortTimingPoints);
    int currentTimingPoint = -1;
    for (int i = 0; i < timingPoints.size(); i++) {
        if (timingPoints.at(i).Time > startTime) {
            currentTimingPoint = i - 1;
            break;
        }
    }

    if (currentTimingPoint < 0) {
        currentTimingPoint = 0;
        //logr->log("first timing point after current hit object ("+ std::to_string(startTime) +"). Defaulting to first timing point of the map.", "warn");
    }

    return timingPoints.at(currentTimingPoint);
}

void BeatmapParser::applyStacking(int startIndex, int endIndex) {
    int stack_distance = 3;
    float TimePreempt;
    
    if (beatmap->difficulty.ApproachRate > 5) {
        TimePreempt = 1200 + (450 - 1200) * (beatmap->difficulty.ApproachRate - 5) / 5;
    } else if (beatmap->difficulty.ApproachRate < 5) {
        TimePreempt = 1200 - (1200 - 1800) * (5 - beatmap->difficulty.ApproachRate) / 5;
    } else {
        TimePreempt = 1200;
    }

    int extendedEndIndex = endIndex;

    if (endIndex < (beatmap->hitObjects.size() - 1)) {
        for (int i = endIndex; i >= startIndex; i--) {
            int stackBaseIndex = i;

            for (int n = (stackBaseIndex + 1); n < beatmap->hitObjects.size(); n++) {
                SRCHitObject* stackBaseObject = beatmap->hitObjects.at(stackBaseIndex);
                if (stackBaseObject->getType() == SRCHitType::HTSpinner) {
                    break;
                }

                SRCHitObject* objectN = beatmap->hitObjects.at(n);
                if (objectN->getType() == SRCHitType::HTSpinner) {
                    continue;
                }

                float endTime;

                if (stackBaseObject->getType() == SRCHitType::HTNormal) {
                    endTime = stackBaseObject->startTime;
                } else {
                    endTime = ((SRCSlider *)stackBaseObject)->EndTime;
                }

                float stackThreshold = (float)TimePreempt * beatmap->stackLeniency;

                if (objectN->startTime - endTime > stackThreshold) {
                    break;
                }

                bool endPositionDistanceCheck;
                if ((stackBaseObject->getType() == SRCHitType::HTSlider)) {
                    endPositionDistanceCheck = ((SRCSlider *) stackBaseObject)->EndPosition->distance(objectN->position) < (float)stack_distance;
                } else {
                    endPositionDistanceCheck = false;
                }

                if(stackBaseObject->position->distance(objectN->position) < (float)stack_distance || endPositionDistanceCheck) {
                    stackBaseIndex = n;
                    objectN->stackHeight = 0;
                }
            }

            if (stackBaseIndex > extendedEndIndex) {
                extendedEndIndex = stackBaseIndex;
                if (extendedEndIndex == beatmap->hitObjects.size()-1) {
                    break;
                }
            }
        }
    }

    int extendedStartIndex = startIndex;

    for (int i = extendedEndIndex; i > startIndex; i--) {
        int n = i;

        SRCHitObject* objectI = beatmap->hitObjects.at(i);
        if (objectI->stackHeight != 0 || objectI->getType() == SRCHitType::HTSpinner) {
            continue;
        }

        float stackThreshold = (float)TimePreempt * beatmap->stackLeniency;
        if (objectI->getType() == SRCHitType::HTNormal) {
            while (--n >= 0) {
                SRCHitObject* objectN = beatmap->hitObjects.at(n);
                if (objectN->getType() == SRCHitType::HTSpinner) {
                    continue;
                }

                float endTime;
                if (objectN->getType() == SRCHitType::HTNormal) {
                    endTime = objectN->startTime;
                } else {
                    endTime = ((SRCSlider *)objectN)->EndTime;
                }

                if (objectI->startTime - endTime > stackThreshold) {
                    break;
                }

                if (n < extendedStartIndex) {
                    objectN->stackHeight = 0;
                    extendedStartIndex = n;
                }

                bool endPositionDistanceCheck;
                if ((objectN->getType() == SRCHitType::HTSlider)) {
                    endPositionDistanceCheck = ((SRCSlider *)objectN)->EndPosition->distance(objectI->position) < (float)stack_distance;
                } else {
                    endPositionDistanceCheck = false;
                }

                if (endPositionDistanceCheck) {
                    int offset = objectI->stackHeight - objectN->stackHeight + 1;
                    for (int j = (n+1); j <= i; j++) {
                        SRCHitObject* objectJ = beatmap->hitObjects.at(j);
                        if(((SRCSlider *)objectN)->EndPosition->distance(objectJ->position) < (float)stack_distance) {
                            objectJ->stackHeight -= offset;
                        }
                    }
                    break;
                }

                if(objectN->position->distance(objectI->position) < (float)stack_distance) {
                    objectN->stackHeight = objectI->stackHeight + 1;
                    objectI = objectN;
                }

            }
        }
        else if (objectI->getType() == SRCHitType::HTSlider) {
            while (--n >= startIndex) {
                SRCHitObject* objectN = beatmap->hitObjects.at(n);
                if (objectN->getType() == SRCHitType::HTSpinner) {
                    continue;
                }

                if (objectI->startTime - objectN->startTime > stackThreshold) {
                    break;
                }

                SRCVector2* objectNEndPosition;
                if (objectN->getType() == SRCHitType::HTNormal) {
                    objectNEndPosition = objectN->position;
                } else {
                    objectNEndPosition = ((SRCSlider *)objectN)->EndPosition;
                }

                if (objectNEndPosition->distance(objectI->position) < (float)stack_distance) {
                    objectN->stackHeight = objectI->stackHeight + 1;
                    objectI = objectN;
                }
            }
        }
    }
}

float BeatmapParser::getCircleSize(const std::vector<std::string>& mods) {
    if (std::find(mods.begin(), mods.end(), "HR") != mods.end()) {
        return fminf(beatmap->difficulty.CircleSize * 1.3f, 10);
    }
    if (std::find(mods.begin(), mods.end(), "EZ") != mods.end()) {
        return beatmap->difficulty.CircleSize * 0.5f;
    }
    return beatmap->difficulty.CircleSize;
}
