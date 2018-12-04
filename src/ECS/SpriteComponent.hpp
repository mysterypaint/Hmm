#pragma once

#include "Components.hpp"
#include "../Resources.hpp"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	PHL_Rect srcRect, destRect;
	CameraComponent* camera;

public:
	SpriteTexture sTex;
	SpriteComponent() = default;
	SpriteComponent(PHL_Surface* _texGroup, Entity* _camera) {
		sTex.cropX = 0;
		sTex.cropY = 0;
		sTex.w = 16;
		sTex.h = 16;
		sTex.originX = 8;
		sTex.originY = 15;
		sTex.texGroup = _texGroup;
		camera = &_camera->GetComponent<CameraComponent>();
	}

	~SpriteComponent() {
		//PHL_FreeSurface(tex);
	}

	void SetTex(PHL_Surface* _newTex)
	{
		sTex.texGroup = _newTex;
	}

	void Create() override {
		transform = &entity->GetComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		destRect.w = destRect.h = 32;
	}

	void Step() override {
		//destRect.x = (int) transform->position.x;
		//destRect.y = (int) transform->position.y;
		srcRect.x = static_cast<int>(transform->position.x);
		srcRect.y = static_cast<int>(transform->position.y);
		srcRect.w = transform->width * transform->scale;
		srcRect.h = transform->height * transform->scale;
	}

	void Draw() override {
		PHL_DrawSurfacePart(srcRect.x - sTex.originX + camera->x, srcRect.y - sTex.originY + camera->y, sTex.cropX, sTex.cropY, sTex.w, sTex.h, *sTex.texGroup);
	}
};