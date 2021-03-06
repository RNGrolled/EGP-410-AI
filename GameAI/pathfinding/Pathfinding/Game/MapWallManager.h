#pragma once
#include "Game.h"
#include "BoxCollider.h"
#include "Grid.h"
#include <vector>

class MapWallManager : public Trackable
{
public:
	MapWallManager();
	~MapWallManager();

	void clear();
	void initWalls(Grid* mapGrid);
	bool checkCollision(BoxCollider* pBox);
	void modBoxPosisions(Vector2D delta);

private:
	std::vector<BoxCollider*> mpWallBoxes;

};
