#pragma once
// Game/Objects/GameObject.h

#include <afxwin.h>

class CGameObject
{
public:
    // 构造函数和析构函数
    CGameObject();
    CGameObject(int x, int y, int width, int height);
    virtual ~CGameObject();

    // 虚函数 - 子类必须实现
    virtual void Update(float deltaTime) = 0;  // 纯虚函数
    virtual void Draw(CDC* pDC) = 0;           // 纯虚函数

    // 通用方法
    virtual void SetPosition(int x, int y) { m_nX = x; m_nY = y; }
    virtual void SetSize(int width, int height) { m_nWidth = width; m_nHeight = height; }

    // 获取位置和大小
    CRect GetRect() const { return CRect(m_nX, m_nY, m_nX + m_nWidth, m_nY + m_nHeight); }
    int GetX() const { return m_nX; }
    int GetY() const { return m_nY; }
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }

    // 碰撞检测
    BOOL CheckCollision(const CGameObject* other) const;
    BOOL CheckCollision(const CRect& rect) const;

    // 状态管理
    BOOL IsVisible() const { return m_bVisible; }
    void SetVisible(BOOL visible) { m_bVisible = visible; }

    BOOL IsActive() const { return m_bActive; }
    void SetActive(BOOL active) { m_bActive = active; }

protected:
    // 位置和大小
    int m_nX, m_nY;
    int m_nWidth, m_nHeight;

    // 状态标志
    BOOL m_bVisible;
    BOOL m_bActive;
};