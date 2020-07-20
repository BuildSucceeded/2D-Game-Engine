#pragma once

class EngineBase;

#include "resource.h"
#include "EngineBase.h"
#include "Point2D.h"

class GameObjectBase
{
public:

	virtual void Logic(double elapsedTime) {}

	virtual void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) {}

	Point2D GetPosition();

	EngineBase* engine;

protected:
	Point2D position;

};

