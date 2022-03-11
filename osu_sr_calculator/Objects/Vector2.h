#ifndef OSU_SR_CALCULATOR_VECTOR2_H
#define OSU_SR_CALCULATOR_VECTOR2_H

#include <cassert>
#include <cmath>

inline bool checkNumbers(float val1, float val2, float acceptableDifference) {
    return abs(val1 - val2) <= acceptableDifference;
}

class Vector2 {
public:
    float x;
    float y;

    Vector2(float m_x, float m_y) {
        x = m_x;
        y = m_y;
    }

    Vector2(Vector2 const &vec) {
        x = vec.x;
        y = vec.y;
    }

    explicit Vector2(Vector2 const *vec) {
        x = vec->x;
        y = vec->y;
    }

    Vector2 add(const Vector2& vec) const {
        return {x + vec.x, y + vec.y};
    }

    Vector2 substract(const Vector2& vec) const {
        return {x - vec.x, y - vec.y};
    }

    Vector2 substract(Vector2* vec) const {
        return {x - vec->x, y - vec->y};
    }

    Vector2 scale(float s_factor) const {
        return {x * s_factor, y * s_factor};
    }

    Vector2 divide(float div_factor) const {
        assert(div_factor != 0);
        return {x / div_factor, y / div_factor};
    }

    float dot(const Vector2& vec) const {
        return x * vec.x + y * vec.y;
    }

    float length() const {
        return sqrtf((x * x) + (y * y));
    }

    float lengthSquared() const {
        return powf(length(), 2);
    }

    float distance(const Vector2& vec) const {
        float m_x = x - vec.x;
        float m_y = y - vec.y;
        return sqrtf((m_x * m_x) + (m_y * m_y));
    }

    float distance(Vector2* vec) const {
        float m_x = x - vec->x;
        float m_y = y - vec->y;
        return sqrtf((m_x * m_x) + (m_y * m_y));
    }

    void normalize() {
        float len = length();
        x /= len;
        y /= len;
    }

    bool almostEquals(const Vector2& vec, float acceptableDifference) const {
        return (checkNumbers(x, vec.x, acceptableDifference) && checkNumbers(y, vec.y, acceptableDifference));
    }
};

#endif //OSU_SR_CALCULATOR_VECTOR2_H
