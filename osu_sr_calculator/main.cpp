#include "BeatmapParser.h"
#include "DifficultyHitObjectCreator.h"
#include "StarRatingCalculator.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Utils.h"
#include "osu_sr_calculator.h"
#include "Objects/osu/SRCBeatmap.h"
#include "Objects/osu/HitObjects/SRCDifficultyHitObject.h"

Output calculateNextModCombination(SRCBeatmap* beatmap, const std::vector<std::string>& mods);
float getTimeRate(const std::vector<std::string>& mods);
std::vector<ModCombination> getAllModCombinations();
std::string getLocalOsuBeatmap(const std::string& filePath);

std::map<std::string, Output> calculateStarRating(
                                const std::string& filepath,
                                const std::vector<std::string>& mods,
                                bool allCombinations,
                                bool verbose) {
    BeatmapParser beatmapParser;
    SRCBeatmap* beatmap = nullptr;
    std::string Map = getLocalOsuBeatmap(filepath);

    if (Map.empty()) {
        fprintf(stderr, "No map found for specified map id. / wrong filepath\n");
    }

    std::map<std::string, Output> output;

    if (!allCombinations) {
        std::string label = (!mods.empty()) ? Utils::str_join(mods, "") : "nomod";
        beatmap = beatmapParser.parseBeatmap(Map, mods, verbose);
        Output response = calculateNextModCombination(beatmap, mods);
        output[label] = response;

    } else {
        std::vector<ModCombination> allModCombinations = getAllModCombinations();

        for (auto &combi : allModCombinations) {
            std::string label = (!combi.mods.empty()) ? Utils::str_join(combi.mods, "") : "nomod";

            if (combi.reParse || beatmap == nullptr) {
                beatmap = beatmapParser.parseBeatmap(Map, mods, verbose);
            }

            Output response = calculateNextModCombination(beatmap, combi.mods);
            output[label] = response;
        }
    }

    return output;
}

Output calculateNextModCombination(SRCBeatmap* beatmap, const std::vector<std::string>& mods) {
    DifficultyHitObjectCreator difficultyHitObjectCreator;
    StarRatingCalculator starRatingCalculator;
    float timeRate = getTimeRate(mods);
    std::vector<SRCDifficultyHitObject*> difficultyHitObjects = difficultyHitObjectCreator.convertToDifficultyHitObjects(beatmap->hitObjects, timeRate);
    return starRatingCalculator.calculate(difficultyHitObjects, timeRate);
}

float getTimeRate(const std::vector<std::string>& mods) {
    if (std::count(mods.begin(), mods.end(), "DT")) {
        return 1.5f;
    }

    if (std::count(mods.begin(), mods.end(), "HT")) {
        return 0.75f;
    }

    return 1.f;
}

std::vector<ModCombination> getAllModCombinations() {
    std::vector <ModCombination> allModCombinations;
    allModCombinations.push_back({{}, true});
    allModCombinations.push_back({{"DT"}, false});
    allModCombinations.push_back({{"HT"}, false});
    allModCombinations.push_back({{"HR"}, true});
    allModCombinations.push_back({{"HR", "DT"}, false});
    allModCombinations.push_back({{"HR", "HT"}, false});
    allModCombinations.push_back({{"EZ"}, true});
    allModCombinations.push_back({{"EZ", "DT"}, false});
    allModCombinations.push_back({{"EZ", "HT"}, false});
    return allModCombinations;
}

std::string getLocalOsuBeatmap(const std::string& filePath) {
    std::ifstream t(filePath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}
