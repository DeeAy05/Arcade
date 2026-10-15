#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Config.h"
#include "GameTime.h"
#include "EntityStructures.h"
#include "Camera.h"

class Logger;

class Game {
private:
	SDL_Window* m_window{ nullptr };
	SDL_Renderer* m_renderer{ nullptr };
	SDL_Event m_event{};
	Logger* m_log{ nullptr };

	bool m_running{ true };

	GameTime m_clock{};

	EntitySystem m_entitySystem{};
	int playerID = 0;
	SDL_FRect playerRect{0.f, 0.f, CFG_PLAYER_SIZE, CFG_PLAYER_SIZE};
	Camera cam{};
public:
	Game();
	~Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;


	bool loadLog(Logger& l);
	void input();
	void update();
	void draw();
	void setTimeScale(float s);

	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer() const;
	bool isRunning() const;
	float getDeltaTime() const;
	SDL_FRect getPlayer() const;
};