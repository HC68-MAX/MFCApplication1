// Game/Core/SpriteRenderer.cpp
#include <afxwin.h>
#include "SpriteRenderer.h"

void CSpriteRenderer::DrawSprite(CDC* pDC, CBitmap* pBitmap, int x, int y,
    int srcX, int srcY, int width, int height,
    BOOL transparent, COLORREF transparentColor)
{
    if (!pBitmap || !pDC) return;

    // 创建内存DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

    // 获取位图尺寸
    BITMAP bmpInfo;
    pBitmap->GetBitmap(&bmpInfo);

    if (width == -1) width = bmpInfo.bmWidth;
    if (height == -1) height = bmpInfo.bmHeight;

    if (transparent)
    {
        // 透明绘制
        pDC->TransparentBlt(x, y, width, height,
            &memDC, srcX, srcY, width, height,
            transparentColor);
    }
    else
    {
        // 普通绘制
        pDC->BitBlt(x, y, width, height, &memDC, srcX, srcY, SRCCOPY);
    }

    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();
}

void CSpriteRenderer::DrawSpriteFrame(CDC* pDC, CBitmap* pBitmap, int x, int y,
    int frameX, int frameY, int frameWidth, int frameHeight,
    BOOL transparent, COLORREF transparentColor)
{
    DrawSprite(pDC, pBitmap, x, y, frameX, frameY, frameWidth, frameHeight, transparent, transparentColor);
}

BOOL CSpriteRenderer::GetBitmapSize(CBitmap* pBitmap, CSize& size)
{
    if (!pBitmap) return FALSE;

    BITMAP bmpInfo;
    if (pBitmap->GetBitmap(&bmpInfo))
    {
        size.cx = bmpInfo.bmWidth;
        size.cy = bmpInfo.bmHeight;
        return TRUE;
    }
    return FALSE;
}