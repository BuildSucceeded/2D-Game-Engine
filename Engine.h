#pragma once

#include "resource.h"

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

class Engine
{
public:
	Engine();
	~Engine();

	HRESULT InitializeD2D(HWND m_hwnd);
	void KeyUp(WPARAM wParam);
	void KeyDown(WPARAM wParam);
	void MousePosition(int x, int y);
	void MouseButtonUp(bool left, bool right);
	void MouseButtonDown(bool left, bool right);
	void Logic(double elapsedTime);
	HRESULT Draw();

private:
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
};

