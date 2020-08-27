#include "framework.h"
#include "Point2D.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

Point2D GameObjectBase::GetPosition()
{
	return this->position;
}

double GameObjectBase::GetRotationAngle()
{
	return this->rotationAngle;
}

boolean GameObjectBase::GetCollisionType()
{
	return this->collisionType;
}

double GameObjectBase::GetCollisionCircleRadius()
{
	return collisionCircleRadius;
}

Point2D* GameObjectBase::GetCollisionPolyPoints()
{
	return collisionPolyPoints;
}

int GameObjectBase::GetNoCollisionPolyPoints()
{
	return noCollisionPolyPoints;
}

Point2D* GameObjectBase::GetRotatedCollisionPolyPoints()
{
	return rotatedCollisionPolyPoints;
}

void GameObjectBase::ClearAutoCollisions()
{
	autoCollisionsDetected.clear();
}

void GameObjectBase::CalculateRotatedCollisionPolyPoints()
{
	if (oldRotationAngle != rotationAngle)
	{

		for (int i = 0; i < noCollisionPolyPoints; i++)
		{
			rotatedCollisionPolyPoints[i] = Point2D::Create(
				collisionPolyPoints[i].x * cos(rotationAngle) - collisionPolyPoints[i].y * sin(rotationAngle),
				collisionPolyPoints[i].x * sin(rotationAngle) + collisionPolyPoints[i].y * cos(rotationAngle)
			);
		}

		oldRotationAngle = rotationAngle;
	}
}

void GameObjectBase::AddAutoCollision(GameObjectBase* gameObj)
{
	autoCollisionsDetected.push_back(gameObj);
}
