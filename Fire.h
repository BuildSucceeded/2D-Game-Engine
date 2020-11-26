#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "ParticleSystem.h"

#define FIRE_PARTICLE_LIFETIME 1

class Fire : public ParticleSystem
{
public:
	Fire(Point2D pos, int spreadSize);

private:
	Particle* EmitParticle() override;

	int spread;
};

