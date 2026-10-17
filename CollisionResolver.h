#pragma once
#include "EntityStructures.h"
#include <algorithm>

struct CollisionInformation {
    Vector2 normal{};
    float overlap{};

    Entity* a{};
    Entity* b{};
};

inline std::optional<CollisionInformation> getCircleCircleCollision(Entity& a, Entity& b) {
    if (&a == &b) return std::nullopt;

    if (!a.colliderComponent || !b.colliderComponent) return std::nullopt;

    const auto* circle1 = std::get_if<CircleCollider>(&*a.colliderComponent);
    const auto* circle2 = std::get_if<CircleCollider>(&*b.colliderComponent);
    if (!circle1 || !circle2) return std::nullopt;

    auto ca = a.getColliderCenter();
    auto cb = b.getColliderCenter();
    if (!ca || !cb) return std::nullopt;

    Vector2 delta = *cb - *ca;
    float distSq = delta.magnitudeSquared();
    float minDist = circle1->radius + circle2->radius;
    if (distSq >= minDist * minDist) return std::nullopt;

    float dist = std::sqrtf(distSq);

    Vector2 normal = (dist > 0.0001f) ? delta * (1.f / dist) : Vector2(1.f, 0.f);
    float overlap = minDist - dist;
    return CollisionInformation{ normal, overlap, &a, &b };
}

inline std::optional<CollisionInformation> getRectangleRectangleCollision(Entity& a, Entity& b) {
    if (&a == &b) return std::nullopt;

    if (!a.colliderComponent || !b.colliderComponent) return std::nullopt;

    const auto* rect1 = std::get_if<RectangleCollider>(&*a.colliderComponent);
    const auto* rect2 = std::get_if<RectangleCollider>(&*b.colliderComponent);
    if (!rect1 || !rect2) return std::nullopt;

    auto ca = a.getColliderCenter();
    auto cb = b.getColliderCenter();
    if (!ca || !cb) return std::nullopt;

    float dx = cb->dx - ca->dx;
    float dy = cb->dy - ca->dy;
    float overlapX = (rect1->w + rect2->w) * 0.5f - std::abs(dx);
    float overlapY = (rect1->h + rect2->h) * 0.5f - std::abs(dy);

    if (overlapX <= 0.f || overlapY <= 0.f) return std::nullopt;

    if (overlapX < overlapY) {
        Vector2 normal(dx < 0.f ? -1.f : 1.f, 0.f);
        return CollisionInformation{ normal, overlapX, &a, &b };
    }
    Vector2 normal(0.f, dy < 0.f ? -1.f : 1.f);
    return CollisionInformation{ normal, overlapY, &a, &b };
}

inline std::optional<CollisionInformation> getRectangleCircleCollision(Entity& a, Entity& b) {
    if (&a == &b) return std::nullopt;

    if (!a.colliderComponent || !b.colliderComponent) return std::nullopt;

    const auto* rect1 = std::get_if<RectangleCollider>(&*a.colliderComponent);
    const auto* circle2 = std::get_if<CircleCollider>(&*b.colliderComponent);
    if (!rect1 || !circle2) return std::nullopt;

    auto ca = a.getColliderCenter();
    auto cb = b.getColliderCenter();
    if (!ca || !cb) return std::nullopt;

    float closestX = std::clamp(cb->dx, ca->dx - rect1->w * 0.5f, ca->dx + rect1->w * 0.5f);
    float closestY = std::clamp(cb->dy, ca->dy - rect1->h * 0.5f, ca->dy + rect1->h * 0.5f);
    Vector2 closestPoint{ closestX, closestY };

    Vector2 delta = *cb - closestPoint;
    float distSq = delta.magnitudeSquared();
    if (distSq > circle2->radius * circle2->radius) return std::nullopt;

    if (distSq > 0.0001f) {
        float dist = std::sqrt(distSq);
        Vector2 normal = delta * (1.f / dist);
        return CollisionInformation{ normal, circle2->radius - dist, &a, &b };
    }

    float dx = cb->dx - ca->dx;
    float dy = cb->dy - ca->dy;
    float overlapX = rect1->w * 0.5f - std::abs(dx);
    float overlapY = rect1->h * 0.5f - std::abs(dy);

    if (overlapX < overlapY) {
        Vector2 normal(dx < 0.f ? -1.f : 1.f, 0.f);
        return CollisionInformation{ normal, overlapX + circle2->radius, &a, &b };
    }
    Vector2 normal(0.f, dy < 0.f ? -1.f : 1.f);
    return CollisionInformation{ normal, overlapY + circle2->radius, &a, &b };
}

inline std::optional<CollisionInformation> getCircleRectangleCollision(Entity& circle, Entity& rect) {
    return getRectangleCircleCollision(rect, circle);
}

inline void resolveCollision(const CollisionInformation& col) {
    Vector2 push = col.normal * (col.overlap * 0.5f);
    col.a->posComponent->position = col.a->posComponent->position - push;
    col.b->posComponent->position = col.b->posComponent->position + push;
}