// Game/Core/SpriteRenderer.cpp
#include <afxwin.h>
#include "SpriteRenderer.h"

// 新增方法：支持分别指定源和目标尺寸
  // 保持原有方法（不修改签名）
void CSpriteRenderer::DrawSprite(CDC* pDC, CBitmap* pBitmap,
    int destX, int destY, int destWidth, int destHeight,
    int srcX, int srcY, int srcWidth, int srcHeight,
    BOOL transparent, COLORREF transparentColor)
{
    if (!pBitmap || !pDC)
    {
        TRACE(_T("DrawSprite: 无效的DC或位图\n"));
        return;
    }

    // 创建内存DC
    CDC memDC;
    if (!memDC.CreateCompatibleDC(pDC))
    {
        TRACE(_T("DrawSprite: 无法创建内存DC\n"));
        return;
    }

    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

    if (transparent)
    {
        // 使用 TransparentBlt 进行透明绘制，支持拉伸
        pDC->TransparentBlt(destX, destY, destWidth, destHeight,
            &memDC, srcX, srcY, srcWidth, srcHeight,
            transparentColor);
    }
    else
    {
        // 使用 StretchBlt 进行拉伸绘制
        pDC->StretchBlt(destX, destY, destWidth, destHeight,
            &memDC, srcX, srcY, srcWidth, srcHeight, SRCCOPY);
    }

    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();
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