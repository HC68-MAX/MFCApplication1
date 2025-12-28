#pragma once
// Game/Map/TileMap.h

#include <afxwin.h>
#include <vector>
#include <string>

#include "../Core/GameConfig.h"
#include "../Core/GameState.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteConfig.h"
#include "../Core/SpriteRenderer.h"
#include "../Objects/Brick.h"
#include "../Objects/Coin.h"
#include "../Objects/Flagpole.h"
#include "../Objects/Mario.h"
#include "../Objects/Monster.h"
#include "../Objects/Pipe.h"

// 瓦片结构体，定义地图的基本单元
struct Tile
{
    int tileID;     // 瓦片ID，用于从图集中选择图像
    int x, y;       // 在地图网格中的坐标
    BOOL solid;     // 是否为实体，即是否会发生碰撞
    CString type;   // 瓦片类型，如 "ground", "brick", "question" 等
};

// CTileMap类，负责管理和渲染游戏关卡地图
class CTileMap
{
public:
    // =================================================================
    // 构造与析构
    // =================================================================
    CTileMap();
    ~CTileMap();

    // =================================================================
    // 地图加载与构建
    // =================================================================
    BOOL LoadMap(int width, int height, int tileSize); // 初始化地图尺寸
    BOOL LoadLevel(int levelNumber); // 加载指定关卡布局
    void ClearObjects();  // 清除所有游戏对象（砖块、管道等）

    // =================================================================
    // 对象添加
    // =================================================================
    void SetTile(int x, int y, int tileID, BOOL solid = FALSE, const CString& type = _T(""));
    void AddFlagpole(int x, int y);
    void AddCoin(int x, int y);
    void AddMonsterAtTile(int tileX, int tileY, int leftBound_, int rightBound_);

    // =================================================================
    // 游戏循环接口
    // =================================================================
    void UpdateCoins(float deltaTime);
    void UpdateMonsters(float deltaTime); // 更新怪物状态
    BOOL CheckWinCondition() const;
    void Draw(CDC* pDC, int offsetX = 0, int offsetY = 0); // 渲染地图

    // =================================================================
    // 碰撞检测
    // =================================================================
    std::vector<CRect> GetSolidTileRects() const; // 获取所有实体瓦片的碰撞矩形
    BOOL CheckCoinCollisions(const CRect& rect);
    BOOL CheckQuestionBlockHit(const CRect& rect, BOOL isMovingUp);
    void CheckMonsterCollisions(CMario* pMario);
    BOOL CheckFlagpoleCollision(const CRect& rect);
    // =================================================================
    // Getters & Setters
    // =================================================================
    void SetMario(CMario* pMario) { m_pMario = pMario; }
    CRect GetTileRect(int tileX, int tileY) const;
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }
    int GetTileSize() const { return m_nTileSize; }
    int GetMarioStartX() const { return m_MarioStartX; }
    int GetMarioStartY() const { return m_MarioStartY; }

    // =================================================================
    // 其他操作
    // =================================================================

private:
    // =================================================================
    // 私有辅助函数
    // =================================================================
    BOOL LoadLevel1();
    BOOL LoadLevel2();
    BOOL LoadLevel3();

private:
    // =================================================================
    // 成员变量
    // =================================================================
    int m_nWidth;           // 地图宽度（瓦片数量）
    int m_nHeight;          // 地图高度（瓦片数量）
    int m_nTileSize;        // 瓦片尺寸（像素）

    // 马里奥初始位置
    int m_MarioStartX;
    int m_MarioStartY;

    // 游戏对象容器
    std::vector<std::vector<Tile>> m_Tiles;  // 瓦片数据
    std::vector<CBrick> m_Bricks;
    std::vector<CPipe> m_Pipes;
    std::vector<CFlagpole> m_Flagpoles;
    std::vector<CCoin> m_Coins;
    std::vector<CMonster> m_Monsters; // 怪物列表

    // 外部对象指针
    CMario* m_pMario = nullptr;  // 指向 Mario 对象的指针
};
