#pragma once
#include <cmath>

struct Vector2 {
	float dx{0.f};
	float dy{0.f};
	Vector2() = default;
	Vector2(float x, float y) : dx(x), dy(y) {}

	float magnitude() const {
		return sqrtf(static_cast<float> (dx * dx + dy * dy));
	}

	Vector2 normalized() const {
		float mg = magnitude();
		if (mg == 0.0f) {
			return Vector2(0.0f, 0.0f);
		}
		return Vector2(dx / mg, dy / mg);
	}

	Vector2 operator*(float f) const {
		return Vector2(dx * f, dy * f);
	}
	Vector2 operator+(const Vector2& v) const {
		return { dx + v.dx, dy + v.dy };
	}
	Vector2 operator-(const Vector2& v) const {
		return { dx - v.dx, dy - v.dy };
	}
};