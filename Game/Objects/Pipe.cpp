// Game/Objects/Pipe.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Pipe.h"

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
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForPipe());
    if (!pBitmap) {
        // 备用绘制
        Draw(pDC);
        return;
    }

    // 计算需要绘制的水管段数
    int segmentHeight = CSpriteConfig::PIPE_BODY_LEFT.height;
    int segments = m_nHeight / segmentHeight;

    if (segments < 1) segments = 1;

    // 绘制水管顶部

    CSpriteRenderer::DrawSprite(pDC, pBitmap,
        screenX, screenY, CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT,
        CSpriteConfig::PIPE_TOP_LEFT.x, CSpriteConfig::PIPE_TOP_LEFT.y,
        CSpriteConfig::PIPE_TOP_LEFT.width, CSpriteConfig::PIPE_TOP_LEFT.height, TRUE);
    CSpriteRenderer::DrawSprite(pDC, pBitmap,
        screenX + CSpriteConfig::PIPE_TOP_LEFT.width, screenY,
        CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT,
        CSpriteConfig::PIPE_TOP_LEFT.x, CSpriteConfig::PIPE_TOP_LEFT.y,
        CSpriteConfig::PIPE_TOP_LEFT.width, CSpriteConfig::PIPE_TOP_LEFT.height, TRUE);

    // 绘制水管身体
    for (int i = 1; i < segments; i++)
    {
        int currentY = screenY + i * segmentHeight;
        CSpriteRenderer::DrawSprite(pDC, pBitmap,
            screenX, currentY, CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT,
            CSpriteConfig::PIPE_BODY_LEFT.x, CSpriteConfig::PIPE_BODY_LEFT.y,
            CSpriteConfig::PIPE_BODY_LEFT.width, CSpriteConfig::PIPE_BODY_LEFT.height, TRUE);

        CSpriteRenderer::DrawSprite(pDC, pBitmap, CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT,
            screenX + CSpriteConfig::PIPE_BODY_LEFT.width, currentY,
            CSpriteConfig::PIPE_BODY_RIGHT.x, CSpriteConfig::PIPE_BODY_RIGHT.y,
            CSpriteConfig::PIPE_BODY_RIGHT.width, CSpriteConfig::PIPE_BODY_RIGHT.height, TRUE);
    }
}