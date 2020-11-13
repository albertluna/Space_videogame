#pragma once
#include "GameObjectMesh.h"

class GameObjectPlayer : public GameObjectMesh {
public:
	virtual void update(float dt);
};
