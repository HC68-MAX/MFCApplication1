// Game/Core/GameState.cpp
#include <afxwin.h>
#include "GameState.h"

CGameState& CGameState::GetInstance()
{
    static CGameState instance;
    return instance;
}

CGameState::CGameState()
{
    Reset();
}

void CGameState::Reset()
{
    m_nScore = 0;
    m_nCoins = 0;
    m_nLives = 3;
}

void CGameState::AddScore(int points)
{
    m_nScore += points;
}

void CGameState::AddCoin()
{
    m_nCoins++;
    AddScore(200); // 每个金币200分

    // 每100个金币增加一条命
    if (m_nCoins >= 100)
    {
        m_nCoins -= 100;
        m_nLives++;
    }
}
// 新增：减少生命值
void CGameState::LoseLife()
{
    if (m_nLives > 0)
    {
        m_nLives--;

        // 可以选择在这里重置一些状态，比如分数或金币
        // 但通常只减少生命值，保持其他状态不变
        TRACE(_T("生命值减少，剩余: %d\n"), m_nLives);
    }

    // 如果生命值为0，游戏结束逻辑由调用者处理
}