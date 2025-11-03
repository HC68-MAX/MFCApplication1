#pragma once
// Game/Objects/Pipe.h
#pragma once

#include "GameObject.h"
#include "../Core/GameConfig.h" 
#include "../Core/SpriteConfig.h"
class CPipe : public CGameObject
{
public:
    CPipe();
    CPipe(int x, int y, int height = CGameConfig::PIPE_HEIGHT);
    virtual ~CPipe();

    // 重写基类虚函数
    void Update(float deltaTime) override;
    void Draw(CDC* pDC) override;

    // 新增：使用精灵渲染器绘制
    void DrawWithSprite(CDC* pDC, int screenX, int screenY);

private:
    
};