#pragma once
// Game/Core/SpriteRenderer.h

#include <afxwin.h>

class CSpriteRenderer
{
public:
    // 绘制精灵
    static void DrawSprite(CDC* pDC, CBitmap* pBitmap, int x, int y,
        int srcX = 0, int srcY = 0,
        int width = -1, int height = -1,
        BOOL transparent = TRUE, COLORREF transparentColor = RGB(255, 0, 255));

    // 绘制精灵的一部分（精灵表）
    static void DrawSpriteFrame(CDC* pDC, CBitmap* pBitmap, int x, int y,
        int frameX, int frameY, int frameWidth, int frameHeight,
        BOOL transparent = TRUE, COLORREF transparentColor = RGB(255, 0, 255));

    // 获取位图尺寸
    static BOOL GetBitmapSize(CBitmap* pBitmap, CSize& size);
};