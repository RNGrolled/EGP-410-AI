#pragma once
#include "Game.h"
#include "GameApp.h"
#include "KinematicUnit.h"
#include "UnitManager.h"
#include "LevelLoader.h"

class Player : public KinematicUnit
{
public:
	Player(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration);
	~Player() {};

	void setLevel(Level* level) { mpLevel = level; }
	int getScore() { return mScore; }
	bool ifPowered() { return mPowerUp; }

	virtual void update(float time);
	void PowerUp();
	void checkBounds(int LevelIndex,GameApp* pGame );
	void stop();
	void reset();
	

private:
	Level* mpLevel;
	UnitManager* mpUnitManger;
	bool mPowerUp;
	int mPUCount;
	int mPUstart;
	int mScore;
};