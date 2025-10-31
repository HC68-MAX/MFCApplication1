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

// 绘制
void CPipe::Draw(CDC* pDC)
{
    if (!m_bVisible) return;

    // 水管颜色
    COLORREF pipeGreen = RGB(0, 150, 0);
    COLORREF pipeDarkGreen = RGB(0, 100, 0);
    COLORREF pipeLightGreen = RGB(0, 200, 0);

    // 绘制水管主体
    pDC->FillSolidRect(m_nX, m_nY, m_nWidth, m_nHeight, pipeGreen);

    // 绘制水管纹理
    for (int i = 0; i < m_nHeight; i += 10)
    {
        pDC->FillSolidRect(m_nX, m_nY + i, 5, 5, pipeLightGreen);
        pDC->FillSolidRect(m_nX + m_nWidth - 5, m_nY + i, 5, 5, pipeDarkGreen);
    }

    // 绘制水管顶部
    pDC->FillSolidRect(m_nX - 5, m_nY, m_nWidth + 10, 15, pipeGreen);
    pDC->Draw3dRect(m_nX - 5, m_nY, m_nWidth + 10, 15, pipeDarkGreen, pipeLightGreen);
}