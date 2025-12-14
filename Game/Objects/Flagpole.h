#pragma once
#include "GameObject.h"
#include "../Core/GameConfig.h"
#include "../Core/SpriteConfig.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"

class CFlagpole : public CGameObject
{
public:
    CFlagpole();
    CFlagpole(int x, int y);
    virtual ~CFlagpole();

    virtual void Update(float deltaTime) override;
    virtual void Draw(CDC* pDC) override;
    void DrawWithSprite(CDC* pDC, int screenX, int screenY);

    // 检查是否触碰到旗杆
    BOOL CheckTouch(const CRect& rect) const;

private:
    // 旗帜的位置（相对于旗杆顶部）
    float m_fFlagY;
    BOOL m_bFlagDown; // 旗帜是否降下
};
