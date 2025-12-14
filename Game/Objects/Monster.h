#pragma once
// Game/Objects/Monster.h
#include "GameObject.h"
#include <vector>
#include "../Core/SpriteConfig.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/GameConfig.h"
#include <vector>
class CMonster : public CGameObject
{
public:
    CMonster(int x, int y);
    virtual ~CMonster();

    virtual void Update(float deltaTime) override;
    virtual void Draw(CDC* pDC) override;
    void DrawAt(CDC* pDC, int screenX, int screenY);

    void CheckCollisions(const std::vector<CRect>& platforms);
    void OnCollisionWithMario(bool fromTop);

    BOOL IsDead() const { return m_bIsDead; }
    BOOL IsSquished() const { return m_bIsSquished; }

private:
    float m_fVelocityX;
    float m_fVelocityY;
    float m_fGravity;
    
    BOOL m_bIsDead;
    BOOL m_bIsSquished;
    float m_fSquishTimer;

    // 动画相关
    float m_fAnimTimer;
    int m_nAnimFrame;
    float m_fAnimSpeed;
};
