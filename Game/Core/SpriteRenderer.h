#pragma once
// Game/Core/SpriteRenderer.h

#include <afxwin.h>

class CSpriteRenderer
{
public:
    // 绘制精灵
   // 修改现有方法或添加新方法，支持分别指定源和目标尺寸
       // 修改现有方法或添加新方法，支持分别指定源和目标尺寸
    static void DrawSprite(CDC* pDC, CBitmap* pBitmap,
        int destX, int destY, int destWidth, int destHeight,
        int srcX, int srcY, int srcWidth, int srcHeight,
        BOOL transparent = TRUE, COLORREF transparentColor = RGB(255, 0, 255));
    // 获取位图尺寸
    static BOOL GetBitmapSize(CBitmap* pBitmap, CSize& size);
};