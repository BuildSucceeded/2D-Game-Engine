#include "framework.h"
#include "Settings.h"
#include "Collisions.h"

CollisionDetails Collisions::ObjectsCollide(GameObjectBase* object1, GameObjectBase* object2)
{
	if (object1->GetCollisionType() == none || object2->GetCollisionType() == none)
	{
		CollisionDetails toReturn;
		toReturn.Collides = false;
		return toReturn;
	}

	// Circle to circle
	if (object1->GetCollisionType() == circle && object2->GetCollisionType() == circle)
	{
		Point2D point1 = object1->GetPosition();
		Point2D point2 = object2->GetPosition();
		double radiuses = object1->GetCollisionCircleRadius() + object2->GetCollisionCircleRadius();
		double distance = sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2));
		if (distance < radiuses)
		{
			CollisionDetails toReturn;
			toReturn.Collides = true;
			toReturn.CollisionDistance = radiuses - distance;

			toReturn.CollisionPointObject1.x = (point2.x - point1.x) / distance * object1->GetCollisionCircleRadius();
			toReturn.CollisionPointObject1.y = (point2.y - point1.y) / distance * object1->GetCollisionCircleRadius();

			toReturn.CollisionPointObject2.x = (point1.x - point2.x) / distance * object2->GetCollisionCircleRadius();
			toReturn.CollisionPointObject2.y = (point1.y - point2.y) / distance * object2->GetCollisionCircleRadius();

			toReturn.ReactionVectorObject1.x = (point2.x - point1.x) - toReturn.CollisionPointObject1.x + toReturn.CollisionPointObject2.x;
			toReturn.ReactionVectorObject1.y = (point2.y - point1.y) - toReturn.CollisionPointObject1.y + toReturn.CollisionPointObject2.y;

			toReturn.ReactionVectorObject2.x = (point1.x - point2.x) - toReturn.CollisionPointObject2.x + toReturn.CollisionPointObject1.x;
			toReturn.ReactionVectorObject2.y = (point1.y - point2.y) - toReturn.CollisionPointObject2.y + toReturn.CollisionPointObject1.y;

			return toReturn;
		}
		else {
			CollisionDetails toReturn;
			toReturn.Collides = false;
			return toReturn;
		}
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
				CollisionDetails toReturn;
				toReturn.Collides = false;
				return toReturn;
			}
		}

		Point2D* obj1Points = object1->GetRotatedCollisionPolyPoints();
		int obj1PointsNumber = object1->GetNoCollisionPolyPoints();
		Point2D* obj2Points = object2->GetRotatedCollisionPolyPoints();
		int obj2PointsNumber = object2->GetNoCollisionPolyPoints();

		CollisionDetails minColDet;
		minColDet.Collides = false;

		// axis of the first object
		for (int i = 0; i < obj1PointsNumber; i++)
		{
			int j = i + 1;
			if (j == obj1PointsNumber)
				j = 0;

			Point2D lineVector;
			lineVector.x = obj1Points[j].y - obj1Points[i].y;
			lineVector.y = -obj1Points[j].x + obj1Points[i].x;

			CollisionDetails cDet = CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, obj2Points, obj2PointsNumber, positionDiff);
			if (!cDet.Collides)
			{
				return cDet;
			}
			else
			{
				if (!minColDet.Collides || minColDet.CollisionDistance > cDet.CollisionDistance)
				{
					minColDet = cDet;
				}
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

			CollisionDetails cDet = CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, obj2Points, obj2PointsNumber, positionDiff);
			if (!cDet.Collides)
			{
				return cDet;
			}
			else
			{
				if (!minColDet.Collides || minColDet.CollisionDistance > cDet.CollisionDistance)
				{
					minColDet = cDet;
				}
			}
		}

		return minColDet;
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
				CollisionDetails toReturn;
				toReturn.Collides = false;
				return toReturn;
			}
		}

		bool swapped = false;
		// Swap them (if necessary) so object1 is always poly and object2 is circle
		if (object1->GetCollisionType() == circle && object2->GetCollisionType() == poly)
		{
			GameObjectBase* objectAux = object1;
			object1 = object2;
			object2 = objectAux;
			swapped = true; // Remember if it's swapped so we do the same in the collision details
		}

		Point2D* obj1Points = object1->GetRotatedCollisionPolyPoints();
		int obj1PointsNumber = object1->GetNoCollisionPolyPoints();
		
		point1 = object1->GetPosition();
		point2 = object2->GetPosition();
		positionDiff = Point2D::Create(point2.x - point1.x, point2.y - point1.y);

		CollisionDetails minColDet;
		minColDet.CollisionDistance = 0;
		minColDet.Collides = false;

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

			CollisionDetails cDet = CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, object2->GetCollisionCircleRadius(), positionDiff);
			if (!cDet.Collides)
			{
				return cDet;
			}
			else
			{
				if (!minColDet.Collides || minColDet.CollisionDistance > cDet.CollisionDistance)
				{
					minColDet = cDet;
				}
			}

			// closest point
			double distance = sqrt(pow(obj1Points[i].x - positionDiff.x, 2) + pow(obj1Points[i].y - positionDiff.y, 2));
			if (distance < minDistance)
			{
				minDistance = distance;
				closest = obj1Points[i];
			}
		}

		// axis of the second object (circle)

		Point2D lineVector;
		lineVector.x = closest.x - positionDiff.x;
		lineVector.y = closest.y - positionDiff.y;

		CollisionDetails cDet = CollidesOnAxis(lineVector, obj1Points, obj1PointsNumber, object2->GetCollisionCircleRadius(), positionDiff);
		if (!cDet.Collides)
		{
			return cDet;
		}
		else
		{
			if (!minColDet.Collides || minColDet.CollisionDistance > cDet.CollisionDistance)
			{
				minColDet = cDet;
			}
		}

		if (swapped)
		{
			minColDet.SwapObjects();
		}
		return minColDet;
	}

	CollisionDetails toReturn;
	toReturn.Collides = false;
	return toReturn;
}

