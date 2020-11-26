#include "Settings.h"
#include "Rain.h"
#include "Engine.h"
#include "RainParticle.h"

Rain::Rain(Point2D pos)
{
	position = pos;
	maxNumberOfParticles = 5000;
}

Particle* Rain::EmitParticle()
{
	Point2D newPosition;
	newPosition.x = rand() % RESOLUTION_X;
	newPosition.y = position.y;

	Point2D newSpeed;
	newSpeed.x = rand() % 40 - 20;
	newSpeed.y = 500 + rand() % (300);

	RainParticle* particle = new RainParticle(newPosition, newSpeed);
	return particle;
}