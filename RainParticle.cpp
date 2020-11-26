#include "framework.h"
#include "Settings.h"
#include "RainParticle.h"
#include "Rain.h"
#include "Engine.h"

RainParticle::RainParticle(Point2D initialPosition, Point2D initialSpeed)
{
	position = initialPosition;
	speed = initialSpeed;
}

void RainParticle::Logic(double elapsedTime)
{
	position.x += speed.x * elapsedTime;
	position.y += speed.y * elapsedTime;
}

void RainParticle::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (m_pBlueBrush == NULL) {
		m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f),
			&m_pBlueBrush
		);
	}

	m_pRenderTarget->DrawLine(D2D1::Point2F(position.x, position.y), D2D1::Point2F(position.x, position.y - 10), m_pBlueBrush, 2.0f);

}

bool RainParticle::IsOld()
{
	return position.y > RESOLUTION_Y;
}