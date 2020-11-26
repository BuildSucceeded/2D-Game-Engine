#include "framework.h"
#include "Settings.h"
#include "FireParticle.h"
#include "Fire.h"
#include "Engine.h"

FireParticle::FireParticle(Point2D initialPosition, Point2D initialSpeed)
{
	position = initialPosition;
	speed = initialSpeed;
	sidewaysMovement = rand() % 40;
	sidewaysMovementSeed = rand() % 40;
}

void FireParticle::Logic(double elapsedTime)
{
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;
}

void FireParticle::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	float amountRed = 1.0f;
	double split = FIRE_PARTICLE_LIFETIME * 2.0 / 3.0;
	if (age > split)
	{
		amountRed = 1 - (age - split) / (FIRE_PARTICLE_LIFETIME - split);
	}
	float amountGreen = 0.0f;
	if (age < split)
	{
		amountGreen = 1 - age / split;
	}

	ID2D1SolidColorBrush* m_pYellowBrush;
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(amountRed, amountGreen, 0.0f),
		&m_pYellowBrush
	);

	D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
		D2D1::Point2F(position.x + 
						sidewaysMovement * sin((position.y + sidewaysMovementSeed) / sidewaysMovement), 
					position.y),
		12, 12
	);
	m_pRenderTarget->FillEllipse(&ellipseBall, m_pYellowBrush);

	SafeRelease(&m_pYellowBrush);
}

bool FireParticle::IsOld()
{
	return age > FIRE_PARTICLE_LIFETIME;
}