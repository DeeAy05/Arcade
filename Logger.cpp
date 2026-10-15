#include "Logger.h"
#include "Game.h"
#include <format>
#include <algorithm>
Logger::Logger(Game& g) {
	game = &g;
	m_font = TTF_OpenFont("assets/font/BounceDash.otf", 16);
	if (!m_font) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Font loading error");
	}
	m_textEngine = TTF_CreateRendererTextEngine(game->getRenderer());
	if (!m_textEngine) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text engine initialization error");
	}
}

Logger::~Logger() {
	m_text.clear();
	TTF_DestroyRendererTextEngine(m_textEngine);
	TTF_CloseFont(m_font);
}

void Logger::update() {
	for (auto& t : m_text) t.update();
	std::erase_if(m_text, [](const Text& t) { return t.alpha <= 0.f; });
}

void Logger::draw() {
	for (size_t i = 0; i < m_text.size(); ++i) {
		TTF_DrawRendererText(m_text[i].text, 0.f, CFG_LOG_BASE_Y - i * CFG_LOG_LINE_HEIGHT);
	}
}

void Logger::push(const char* str) {
	TTF_Text* t = TTF_CreateText(m_textEngine, m_font, str, 0);
	if (!t) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Text creation error: %s", SDL_GetError());
		return;
	}
	if (m_text.size() >= CFG_LOG_MAX_LINES)
		m_text.erase(m_text.begin());
	m_text.emplace_back(t);
}
