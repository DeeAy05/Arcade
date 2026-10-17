#pragma once
#include <cmath>
#include <SDL3/SDL.h>
#include "Config.h"
#include "EntitySystem.h"
class Camera {
private:
	float cameraOffsetX{};
	float cameraOffsetY{};
public:
	Camera() {
		cameraOffsetX = 0;
		cameraOffsetY = 0;
	}

	static float clampAxis(float target, float minBound, float maxBound, float halfView) {
		float lo = minBound + halfView;
		float hi = maxBound - halfView;
		if (lo > hi) return (minBound + maxBound) * 0.5f;
		return std::clamp(target, lo, hi);
	}

	Vector2 desiredOffset(const Vector2& p) const {
		return {
			clampAxis(p.dx, CFG_WORLD_LEFT_BOUND, CFG_WORLD_RIGHT_BOUND,  CFG_CENTER_X),
			clampAxis(p.dy, CFG_WORLD_TOP_BOUND,  CFG_WORLD_BOTTOM_BOUND, CFG_CENTER_Y)
		};
	}

	void launch(const Entity& target) {
		auto p = target.getWorldPos();
		if (p) {
			Vector2 d = desiredOffset(*p);
			cameraOffsetX = d.dx;
			cameraOffsetY = d.dy;
		}
	}

	void update(const Entity& e, float dt) {
		auto p = e.getWorldPos();
		if (!p) return;

		Vector2 d = desiredOffset(*p);
		float t = 1.f - std::exp(-CFG_CAMERA_SMOOTHING * dt);
		cameraOffsetX += (d.dx - cameraOffsetX) * t;
		cameraOffsetY += (d.dy - cameraOffsetY) * t;
	}

	SDL_FRect worldToCamera(const SDL_FRect& r) const {
		return { r.x - cameraOffsetX + CFG_CENTER_X,
				 r.y - cameraOffsetY + CFG_CENTER_Y, r.w, r.h };
	}

	Vector2 worldToCamera(const Vector2& v) const {
		return { v.dx - cameraOffsetX + CFG_CENTER_X, v.dy - cameraOffsetY + CFG_CENTER_Y};
	}
};