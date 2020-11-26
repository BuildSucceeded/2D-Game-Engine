#include "framework.h"
#include "Settings.h"
#include "ExplosionParticle.h"
#include "Explosion.h"
#include "Engine.h"

ExplosionParticle::ExplosionParticle(Point2D initialPosition, Point2D initialSpeed)
{
	position = initialPosition;
	speed = initialSpeed;
}

void ExplosionParticle::Logic(double elapsedTime)
{
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;

	speed.y += 200 * elapsedTime;
}

void ExplosionParticle::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	float amountRed = 1.0f;
	double split = EXPLOSION_PARTICLE_LIFETIME * 2.0 / 3.0;
	if (age > split)
	{
		amountRed = 1 - (age - split) / (EXPLOSION_PARTICLE_LIFETIME - split);
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
		D2D1::Point2F(position.x, position.y),
		8, 8
	);
	m_pRenderTarget->FillEllipse(&ellipseBall, m_pYellowBrush);

	SafeRelease(&m_pYellowBrush);
}

bool ExplosionParticle::IsOld()
{
	return age > EXPLOSION_PARTICLE_LIFETIME;
}