CollisionDetails Collisions::CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber,
													   Point2D* obj2Points, int obj2PointsNumber, Point2D positionDifference)
{
	double obj1Min = DBL_MAX;
	int obj1MinPoint;
	double obj1Max = -DBL_MAX;
	int obj1MaxPoint;
	double obj2Min = DBL_MAX;
	int obj2MinPoint;
	double obj2Max = -DBL_MAX;
	int obj2MaxPoint;

	double obj1Distances[16];
	double obj2Distances[16];

	for (int i = 0; i < obj1PointsNumber; i++)
	{

		Point2D projected = GetProjectedPointOnLine(obj1Points[i], lineVector);

		double distanceOnLine = sqrt(projected.x * projected.x + projected.y * projected.y);
		if (projected.x + projected.y < 0)
			distanceOnLine = -distanceOnLine;

		obj1Distances[i] = distanceOnLine;

		if (obj1Min > distanceOnLine)
		{
			obj1Min = distanceOnLine;
			obj1MinPoint = i;
		}

		if (obj1Max < distanceOnLine)
		{
			obj1Max = distanceOnLine;
			obj1MaxPoint = i;
		}
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

		obj2Distances[i] = distanceOnLine;

		if (obj2Min > distanceOnLine)
		{
			obj2Min = distanceOnLine;
			obj2MinPoint = i;
		}

		if (obj2Max < distanceOnLine)
		{
			obj2Max = distanceOnLine;
			obj2MaxPoint = i;
		}
	}

	CollisionDetails toReturn;

	if (obj1Max <= obj2Min || obj2Max <= obj1Min)
	{
		toReturn.Collides = false;
	}
	else
	{
		toReturn.Collides = true;
		toReturn.CollisionDistance = min(abs(obj1Max - obj2Min), abs(obj1Min - obj2Max));
		toReturn.CollisionPointObject1.x = 0;
		toReturn.CollisionPointObject1.y = 0;
		toReturn.CollisionPointObject2.x = 0;
		toReturn.CollisionPointObject2.y = 0;

		double vectorLength = sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);

		if (abs(obj1Max - obj2Min) < abs(obj1Min - obj2Max))
		{

			double minDistanceToOtherObject = DBL_MAX;
			int noObj1LinePoints = 0;
			Point2D closestObj1Point;
			for (int i = 0; i < obj1PointsNumber; i++)
			{
				if (abs(obj1Distances[i] - obj1Distances[obj1MaxPoint]) < 0.01)
				{
					double distanceToOtherObject = sqrt(pow((obj1Points[i].x - positionDifference.x), 2) + pow((obj1Points[i].y - positionDifference.y), 2));
					if (minDistanceToOtherObject > distanceToOtherObject)
					{
						minDistanceToOtherObject = distanceToOtherObject;
						closestObj1Point = obj1Points[i];
					}
					noObj1LinePoints++;
				}
			}

			minDistanceToOtherObject = DBL_MAX;
			int noObj2LinePoints = 0;
			Point2D closestObj2Point;
			for (int i = 0; i < obj2PointsNumber; i++)
			{
				if (abs(obj2Distances[i] - obj2Distances[obj2MinPoint]) < 0.01)
				{
					double distanceToOtherObject = sqrt(pow((obj2Points[i].x + positionDifference.x), 2) + pow((obj2Points[i].y + positionDifference.y), 2));
					if (minDistanceToOtherObject > distanceToOtherObject)
					{
						minDistanceToOtherObject = distanceToOtherObject;
						closestObj2Point = obj2Points[i];
					}
					noObj2LinePoints++;
				}
			}

			if (noObj1LinePoints == 1 && noObj2LinePoints > 1)
			{

				toReturn.CollisionPointObject1.x = obj1Points[obj1MaxPoint].x;
				toReturn.CollisionPointObject1.y = obj1Points[obj1MaxPoint].y;
				toReturn.ReactionVectorObject1.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject1.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject1, toReturn.CollisionPointObject1) > 0)
				{
					toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject1.x;
					toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject1.y;
				}

				toReturn.CollisionPointObject2.x = toReturn.CollisionPointObject1.x - positionDifference.x + toReturn.ReactionVectorObject1.x;
				toReturn.CollisionPointObject2.y = toReturn.CollisionPointObject1.y - positionDifference.y + toReturn.ReactionVectorObject1.y;
				toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject1.x;
				toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject1.y;
			}

			if (noObj1LinePoints > 1 && noObj2LinePoints == 1)
			{
				toReturn.CollisionPointObject2.x = obj2Points[obj2MinPoint].x;
				toReturn.CollisionPointObject2.y = obj2Points[obj2MinPoint].y;
				toReturn.ReactionVectorObject2.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject2.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject2, toReturn.CollisionPointObject2) > 0)
				{
					toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject2.x;
					toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject2.y;
				}

				toReturn.CollisionPointObject1.x = toReturn.CollisionPointObject2.x + positionDifference.x + toReturn.ReactionVectorObject2.x;
				toReturn.CollisionPointObject1.y = toReturn.CollisionPointObject2.y + positionDifference.y + toReturn.ReactionVectorObject2.y;
				toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject2.x;
				toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject2.y;
			}

			if (noObj1LinePoints > 1 && noObj2LinePoints > 1)
			{
				toReturn.CollisionPointObject1.x = closestObj1Point.x;
				toReturn.CollisionPointObject1.y = closestObj1Point.y;
				toReturn.ReactionVectorObject1.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject1.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject1, toReturn.CollisionPointObject1) > 0)
				{
					toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject1.x;
					toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject1.y;
				}

				toReturn.CollisionPointObject2.x = closestObj2Point.x;
				toReturn.CollisionPointObject2.y = closestObj2Point.y;
				toReturn.ReactionVectorObject2.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject2.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject2, toReturn.CollisionPointObject2) > 0)
				{
					toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject2.x;
					toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject2.y;
				}
			}
		}
		else
		{

			double minDistanceToOtherObject = DBL_MAX;
			int noObj1LinePoints = 0;
			Point2D closestObj1Point;
			for (int i = 0; i < obj1PointsNumber; i++)
			{
				if (abs(obj1Distances[i] - obj1Distances[obj1MinPoint]) < 0.01)
				{
					double distanceToOtherObject = sqrt(pow((obj1Points[i].x - positionDifference.x), 2) + pow((obj1Points[i].y - positionDifference.y), 2));
					if (minDistanceToOtherObject > distanceToOtherObject)
					{
						minDistanceToOtherObject = distanceToOtherObject;
						closestObj1Point = obj1Points[i];
					}
					noObj1LinePoints++;
				}
			}

			minDistanceToOtherObject = DBL_MAX;
			int noObj2LinePoints = 0;
			Point2D closestObj2Point;
			for (int i = 0; i < obj2PointsNumber; i++)
			{
				if (abs(obj2Distances[i] - obj2Distances[obj2MaxPoint]) < 0.01)
				{
					double distanceToOtherObject = sqrt(pow((obj2Points[i].x + positionDifference.x), 2) + pow((obj2Points[i].y + positionDifference.y), 2));
					if (minDistanceToOtherObject > distanceToOtherObject)
					{
						minDistanceToOtherObject = distanceToOtherObject;
						closestObj2Point = obj2Points[i];
					}
					noObj2LinePoints++;
				}
			}

			if (noObj1LinePoints == 1 && noObj2LinePoints > 1)
			{
				toReturn.CollisionPointObject1.x = obj1Points[obj1MinPoint].x;
				toReturn.CollisionPointObject1.y = obj1Points[obj1MinPoint].y;
				toReturn.ReactionVectorObject1.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject1.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject1, toReturn.CollisionPointObject1) > 0)
				{
					toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject1.x;
					toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject1.y;
				}

				toReturn.CollisionPointObject2.x = toReturn.CollisionPointObject1.x - positionDifference.x + toReturn.ReactionVectorObject1.x;
				toReturn.CollisionPointObject2.y = toReturn.CollisionPointObject1.y - positionDifference.y + toReturn.ReactionVectorObject1.y;
				toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject1.x;
				toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject1.y;
			}

			if (noObj1LinePoints > 1 && noObj2LinePoints == 1)
			{
				toReturn.CollisionPointObject2.x = obj2Points[obj2MaxPoint].x;
				toReturn.CollisionPointObject2.y = obj2Points[obj2MaxPoint].y;
				toReturn.ReactionVectorObject2.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject2.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject2, toReturn.CollisionPointObject2) > 0)
				{
					toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject2.x;
					toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject2.y;
				}

				toReturn.CollisionPointObject1.x = toReturn.CollisionPointObject2.x + positionDifference.x + toReturn.ReactionVectorObject2.x;
				toReturn.CollisionPointObject1.y = toReturn.CollisionPointObject2.y + positionDifference.y + toReturn.ReactionVectorObject2.y;
				toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject2.x;
				toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject2.y;
			}

			if (noObj1LinePoints > 1 && noObj2LinePoints > 1)
			{
				toReturn.CollisionPointObject1.x = closestObj1Point.x;
				toReturn.CollisionPointObject1.y = closestObj1Point.y;
				toReturn.ReactionVectorObject1.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject1.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject1, toReturn.CollisionPointObject1) > 0)
				{
					toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject1.x;
					toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject1.y;
				}

				toReturn.CollisionPointObject2.x = closestObj2Point.x;
				toReturn.CollisionPointObject2.y = closestObj2Point.y;
				toReturn.ReactionVectorObject2.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
				toReturn.ReactionVectorObject2.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
				if (DotProduct(toReturn.ReactionVectorObject2, toReturn.CollisionPointObject2) > 0)
				{
					toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject2.x;
					toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject2.y;
				}
			}
		}
	}

	return toReturn;
}

