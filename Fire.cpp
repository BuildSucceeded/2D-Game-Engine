#include "Settings.h"
#include "Fire.h"
#include "Engine.h"
#include "FireParticle.h"

Fire::Fire(Point2D pos, int spreadSize)
{
	position = pos;
	spread = spreadSize;
	particleLifetime = FIRE_PARTICLE_LIFETIME;
	maxNumberOfParticles = 3000;
}

Particle* Fire::EmitParticle()
{
	Point2D newPosition;
	newPosition.x = position.x + rand() % (2 * spread) - spread;
	newPosition.y = position.y;

	Point2D newSpeed;
	newSpeed.x = 0;
	newSpeed.y = -200 - rand() % (200);

	FireParticle* particle = new FireParticle(newPosition, newSpeed);
	return particle;
}