// Game/Objects/Brick.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Brick.h"

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

// 绘制
void CBrick::Draw(CDC* pDC)
{
    if (!m_bVisible) return;

    switch (m_Type)
    {
    case NORMAL:
        // 绘制普通砖块
        pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(180, 90, 40));
        pDC->Draw3dRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(120, 60, 20), RGB(220, 150, 100));
        break;

    case QUESTION:
        // 绘制问号砖块
        if (!m_bIsEmpty)
        {
            pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(255, 200, 0));
            pDC->Draw3dRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(180, 140, 0), RGB(255, 255, 100));

            // 绘制问号
            CString strQuestion = _T("?");
            pDC->SetTextColor(RGB(0, 0, 0));
            pDC->SetBkMode(TRANSPARENT);
            pDC->SetTextAlign(TA_CENTER);
            pDC->TextOut(m_nX + m_nWidth / 2, m_nY + m_nHeight / 4, strQuestion);
            pDC->SetTextAlign(TA_LEFT);
        }
        else
        {
            // 被顶过的问号砖块
            pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(120, 120, 120));
        }
        break;

    case HARD:
        // 绘制硬砖块
        pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(120, 120, 120));
        pDC->Draw3dRect(m_nX, m_nY, m_nWidth, m_nHeight, RGB(80, 80, 80), RGB(180, 180, 180));
        break;
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