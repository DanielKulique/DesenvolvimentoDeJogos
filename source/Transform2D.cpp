#include "Transform2D.hpp"

#include <cassert>
#include <cmath>

Transform2D Transform2D::translation(float tx, float ty) noexcept {
    Transform2D t;
    t.m[2][0] = tx;
    t.m[2][1] = ty;
    return t;
}

Transform2D Transform2D::rotation(float angle_rad) noexcept {
    const float c = std::cos(angle_rad);
    const float s = std::sin(angle_rad);
    Transform2D t;
    t.m[0][0] =  c;  t.m[0][1] = s;
    t.m[1][0] = -s;  t.m[1][1] = c;
    return t;
}

Transform2D Transform2D::scale(float sx, float sy) noexcept {
    Transform2D t;
    t.m[0][0] = sx;
    t.m[1][1] = sy;
    return t;
}

Transform2D Transform2D::operator*(const Transform2D& rhs) const noexcept {
    Transform2D result;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < 3; ++k) {
                sum += m[row][k] * rhs.m[k][col];
            }
            result.m[row][col] = sum;
        }
    }
    return result;
}

Transform2D& Transform2D::operator*=(const Transform2D& rhs) noexcept {
    *this = *this * rhs;
    return *this;
}

Vector2D Transform2D::transform_point(const Vector2D& point) const noexcept {
    return {
        point.x * m[0][0] + point.y * m[1][0] + m[2][0],
        point.x * m[0][1] + point.y * m[1][1] + m[2][1]
    };
}

Vector2D Transform2D::transform_vector(const Vector2D& direction) const noexcept {
    return {
        direction.x * m[0][0] + direction.y * m[1][0],
        direction.x * m[0][1] + direction.y * m[1][1]
    };
}

float Transform2D::determinant() const noexcept {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Transform2D Transform2D::inverse() const {
    const float det = determinant();
    assert(std::abs(det) > EPSILON && "inverse(): transform is not invertible");
    const float inv_det = 1.0f / det;

    Transform2D inv;
    inv.m[0][0] =  m[1][1] * inv_det;
    inv.m[0][1] = -m[0][1] * inv_det;
    inv.m[1][0] = -m[1][0] * inv_det;
    inv.m[1][1] =  m[0][0] * inv_det;

    const float tx = m[2][0];
    const float ty = m[2][1];
    inv.m[2][0] = -(tx * inv.m[0][0] + ty * inv.m[1][0]);
    inv.m[2][1] = -(tx * inv.m[0][1] + ty * inv.m[1][1]);
    return inv;
}
