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

    // 创建源图像内存DC（用于透明处理）
    CDC srcMemDC;
    if (!srcMemDC.CreateCompatibleDC(pDC))
    {
        TRACE(_T("DrawSprite: 无法创建源内存DC\n"));
        return;
    }
    CBitmap* pOldSrcBitmap = srcMemDC.SelectObject(pBitmap);

    // 如果需要翻转，创建临时DC用于翻转处理
    if (flipHorizontal)
    {
        // 创建临时DC和位图（存储翻转前的透明图像）
        CDC tempDC;
        CBitmap tempBmp;
        if (!tempDC.CreateCompatibleDC(pDC) ||
            !tempBmp.CreateCompatibleBitmap(pDC, destWidth, destHeight))
        {
            TRACE(_T("DrawSprite: 无法创建临时DC/位图\n"));
            srcMemDC.SelectObject(pOldSrcBitmap);
            return;
        }
        CBitmap* pOldTempBmp = tempDC.SelectObject(&tempBmp);

        tempDC.FillSolidRect(0, 0, destWidth, destHeight, RGB(135, 206, 235));
        // 先将透明图像绘制到临时DC（不翻转）
        TransparentBlt(
            tempDC.m_hDC,
            0, 0,                  // 临时DC中的位置（左上角）
            destWidth, destHeight, // 临时DC中的尺寸
            srcMemDC.m_hDC,
            srcX, srcY,
            srcWidth, srcHeight,
            transparentColor
        );

        // 水平翻转复制到目标DC：通过BitBlt的镜像模式实现
        pDC->StretchBlt(
            destX, destY,                  // 目标位置
            destWidth, destHeight,         // 目标尺寸
            &tempDC,
            destWidth, 0,                  // 源起点（临时DC右侧）
            -destWidth, destHeight,        // 负宽度实现翻转
            SRCCOPY                        // 绘制模式
        );
        // 清理临时资源
        tempDC.SelectObject(pOldTempBmp);
        tempBmp.DeleteObject();
        tempDC.DeleteDC();
    }
    else
    {
        // 不翻转：直接绘制透明图像
        TransparentBlt(
            pDC->m_hDC,
            destX, destY,
            destWidth, destHeight,
            srcMemDC.m_hDC,
            srcX, srcY,
            srcWidth, srcHeight,
            transparentColor
        );
    }

    // 清理源DC资源
    srcMemDC.SelectObject(pOldSrcBitmap);
    srcMemDC.DeleteDC();
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