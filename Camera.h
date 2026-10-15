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

	void launch(const Entity& target) {
		auto p = target.getWorldPos();
		if (p) {
			cameraOffsetX = p->dx;
			cameraOffsetY = p->dy;
		}
	}

	void update(const Entity& p, float dt) {
		auto target = p.getWorldPos();

		float t = 1.f - std::exp(-CFG_CAMERA_SMOOTHING * dt);
		if (target) {
			cameraOffsetX += (target->dx - cameraOffsetX) * t;
			cameraOffsetY += (target->dy - cameraOffsetY) * t;
		}
	}

	SDL_FRect worldToCamera(const SDL_FRect& r) const {
		return { r.x - cameraOffsetX + CFG_CENTER_X,
				 r.y - cameraOffsetY + CFG_CENTER_Y, r.w, r.h };
	}

	Vector2 worldToCamera(const Vector2& v) const {
		return { v.dx - cameraOffsetX + CFG_CENTER_X, v.dy - cameraOffsetY + CFG_CENTER_Y};
	}
};