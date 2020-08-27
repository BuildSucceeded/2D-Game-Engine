#include "framework.h"
#include "Settings.h"
#include "Collisions.h"

bool Collisions::ObjectsCollide(GameObjectBase* object1, GameObjectBase* object2)
{
	if (object1->GetCollisionType() == none || object2->GetCollisionType() == none)
	{
		return false;
	}

	// Circle to circle
	if (object1->GetCollisionType() == circle && object2->GetCollisionType() == circle)
	{
		Point2D point1 = object1->GetPosition();
		Point2D point2 = object2->GetPosition();
		double radiuses = object1->GetCollisionCircleRadius() + object2->GetCollisionCircleRadius();
		double distance = sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
		return distance < radiuses;
	}

	// Poly to Poly
	if (object1->GetCollisionType() == poly && object2->GetCollisionType() == poly)
	{
		// If possible, we do a stage 1 collision detection, using radiuses
		Point2D point1 = object1->GetPosition();
		Point2D point2 = object2->GetPosition();
		Point2D positionDiff = Point2D::Create(point2.x - point1.x, point2.y - point1.y);
		if (object1->GetCollisionCircleRadius() > 0 && object2->GetCollisionCircleRadius() > 0)
		{
			double radiuses = object1->GetCollisionCircleRadius() + object2->GetCollisionCircleRadius();
			double distance = sqrt(pow(positionDiff.x, 2) + pow(positionDiff.y, 2));
			if (distance > radiuses)
			{
				return false;
			}
		}

		Point2D* obj1Points = object1->GetRotatedCollisionPolyPoints();
		int obj1PointsNumber = object1->GetNoCollisionPolyPoints();
		Point2D* obj2Points = object2->GetRotatedCollisionPolyPoints();
		int obj2PointsNumber = object2->GetNoCollisionPolyPoints();

		// axis of the first object
		for (int i = 0; i < obj1PointsNumber; i++)
		{
			int j = i + 1;
			if (j == obj1PointsNumber)
				j = 0;

			Point2D lineVector;
			lineVector.x = obj1Points[j].y - obj1Points[i].y;
			lineVector.y = -obj1Points[j].x + obj1Points[i].x;

			if (!CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, obj2Points, obj2PointsNumber, positionDiff))
			{
				return false;
			}
		}
		// axis of the second object
		for (int i = 0; i < obj2PointsNumber; i++)
		{
			int j = i + 1;
			if (j == obj2PointsNumber)
				j = 0;

			Point2D lineVector;
			lineVector.x = obj2Points[j].y - obj2Points[i].y;
			lineVector.y = -obj2Points[j].x + obj2Points[i].x;

			if (!CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, obj2Points, obj2PointsNumber, positionDiff))
			{
				return false;
			}
		}

		return true;
	}

	// Poly to Circle
	if ((object1->GetCollisionType() == poly && object2->GetCollisionType() == circle) ||
		(object1->GetCollisionType() == circle && object2->GetCollisionType() == poly))
	{
		// If possible, we do a stage 1 collision detection, using radiuses
		Point2D point1 = object1->GetPosition();
		Point2D point2 = object2->GetPosition();
		Point2D positionDiff = Point2D::Create(point2.x - point1.x, point2.y - point1.y);
		if (object1->GetCollisionCircleRadius() > 0 && object2->GetCollisionCircleRadius() > 0)
		{
			double radiuses = object1->GetCollisionCircleRadius() + object2->GetCollisionCircleRadius();
			double distance = sqrt(pow(positionDiff.x, 2) + pow(positionDiff.y, 2));
			if (distance > radiuses)
			{
				return false;
			}
		}

		// Swap them (if necessary) so object1 is always poly and object2 is circle
		if (object1->GetCollisionType() == circle && object2->GetCollisionType() == poly)
		{
			GameObjectBase* objectAux = object1;
			object1 = object2;
			object2 = objectAux;
		}

		Point2D* obj1Points = object1->GetRotatedCollisionPolyPoints();
		int obj1PointsNumber = object1->GetNoCollisionPolyPoints();
		
		point1 = object1->GetPosition();
		point2 = object2->GetPosition();
		positionDiff = Point2D::Create(point2.x - point1.x, point2.y - point1.y);

		// axis of the first object (poly)
		Point2D closest; // + find closest point
		double minDistance = DBL_MAX;
		for (int i = 0; i < obj1PointsNumber; i++)
		{
			int j = i + 1;
			if (j == obj1PointsNumber)
				j = 0;

			Point2D lineVector;
			lineVector.x = obj1Points[j].y - obj1Points[i].y;
			lineVector.y = -obj1Points[j].x + obj1Points[i].x;

			if (!CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, object2->GetCollisionCircleRadius(), positionDiff))
			{
				return false;
			}

			// closest point
			double distance = sqrt(pow(obj1Points[i].x - object2->GetPosition().x, 2) + pow(obj1Points[i].y - object2->GetPosition().y, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				closest = obj1Points[i];
			}
		}

		// axis of the second object (circle)

		Point2D lineVector;
		lineVector.x = object2->GetPosition().x - closest.x;
		lineVector.y = object2->GetPosition().y - closest.y;

		if (!CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, object2->GetCollisionCircleRadius(), positionDiff))
		{
			return false;
		}

		return true;
	}

	return false;
}

