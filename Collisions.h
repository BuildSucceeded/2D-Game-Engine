#pragma once
#include "resource.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

class Collisions
{
public:
	static bool ObjectsCollide(GameObjectBase *object1, GameObjectBase* object2);

	static Point2D GetProjectedPointOnLine(Point2D point, Point2D line);
	static boolean CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, Point2D* obj2Points, int obj2PointsNumber, Point2D positionDifference);
	static boolean CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, double obj2Radius, Point2D positionDifference);
};

