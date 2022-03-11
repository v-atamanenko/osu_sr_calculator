#ifndef OSU_SR_CALCULATOR_OSU_SR_CALCULATOR_H
#define OSU_SR_CALCULATOR_OSU_SR_CALCULATOR_H

#include "vector"
#include "map"

#ifndef TYPE_OUTPUT
#define TYPE_OUTPUT
struct Output {
    float aim;
    float speed;
    float total;
};
#endif

struct ModCombination {
    std::vector<std::string> mods;
    bool reParse;
};

/**
 * @param filepath: string
        path to .osu file (no need if map_id is set)

 * @param mods (optional): list of string
        Specify which mods to include during star rating calculation
        examples:
            mods=['DT']
            mods=['EZ', 'HD', 'DT']
            mods=[]

 * @param allCombinations = False
        when set to True, will return star rating of every possible mod combination

 * @param verbose = False
        calculator will log everything when set to True
 */
std::map<std::string, Output> calculateStarRating(const std::string& filepath, const std::vector<std::string>& mods = {}, bool allCombinations=false, bool verbose=false);

#endif //OSU_SR_CALCULATOR_OSU_SR_CALCULATOR_H
