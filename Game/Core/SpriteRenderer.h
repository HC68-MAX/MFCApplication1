#pragma once
// Game/Core/SpriteRenderer.h

#include <afxwin.h>

class CSpriteRenderer
{
public:
   /*
   CDC* pDC   文本上下文（不用动） CBitmap* pBitmap(图集，使用不同图集要注意）
    int destX, int destY,                贴图要贴在的位置
    int destWidth, int destHeight,       贴图要贴的大小
    int srcX, int srcY,                  贴图在图集的坐标
    int srcWidth, int srcHeight          贴图在图集需要裁剪的长宽
     BOOL flipHorizontal = FALSE         翻转处理
    */ 
    static void DrawSprite(CDC* pDC, CBitmap* pBitmap,
        int destX, int destY, int destWidth, int destHeight,
        int srcX, int srcY, int srcWidth, int srcHeight,
        BOOL flipHorizontal = FALSE, COLORREF transparentColor = RGB(135, 206, 235));
    // 获取位图尺寸
    static BOOL GetBitmapSize(CBitmap* pBitmap, CSize& size);
};