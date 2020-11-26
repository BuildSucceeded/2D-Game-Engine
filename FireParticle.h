#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "Particle.h"

class FireParticle : public Particle
{
public:
	FireParticle(Point2D initialPosition, Point2D initialSpeed);

	void Logic(double elapsedTime) override;

	bool IsOld() override;

	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

private:
	double sidewaysMovement;
	double sidewaysMovementSeed;

};