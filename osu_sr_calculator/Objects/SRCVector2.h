#ifndef OSU_SR_CALCULATOR_SRCVECTOR2_H
#define OSU_SR_CALCULATOR_SRCVECTOR2_H

#include <cassert>
#include <cmath>

inline bool checkNumbers(float val1, float val2, float acceptableDifference) {
    return abs(val1 - val2) <= acceptableDifference;
}

class SRCVector2 {
public:
    float x;
    float y;

    SRCVector2(float m_x, float m_y) {
        x = m_x;
        y = m_y;
    }

    SRCVector2(SRCVector2 const &vec) {
        x = vec.x;
        y = vec.y;
    }

    explicit SRCVector2(SRCVector2 const *vec) {
        x = vec->x;
        y = vec->y;
    }

    SRCVector2 add(const SRCVector2& vec) const {
        return {x + vec.x, y + vec.y};
    }

    SRCVector2 substract(const SRCVector2& vec) const {
        return {x - vec.x, y - vec.y};
    }

    SRCVector2 substract(SRCVector2* vec) const {
        return {x - vec->x, y - vec->y};
    }

    SRCVector2 scale(float s_factor) const {
        return {x * s_factor, y * s_factor};
    }

    SRCVector2 divide(float div_factor) const {
        assert(div_factor != 0);
        return {x / div_factor, y / div_factor};
    }

    float dot(const SRCVector2& vec) const {
        return x * vec.x + y * vec.y;
    }

    float length() const {
        return sqrtf((x * x) + (y * y));
    }

    float lengthSquared() const {
        return powf(length(), 2);
    }

    float distance(const SRCVector2& vec) const {
        float m_x = x - vec.x;
        float m_y = y - vec.y;
        return sqrtf((m_x * m_x) + (m_y * m_y));
    }

    float distance(SRCVector2* vec) const {
        float m_x = x - vec->x;
        float m_y = y - vec->y;
        return sqrtf((m_x * m_x) + (m_y * m_y));
    }

    void normalize() {
        float len = length();
        x /= len;
        y /= len;
    }

    bool almostEquals(const SRCVector2& vec, float acceptableDifference) const {
        return (checkNumbers(x, vec.x, acceptableDifference) && checkNumbers(y, vec.y, acceptableDifference));
    }
};

#endif //OSU_SR_CALCULATOR_SRCVECTOR2_H
