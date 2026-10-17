#include "Game.h"
#include "Logger.h"

#include "Draw.h"

#include <iostream>
#include <string>
#include <format>

Game::Game() {
	////////////////////////////    INITIALIZATION    ////////////////////////////////////////
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Initialization error");
		m_running = false;
	}
	if (!TTF_Init()) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Font initialization error");
		m_running = false;
	}
	if (!SDL_CreateWindowAndRenderer("Arcade", CFG_LOGICAL_W * CFG_WINDOW_SCALE, CFG_LOGICAL_H * CFG_WINDOW_SCALE, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Creating window or renderer error");
		m_running = false;
	}
	SDL_SetRenderVSync(m_renderer, 1);
	SDL_SetRenderLogicalPresentation(m_renderer, CFG_LOGICAL_W, CFG_LOGICAL_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	//////////////////////////////////////////////////////////////////////////////////////////
	// TO REFACTOR (see EntitySystem.h)
	Entity player;
	player.posComponent = Position{};
	player.velComponent = Velocity{};
	player.speedComponent = MoveSpeed{200.f};
	player.colliderComponent = RectangleCollider{ {CFG_PLAYER_SIZE * .5f, CFG_PLAYER_SIZE * .5f}, CFG_PLAYER_SIZE, CFG_PLAYER_SIZE };
	//player.colliderComponent = CircleCollider{ {CFG_PLAYER_SIZE / 2, CFG_PLAYER_SIZE / 2}, 15 };
	m_entitySystem.entities.push_back(std::move(player));
	cam.launch(m_entitySystem.entities[playerID]);


	// TO REFACTOR (see EntitySystem.h)
	for (int i = 0; i < 20; ++i) {
		Entity enemy;
		enemy.posComponent = Position{ Vector2(i * 10.f, 50.f) };
		enemy.velComponent = Velocity{ Vector2( 100.f, 0.f) };
		if (i % 2 == 0) {
			enemy.colliderComponent = CircleCollider{ {CFG_PLAYER_SIZE / 2, CFG_PLAYER_SIZE / 2}, 10 };
		}
		else {
			enemy.colliderComponent = RectangleCollider{ {CFG_PLAYER_SIZE * .5f, CFG_PLAYER_SIZE * .5f}, CFG_PLAYER_SIZE, CFG_PLAYER_SIZE };
		}
		enemy.healthComponent = Health{ 30 };
		m_entitySystem.entities.push_back(enemy);
	}
}

Game::~Game() {
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	TTF_Quit();
}

bool Game::loadLog(Logger& l)
{
	m_log = &l;
	if (m_log) return 1;
	return 0;
}

void Game::input() {
	while (SDL_PollEvent(&m_event)) {
		if (m_event.type == SDL_EVENT_QUIT) m_running = false;
		if (m_event.type == SDL_EVENT_KEY_DOWN &&
			m_event.key.scancode == SDL_SCANCODE_ESCAPE)
			m_running = false;
		if (m_event.type == SDL_EVENT_KEY_DOWN &&
			m_event.key.scancode == SDL_SCANCODE_Z)
			m_log->push(std::format("Player's position: X = {:.2f}; Y = {:.2f}", 
				m_entitySystem.entities[playerID].posComponent->position.dx, 
				m_entitySystem.entities[playerID].posComponent->position.dy).c_str());

	}

	const bool* keys = SDL_GetKeyboardState(nullptr);
	auto& player = m_entitySystem.entities[playerID];
	if (player.speedComponent && player.velComponent) {
		Vector2 dir(
			float(keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]),
			float(keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]));
		if (dir.dx != 0.f || dir.dy != 0.f) dir = dir.normalized();
		player.velComponent->velocity = dir * player.speedComponent->speed;
	}
}

void Game::draw()
{
	// Real objects' position in the world -> camera for rendering. HUD, Log and UI -> screen for rendering.
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// Entities drawer
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	for (const auto& e : m_entitySystem.entities) {
		if (e.posComponent) {
			SDL_FRect world{ e.posComponent->position.dx, e.posComponent->position.dy, CFG_PLAYER_SIZE, CFG_PLAYER_SIZE };
			SDL_FRect dst = cam.worldToCamera(world);
			SDL_RenderFillRect(m_renderer, &dst);
		}
	}
	// Debug drawer
	if (CFG_DBG_SHOW_COLLISIONS) {
		for (const auto& e : m_entitySystem.entities) {
			DrawCollision(m_renderer, e, cam);
		}
		SDL_FRect worldBound = { CFG_WORLD_LEFT_BOUND, CFG_WORLD_TOP_BOUND, CFG_WORLD_RIGHT_BOUND - CFG_WORLD_LEFT_BOUND, CFG_WORLD_BOTTOM_BOUND - CFG_WORLD_TOP_BOUND };
		SDL_FRect dst = cam.worldToCamera(worldBound);
		SDL_RenderRect(m_renderer, &dst);
	}
	// Log drawer
	if (m_log) m_log->draw();

	// Render finishes.
	SDL_RenderPresent(m_renderer);
}

void Game::setTimeScale(float s)
{
	m_clock.setTimeScale(s);
}

void Game::update()
{
	// Delta time
	m_clock.tick();

	// Input update
	m_entitySystem.update(m_clock.dt());
	auto playerPos = m_entitySystem.entities[playerID].getWorldPos();
	if (playerPos) {
		playerRect = { playerPos->dx, playerPos->dy, CFG_PLAYER_SIZE, CFG_PLAYER_SIZE };
	}


	// Camera update
	cam.update(m_entitySystem.entities[playerID], m_clock.dt());

	// Log update
	if (m_log) m_log->update();
}

SDL_Window* Game::getWindow() const
{
	return m_window;
}

SDL_Renderer* Game::getRenderer() const
{
	return m_renderer;
}

bool Game::isRunning() const
{
	return m_running;
}

float Game::getDeltaTime() const
{
	return m_clock.dt();
}

SDL_FRect Game::getPlayer() const
{
	return playerRect;
}
