#ifndef OSU_SR_CALCULATOR_TIMINGPOINT_H
#define OSU_SR_CALCULATOR_TIMINGPOINT_H

class TimingPointContainer {
public:
    float Time;
    explicit TimingPointContainer(float time) {
        Time = time;
    }
};

class TimingPoint : public TimingPointContainer {
public:
    float BeatLength;
    int TimeSignature;

    TimingPoint(float time, float beat_length, int time_signature) : TimingPointContainer(time) {
        BeatLength = beat_length;
        TimeSignature = time_signature;
    }
};

class DifficultyTimingPoint : public TimingPointContainer {
public:
    float SpeedMultiplier;

    DifficultyTimingPoint(float time, float speed_multiplier) : TimingPointContainer(time) {
        SpeedMultiplier = speed_multiplier;
    }
};

#endif //OSU_SR_CALCULATOR_TIMINGPOINT_H
