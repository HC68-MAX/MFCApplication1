// Game/Core/GameState.cpp
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