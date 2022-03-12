#ifndef OSU_SR_CALCULATOR_PATHAPPROXIMATOR_H
#define OSU_SR_CALCULATOR_PATHAPPROXIMATOR_H

#include "Objects/SRCVector2.h"
#include "Precision.h"

#include "cmath"
#include "vector"

#define bezier_tolerance 0.25f
#define catmull_detail 50
#define circular_arc_tolerance 0.1f

class PathApproximator {
public:
    std::vector<SRCVector2> approximateBezier(const std::vector<SRCVector2> &controlPoints) {
        std::vector<SRCVector2> output;
        int count = (int)controlPoints.size();

        if (count == 0) {
            return output;
        }

        std::vector<SRCVector2> subdivisionBuffer1;
        std::vector<SRCVector2> subdivisionBuffer2;
        for (int i = 0; i < count; ++i) {
            subdivisionBuffer1.emplace_back(SRCVector2(0, 0));
        }

        for (int i = 0; i < (count * 2 - 1); ++i) {
            subdivisionBuffer2.emplace_back(SRCVector2(0, 0));
        }

        std::vector<std::vector<SRCVector2>> toFlatten;
        std::vector<std::vector<SRCVector2>> freeBuffers;

        std::vector<SRCVector2> deepCopy;
        for (const SRCVector2 &c : controlPoints) {
            deepCopy.emplace_back(c);
        }

        toFlatten.push_back(deepCopy);

        std::vector<SRCVector2> leftChild = subdivisionBuffer2;

        while (!toFlatten.empty()) {
            std::vector<SRCVector2> parent = toFlatten.back();
            toFlatten.pop_back();

            if (bezierIsFlatEnough(parent)) {
                bezierApproximate(parent, output, subdivisionBuffer1, subdivisionBuffer2, count);
                freeBuffers.emplace_back(parent);
                continue;
            }

            std::vector<SRCVector2> rightChild;

            if (!freeBuffers.empty()) {
                rightChild = freeBuffers.back();
                freeBuffers.pop_back();
            } else {
                for (int i = 0; i < count; ++i) {
                    rightChild.emplace_back(SRCVector2(0, 0));
                }
            }

            bezierSubdivide(parent, leftChild, rightChild, subdivisionBuffer1, count);

            for (int i = 0; i < count; ++i) {
                parent.at(i) = leftChild.at(i);
            }

            toFlatten.push_back(rightChild);
            toFlatten.push_back(parent);
        }

        output.push_back(controlPoints.at(count-1));
        return output;
    }

    std::vector<SRCVector2> approximateCatmull(std::vector<SRCVector2> controlPoints) {
        std::vector<SRCVector2> result;

        for (int i = 0; i < (controlPoints.size()-1); ++i) {
            SRCVector2 v1 = (i > 0) ? controlPoints.at(i - 1) : controlPoints.at(i);
            SRCVector2 v2 = controlPoints.at(i);
            SRCVector2 v3 = (i < (controlPoints.size() - 1)) ? controlPoints.at(i + 1) : v2.add(v2).substract(v1);
            SRCVector2 v4 = (i < (controlPoints.size() - 2)) ? controlPoints.at(i + 2) : v3.add(v3).substract(v2);

            for (int c = 0; c < catmull_detail; ++c) {
                result.push_back(catmullFindPoint(v1, v2, v3, v4, (float)c / catmull_detail));
                result.push_back(catmullFindPoint(v1, v2, v3, v4, ((float)c + 1) / catmull_detail));
            }
        }

        return result;
    }

