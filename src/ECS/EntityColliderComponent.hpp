#pragma once
#include <string>
//#include "../PHL.hpp"
#include "Components.hpp"

class EntityColliderComponent : public Component {
public:
	PHL_Rect collider;
	int tag;

	TransformComponent* transform;

	EntityColliderComponent(int _t) {
		tag = _t;
	}

	void Create() override {
		// First ensure that this entity has a Transform component. Give it one if it doesn't have one already.
		/*
		if(!entity->HasComponent<TransformComponent>()) {
			entity->AddComponent<TransformComponent>();
		}*/
		transform = &entity->GetComponent<TransformComponent>();
	}

	void Step() override {
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
private:
};