#pragma once
// Game/Core/TextureManager.h

#include <afxwin.h>
#include "SpriteConfig.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

class CTextureManager
{
public:
    static CTextureManager& GetInstance();

    // 绘制精灵
    static void DrawSprite(CDC* pDC, const SSpriteCoord& spriteCoord,
        int screenX, int screenY,
        const CString& spritesheetName = _T("SpritesheetMain"));

    // 绘制缩放后的精灵
    static void DrawSpriteScaled(CDC* pDC, const SSpriteCoord& spriteCoord,
        int screenX, int screenY, int destWidth, int destHeight,
        const CString& spritesheetName = _T("SpritesheetMain"));

    // 自动选择精灵表并绘制
    static void DrawSpriteAuto(CDC* pDC, const SSpriteCoord& spriteCoord,
        int screenX, int screenY);

private:
    CTextureManager() = default;
    ~CTextureManager() = default;
};