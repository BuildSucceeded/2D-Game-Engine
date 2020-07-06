#pragma once

#include "resource.h"
#include "Point2D.h"

class GameObjectBase
{
public:

	virtual void Logic(double elapsedTime) {}

	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) {}

	Point2D GetPosition();

protected:
	Point2D position;

};

