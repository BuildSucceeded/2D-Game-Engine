#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "ParticleSystem.h"

#define EXPLOSION_PARTICLE_LIFETIME 3

class Explosion : public ParticleSystem
{
public:
	Explosion(Point2D pos);

private:
	Particle* EmitParticle() override;

	int spread;
};

