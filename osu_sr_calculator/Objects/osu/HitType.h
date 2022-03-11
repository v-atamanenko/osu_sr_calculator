#ifndef OSU_SR_CALCULATOR_HITTYPE_H
#define OSU_SR_CALCULATOR_HITTYPE_H

typedef enum HitType {
    HTNormal      = 1 << 0,
    HTSlider      = 1 << 1,
    HTNewCombo    = 1 << 2,
    HTSpinner     = 1 << 3,
    HTComboSkip1  = 1 << 4,
    HTComboSkip2  = 1 << 5,
    HTComboSkip3  = 1 << 6,
    HTHold        = 1 << 7,

    // Custom types for the calculator
    HTRepeatPoint = 1 << 14,
    HTSliderTick  = 1 << 15,
    HTDifficulty  = 1 << 16
} HitType;

#endif //OSU_SR_CALCULATOR_HITTYPE_H
