// Game/Core/SpriteRenderer.cpp
#include <afxwin.h>
#include "SpriteRenderer.h"

// 新增方法：支持分别指定源和目标尺寸
  // 保持原有方法（不修改签名）
void CSpriteRenderer::DrawSprite(CDC* pDC, CBitmap* pBitmap,
    int destX, int destY, int destWidth, int destHeight,
    int srcX, int srcY, int srcWidth, int srcHeight,
   BOOL flipHorizontal,COLORREF transparentColor)
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

    if (flipHorizontal)
    {
        // 水平翻转：目标宽度设为负值，x坐标偏移到右侧起点
        TransparentBlt(
            pDC->m_hDC,
            destX + destWidth, destY,  // 翻转时从右侧开始绘制
            -destWidth, destHeight,    // 负宽度实现水平翻转
            memDC.m_hDC,
            srcX, srcY,
            srcWidth, srcHeight,
            transparentColor           // 透明色参数
        );
    }
    else
    {
        // 正常绘制（带透明处理）
        TransparentBlt(
            pDC->m_hDC,
            destX, destY,
            destWidth, destHeight,
            memDC.m_hDC,
            srcX, srcY,
            srcWidth, srcHeight,
            transparentColor           // 透明色参数
        );
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