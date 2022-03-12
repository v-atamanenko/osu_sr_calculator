#ifndef OSU_SR_CALCULATOR_SLIDERPATH_H
#define OSU_SR_CALCULATOR_SLIDERPATH_H

#include "Objects/osu/SRCPathType.h"
#include "PathApproximator.h"
#include "Objects/SRCVector2.h"
#include "Precision.h"
#include "Utils.h"

#include "vector"

class SliderPath {
public:
    SRCPathType pathType;
    std::vector<SRCVector2> controlPoints;
    float expectedDistance;
    bool isInitialised = false;

    std::vector<SRCVector2> calculatedPath;
    std::vector<float> cumulativeLength;

    PathApproximator pathApproximator;

    SliderPath(SRCPathType path_type, const std::vector<SRCVector2>& control_points, float expected_distance) {
        pathType = path_type;
        controlPoints = control_points;
        expectedDistance = expected_distance;

        ensureInitialised();
    }

    void ensureInitialised() {
        if (isInitialised) return;

        calculatePath();
        calculateCumulativeLength();
        isInitialised = true;
    }

    void calculatePath() {
        calculatedPath = {};
        int start = 0;
        int end = 0;

        for (int i = 0; i < controlPoints.size(); ++i) {
            end++;
            if ( (i == (controlPoints.size() - 1)) ||
                 (controlPoints.at(i).x == controlPoints.at(i + 1).x &&
                  controlPoints.at(i).y == controlPoints.at(i + 1).y)
                  )
            {
                std::vector<SRCVector2> cpSpan = Utils::vec_slice(controlPoints, start, end - 1);
                std::vector<SRCVector2> subpath = calculateSubPath(cpSpan);
                for (auto &t : subpath) {
                    if (calculatedPath.empty() || calculatedPath.at(calculatedPath.size() - 1).x != t.x || calculatedPath.at(calculatedPath.size() - 1).y != t.y) {
                        calculatedPath.emplace_back(t.x, t.y);
                    }
                }

                start = end;
            }
        }
    }

    std::vector<SRCVector2> calculateSubPath(const std::vector<SRCVector2>& subControlPoints) {
        if (pathType == SRCPathType::Linear) {
            return pathApproximator.approximateLinear(subControlPoints);
        } else if (pathType == SRCPathType::PerfectCurve) {
            if (controlPoints.size() != 3 || subControlPoints.size() != 3) {
                return pathApproximator.approximateBezier(subControlPoints);
            }

            std::vector<SRCVector2> subPath = pathApproximator.approximateCircularArc(subControlPoints);
            if (subPath.empty()) {
                return pathApproximator.approximateBezier(subControlPoints);
            }
            return subPath;
        } else if (pathType == SRCPathType::Catmull) {
            return pathApproximator.approximateCatmull(subControlPoints);
        } else {
            return pathApproximator.approximateBezier(subControlPoints);
        }
    }
    
    void calculateCumulativeLength() {
        float l = 0;
        cumulativeLength = {};
        cumulativeLength.push_back(l);

        for (int i = 0; i < (calculatedPath.size() - 1); ++i) {
            SRCVector2 diff = calculatedPath.at(i + 1).substract(calculatedPath[i]);
            float d = diff.length();
            
            if (expectedDistance - l < d) {
                calculatedPath.at(i+1) = calculatedPath.at(i).add(diff.scale((expectedDistance - l) / d));
                Utils::vec_splice(calculatedPath, i+2, (int)calculatedPath.size() - 2 - i);

                l = expectedDistance;
                cumulativeLength.push_back(l);
                break;
            }

            l += d;
            cumulativeLength.push_back(l);
        }

        if (l < expectedDistance && calculatedPath.size() > 1) {
            SRCVector2 diff = calculatedPath.at(calculatedPath.size() - 1).substract(calculatedPath.at(calculatedPath.size() - 2));
            float d = diff.length();

            if (d <= 0) return;

            calculatedPath.at(calculatedPath.size() - 1).add(diff.scale((expectedDistance - l) / d));
            cumulativeLength.at(calculatedPath.size() - 1) = expectedDistance;
        }
    }

    SRCVector2 PositionAt(float progress) {
        ensureInitialised();
        float d = progressToDistance(progress);
        return interpolateVertices(indexOfDistance(d), d);
    }

private:
    float progressToDistance(float progress) const {
        return fminf(fmaxf(progress, 0), 1) * expectedDistance;
    }

    SRCVector2 interpolateVertices(int i, float d) {
        if (calculatedPath.empty()) {
            return {0,0};
        }

        if (i <= 0) {
            return calculatedPath[0];
        }
        if (i >= (int)calculatedPath.size()) {
            return calculatedPath.at(calculatedPath.size() - 1);
        }

        SRCVector2 p0 = calculatedPath.at(i - 1);
        SRCVector2 p1 = calculatedPath.at(i);

        float d0 = cumulativeLength.at(i-1);
        float d1 = cumulativeLength.at(i);

        if (Precision::almostEqualsNumber(d0, d1)) { // uhh what
            return p0;
        }

        float w = (d - d0) / (d1 - d0);
        return p0.add(p1.substract(p0).scale(w));
    }

    int indexOfDistance(float d) {
        for (auto &cL : cumulativeLength) {
            if (cL > d) {
                return Utils::vec_getValIndex(cumulativeLength, cL);
            }
        }

        return (int)cumulativeLength.size();
    }
};

#endif //OSU_SR_CALCULATOR_SLIDERPATH_H
