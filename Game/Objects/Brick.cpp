// Game/Objects/Brick.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Brick.h"
#include "GameObject.h"
#include "..\Core\SpriteConfig.h"
// 构造函数
CBrick::CBrick() : CGameObject(0, 0, CGameConfig::BRICK_WIDTH, CGameConfig::BRICK_HEIGHT)
{
    m_Type = NORMAL;
    m_bIsEmpty = FALSE;
    m_bIsHit = FALSE;
    m_nHitTimer = 0.0f;
    m_nOriginalY = m_nY;
}

// 带参数的构造函数
CBrick::CBrick(int x, int y, int width, int height)
    : CGameObject(x, y, width, height)
{
    m_Type = NORMAL;
    m_bIsEmpty = FALSE;
    m_bIsHit = FALSE;
    m_nHitTimer = 0.0f;
    m_nOriginalY = m_nY;
}

// 析构函数
CBrick::~CBrick()
{
}

// 更新
void CBrick::Update(float deltaTime)
{
    // 砖块是静态的，不需要更新
    if (m_bIsHit)
    {
        UpdateHitAnimation(deltaTime);
    }
}

// 新增：更新击中动画
void CBrick::UpdateHitAnimation(float deltaTime)
{
    m_nHitTimer += deltaTime;

    if (m_nHitTimer < 0.1f) // 上升阶段
    {
        m_nY = m_nOriginalY - 5; // 向上移动5像素
    }
    else if (m_nHitTimer < 0.2f) // 下降阶段
    {
        m_nY = m_nOriginalY; // 回到原位
    }
    else // 动画结束
    {
        m_bIsHit = FALSE;
        m_nHitTimer = 0.0f;

        // 如果是问号砖块并且是第一次被顶，设置为空
        if (m_Type == QUESTION && !m_bIsEmpty)
        {
            m_bIsEmpty = TRUE;
        }
    }
}

// 绘制 - 使用精灵渲染器
void CBrick::Draw(CDC* pDC)
{
    DrawWithSprite(pDC, m_nX, m_nY);
}

// 新增：使用精灵渲染器绘制
void CBrick::DrawWithSprite(CDC* pDC, int screenX, int screenY)
{
   // if (!m_bVisible) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(_T("TilesetMain"));
    TRACE(_T("Brick构造函数被调用 - 位置: (%d, %d)\n"), screenX + m_nWidth, screenY + m_nHeight);
    if (!pBitmap) {
        // 备用绘制
        CBrush brush(RGB(255, 0, 0)); // 红色作为备用
        pDC->FillRect(CRect(screenX, screenY, screenX + m_nWidth, screenY + m_nHeight), &brush);
        return;
    }

    // 硬编码测试：假设我们想要从(0,0)位置裁剪16x16，然后绘制到32x32
    int srcX = 0;   // 贴图集中的X坐标
    int srcY = 0;   // 贴图集中的Y坐标  
    int srcWidth = 16;  // 要裁剪的宽度
    int srcHeight = 16; // 要裁剪的高度

    int destX = screenX;     // 屏幕X坐标
    int destY = screenY;     // 屏幕Y坐标
    int destWidth = 32;      // 绘制宽度
    int destHeight = 32;     // 绘制高度

    // 使用StretchBlt直接测试
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

    // 直接拉伸绘制
    pDC->StretchBlt(destX, destY, destWidth, destHeight,
        &memDC, srcX, srcY, srcWidth, srcHeight,
        SRCCOPY);
    TRACE(_T("Brick Draw - Screen: (%d, %d), Size: (%d, %d)\n"),
        screenX, screenY, m_nWidth, m_nHeight);

    SSpriteCoord spriteCoord = CSpriteConfig::BRICK_NORMAL;
    TRACE(_T("Sprite Coord - Source: (%d, %d), Size: (%d, %d)\n"),
        spriteCoord.x, spriteCoord.y, spriteCoord.width, spriteCoord.height);

    memDC.SelectObject(pOldBitmap);
}
// 被从下方撞击
void CBrick::OnHitFromBelow()
{
    if (m_Type == QUESTION && !m_bIsEmpty)
    {
        SetHitAnimation(TRUE);
        // 注意：不再直接设置为空，而是在动画结束后设置
    }
    else if (m_Type == NORMAL)
    {
        // 普通砖块也可以有被顶的动画效果
        SetHitAnimation(TRUE);
    }
    // 硬砖块不会被顶动
}