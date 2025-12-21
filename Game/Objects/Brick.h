#pragma once
// Game/Objects/Brick.h

#include "GameObject.h"
#include "../Core/GameConfig.h"  
#include "../Core/ResourceManager.h"  
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"

class CBrick : public CGameObject
{
public:
    CBrick();
    CBrick(int x, int y, int width = CGameConfig::BRICK_WIDTH, int height = CGameConfig::BRICK_HEIGHT);
    virtual ~CBrick();

    // 新增：使用屏幕坐标绘制
    void DrawAt(CDC* pDC, int screenX, int screenY);

    // 碰撞相关方法
    BOOL CanBeHitFromBelow() const { return m_Type == QUESTION && !m_bIsEmpty; }
    void SetHitAnimation(BOOL hit) { m_bIsHit = hit; m_fHitTimer = 0; }
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

    // 道具相关
    BOOL ShouldSpawnItem() const { return m_bShouldSpawnItem; }
    void SetSpawnItem(BOOL spawn) { m_bShouldSpawnItem = spawn; }
    int GetSpawnItemType() const { return m_nSpawnItemType; }

private:
    BrickType m_Type;
    BOOL m_bIsEmpty;  // 问号砖块是否已被顶过

    // 动画相关
    BOOL m_bIsHit;
    float m_fHitTimer;
    int m_nOriginalY;

    // 道具生成
    BOOL m_bShouldSpawnItem;
    int m_nSpawnItemType; // 0=金币, 1=蘑菇, 2=火焰花等
};