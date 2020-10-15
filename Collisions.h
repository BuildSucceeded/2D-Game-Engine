#pragma once
#include "resource.h"
#include "CollisionDetails.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

class Collisions
{
public:
	static CollisionDetails ObjectsCollide(GameObjectBase *object1, GameObjectBase* object2);

	static Point2D GetProjectedPointOnLine(Point2D point, Point2D line);
	static double DotProduct(Point2D point1, Point2D point2);
	static CollisionDetails CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, Point2D* obj2Points, int obj2PointsNumber, Point2D positionDifference);
	static CollisionDetails CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, double obj2Radius, Point2D positionDifference);
};

