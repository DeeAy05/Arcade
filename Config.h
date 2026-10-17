#pragma once
#include <cstddef>
#include <SDL3/SDL.h>
// WINDOW PARAMETER
inline constexpr int CFG_LOGICAL_W = 400;
inline constexpr int CFG_LOGICAL_H = 300;
inline constexpr int CFG_WINDOW_SCALE = 4;
// WORLD PARAMETER
inline constexpr float CFG_WORLD_TOP_BOUND = -1000.f;
inline constexpr float CFG_WORLD_LEFT_BOUND = 0.f;
inline constexpr float CFG_WORLD_BOTTOM_BOUND = 2000.f;
inline constexpr float CFG_WORLD_RIGHT_BOUND = 2000.f;
// PLAYER PARAMETER
inline constexpr float CFG_PLAYER_SIZE = 20.f;
// SCREEN CENTER
inline constexpr float CFG_CENTER_X = CFG_LOGICAL_W / 2.f;
inline constexpr float CFG_CENTER_Y = CFG_LOGICAL_H / 2.f;
// TIME PARAMETERS
inline constexpr float CFG_MAX_DELTA_TIME{ 0.1f };
// LOG PARAMETERS
inline constexpr size_t CFG_LOG_MAX_LINES = 10;
inline constexpr float CFG_LOG_LINE_HEIGHT = 20.f;
inline constexpr float CFG_LOG_BASE_Y = CFG_LOGICAL_H - CFG_LOG_LINE_HEIGHT;
// CAMERA SETTINGS
inline constexpr float CFG_CAMERA_SMOOTHING = 8.f;
// DEBUG SETTINGS
inline constexpr bool CFG_DBG_SHOW_COLLISIONS = true;
inline constexpr SDL_Color CFG_DBG_COLLISIONS_COLOR = { 200, 200, 0, 255 };