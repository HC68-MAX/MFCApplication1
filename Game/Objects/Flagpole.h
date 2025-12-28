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

    void DrawWithSprite(CDC* pDC, int screenX, int screenY);
    // 触发旗子下落（碰撞后调用）
    // 绘制旗子（核心：在这里处理下落的Y偏移）
    void DrawFlag(CDC* pDC, int screenX, int screenY);
    // 检查是否触碰到旗杆
    BOOL CheckTouch(const CRect& rect) const;
    void TriggerFlagDown(){ m_bFlagDown = true; }
    BOOL IsWinTriggered() const { return m_bWinTriggered; }
private:
    // 旗帜的位置（相对于旗杆顶部）
    bool m_bFlagDown;  // 是否触发旗子下落（默认false）
    int m_nFlagY;      // 旗子下落的Y轴偏移量（初始为0，向下增加）
    bool m_bWinTriggered;   // 新增：是否触发了胜利
    const int m_nFallStep = 1;     // 每次绘制旗子的下落步长（数字越大下落越快）
    const int m_nMaxFallY = 128;   // 旗子最大下落距离（限制下落的最低位置）

};
