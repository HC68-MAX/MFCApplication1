// Game/Core/TextureManager.cpp
#include "TextureManager.h"

CTextureManager& CTextureManager::GetInstance()
{
    static CTextureManager instance;
    return instance;
}

// 绘制精灵
void CTextureManager::DrawSprite(CDC* pDC, const SSpriteCoord& spriteCoord,
    int screenX, int screenY,
    const CString& spritesheetName)
{
    if (!pDC) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(spritesheetName);

    if (pBitmap)
    {
        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
            spriteCoord.x, spriteCoord.y,
            spriteCoord.width, spriteCoord.height, TRUE);
    }
    else
    {
        // 备用：绘制彩色矩形
        pDC->FillSolidRect(screenX, screenY,
            spriteCoord.width, spriteCoord.height,
            RGB(255, 0, 255)); // 品红色表示贴图缺失
    }
}

// 绘制缩放后的精灵
void CTextureManager::DrawSpriteScaled(CDC* pDC, const SSpriteCoord& spriteCoord,
    int screenX, int screenY, int destWidth, int destHeight,
    const CString& spritesheetName)
{
    if (!pDC) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(spritesheetName);

    if (pBitmap)
    {
        // 注意：这里需要扩展SpriteRenderer来支持缩放
        // 暂时使用普通绘制
        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
            spriteCoord.x, spriteCoord.y,
            destWidth, destHeight, TRUE);
    }
    else
    {
        pDC->FillSolidRect(screenX, screenY, destWidth, destHeight, RGB(255, 0, 255));
    }
}

// 自动选择精灵表并绘制
void CTextureManager::DrawSpriteAuto(CDC* pDC, const SSpriteCoord& spriteCoord,
    int screenX, int screenY)
{
    CString spritesheetName = CSpriteConfig::GetSpritesheetForSprite(spriteCoord);
    DrawSprite(pDC, spriteCoord, screenX, screenY, spritesheetName);
}