// Game/Objects/Pipe.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Pipe.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
// 构造函数使用全局配置
CPipe::CPipe() : CGameObject(0, 0, CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT)
{
}

// 带参数的构造函数
CPipe::CPipe(int x, int y, int height)
    : CGameObject(x, y, CGameConfig::PIPE_WIDTH, height)
{
}

// 析构函数
CPipe::~CPipe()
{
}

// 更新
void CPipe::Update(float deltaTime)
{
    // 水管是静态的，不需要更新
}

// 绘制 - 使用精灵渲染器
void CPipe::Draw(CDC* pDC)
{
    DrawWithSprite(pDC, m_nX, m_nY);
}

// 新增：使用精灵渲染器绘制水管
void CPipe::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(_T("Pipe"));

    if (pBitmap)
    {
        // 计算需要绘制的水管段数
        int segmentHeight = 16; // 每个水管段的高度
        int segments = m_nHeight / segmentHeight;

        // 绘制水管顶部
        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
            CSpriteConfig::PIPE_TOP_LEFT.x, CSpriteConfig::PIPE_TOP_LEFT.y,
            CSpriteConfig::PIPE_TOP_LEFT.width, CSpriteConfig::PIPE_TOP_LEFT.height, TRUE);

        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX + 16, screenY,
            CSpriteConfig::PIPE_TOP_RIGHT.x, CSpriteConfig::PIPE_TOP_RIGHT.y,
            CSpriteConfig::PIPE_TOP_RIGHT.width, CSpriteConfig::PIPE_TOP_RIGHT.height, TRUE);

        // 绘制水管身体
        for (int i = 1; i < segments; i++)
        {
            int currentY = screenY + i * segmentHeight;
            CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, currentY,
                CSpriteConfig::PIPE_BODY_LEFT.x, CSpriteConfig::PIPE_BODY_LEFT.y,
                CSpriteConfig::PIPE_BODY_LEFT.width, CSpriteConfig::PIPE_BODY_LEFT.height, TRUE);

            CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX + 16, currentY,
                CSpriteConfig::PIPE_BODY_RIGHT.x, CSpriteConfig::PIPE_BODY_RIGHT.y,
                CSpriteConfig::PIPE_BODY_RIGHT.width, CSpriteConfig::PIPE_BODY_RIGHT.height, TRUE);
        }
    }
    else
    {
        // 备用：如果没有贴图，使用颜色绘制
        COLORREF pipeGreen = RGB(0, 150, 0);
        COLORREF pipeDarkGreen = RGB(0, 100, 0);
        COLORREF pipeLightGreen = RGB(0, 200, 0);

        // 绘制水管主体
        pDC->FillSolidRect(screenX, screenY, m_nWidth, m_nHeight, pipeGreen);

        // 绘制水管纹理
        for (int i = 0; i < m_nHeight; i += 10)
        {
            pDC->FillSolidRect(screenX, screenY + i, 5, 5, pipeLightGreen);
            pDC->FillSolidRect(screenX + m_nWidth - 5, screenY + i, 5, 5, pipeDarkGreen);
        }

        // 绘制水管顶部
        pDC->FillSolidRect(screenX - 5, screenY, m_nWidth + 10, 15, pipeGreen);
        pDC->Draw3dRect(screenX - 5, screenY, m_nWidth + 10, 15, pipeDarkGreen, pipeLightGreen);
    }
}