#pragma once
#include <SDL3/SDL.h>
#include <vector>

#include "Config.h"
#include "MathStructures.h"
#include "Camera.h"

inline void DrawCollisionCircle(SDL_Renderer* renderer, const Vector2& v2, float radius) {
    int segments = static_cast<int>(radius * 2.0f * SDL_PI_F);
    if (segments < 16) segments = 16;

    std::vector<SDL_FPoint> points(segments + 1);

    for (int i = 0; i <= segments; ++i) {
        float angle = (i * 2.0f * SDL_PI_F) / segments;
        points[i].x = v2.dx + (radius * SDL_cosf(angle));
        points[i].y = v2.dy + (radius * SDL_sinf(angle));
    }
    SDL_SetRenderDrawColor(renderer, CFG_DBG_COLLISIONS_COLOR.r, CFG_DBG_COLLISIONS_COLOR.g, CFG_DBG_COLLISIONS_COLOR.b, CFG_DBG_COLLISIONS_COLOR.a);

    SDL_RenderLines(renderer, points.data(), (int)points.size());
}

inline void DrawCollisionRectangle(SDL_Renderer* renderer, const Vector2& v2, float w, float h) {
    SDL_SetRenderDrawColor(renderer, CFG_DBG_COLLISIONS_COLOR.r, CFG_DBG_COLLISIONS_COLOR.g, CFG_DBG_COLLISIONS_COLOR.b, CFG_DBG_COLLISIONS_COLOR.a);
    SDL_FRect rect = { v2.dx - w * .5f, v2.dy - h * .5f, w, h };
    SDL_RenderRect(renderer, &rect);
}

inline void DrawCollision(SDL_Renderer* renderer, const Entity& e, const Camera& cam) {
    auto center = e.getColliderCenter();
    if (!center) return;

    Vector2 dst = cam.worldToCamera(*center);

    if (const auto* circle = std::get_if<CircleCollider>(&*e.colliderComponent)) {
        DrawCollisionCircle(renderer, dst, circle->radius);
    }
    else if (const auto* rect = std::get_if<RectangleCollider>(&*e.colliderComponent)) {
        DrawCollisionRectangle(renderer, dst, rect->w, rect->h);
    }
}