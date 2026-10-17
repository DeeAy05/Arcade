#pragma once
#include "EntityStructures.h"
#include "CollisionResolver.h"
#include "Config.h"
// TO-DO
// 1. Spawn entity function.
// 2. makePlayer, makeEnemy functions.
struct EntitySystem {
	std::vector<Entity> entities;

	void update(float dt) {
		for (auto& e : entities) {
			positionUpdate(dt, e);
		}

		for (size_t i = 0; i < entities.size(); ++i) {
			for (size_t j = i + 1; j < entities.size(); ++j) {
				collisionResolveUpdate(entities[i], entities[j]);
			}
		}
	}

	void positionUpdate(float dt, Entity& e) {
		if (!e.posComponent || !e.velComponent) return;

		auto& vel = e.velComponent->velocity;
		auto& pos = e.posComponent->position;

		pos = pos + vel * dt;


		// World boundary logic
		if (pos.dx < CFG_WORLD_LEFT_BOUND) {
			pos.dx = CFG_WORLD_LEFT_BOUND;
			vel.dx *= -1.f;
		}
		if (pos.dx + CFG_PLAYER_SIZE > CFG_WORLD_RIGHT_BOUND) {
			pos.dx = CFG_WORLD_RIGHT_BOUND - CFG_PLAYER_SIZE;
			vel.dx *= -1.f;
		}
		if (pos.dy < CFG_WORLD_TOP_BOUND) {
			pos.dy = CFG_WORLD_TOP_BOUND;
			vel.dy *= -1.f;
		}
		if (pos.dy + CFG_PLAYER_SIZE > CFG_WORLD_BOTTOM_BOUND) {
			pos.dy = CFG_WORLD_BOTTOM_BOUND - CFG_PLAYER_SIZE;
			vel.dy *= -1.f;
		}
	}

	void collisionResolveUpdate(Entity& e1, Entity& e2) {
		if (auto hit = getRectangleRectangleCollision(e1, e2)) resolveCollision(*hit);
		if (auto hit = getCircleCircleCollision(e1, e2)) resolveCollision(*hit);
		if (auto hit = getRectangleCircleCollision(e1, e2)) resolveCollision(*hit);
		if (auto hit = getCircleRectangleCollision(e1, e2)) resolveCollision(*hit);
	}

};