#pragma once
// Game/Core/GameState.h

class CGameState
{
public:
    static CGameState& GetInstance();

    void Reset();
    void AddScore(int points);
    void AddCoin();
    void SetLives(int lives) { m_nLives = lives; }
    void LoseLife();  // 新增：减少生命值
    int GetScore() const { return m_nScore; }
    int GetCoins() const { return m_nCoins; }
    int GetLives() const { return m_nLives; }

private:
    CGameState();

    int m_nScore;
    int m_nCoins;
    int m_nLives;
};