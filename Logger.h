#pragma once
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game;

struct Text {
	TTF_Text* text{ nullptr };
	float alpha{ 1 };
	float dur{ 3 };
	Uint64 last = SDL_GetTicksNS();

	explicit Text(TTF_Text* t) : text(t) {
		TTF_SetTextColor(text, 255, 255, 0, static_cast<Uint8>(255.f * alpha));
	}

	~Text() {
		if (text) TTF_DestroyText(text);
	}

	Text(const Text&) = delete;
	Text& operator=(const Text&) = delete;

	Text(Text&& o) noexcept
		: text(o.text), alpha(o.alpha), dur(o.dur), last(o.last) {
		o.text = nullptr;
	}

	Text& operator=(Text&& o) noexcept {
		if (this != &o) {
			if (text) TTF_DestroyText(text);
			text = o.text;
			alpha = o.alpha;
			dur = o.dur;
			last = o.last;
			o.text = nullptr;
		}
		return *this;
	}

	void update() {
		Uint64 now = SDL_GetTicksNS();
		float dt = static_cast<float>(now - last) / 1e9f;
		last = now;
		dur = std::max(0.f, dur - dt);
		alpha = dur / 3.f;
		TTF_SetTextColor(text, 255, 255, 0, static_cast<Uint8>(255.f * alpha));
	}
};

class Logger {
private:

	TTF_Font* m_font{ nullptr };
	TTF_TextEngine* m_textEngine{ nullptr };
	std::vector<Text> m_text;
	Game* game{ nullptr };
public:
	Logger(Game& g);
	~Logger();
	void update();
	void draw();
	void push(const char* str);
};


