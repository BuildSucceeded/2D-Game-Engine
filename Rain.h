#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "ParticleSystem.h"

class Rain : public ParticleSystem
{
public:
	Rain(Point2D pos);

private:
	Particle* EmitParticle() override;

	int spread;
};

