#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "GameObjectBase.h"
#include "Particle.h"

class ParticleSystem : public GameObjectBase
{
public:

	void Logic(double elapsedTime) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

protected:
	virtual Particle* EmitParticle();

	Particle* particles[32768];
	int numberOfParticles;
	int maxNumberOfParticles;
	double timeSinceLastEmission = 0;

	double particleLifetime;

	bool oneTimeEmission = false;
	bool emitted = false;

};

