#pragma once
#include "EntityStructures.h"

// TO-DO 
// 1. Circle-Rectangle & Rectangle-Rectangle Resolvers.
// 2. ResolveCollision function that handles all collisions.

inline void resolveCircleCollision(Entity& a, Entity& b) {
	if (&a == &b) return;

	if (!a.colliderComponent || !b.colliderComponent) return;

	const auto* circle1 = std::get_if<CircleCollider>(&*a.colliderComponent);
	const auto* circle2 = std::get_if<CircleCollider>(&*b.colliderComponent);
	if (!circle1 || !circle2) return;

    auto ca = a.getColliderCenter();
    auto cb = b.getColliderCenter();
    if (!ca || !cb) return;

    Vector2 delta = *cb - *ca;
    float dist = delta.magnitude();
    float minDist = circle1->radius + circle2->radius;
    if (dist >= minDist) return;

    Vector2 normal = (dist > 0.0001f) ? delta * (1.f / dist) : Vector2(1.f, 0.f);
    float overlap = minDist - dist;
    a.posComponent->position = a.posComponent->position - normal * (overlap * 0.5f);
    b.posComponent->position = b.posComponent->position + normal * (overlap * 0.5f);

}