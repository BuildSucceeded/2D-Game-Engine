#pragma once

struct Point2D
{
	double x;
	double y;

	static Point2D Create(double x, double y)
	{
		Point2D toReturn;
		toReturn.x = x;
		toReturn.y = y;
		return toReturn;
	}
};