#pragma once

#include "Point2D.h"

struct CollisionDetails
{
	bool Collides;
	double CollisionDistance;
	Point2D CollisionPointObject1;
	Point2D CollisionPointObject2;
	Point2D ReactionVectorObject1;
	Point2D ReactionVectorObject2;

	void SwapObjects() {
		double aux;

		aux = CollisionPointObject1.x;
		CollisionPointObject1.x = CollisionPointObject2.x;
		CollisionPointObject2.x = aux;
		aux = CollisionPointObject1.y;
		CollisionPointObject1.y = CollisionPointObject2.y;
		CollisionPointObject2.y = aux;

		aux = ReactionVectorObject1.x;
		ReactionVectorObject1.x = ReactionVectorObject2.x;
		ReactionVectorObject2.x = aux;
		aux = ReactionVectorObject1.y;
		ReactionVectorObject1.y = ReactionVectorObject2.y;
		ReactionVectorObject2.y = aux;
	}
};