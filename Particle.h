#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

class Particle : public GameObjectBase
{
public:

	void GrowOld(double elapsedTime);

	virtual bool IsOld();

	double GetAge();

protected:
	double age = 0;

};

