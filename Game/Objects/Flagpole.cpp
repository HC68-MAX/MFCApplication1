#include <afxwin.h>
#include "Flagpole.h"

CFlagpole::CFlagpole() : CGameObject(0, 0, CGameConfig::FLAGPOLE_WIDTH, CGameConfig::FLAGPOLE_HEIGHT)
{
    m_fFlagY = 0;
    m_bFlagDown = FALSE;
}

CFlagpole::CFlagpole(int x, int y) : CGameObject(x, y, CGameConfig::FLAGPOLE_WIDTH, CGameConfig::FLAGPOLE_HEIGHT)
{
    m_fFlagY = 0;
    m_bFlagDown = FALSE;
}

CFlagpole::~CFlagpole()
{
}

void CFlagpole::Update(float deltaTime)
{
    // 如果旗帜正在降下，更新旗帜位置
    if (m_bFlagDown && m_fFlagY < m_nHeight - CGameConfig::TILE_SIZE)
    {
        m_fFlagY += 100.0f * deltaTime; // 降旗速度
    }
}

void CFlagpole::Draw(CDC* pDC)
{
    // 这里的Draw通常不被直接调用，而是由TileMap调用DrawWithSprite
}

void CFlagpole::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::GetSpritesheetForTile(0)); // 假设在主图集�?

    int drawX = screenX;
    int drawY = screenY;

    // 绘制旗杆顶部
    CSpriteRenderer::DrawSprite(pDC, pBitmap, drawX, drawY, CGameConfig::TILE_SIZE, CGameConfig::TILE_SIZE,
        CSpriteConfig::FLAGPOLE_TOP.x, CSpriteConfig::FLAGPOLE_TOP.y, CSpriteConfig::FLAGPOLE_TOP.width, CSpriteConfig::FLAGPOLE_TOP.height);

    // 绘制旗杆主体
    for (int i = 1; i < m_nHeight / CGameConfig::TILE_SIZE; ++i)
    {
        CSpriteRenderer::DrawSprite(pDC, pBitmap, drawX, drawY + i * CGameConfig::TILE_SIZE, CGameConfig::TILE_SIZE, CGameConfig::TILE_SIZE,
            CSpriteConfig::FLAGPOLE_POLE.x, CSpriteConfig::FLAGPOLE_POLE.y, CSpriteConfig::FLAGPOLE_POLE.width, CSpriteConfig::FLAGPOLE_POLE.height);
    }

    // 绘制旗帜
    // 旗帜在旗杆左侧
    CSpriteRenderer::DrawSprite(pDC, pBitmap, drawX - CGameConfig::TILE_SIZE / 2, drawY + (int)m_fFlagY + CGameConfig::TILE_SIZE / 2, CGameConfig::TILE_SIZE, CGameConfig::TILE_SIZE,
        CSpriteConfig::FLAG.x, CSpriteConfig::FLAG.y, CSpriteConfig::FLAG.width, CSpriteConfig::FLAG.height);
}

BOOL CFlagpole::CheckTouch(const CRect& rect) const
{
    // 简单的矩形碰撞检测
    CRect flagpoleRect = GetRect();
    // 稍微缩小一点碰撞范围，只检测旗杆主体
    flagpoleRect.left += 10;
    flagpoleRect.right -= 10;
    
    CRect intersect;
    return intersect.IntersectRect(&flagpoleRect, &rect);
}
