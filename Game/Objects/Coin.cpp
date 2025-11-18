// Game/Objects/Coin.cpp
#include <afxwin.h>
#include "Coin.h"

// 静态成员初始化
int CCoin::s_nGlobalFrame = 0;
float CCoin::s_fGlobalSpinTimer = 0.0f;

// 构造函数
CCoin::CCoin()
    : CGameObject(0, 0, CGameConfig::COIN_SIZE, CGameConfig::COIN_SIZE)
{
    m_bCollected = FALSE;
}

CCoin::CCoin(int x, int y)
    : CGameObject(x, y, CGameConfig::COIN_SIZE, CGameConfig::COIN_SIZE)
{
    m_bCollected = FALSE;
}

CCoin::~CCoin()
{
}

void CCoin::Update(float deltaTime)
{
    // 现在动画由静态方法统一更新，这里不需要做任何事情
}

void CCoin::Draw(CDC* pDC)
{
    // 这个方法不应该被直接调用
    ASSERT(FALSE);
}

void CCoin::DrawAt(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible || m_bCollected) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::TILESET_MAIN);

    if (!pBitmap)
    {
        // 备用绘制：黄色圆形
        CBrush brush(RGB(255, 215, 0));
        CPen pen(PS_SOLID, 1, RGB(180, 150, 0));
        CBrush* pOldBrush = pDC->SelectObject(&brush);
        CPen* pOldPen = pDC->SelectObject(&pen);

        pDC->Ellipse(screenX, screenY, screenX + m_nWidth, screenY + m_nHeight);

        pDC->SelectObject(pOldBrush);
        pDC->SelectObject(pOldPen);
        return;
    }

    // 获取当前帧的精灵坐标
    SSpriteCoord spriteCoord = GetCurrentSpriteCoord();

    // 使用精灵渲染器绘制
    CSpriteRenderer::DrawSprite(pDC, pBitmap,
        screenX, screenY, m_nWidth, m_nHeight,
        spriteCoord.x, spriteCoord.y,
        spriteCoord.width, spriteCoord.height);
}

// 静态方法：更新所有金币的动画
void CCoin::UpdateGlobalAnimation(float deltaTime)
{
    s_fGlobalSpinTimer += deltaTime;

    // 检查是否需要切换到下一帧
    if (s_fGlobalSpinTimer >= CGameConfig::SPIN_FRAME_TIME)
    {
        s_fGlobalSpinTimer = 0.0f;
        s_nGlobalFrame = (s_nGlobalFrame + 1) % CGameConfig::COIN_SPIN_FRAMES;
    }
}

SSpriteCoord CCoin::GetCurrentSpriteCoord() const
{
    // 所有金币使用相同的全局帧
    switch (s_nGlobalFrame)
    {
    case 0:
        return CSpriteConfig::COIN_SPIN1;
    case 1:
        return CSpriteConfig::COIN_SPIN2;
    case 2:
        return CSpriteConfig::COIN_SPIN3;
    case 3:
        return CSpriteConfig::COIN_SPIN4;
    default:
        return CSpriteConfig::COIN_SPIN1;
    }
}