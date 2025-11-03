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

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = nullptr;
    SSpriteCoord spriteCoord;

    // 根据砖块类型选择贴图和精灵坐标
    switch (m_Type)
    {
    case NORMAL:
        pBitmap = resMgr.GetBitmap(_T("Brick"));
        spriteCoord = CSpriteConfig::BRICK_NORMAL;
        break;

    case QUESTION:
        if (!m_bIsEmpty)
        {
            pBitmap = resMgr.GetBitmap(_T("QuestionBlock"));
            // 根据是否被击中显示不同的问号砖块状态
            if (m_bIsHit && m_nHitTimer < 0.1f)
                spriteCoord = CSpriteConfig::BRICK_QUESTION_HIT;
            else
                spriteCoord = CSpriteConfig::BRICK_QUESTION;
        }
        else
        {
            // 被顶过的问号砖块显示为硬砖块
            pBitmap = resMgr.GetBitmap(_T("HardBrick"));
            spriteCoord = CSpriteConfig::BRICK_HARD;
        }
        break;

    case HARD:
        pBitmap = resMgr.GetBitmap(_T("HardBrick"));
        spriteCoord = CSpriteConfig::BRICK_HARD;
        break;
    }

    if (pBitmap)
    {
        // 使用精灵渲染器绘制
        CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
            spriteCoord.x, spriteCoord.y,
            spriteCoord.width, spriteCoord.height, TRUE);
    }
    else
    {
        // 备用：如果没有贴图，使用颜色绘制
        COLORREF color = RGB(128, 128, 128);
        switch (m_Type)
        {
        case NORMAL: color = RGB(180, 90, 40); break;
        case QUESTION: color = m_bIsEmpty ? RGB(120, 120, 120) : RGB(255, 200, 0); break;
        case HARD: color = RGB(120, 120, 120); break;
        }
        pDC->FillSolidRect(screenX, screenY, m_nWidth, m_nHeight, color);
        pDC->Draw3dRect(screenX, screenY, m_nWidth, m_nHeight,
            RGB(80, 80, 80), RGB(200, 200, 200));
    }
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