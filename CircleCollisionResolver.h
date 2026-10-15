#pragma once
#include "EntityStructures.h"

inline float dot(const Vector2& a, const Vector2& b) {
	return a.dx * b.dx + a.dy * b.dy;
}

inline void resolveCircleCollision(Entity& a, Entity& b) {
	if (&a == &b) return;
	auto ca = a.getColliderCenter();
	auto cb = b.getColliderCenter();

	if (!ca || !cb) return;

	Vector2 delta = *cb - *ca;
	float dist = delta.magnitude();
	float minDist = a.colliderComponent->radius + b.colliderComponent->radius;
	if (dist >= minDist) return;

	Vector2 normal = (dist > 0.0001f) ? delta * (1.f / dist) : Vector2(1.f, 0.f);
	float overlap = minDist - dist;
	a.posComponent->position = a.posComponent->position - normal * (overlap * 0.5f);
	b.posComponent->position = b.posComponent->position + normal * (overlap * 0.5f);
}