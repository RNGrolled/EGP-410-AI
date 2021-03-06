#include "Game.h"
#include "Kinematic.h"
#include "KinematicUnit.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "Steering.h"
#include "KinematicSeekSteering.h"
#include "KinematicArriveSteering.h"
#include "KinematicWanderSteering.h"
#include "DynamicSeekSteering.h"
#include "DynamicArriveSteering.h"
#include "DynamicWanderSeekSteering.h"
#include "BoidSteering.h"
#include "UnitManager.h"
#include "WallManager.h"
#include "Pillar.h"

using namespace std;

Steering gNullSteering( gZeroVector2D, 0.0f );

KinematicUnit::KinematicUnit(Sprite *pSprite, const Vector2D &position, float orientation, const Vector2D &velocity, float rotationVel, float maxVelocity, float maxAcceleration)
:Kinematic( position, orientation, velocity, rotationVel )
,mpSprite(pSprite)
,mpCurrentSteering(NULL)
,mpGroupSteering(NULL)
,mMaxVelocity(maxVelocity)
,mMaxAcceleration(maxAcceleration)
,mIsPlayer(false)
{
	mBox = BoxCollider(mPosition,pSprite->getDim());
}

KinematicUnit::~KinematicUnit()
{
	delete mpCurrentSteering;
	//delete mpGroupSteering;
}

void KinematicUnit::draw( GraphicsBuffer* pBuffer )
{
	mpSprite->draw( *pBuffer, mPosition.getX(), mPosition.getY(), mOrientation );
}

void KinematicUnit::update(float time, const std::vector<KinematicUnit*> &units)
{
	Steering* steering;
	mBox.setPos(mPosition);
	if( mpCurrentSteering != NULL )
	{
		steering = mpCurrentSteering->getSteering();
	}
	else
	{
		steering = &gNullSteering;
	}

	if( steering->shouldApplyDirectly() )
	{
		//not stopped
		if( getVelocity().getLengthSquared() > MIN_VELOCITY_TO_TURN_SQUARED )
		{
			setVelocity( steering->getLinear() );
			setOrientation( steering->getAngular() );
		}

		//since we are applying the steering directly we don't want any rotational velocity
		setRotationalVelocity( 0.0f );
		steering->setAngular( 0.0f );
	}
	if (gpGame->getWallManager()->checkCollision(&mBox))
	{
		//cout << "COLLISION";
		Vector2D bounce =  Vector2D((WIDTH / 2), (HEIGHT / 2)) - mPosition;  // seek center
		this->setVelocity(bounce);
		this->mpCurrentSteering->setLinear(bounce);
		steering= &gNullSteering;
		if (mIsPlayer)
		{
			arrive(Vector2D(mPosition-(mVelocity*-0.25))); // will smash into wall infinitely otherwise without pathfinding
		}
	}
	if (gpGame->getPillars()->checkCollision(&mBox))
	{
		Vector2D swerve = (gpGame->getPillars()->getPos() - mPosition)*5; 
		mVelocity += Vector2D(swerve.getY(),swerve.getX());//swerve vector that moves to side of circle
		mpCurrentSteering->setLinear(Vector2D(swerve.getY(), swerve.getX()));
	}

	//move the unit using current velocities
	Kinematic::update( time );
	//calculate new velocities
	calcNewVelocities( *steering, time, mMaxVelocity, 25.0f );


	//move to oposite side of screen if we are off
	GRAPHICS_SYSTEM->wrapCoordinates( mPosition );

	//set the orientation to match the direction of travel
	setNewOrientation();
}

//private - deletes old Steering before setting
void KinematicUnit::setSteering( Steering* pSteering )
{
	delete mpCurrentSteering;
	mpCurrentSteering = pSteering;
}

void KinematicUnit::setGroupSteering(Steering* pSteering)
{
	delete mpGroupSteering;
	mpGroupSteering = pSteering;
}

float KinematicUnit::getDistance(KinematicUnit * other)
{
	Vector2D distanceVec = other->getPosition() - this->getPosition();
	float distance = distanceVec.getLength();
	return distance;
}

void KinematicUnit::setNewOrientation()
{ 
	mOrientation = getOrientationFromVelocity( mOrientation, mVelocity ); 
}

void KinematicUnit::seek(const Vector2D &target)
{
	KinematicSeekSteering* pSeekSteering = new KinematicSeekSteering( this, target );
	setSteering( pSeekSteering );
}

void KinematicUnit::arrive(const Vector2D &target)
{
	KinematicArriveSteering* pArriveSteering = new KinematicArriveSteering( this, target );
	setSteering( pArriveSteering );
}

void KinematicUnit::wander()
{
	KinematicWanderSteering* pWanderSteering = new KinematicWanderSteering( this );
	setSteering( pWanderSteering );
}

void KinematicUnit::dynamicSeek( KinematicUnit* pTarget )
{
	DynamicSeekSteering* pDynamicSeekSteering = new DynamicSeekSteering( this, pTarget);//gpGame->getPlayerUnit()
	setSteering( pDynamicSeekSteering );
}

void KinematicUnit::dynamicFlee( KinematicUnit* pTarget )
{
	DynamicSeekSteering* pDynamicSeekSteering = new DynamicSeekSteering( this, pTarget, true );
	setSteering( pDynamicSeekSteering );
}

void KinematicUnit::dynamicArrive( KinematicUnit* pTarget )
{
	DynamicArriveSteering* pDynamicArriveSteering = new DynamicArriveSteering( this, pTarget );
	setSteering( pDynamicArriveSteering );
}

void KinematicUnit::wanderSeek(KinematicUnit * pTarget)
{
	DynamicWanderSeekSteering* pDynamicWanderSeekSteering = new DynamicWanderSeekSteering(this, pTarget);
	setSteering(pDynamicWanderSeekSteering);

}

void KinematicUnit::wanderFlee(KinematicUnit * pTarget)
{
	DynamicWanderSeekSteering* pDynamicWanderSeekSteering = new DynamicWanderSeekSteering(this, pTarget, true);
	setSteering(pDynamicWanderSeekSteering);

}

void KinematicUnit::boid(KinematicUnit * pTarget)
{
	BoidSteering* pBoidSteering = new BoidSteering(this);
	setSteering(pBoidSteering);

}


