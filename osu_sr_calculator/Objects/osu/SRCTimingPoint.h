#ifndef OSU_SR_CALCULATOR_SRCTIMINGPOINT_H
#define OSU_SR_CALCULATOR_SRCTIMINGPOINT_H

class SRCTimingPointContainer {
public:
    float Time;
    explicit SRCTimingPointContainer(float time) {
        Time = time;
    }
};

class SRCTimingPoint : public SRCTimingPointContainer {
public:
    float BeatLength;
    int TimeSignature;

    SRCTimingPoint(float time, float beat_length, int time_signature) : SRCTimingPointContainer(time) {
        BeatLength = beat_length;
        TimeSignature = time_signature;
    }
};

class SRCDifficultyTimingPoint : public SRCTimingPointContainer {
public:
    float SpeedMultiplier;

    SRCDifficultyTimingPoint(float time, float speed_multiplier) : SRCTimingPointContainer(time) {
        SpeedMultiplier = speed_multiplier;
    }
};

#endif //OSU_SR_CALCULATOR_SRCTIMINGPOINT_H
