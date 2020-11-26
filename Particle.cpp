#include "Settings.h"
#include "Particle.h"
#include "Engine.h"

void Particle::GrowOld(double elapsedTime)
{
	age += elapsedTime;
}

double Particle::GetAge()
{
	return age;
}

bool Particle::IsOld()
{
	return false;
}