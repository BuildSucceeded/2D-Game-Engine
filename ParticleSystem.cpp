#include "Settings.h"
#include "ParticleSystem.h"
#include "Engine.h"

void ParticleSystem::Logic(double elapsedTime)
{
	for (int i = 0; i < numberOfParticles; i++)
	{
		particles[i]->Logic(elapsedTime);
		particles[i]->GrowOld(elapsedTime);
	}

	// Remove old particles
	for (int i = 0; i < numberOfParticles; i++)
	{
		if (particles[i]->IsOld())
		{
			Particle* particle = particles[i];
			for (int j = i; j < numberOfParticles - 1; j++)
			{
				particles[j] = particles[j + 1];
			}
			particles[numberOfParticles - 1] = NULL;
			delete particle;
			numberOfParticles--;
			i--;
		}
	}

	// Emit new particles
	if (oneTimeEmission)
	{
		if (!emitted)
		{
			for (int i = 0; i < maxNumberOfParticles; i++)
			{
				particles[i] = EmitParticle();
				numberOfParticles = maxNumberOfParticles;
			}
			emitted = true;
		}
	}
	else
	{
		timeSinceLastEmission += elapsedTime;
		if (timeSinceLastEmission > particleLifetime / maxNumberOfParticles)
		{
			timeSinceLastEmission = 0;
			particles[numberOfParticles] = EmitParticle();
			numberOfParticles++;
		}
	}
}

void ParticleSystem::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	for (int i = 0; i < numberOfParticles; i++) {
		particles[i]->Draw(m_pRenderTarget);
	}
}

Particle* ParticleSystem::EmitParticle()
{
	return NULL;
}