#pragma once
// Game/Objects/Pipe.h
#pragma once

#include "GameObject.h"
#include "../Core/GameConfig.h"  
class CPipe : public CGameObject
{
public:
    CPipe();
    CPipe(int x, int y, int height = CGameConfig::PIPE_HEIGHT);
    virtual ~CPipe();

    // 重写基类虚函数
    void Update(float deltaTime) override;
    void Draw(CDC* pDC) override;

private:
    void DrawPipeSegment(CDC* pDC, int x, int y, int width, int height);
};