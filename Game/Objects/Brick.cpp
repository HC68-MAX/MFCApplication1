// Game/Objects/Brick.cpp
#include <afxwin.h>
#include "Brick.h"

// 构造函数
CBrick::CBrick()
    : CGameObject(0, 0, CGameConfig::BRICK_WIDTH, CGameConfig::BRICK_HEIGHT)
{
    m_Type = NORMAL;
    m_bIsEmpty = FALSE;
    m_bIsHit = FALSE;
    m_fHitTimer = 0.0f;
    m_nOriginalY = m_nY;
    m_bShouldSpawnItem = FALSE;
    m_nSpawnItemType = 0;
}

// 带参数的构造函数
CBrick::CBrick(int x, int y, int width, int height)
    : CGameObject(x, y, width, height)
{
    m_Type = NORMAL;
    m_bIsEmpty = FALSE;
    m_bIsHit = FALSE;
    m_fHitTimer = 0.0f;
    m_nOriginalY = m_nY;
    m_bShouldSpawnItem = FALSE;
    m_nSpawnItemType = 0;
}

// 析构函数
CBrick::~CBrick()
{
}
// 更新击中动画
void CBrick::UpdateHitAnimation(float deltaTime)
{
    m_fHitTimer += deltaTime;

    if (m_fHitTimer < 0.1f) // 上升阶段
    {
        m_nY = m_nOriginalY - 5; // 向上移动5像素
    }
    else if (m_fHitTimer < 0.2f) // 下降阶段
    {
        m_nY = m_nOriginalY; // 回到原位
    }
    else // 动画结束
    {
        m_bIsHit = FALSE;
        m_fHitTimer = 0.0f;

        // 如果是问号砖块并且是第一次被顶，设置为空
        if (m_Type == QUESTION && !m_bIsEmpty)
        {
            m_bIsEmpty = TRUE;
            m_bShouldSpawnItem = TRUE; // 标记需要生成道具
        }
    }
}
// 使用精灵渲染器绘制
void CBrick::DrawAt(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return;

    TRACE(_T("绘制砖块 - 屏幕位置: (%d, %d), 世界位置: (%d, %d), 类型: %d, 是否为空: %d\n"),
        screenX, screenY, m_nX, m_nY, m_Type, m_bIsEmpty);

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::TILESET_MAIN);

    if (!pBitmap) {
        TRACE(_T("错误: TilesetMain 贴图未找到!\n"));
        // 使用纯色绘制作为备用
        COLORREF color = RGB(255, 0, 0);
        switch (m_Type)
        {
        case NORMAL: color = RGB(180, 80, 0); break;
        case QUESTION: color = m_bIsEmpty ? RGB(128, 128, 128) : RGB(255, 220, 0); break;
        case HARD: color = RGB(150, 150, 150); break;
        }

        CBrush brush(color);
        pDC->FillRect(CRect(screenX, screenY, screenX + m_nWidth, screenY + m_nHeight), &brush);
        return;
    }

    SSpriteCoord spriteCoord;

    // 根据砖块类型和状态选择精灵
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
            spriteCoord = CSpriteConfig::BRICK_HARD; // 空问号砖块显示为硬砖块
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
        m_nWidth, m_nHeight,                 // 目标尺寸
        spriteCoord.x, spriteCoord.y,        // 源位置
        spriteCoord.width, spriteCoord.height); // 源尺寸
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