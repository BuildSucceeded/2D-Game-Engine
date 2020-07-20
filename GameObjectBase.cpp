#include "framework.h"
#include "Point2D.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

Point2D GameObjectBase::GetPosition()
{
	return this->position;
}
