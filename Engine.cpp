#include "framework.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")

Engine::Engine() : m_pDirect2dFactory(NULL), m_pRenderTarget(NULL)
{
    // Constructor
    // Initialize your game elements here

}

Engine::~Engine()
{
    // Destructor

    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);

    // Safe-release your game elements here
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
    // Initializes Direct2D, for drawing
    D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        &m_pRenderTarget
    );

    // Initialize the D2D part of your game elements here

    return S_OK;
}

void Engine::KeyUp(WPARAM wParam)
{
    // If keyup, un-set the keys flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::KeyDown(WPARAM wParam)
{
    // If keyup, set the keys flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::MousePosition(int x, int y)
{
    // Campture mouse position when the mouse moves
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::MouseButtonUp(bool left, bool right)
{
    // If mouse button is released, set the mouse flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::MouseButtonDown(bool left, bool right)
{
    // If mouse button is pressed, set the mouse flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::Logic(double elapsedTime)
{
    // This is the logic part of the engine. Add your game logic here.
    // It runs every frame and it receives the elapsed time between the last frame and this one, in seconds.
    // Use this value for a smooth and consistent movement, regardless of the CPU or graphics speed


}

HRESULT Engine::Draw()
{
    // This is the drawing method of the engine.
    // It runs every frame

    // Draws the elements in the game using Direct2D
    HRESULT hr;

    m_pRenderTarget->BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Below you can add drawing logic for your game elements

    
    hr = m_pRenderTarget->EndDraw();

    return S_OK;
}