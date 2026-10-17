#pragma once
#include <optional>
#include <variant>
#include <vector>
#include "MathStructures.h"
#include "Config.h"
#include <SDL3/SDL.h>
// TO-DO 
// 1. Size component used for drawing and collider setup.
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
struct RectangleCollider {
	Vector2 offset{ 0.f, 0.f };
	float w{ 0.f }, h{ 0.f };
};

struct Entity {
	std::optional<Position> posComponent{};
	std::optional<Velocity> velComponent{};
	std::optional<MoveSpeed> speedComponent{};
	std::optional<Health> healthComponent{};
	std::optional<std::variant<CircleCollider, RectangleCollider>> colliderComponent{};

	std::optional<Vector2> getWorldPos() const {
		if (!posComponent) return std::nullopt;
		return Vector2{ posComponent->position.dx + CFG_PLAYER_SIZE / 2, posComponent->position.dy + CFG_PLAYER_SIZE / 2 };
	}

	std::optional<Vector2> getColliderCenter() const {
		if (!posComponent || !colliderComponent) return std::nullopt;
		if (const auto* circle = std::get_if<CircleCollider>(&*colliderComponent)) {
			return posComponent->position + circle->offset;
		}
		if (const auto* rect = std::get_if<RectangleCollider>(&*colliderComponent)) {
			return posComponent->position + rect->offset;
		}
		return std::nullopt;
	}
};