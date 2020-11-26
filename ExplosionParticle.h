#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "Particle.h"

class ExplosionParticle : public Particle
{
public:
	ExplosionParticle(Point2D initialPosition, Point2D initialSpeed);

	void Logic(double elapsedTime) override;

	bool IsOld() override;

	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;


};