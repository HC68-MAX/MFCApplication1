#pragma once
// Game/Objects/Brick.h

#include "GameObject.h"
#include "../Core/GameConfig.h"  
#include "../Core/ResourceManager.h"  
#include "../Core/SpriteRenderer.h"

class CBrick : public CGameObject
{
public:
    CBrick();
    CBrick(int x, int y, int width = CGameConfig::BRICK_WIDTH, int height = CGameConfig::BRICK_HEIGHT);
    virtual ~CBrick();

    // 重写基类虚函数
    void Update(float deltaTime) override;
    void Draw(CDC* pDC) override;
    // 新增：使用精灵渲染器绘制
    void DrawWithSprite(CDC* pDC, int screenX, int screenY);

    // 新增碰撞相关方法
    BOOL CanBeHitFromBelow() const { return m_Type == QUESTION && !m_bIsEmpty; }
    void SetHitAnimation(BOOL hit) { m_bIsHit = hit; m_nHitTimer = 0; }
    BOOL IsHitAnimation() const { return m_bIsHit; }
    void UpdateHitAnimation(float deltaTime);

    // 砖块类型
    enum BrickType
    {
        NORMAL,     // 普通砖块
        QUESTION,   // 问号砖块
        HARD        // 硬砖块
    };

    void SetBrickType(BrickType type) { m_Type = type; }
    BrickType GetBrickType() const { return m_Type; }

    // 碰撞响应
    void OnHitFromBelow();

private:
    BrickType m_Type;
    BOOL m_bIsEmpty;  // 问号砖块是否已被顶过
    // 新增动画相关
    BOOL m_bIsHit;
    float m_nHitTimer;
    int m_nOriginalY;
};