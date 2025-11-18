#pragma once
// Game/Objects/Coin.h

#include "GameObject.h"
#include "../Core/GameConfig.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"

class CCoin : public CGameObject
{
public:
    CCoin();
    CCoin(int x, int y);
    virtual ~CCoin();

    // 重写基类虚函数
    virtual void Update(float deltaTime) override;
    virtual void Draw(CDC* pDC) override;

    // 新增：使用屏幕坐标绘制
    void DrawAt(CDC* pDC, int screenX, int screenY);

    // 金币状态
    void Collect() { m_bCollected = TRUE; m_bActive = FALSE; }
    BOOL IsCollected() const { return m_bCollected; }

    // 动画控制 - 现在使用静态计时器
    static void UpdateGlobalAnimation(float deltaTime);
    SSpriteCoord GetCurrentSpriteCoord() const;

private:
    BOOL m_bCollected;

    // 静态动画参数 - 所有金币共享
    static int s_nGlobalFrame;
    static float s_fGlobalSpinTimer;

  
};