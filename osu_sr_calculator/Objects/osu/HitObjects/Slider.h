#ifndef OSU_SR_CALCULATOR_SLIDER_H
#define OSU_SR_CALCULATOR_SLIDER_H

#include "HitObject.h"
#include "Objects/Vector2.h"
#include "SliderPath.h"
#include "HitCircle.h"
#include "Objects/osu/Beatmap.h"
#include "SliderObjects/HeadCircle.h"
#include "SliderObjects/TailCircle.h"
#include "SliderObjects/SliderTick.h"
#include "SliderObjects/RepeatPoint.h"

#include "vector"

inline bool sortHitObjects(const HitObject* a, const HitObject* b) {
    return a->startTime < b->startTime;
}

/**
 * Class for sliders
 */
class Slider : public HitObject {
public:
    Vector2* EndPosition = nullptr;
    float EndTime;
    float Duration;
    SliderPath* Path = nullptr;
    int RepeatCount;
    std::vector<HitObject*> NestedHitObjects;
    float TickDistance;
    Vector2* LazyEndPosition = nullptr;
    float LazyTravelDistance = -999.f;
    float SpanDuration;
    int LegacyLastTickOffset = 36;
    HeadCircle* headCircle = nullptr;
    TailCircle* tailCircle = nullptr;

    /**
     *
     * @param pos The raw position of the slider (as listed in the .osu file)
     * @param start_time The start time of the slider
     * @param path The calculated slider path of the slider
     * @param repeatCount The number of repeats this slider has
     * @param speedMultiplier The speed multiplier of this slider compared to the base bpm
     * @param beatLength The length of one beat in ms at this point in the map
     * @param mapDifficulty The difficulty settings of the beatmap
     * @param m_radius The radius of the slider head circle
     */
    Slider(const Vector2& pos, float start_time, SliderPath* path, int repeatCount, float speedMultiplier, float beatLength,
           Beatmap::Difficulty mapDifficulty, float m_radius = 0) : HitObject(pos, start_time, m_radius) {
        Path = path;
        EndPosition = new Vector2(position->add(Path->PositionAt(1)));

        calculateEndTimeAndTickDistance(speedMultiplier, beatLength, mapDifficulty, repeatCount, startTime,
                                        path->expectedDistance);
        Duration = EndTime - startTime;
        RepeatCount = repeatCount;

        createNestedHitObjects();
    }

    ~Slider() override {
        delete position;
        delete stackedPosition;

        delete Path;
        delete EndPosition;
        for (auto &ho : NestedHitObjects) {
            delete ho;
        }
        NestedHitObjects.clear();
        delete LazyEndPosition;
        delete headCircle;
        delete tailCircle;
    }

    HitType getType() override { return HitType::HTSlider; }

private:
    float Velocity;
    int SpanCount;

    void calculateEndTimeAndTickDistance(float speedMultiplier, float beatLength, Beatmap::Difficulty mapDifficulty,
                                         int repeatCount, float startTime, float expectedDistance) {
        float scoringDistance = 100 * mapDifficulty.SliderMultiplier * speedMultiplier;
        Velocity = scoringDistance / beatLength;
        SpanCount = repeatCount + 1;
        TickDistance = scoringDistance / mapDifficulty.SliderTickRate * 1;
        EndTime = startTime + (float)SpanCount * expectedDistance / Velocity;
    }

    void createNestedHitObjects() {
        NestedHitObjects = {};

        createSliderEnds();
        createSliderTicks();
        createRepeatPoints();

        std::sort(NestedHitObjects.begin(), NestedHitObjects.end(), sortHitObjects);
        tailCircle->startTime = fmax(startTime + Duration / 2, tailCircle->startTime - (float)LegacyLastTickOffset);
    }

    void createSliderEnds() {
        headCircle = new HeadCircle(Vector2(position), startTime, radius);
        tailCircle = new TailCircle(Vector2(EndPosition), EndTime, radius);

        NestedHitObjects.push_back(headCircle);
        NestedHitObjects.push_back(tailCircle);
    }

    void createSliderTicks() {
        const float max_length = 100000;

        const float length = fmin(max_length, Path->expectedDistance);
        const float tickDistance = fminf(fmaxf(TickDistance, 0), length);

        if (tickDistance == 0) return;

        const float minDistanceFromEnd = Velocity * 10;
        SpanDuration = Duration / (float)SpanCount;

        for (int span = 0; span < SpanCount; span++) {
            const float spanStartTime = startTime + (float)span * SpanDuration;
            const bool reversed = (span % 2 == 1);

            float d = tickDistance;
            while (d <= length) {
                if (d > length - minDistanceFromEnd) break;

                const float distanceProgress = d / length;
                const float timeProgress = reversed ? (1 - distanceProgress) : distanceProgress;

                Vector2 sliderTickPosition = position->add(Path->PositionAt(distanceProgress));
                auto* sliderTick = new SliderTick(sliderTickPosition, spanStartTime + timeProgress * SpanDuration, span, spanStartTime, radius);
                NestedHitObjects.push_back(sliderTick);

                d += tickDistance;
            }
        }
    }

    void createRepeatPoints() {
        for (int repeatIndex = 0, repeat = 1; repeatIndex < RepeatCount; repeatIndex++, repeat++) {
            Vector2 repeatPosition = position->add(Path->PositionAt((float)(repeat % 2)));
            auto* repeatPoint = new RepeatPoint(repeatPosition, startTime + (float)repeat * SpanDuration, repeatIndex, SpanDuration, radius);
            NestedHitObjects.push_back(repeatPoint);
        }
    }
};

#endif //OSU_SR_CALCULATOR_SLIDER_H
