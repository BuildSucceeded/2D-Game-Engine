#include "framework.h"
#include "Point2D.h"
#include "Collisions.h"
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
	noCollisionsDetected = 0;
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

void GameObjectBase::AddAutoCollision(GameObjectBase* gameObj, CollisionDetails cDet)
{
	autoCollisionsDetected[noCollisionsDetected] = gameObj;
	autoCollisionsDetails[noCollisionsDetected] = cDet;
	noCollisionsDetected++;
}

void GameObjectBase::ReactToCollisions(double elapsedTime)
{
	if (collisionType == circle)
	{
		for (int i = 0; i < noCollisionsDetected; i++)
		{
			position.x += autoCollisionsDetails[i].ReactionVectorObject1.x;
			position.y += autoCollisionsDetails[i].ReactionVectorObject1.y;
			speed.x += autoCollisionsDetails[i].ReactionVectorObject1.x;
			speed.y += autoCollisionsDetails[i].ReactionVectorObject1.y;
		}
	}

	if (collisionType == poly)
	{
		for (int i = 0; i < noCollisionsDetected; i++)
		{
			Point2D centerAx = Point2D::Create(-autoCollisionsDetails[i].CollisionPointObject1.x, -autoCollisionsDetails[i].CollisionPointObject1.y);
			Point2D centerPP = Point2D::Create(autoCollisionsDetails[i].CollisionPointObject1.y, -autoCollisionsDetails[i].CollisionPointObject1.x);

			Point2D speedChange = Collisions::GetProjectedPointOnLine(autoCollisionsDetails[i].ReactionVectorObject1, centerAx);
			position.x += speedChange.x;
			position.y += speedChange.y;
			speed.x += speedChange.x * elapsedTime * 50;
			speed.y += speedChange.y * elapsedTime * 50;

			Point2D rotationChangePoint = Collisions::GetProjectedPointOnLine(autoCollisionsDetails[i].ReactionVectorObject1, centerPP);
			double rotationChangeAmount = sqrt(pow(rotationChangePoint.x, 2) + pow(rotationChangePoint.y, 2));
			if (Collisions::DotProduct(centerPP, rotationChangePoint) > 0) {
				rotationChangeAmount = -rotationChangeAmount;
			}
			rotationSpeed += rotationChangeAmount * elapsedTime * 2;
		}
	}
}