// Game/Objects/GameObject.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "GameObject.h"

// 构造函数
CGameObject::CGameObject()
{
    m_nX = 0;
    m_nY = 0;
    m_nWidth = 0;
    m_nHeight = 0;
    m_bVisible = TRUE;
    m_bActive = TRUE;
}

// 带参数的构造函数
CGameObject::CGameObject(int x, int y, int width, int height)
{
    m_nX = x;
    m_nY = y;
    m_nWidth = width;
    m_nHeight = height;
    m_bVisible = TRUE;
    m_bActive = TRUE;
}

// 析构函数
CGameObject::~CGameObject()
{
    // 基类没有需要特殊清理的资源
}

// 碰撞检测 - 与另一个游戏对象
BOOL CGameObject::CheckCollision(const CGameObject* other) const
{
    if (!other || !m_bActive || !other->m_bActive)
        return FALSE;

    CRect rect1 = GetRect();
    CRect rect2 = other->GetRect();

    return rect1.IntersectRect(&rect1, &rect2);
}

/// 碰撞检测 - 与矩形
BOOL CGameObject::CheckCollision(const CRect& rect) const
{
    if (!m_bActive)
        return FALSE;

    CRect myRect = GetRect();

    // 直接检查两个矩形是否相交
    // 如果 rect 在 myRect 的左边
    if (rect.right < myRect.left)
        return FALSE;

    // 如果 rect 在 myRect 的右边  
    if (rect.left > myRect.right)
        return FALSE;

    // 如果 rect 在 myRect 的上边
    if (rect.bottom < myRect.top)
        return FALSE;

    // 如果 rect 在 myRect 的下边
    if (rect.top > myRect.bottom)
        return FALSE;

    // 所有边界检查都通过，说明有碰撞
    return TRUE;
}