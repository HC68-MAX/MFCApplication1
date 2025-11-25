// Game/Objects/Pipe.cpp
#include <afxwin.h>  // 直接包含MFC头文件
#include "Pipe.h"

// 构造函数使用全局配置
CPipe::CPipe() : CGameObject(0, 0, CGameConfig::PIPE_WIDTH, CGameConfig::PIPE_HEIGHT)
{
}

// 带参数的构造函数
CPipe::CPipe(int x, int y, int height)
    : CGameObject(x, y, CGameConfig::PIPE_WIDTH, height)
{
}

// 析构函数
CPipe::~CPipe()
{
}

// 更新
void CPipe::Update(float deltaTime)
{
    // 水管是静态的，不需要更新
}

// 绘制 - 使用精灵渲染器
void CPipe::Draw(CDC* pDC)
{

}

     