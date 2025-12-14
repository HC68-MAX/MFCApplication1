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
    int tileID;     // 瓦片ID
    int x, y;       // 位置
    BOOL solid;     // 是否可碰撞
    CString type;   // 类型（ground, brick, question, etc.）
};

class CTileMap
{
public:
    CTileMap();
    ~CTileMap();

    // 地图操作
    BOOL LoadMap(int width, int height, int tileSize);
    // 新增：加载指定关卡
    BOOL LoadLevel(int levelNumber); 
    // 生成瓦片
    void SetTile(int x, int y, int tileID, BOOL solid = FALSE, const CString& type = _T(""));
    // 独立对象管理
    void AddBrick(int x, int y, CBrick::BrickType type = CBrick::NORMAL);
    void AddPipe(int x, int y, int height = CGameConfig::PIPE_HEIGHT);  
    void AddCoin(int x, int y);
    void AddMonster(int x, int y); // 新增：添加怪物
    void AddMonsterAtTile(int tileX, int tileY); // 新增：在瓦片坐标添加怪物
    void ClearObjects();  // 清空所有对象

    // 新增：设置 Mario 指针
    void SetMario(CMario* pMario) { m_pMario = pMario; }
    // 渲染
    void Draw(CDC* pDC, int offsetX = 0, int offsetY = 0);

    // 获取所有矩形
    CRect GetTileRect(int tileX, int tileY) const;
    // 获取固体矩形
    std::vector<CRect> GetSolidTileRects() const;

    // 地图信息
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }
    int GetTileSize() const { return m_nTileSize; }
   
    // 金币更新
    void ClearCoins();
    void UpdateCoins(float deltaTime);
    void UpdateMonsters(float deltaTime); // 新增：更新怪物

    // 金币碰撞检测
    BOOL CheckCoinCollisions(const CRect& rect);
    // 砖块碰撞检测
    BOOL CheckBrickCollisions(const CRect& rect);
    // 检查问号砖块碰撞
    BOOL CheckQuestionBlockHit(const CRect& rect, BOOL isMovingUp);
    // 检查怪物碰撞
    void CheckMonsterCollisions(CMario* pMario);

    void RemoveCoin(int index);
private:

    int m_nWidth;           // 地图宽度（瓦片数）
    int m_nHeight;          // 地图高度（瓦片数）
    int m_nTileSize;        // 瓦片尺寸（像素）

    // 马里奥初始位置
    int m_MarioStartX;
    int m_MarioStartY;

    std::vector<std::vector<Tile>> m_Tiles;  // 瓦片数据
    std::vector<CBrick> m_Bricks;
    std::vector<CPipe> m_Pipes;
    std::vector<CCoin> m_Coins;
    std::vector<CMonster> m_Monsters; // 新增：怪物列表
    CMario* m_pMario = nullptr;  // 指向 Mario 对象的指针
    // 新增：关卡加载方法
    BOOL LoadLevel1();
    BOOL LoadLevel2();
    BOOL LoadLevel3();
};