    std::vector<SRCVector2> approximateCircularArc(std::vector<SRCVector2> controlPoints) {
        std::vector<SRCVector2> output;

        SRCVector2 a = controlPoints.at(0);
        SRCVector2 b = controlPoints.at(1);
        SRCVector2 c = controlPoints.at(2);

        float aSq = (b.substract(c)).lengthSquared();
        float bSq = (a.substract(c)).lengthSquared();
        float cSq = (a.substract(b)).lengthSquared();

        if (Precision::almostEqualsNumber(aSq, 0) || Precision::almostEqualsNumber(bSq, 0) || Precision::almostEqualsNumber(cSq, 0)) {
            return output;
        }

        float s = aSq * (bSq + cSq - aSq);
        float t = bSq * (aSq + cSq - bSq);
        float u = cSq * (aSq + bSq - cSq);

        float Sum = s + t + u;

        if (Precision::almostEqualsNumber(Sum, 0)) {
            return output;
        }

        SRCVector2 centre = (a.scale(s).add(b.scale(t)).add(c.scale(u))).divide(Sum);
        SRCVector2 dA = a.substract(centre);
        SRCVector2 dC = c.substract(centre);

        float r = dA.length();

        float thetaStart = atan2(dA.y, dA.x);
        float thetaEnd = atan2(dC.y, dC.x);

        while (thetaEnd < thetaStart) {
            thetaEnd += 2 * M_PI;
        }

        int Dir = 1;
        float thetaRange = thetaEnd - thetaStart;

        SRCVector2 orthoAtoC = c.substract(a);
        orthoAtoC = SRCVector2(orthoAtoC.y, -1 * orthoAtoC.x);

        if(orthoAtoC.dot(b.substract(a)) < 0) {
            Dir = -1 * Dir;
            thetaRange = (float)2 * (float)M_PI - thetaRange;
        }

        int amountPoints;

        if (2*r <= circular_arc_tolerance) {
            amountPoints = 2;
        } else {
            amountPoints = (int)fmax(2, ceil(thetaRange / (2 * acos(1 - circular_arc_tolerance / r))));
        }

        for (int i = 0; i < amountPoints; ++i) {
            float fract = (float)i / ((float)amountPoints - 1);
            float theta = thetaStart + (float)Dir * fract * thetaRange;
            SRCVector2 o = SRCVector2(cos(theta), sin(theta)).scale(r);
            output.push_back(centre.add(o));
        }

        return output;
    }

    std::vector<SRCVector2> approximateLinear(std::vector<SRCVector2> controlPoints) {
        return controlPoints;
    }

private:
    bool bezierIsFlatEnough(const std::vector<SRCVector2> &controlPoints) {
        for (int i = 1; i < (controlPoints.size() - 1); ++i) {
            if ((controlPoints.at(i - 1).substract(controlPoints.at(i).scale(2)).add(controlPoints.at(i + 1))).lengthSquared() > bezier_tolerance * bezier_tolerance * 4) {
                return false;
            }
        }

        return true;
    }

    void bezierApproximate(const std::vector<SRCVector2>& controlPoints, std::vector<SRCVector2> &output, const std::vector<SRCVector2>& subdivisionBuffer1, const std::vector<SRCVector2>& subdivisionBuffer2, int count) {
        std::vector<SRCVector2> l = subdivisionBuffer2;
        std::vector<SRCVector2> r = subdivisionBuffer1;

        bezierSubdivide(controlPoints, l, r, subdivisionBuffer1, count);

        for (int i = 0; i < (count - 1); ++i) {
            l.at(count + i) = r.at(i + 1);
        }

        output.push_back(controlPoints[0]);
        for (int i = 1; i < (count - 1); ++i) {
            int index = 2*i;
            SRCVector2 p = (l.at(index - 1).add(l.at(index).scale(2)).add(l.at(index + 1))).scale(0.25);
            output.push_back(p);
        }
    }
    
    void bezierSubdivide(const std::vector<SRCVector2> &controlPoints, std::vector<SRCVector2>& l, std::vector<SRCVector2>& r, const std::vector<SRCVector2> &subdivisionBuffer, int count) {
        std::vector<SRCVector2> midpoints = subdivisionBuffer;

        for (int i = 0; i < count; ++i) {
            midpoints.at(i) = controlPoints.at(i);
        }

        for (int i = 0; i < count; ++i) {
            l.at(i) = midpoints.at(0);
            r.at(count-i-1) = midpoints.at(count - i - 1);

            for (int j = 0; j < (count-i-1); ++j) {
                midpoints.at(j) = (midpoints.at(j).add(midpoints.at(j + 1))).divide(2);
            }
        }
    }

    SRCVector2 catmullFindPoint(const SRCVector2& vec1, const SRCVector2& vec2, const SRCVector2& vec3, const SRCVector2& vec4, float t) {
        float t2 = t * t;
        float t3 = t * t2;
        return {
        0.5f * (2 * vec2.x + (-vec1.x + vec3.x) * t + (2 * vec1.x - 5 * vec2.x + 4 * vec3.x - vec4.x) * t2 + (-vec1.x + 3 * vec2.x - 3 * vec3.x + vec4.x) * t3),
        0.5f * (2 * vec2.y + (-vec1.y + vec3.y) * t + (2 * vec1.y - 5 * vec2.y + 4 * vec3.y - vec4.y) * t2 + (-vec1.y + 3 * vec2.y - 3 * vec3.y + vec4.y) * t3)
        };
    }
};

#endif //OSU_SR_CALCULATOR_PATHAPPROXIMATOR_H
