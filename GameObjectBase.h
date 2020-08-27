#pragma once

class EngineBase;

#include "resource.h"
#include "EngineBase.h"
#include "Point2D.h"

enum CollisionType { none, circle, poly };

class GameObjectBase
{
public:

	virtual void Logic(double elapsedTime) {}

	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) {}

	Point2D GetPosition();

	EngineBase* engine;
	
	double GetRotationAngle();

	// Collision detection
	boolean GetCollisionType();
	double GetCollisionCircleRadius();
	Point2D* GetCollisionPolyPoints();
	int GetNoCollisionPolyPoints();
	Point2D* GetRotatedCollisionPolyPoints();
	void ClearAutoCollisions();
	void CalculateRotatedCollisionPolyPoints();
	void AddAutoCollision(GameObjectBase* gameObj);

protected:
	Point2D position;
	Point2D speed;

	double rotationAngle;
	double oldRotationAngle;

	// Collision detection
	CollisionType collisionType = none;
	double collisionCircleRadius = 0;
	Point2D collisionPolyPoints[16];
	int noCollisionPolyPoints = 0;
	Point2D rotatedCollisionPolyPoints[16];
	std::list<GameObjectBase*> autoCollisionsDetected;

};

