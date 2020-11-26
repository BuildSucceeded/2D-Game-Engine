#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "Particle.h"

class RainParticle : public Particle
{
public:
	RainParticle(Point2D initialPosition, Point2D initialSpeed);

	void Logic(double elapsedTime) override;

	bool IsOld() override;

	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

private:
	ID2D1SolidColorBrush* m_pBlueBrush;

};