#pragma once
// Game/Map/TileMap.h
#pragma once

#include <afxwin.h>
#include <vector>
#include <string>

#include "../Objects/Brick.h"
#include "../Objects/Pipe.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"
#include "../Objects/Coin.h"  
#include "../Core/GameState.h"   
#include "../Objects/Mario.h"
#include "../Objects/Monster.h"

struct Tile
{
    int tileID;     // ��ƬID
    int x, y;       // λ��
    BOOL solid;     // �Ƿ����ײ
    CString type;   // ���ͣ�ground, brick, question, etc.��
};

class CTileMap
{
public:
    CTileMap();
    ~CTileMap();

    // ��ͼ����
    BOOL LoadMap(int width, int height, int tileSize);
    // ����������ָ���ؿ�
    BOOL LoadLevel(int levelNumber); 
    // ������Ƭ
    void SetTile(int x, int y, int tileID, BOOL solid = FALSE, const CString& type = _T(""));
    // �����������
    void AddBrick(int x, int y, CBrick::BrickType type = CBrick::NORMAL);
    void AddPipe(int x, int y, int height = CGameConfig::PIPE_HEIGHT);  
    void AddCoin(int x, int y);
    void AddMonster(int x, int y); // ���������ӹ���
    void AddMonsterAtTile(int tileX, int tileY); // ����������Ƭ�������ӹ���
    void ClearObjects();  // ������ж���

    // ���������� Mario ָ��
    void SetMario(CMario* pMario) { m_pMario = pMario; }
    // ��Ⱦ
    void Draw(CDC* pDC, int offsetX = 0, int offsetY = 0);

    // ��ȡ���о���
    CRect GetTileRect(int tileX, int tileY) const;
    // ��ȡ�������
    std::vector<CRect> GetSolidTileRects() const;

    // ��ͼ��Ϣ
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }
    int GetTileSize() const { return m_nTileSize; }
   
    // ��Ҹ���
    void ClearCoins();
    void UpdateCoins(float deltaTime);
    void UpdateMonsters(float deltaTime); // ���������¹���

    // �����ײ���
    BOOL CheckCoinCollisions(const CRect& rect);
    // ש����ײ���
    BOOL CheckBrickCollisions(const CRect& rect);
    // ����ʺ�ש����ײ
    BOOL CheckQuestionBlockHit(const CRect& rect, BOOL isMovingUp);
    // ��������ײ
    void CheckMonsterCollisions(CMario* pMario);

    void RemoveCoin(int index);
    // 新增：获取马里奥初始位置
    int GetMarioStartX() const { return m_MarioStartX; }
    int GetMarioStartY() const { return m_MarioStartY; }
private:

    int m_nWidth;           // ��ͼ���ȣ���Ƭ����
    int m_nHeight;          // ��ͼ�߶ȣ���Ƭ����
    int m_nTileSize;        // ��Ƭ�ߴ磨���أ�

    // ����³�ʼλ��
    int m_MarioStartX;
    int m_MarioStartY;

    std::vector<std::vector<Tile>> m_Tiles;  // ��Ƭ����
    std::vector<CBrick> m_Bricks;
    std::vector<CPipe> m_Pipes;
    std::vector<CCoin> m_Coins;
    std::vector<CMonster> m_Monsters; // �����������б�
    CMario* m_pMario = nullptr;  // ָ�� Mario �����ָ��
    // �������ؿ����ط���
    BOOL LoadLevel1();
    BOOL LoadLevel2();
    BOOL LoadLevel3();
};