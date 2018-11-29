#include "PHL.hpp"
#include "Collision.hpp"

bool Collision::AABB(PHL_Rect& _recA, PHL_Rect& _recB) {
	if (
		_recA.x + _recA.w >= _recB.x &&
		_recB.x + _recB.w >= _recA.x &&
		_recA.y + _recA.h >= _recB.y &&
		_recB.y + _recB.h >= _recA.y
		)
		return true; // There was a collision
	return false; // There wasn't
}