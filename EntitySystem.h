#pragma once
#include "EntityStructures.h"
#include "CircleCollisionResolver.h"

struct EntitySystem {
	std::vector<Entity> entities;

    void update(float dt) {
        for (auto& e : entities) {
            if (!e.posComponent || !e.velComponent) continue;

            auto& vel = e.velComponent->velocity;
            auto& pos = e.posComponent->position;

            pos = pos + vel * dt;
        }

        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                resolveCircleCollision(entities[i], entities[j]);
            }
        }
    }
	
};