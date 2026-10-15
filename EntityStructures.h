#pragma once
#include <optional>
#include <vector>
#include "MathStructures.h"
#include <SDL3/SDL.h>

// Position
struct Position {
	Vector2 position{0.f, 0.f};
};
// Movement
struct Velocity {
	Vector2 velocity{ 0.f, 0.f };
};
struct MoveSpeed {
	float speed{ 0.f };
};
struct Health {
	int hp{0};
};
struct CircleCollider {
	Vector2 offset{0.f, 0.f};
	float radius{0.f};
};

struct Entity {
	std::optional<Position> posComponent{};
	std::optional<Velocity> velComponent{};
	std::optional<MoveSpeed> speedComponent{};
	std::optional<Health> healthComponent{};
	std::optional<CircleCollider> colliderComponent{};

	std::optional<Vector2> getWorldPos() const {
		if (!posComponent) return std::nullopt;
		return posComponent->position;
	}

	std::optional<Vector2> getColliderCenter() const {
		if (!posComponent || !colliderComponent) return std::nullopt;
		return posComponent->position + colliderComponent->offset;
	}
};