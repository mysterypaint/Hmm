#pragma once

#include "Components.hpp"
#include "../Resources.hpp"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	SpriteTexture sTex;
	PHL_Rect srcRect, destRect;

public:
	SpriteComponent() = default;
	SpriteComponent(PHL_Surface* _texGroup) {
		sTex.cropX = 0;
		sTex.cropY = 0;
		sTex.w = 16;
		sTex.h = 16;
		sTex.originX = 8;
		sTex.originY = 15;
		sTex.bboxLeft = -sTex.originX;
		sTex.bboxRight = sTex.w - sTex.originX;
		sTex.bboxTop = -sTex.originY;
		sTex.bboxBottom = sTex.h - sTex.originY;
		sTex.texGroup = _texGroup;
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
		PHL_DrawSurfacePart(srcRect.x - sTex.originX, srcRect.y - sTex.originY, sTex.cropX, sTex.cropY, sTex.w, sTex.h, *sTex.texGroup);
	}
};