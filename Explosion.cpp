#include "Settings.h"
#include "Explosion.h"
#include "Engine.h"
#include "ExplosionParticle.h"

Explosion::Explosion(Point2D pos)
{
	position = pos;
	maxNumberOfParticles = 200;

	oneTimeEmission = true;
}

Particle* Explosion::EmitParticle()
{
	Point2D newPosition;
	newPosition.x = position.x;
	newPosition.y = position.y;

	Point2D newSpeed;
	int angle = rand() % 180 + 90;
	newSpeed.x = (rand() % 200) * sin(angle * 3.14159 / 180);
	newSpeed.y = (rand() % 300) * cos(angle * 3.14159 / 180);

	ExplosionParticle* particle = new ExplosionParticle(newPosition, newSpeed);
	return particle;
}