CollisionDetails Collisions::CollidesOnAxis(Point2D lineVector, Point2D* obj1Points, int obj1PointsNumber,
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
	{
		CollisionDetails toReturn;
		toReturn.Collides = false;
		return toReturn;
	}

	double vectorLength = sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);
	double collX = lineVector.x / vectorLength * obj2Radius;
	double collY = lineVector.y / vectorLength * obj2Radius;
	if (DotProduct(positionDifference, Point2D::Create(collX, collY)) > 0)
	{
		collX = -collX;
		collY = -collY;
	}

	CollisionDetails toReturn;
	toReturn.Collides = true;
	toReturn.CollisionDistance = min(abs(obj1Max - obj2Min), abs(obj1Min - obj2Max));

	toReturn.CollisionPointObject2.x = collX;
	toReturn.CollisionPointObject2.y = collY;
	toReturn.ReactionVectorObject2.x = lineVector.x / vectorLength * toReturn.CollisionDistance;
	toReturn.ReactionVectorObject2.y = lineVector.y / vectorLength * toReturn.CollisionDistance;
	if (DotProduct(toReturn.ReactionVectorObject2, Point2D::Create(collX - positionDifference.x, collY - positionDifference.y)) > 0)
	{
		toReturn.ReactionVectorObject2.x = -toReturn.ReactionVectorObject2.x;
		toReturn.ReactionVectorObject2.y = -toReturn.ReactionVectorObject2.y;
	}

	toReturn.CollisionPointObject1.x = collX + positionDifference.x + toReturn.ReactionVectorObject2.x;
	toReturn.CollisionPointObject1.y = collY + positionDifference.y + toReturn.ReactionVectorObject2.y;
	toReturn.ReactionVectorObject1.x = -toReturn.ReactionVectorObject2.x;
	toReturn.ReactionVectorObject1.y = -toReturn.ReactionVectorObject2.y;
	return toReturn;
}

Point2D Collisions::GetProjectedPointOnLine(Point2D point, Point2D line)
{
	Point2D proj;
	proj.x = ((point.x * line.x + point.y * line.y) / (line.x * line.x + line.y * line.y)) * line.x;
	proj.y = ((point.x * line.x + point.y * line.y) / (line.x * line.x + line.y * line.y)) * line.y;
	return proj;
}

double Collisions::DotProduct(Point2D point1, Point2D point2)
{
	return point1.x * point2.x + point1.y * point2.y;
}
