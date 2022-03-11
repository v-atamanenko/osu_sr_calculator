#include "BeatmapParser.h"
#include "DifficultyHitObjectCreator.h"
#include "StarRatingCalculator.h"

#include "vector"
#include "map"
#include <fstream>
#include <sstream>
#include <iostream>

#include "Utils.h"
#include "types.h"
#include "Objects/osu/Beatmap.h"
#include "Objects/osu/HitObjects/DifficultyHitObject.h"

std::map<std::string, Output> calculateStarRating(const std::string& filepath, const std::vector<std::string>& mods = {}, bool allCombinations=false, bool verbose=false);
Output calculateNextModCombination(Beatmap* beatmap, const std::vector<std::string>& mods);
float getTimeRate(const std::vector<std::string>& mods);
std::vector<ModCombination> getAllModCombinations();
std::string getLocalOsuBeatmap(const std::string& filePath);

int main() {
    std::map<std::string, Output> o = calculateStarRating("test1.osu");
    for (auto const& x : o)
    {
        std::cout << x.first  // string (key)
                  << ":aim "
                  << x.second.aim // string's value
                << ":speed "
                << x.second.speed // string's value
                << ":total "
                << x.second.total // string's value
                  << std::endl;
    }

    std::map<std::string, Output> p = calculateStarRating("test2.osu");
    for (auto const& x : p)
    {
        std::cout << x.first  // string (key)
                  << ":aim "
                  << x.second.aim // string's value
                  << ":speed "
                  << x.second.speed // string's value
                  << ":total "
                  << x.second.total // string's value
                  << std::endl;
    }

    std::map<std::string, Output> u = calculateStarRating("test3.osu");
    for (auto const& x : u)
    {
        std::cout << x.first  // string (key)
                  << ":aim "
                  << x.second.aim // string's value
                  << ":speed "
                  << x.second.speed // string's value
                  << ":total "
                  << x.second.total // string's value
                  << std::endl;
    }
}

std::map<std::string, Output> calculateStarRating(
                                const std::string& filepath,
                                const std::vector<std::string>& mods,
                                bool allCombinations,
                                bool verbose) {
    /*Parameters:
    returnAllDifficultyValues = False
        returns total star rating value if False
        when set to True, method will also return aim and speed difficulty

    allCombinations = False
        when set to True, will return star rating of every possible mod combination

    verbose = False
        calculator will log everything when set to True

    filepath: string
        path to .osu file (no need if map_id is set)

    map_id: integer
        BeatmapID number of a beatmap (no need if filepath is set)

    mods (optional): list of string
        Specify which mods to include during star rating calculation
        examples:
            mods=['DT']
            mods=['EZ', 'HD', 'DT']
            mods=[]
    */
    BeatmapParser beatmapParser;
    Beatmap* beatmap = nullptr;
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

Output calculateNextModCombination(Beatmap* beatmap, const std::vector<std::string>& mods) {
    DifficultyHitObjectCreator difficultyHitObjectCreator;
    StarRatingCalculator starRatingCalculator;
    float timeRate = getTimeRate(mods);
    std::vector<DifficultyHitObject*> difficultyHitObjects = difficultyHitObjectCreator.convertToDifficultyHitObjects(beatmap->hitObjects, timeRate);
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
