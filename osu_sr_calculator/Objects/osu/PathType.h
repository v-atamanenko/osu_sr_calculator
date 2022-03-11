#ifndef OSU_SR_CALCULATOR_PATHTYPE_H
#define OSU_SR_CALCULATOR_PATHTYPE_H

typedef enum PathType {
    Catmull      = 0,
    Bezier       = 1,
    Linear       = 2,
    PerfectCurve = 3
} PathType;

#endif //OSU_SR_CALCULATOR_PATHTYPE_H
