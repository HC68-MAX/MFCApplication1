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
    BOOL LoadLevel(int levelNumber);  // 新增：加载指定关卡

    void SetTile(int x, int y, int tileID, BOOL solid = FALSE, const CString& type = _T(""));
    int GetTile(int x, int y) const;

    // 独立对象管理 - 新增
    void AddBrick(int x, int y, CBrick::BrickType type = CBrick::NORMAL);
    void AddPipe(int x, int y, int height = CGameConfig::PIPE_HEIGHT);
    void ClearObjects();  // 清空所有对象

    // 渲染
    void Draw(CDC* pDC, int offsetX = 0, int offsetY = 0);

    // 碰撞检测
    BOOL IsSolid(int worldX, int worldY) const;
    CRect GetTileRect(int tileX, int tileY) const;
    std::vector<CRect> GetSolidTileRects() const;

    // 新增：获取所有碰撞矩形（包括独立对象）
    std::vector<CRect> GetAllCollisionRects() const;

    // 地图信息
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }
    int GetTileSize() const { return m_nTileSize; }

    // 新增：获取独立对象（用于渲染）
    const std::vector<CBrick>& GetBricks() const { return m_Bricks; }
    const std::vector<CPipe>& GetPipes() const { return m_Pipes; }
    void AddCoin(int x, int y);
    const std::vector<CCoin>& GetCoins() const { return m_Coins; }
    void ClearCoins();
    void UpdateCoins(float deltaTime);
    BOOL CheckCoinCollisions(const CRect& rect);
    void RemoveCoin(int index);
private:
    int m_nWidth;           // 地图宽度（瓦片数）
    int m_nHeight;          // 地图高度（瓦片数）
    int m_nTileSize;        // 瓦片尺寸（像素）

    std::vector<std::vector<Tile>> m_Tiles;  // 瓦片数据

    // 新增：独立对象容器
    std::vector<CBrick> m_Bricks;
    std::vector<CPipe> m_Pipes;
    std::vector<CCoin> m_Coins;
    // 新增：关卡加载方法
    BOOL LoadLevel1();
    BOOL LoadLevel2();
    BOOL LoadLevel3();
};