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
    if (!m_bVisible) return;

    TRACE(_T("绘制砖块 - 屏幕位置: (%d, %d), 世界位置: (%d, %d)\n"),
        screenX, screenY, m_nX, m_nY);

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(_T("TilesetMain"));

    if (!pBitmap) {
        TRACE(_T("错误: TilesetMain 贴图未找到!\n"));
        // 使用纯色绘制作为备用
        CBrush brush(RGB(255, 0, 0));
        pDC->FillRect(CRect(screenX, screenY, screenX + m_nWidth, screenY + m_nHeight), &brush);
        return;
    }

    SSpriteCoord spriteCoord;

    // 根据砖块类型选择精灵
    switch (m_Type)
    {
    case NORMAL:
        spriteCoord = CSpriteConfig::BRICK_NORMAL;
        break;
    case QUESTION:
        if (!m_bIsEmpty) {
            spriteCoord = CSpriteConfig::BRICK_QUESTION;
        }
        else {
            spriteCoord = CSpriteConfig::BRICK_HARD;
        }
        break;
    case HARD:
        spriteCoord = CSpriteConfig::BRICK_HARD;
        break;
    }

    TRACE(_T("使用精灵坐标: (%d, %d), 尺寸: (%d, %d)\n"),
        spriteCoord.x, spriteCoord.y, spriteCoord.width, spriteCoord.height);

    // 关键：确保使用正确的源尺寸和目标尺寸
    CSpriteRenderer::DrawSprite(pDC, pBitmap,
        screenX, screenY,                    // 目标位置
        CGameConfig::BRICK_WIDTH, CGameConfig::BRICK_HEIGHT, // 目标尺寸
        spriteCoord.x, spriteCoord.y,        // 源位置
        spriteCoord.width, spriteCoord.height, // 源尺寸
        TRUE);
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