boolean Collisions::CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, 
													   Point2D* obj2Points, int obj2PointsNumber, Point2D positionDifference)
{
	double obj1Min = DBL_MAX;
	double obj1Max = -DBL_MAX;
	double obj2Min = DBL_MAX;
	double obj2Max = -DBL_MAX;

	for (int i = 0; i < obj1PointsNumber; i++)
	{

		Point2D projected = GetProjectedPointOnLine(obj1Points[i], lineVector);

		double distanceOnLine = sqrt(projected.x * projected.x + projected.y * projected.y);
		if (projected.x + projected.y < 0)
			distanceOnLine = -distanceOnLine;

		if (obj1Min > distanceOnLine)
			obj1Min = distanceOnLine;

		if (obj1Max < distanceOnLine)
			obj1Max = distanceOnLine;
	}

	for (int i = 0; i < obj2PointsNumber; i++)
	{
		Point2D adjustedPoint = Point2D::Create(
			obj2Points[i].x + positionDifference.x,
			obj2Points[i].y + positionDifference.y
		);

		Point2D projected = GetProjectedPointOnLine(adjustedPoint, lineVector);

		double distanceOnLine = sqrt(projected.x * projected.x + projected.y * projected.y);
		if (projected.x + projected.y < 0)
			distanceOnLine = -distanceOnLine;

		if (obj2Min > distanceOnLine)
			obj2Min = distanceOnLine;

		if (obj2Max < distanceOnLine)
			obj2Max = distanceOnLine;
	}

	if (obj1Max < obj2Min || obj2Max < obj1Min)
		return false;

	return true;
}

boolean Collisions::CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber, 
													double obj2Radius, Point2D positionDifference)
{
	std::list<Point2D>::iterator pointsIter;
	double obj1Min = DBL_MAX;
	double obj1Max = -DBL_MAX;
	double obj2Min = DBL_MAX;
	double obj2Max = -DBL_MAX;

	for (int i = 0; i < obj1PointsNumber; i++)
	{
		Point2D projected = GetProjectedPointOnLine(obj1Points[i], lineVector);

		double distanceOnLine = sqrt(projected.x * projected.x + projected.y * projected.y);
		if (projected.x + projected.y < 0)
			distanceOnLine = -distanceOnLine;

		if (obj1Min > distanceOnLine)
			obj1Min = distanceOnLine;

		if (obj1Max < distanceOnLine)
			obj1Max = distanceOnLine;
	}

	Point2D projected = GetProjectedPointOnLine(positionDifference, lineVector);
	double distanceOnLine = sqrt(projected.x * projected.x + projected.y * projected.y);
	if (projected.x + projected.y < 0)
		distanceOnLine = -distanceOnLine;
	obj2Min = distanceOnLine - obj2Radius;
	obj2Max = distanceOnLine + obj2Radius;

	if (obj1Max < obj2Min || obj2Max < obj1Min)
		return false;

	return true;
}

Point2D Collisions::GetProjectedPointOnLine(Point2D point, Point2D line)
{
	Point2D proj;
	proj.x = ((point.x * line.x + point.y * line.y) / (line.x * line.x + line.y * line.y)) * line.x;
	proj.y = ((point.x * line.x + point.y * line.y) / (line.x * line.x + line.y * line.y)) * line.y;
	return proj;
}

