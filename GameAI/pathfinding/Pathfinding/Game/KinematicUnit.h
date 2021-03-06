#pragma once

#include "Kinematic.h"
#include "Steering.h"
#include <vector>
#include "BoxCollider.h"

/*KinematicUnit - a unit that is derived from the Kinematic class.  Adds behaviors and max speeds and a current Steering.

Dean Lawson
Champlain College
2011
*/

//forward declarations
class Sprite;
class GraphicsBuffer;
class Enemy;

extern Steering gNullSteering;//global object - can point to it for a "NULL" Steering

//minmimum forward speed a unit has to have inorder to rotate 
//(keeps unit from spinning in place after stopping
const float MIN_VELOCITY_TO_TURN_SQUARED = 1.0f;

class KinematicUnit: public Kinematic
{
public:
	KinematicUnit( Sprite* pSprite, const Vector2D& position, float orientation, const Vector2D& velocity, float rotationVel, float maxVelocity = 1.0f, float maxAcceleration = 1.0f );
	~KinematicUnit();

	//getters and setters
	void setTarget( const Vector2D& target ) { mTarget = target; };
	const Vector2D& getPosition() const { return mPosition; };
	void setPostion(Vector2D pos) { mPosition = pos; }
	float getMaxVelocity() const { return mMaxVelocity; };
	void setMaxVelocity(float max) { mMaxVelocity = max; };
	Vector2D getVelocity() const { return mVelocity; };
	float getMaxAcceleration() const { return mMaxAcceleration; };
	void setVelocity( const Vector2D& velocity ){ mVelocity = velocity; };
	inline void setPlayer() { mIsPlayer = true; }
	float getDistance(KinematicUnit* other);
	BoxCollider* getCollider() { return &mBox; }
	void resetDeltaPosistion(Vector2D delta) { mDeltaPosition = delta; }
	Vector2D& getdelta(){ return mDeltaPosition; }
	void modDelta(Vector2D delta) { mDeltaPosition += delta; }
	virtual void setNewOrientation();//face the direction you are moving
	bool isActive() { return mActive; }
	//draw yourself to the indicated buffer
	void draw( GraphicsBuffer* pBuffer );
	//move according to the current velocities and update velocities based on current Steering
	virtual void update(float time);
	void modPosistion(Vector2D delta);
	void setselfEnemy(Enemy* enemyptr);
	Enemy* getEnemyptr();
	
	//initiate behaviors
	
	void wander();
	void seek(KinematicUnit* target);
	void flee(KinematicUnit* target);
	void inActive();
	
	void switchSprite(Sprite* spr);
	
protected:
	Sprite* mpSprite;
	Sprite* mpNormSprite;
	Steering* mpCurrentSteering;
	Steering* mpGroupSteering;
	Vector2D mTarget;//used only for Kinematic seek and arrive
	float mMaxVelocity;
	float mMaxAcceleration;
	BoxCollider mBox;
	Vector2D mDeltaPosition;
	Enemy* mpEnemyptr;

	bool mIsPlayer;
	bool mActive;

	void setSteering(Steering* pSteering );
	void setGroupSteering(Steering* pSteering);

};