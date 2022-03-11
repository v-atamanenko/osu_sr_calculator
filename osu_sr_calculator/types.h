#ifndef OSU_SR_CALCULATOR_TYPES_H
#define OSU_SR_CALCULATOR_TYPES_H

struct Output {
    float aim;
    float speed;
    float total;
};

struct ModCombination {
    std::vector<std::string> mods;
    bool reParse;
};

#endif //OSU_SR_CALCULATOR_TYPES_H