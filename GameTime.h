#pragma once
#include <SDL3/SDL.h>
#include "Config.h"
class GameTime {
private:
	Uint64 m_last = SDL_GetTicksNS();
	float m_rawDt{ 0.0 }, m_dt{ 0.0 }; // rawDt for UI animations, dt for gameplay.
	float m_scale{ 1.0 }; // 0 = pause, 0.5 = slow-mo.
public:
	void tick() {
		Uint64 now = SDL_GetTicksNS();
		m_rawDt = (now - m_last) / 1e9f;
		m_last = now;

		if (m_rawDt > CFG_MAX_DELTA_TIME) m_rawDt = CFG_MAX_DELTA_TIME;

		m_dt = m_rawDt * m_scale;
	}
	float dt() const {
		return m_dt;
	}
	float rawDt() const {
		return m_rawDt;
	}
	void setTimeScale(float s) {
		m_scale = s;
	}
};