#include "framework.h"
#include "Settings.h"
#include "Point2D.h"
#include "GameObjectBase.h"
#include "EngineBase.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")

EngineBase::EngineBase() : m_pDirect2dFactory(NULL), m_pRenderTarget(NULL)
{
}

EngineBase::~EngineBase()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
}

HRESULT EngineBase::InitializeD2D(HWND m_hwnd)
{
    // Initializes Direct2D, for drawing
    D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size, V_SYNC ? D2D1_PRESENT_OPTIONS_NONE : D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        &m_pRenderTarget
    );

    return S_OK;
}

void EngineBase::MousePosition(int x, int y)
{
    mousePosition.x = x;
    mousePosition.y = y;
}

void EngineBase::KeyUp(WPARAM wParam)
{
}

void EngineBase::KeyDown(WPARAM wParam)
{
}

void EngineBase::MouseButtonUp(bool left, bool right)
{
}

void EngineBase::MouseButtonDown(bool left, bool right)
{
}

void EngineBase::AddGameObject(GameObjectBase* gameObj)
{
    objectList.push_back(gameObj);
}

void EngineBase::RemoveGameObject(GameObjectBase* gameObj)
{
    objectList.remove(gameObj);
}

void EngineBase::Logic(double elapsedTime)
{
    std::list<GameObjectBase*>::iterator iter;

    for (iter = objectList.begin(); iter != objectList.end(); iter++)
       (*iter)->Logic(elapsedTime);
    
}

HRESULT EngineBase::Draw()
{
    // Draws the elements in the game using Direct2D
    HRESULT hr;

    m_pRenderTarget->BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    m_pRenderTarget->Clear(D2D1::ColorF(D2D_BACKGROUND_COLOR));

    

    std::list<GameObjectBase*>::iterator iter;
    for (iter = objectList.begin(); iter != objectList.end(); iter++)
        (*iter)->Draw(m_pRenderTarget);



    hr = m_pRenderTarget->EndDraw();

    return S_OK;
}
