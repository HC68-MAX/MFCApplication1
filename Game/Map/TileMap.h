#pragma once
// Game/Map/TileMap.h
#pragma once

#include <afxwin.h>
#include <vector>
#include <string>

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
    void SetTile(int x, int y, int tileID, BOOL solid = FALSE, const CString& type = _T(""));
    int GetTile(int x, int y) const;

    // 渲染
    void Draw(CDC* pDC, int offsetX = 0, int offsetY = 0);

    // 碰撞检测
    BOOL IsSolid(int worldX, int worldY) const;
    CRect GetTileRect(int tileX, int tileY) const;
    std::vector<CRect> GetSolidTileRects() const;

    // 地图信息
    int GetWidth() const { return m_nWidth; }
    int GetHeight() const { return m_nHeight; }
    int GetTileSize() const { return m_nTileSize; }

private:
    int m_nWidth;           // 地图宽度（瓦片数）
    int m_nHeight;          // 地图高度（瓦片数）
    int m_nTileSize;        // 瓦片尺寸（像素）

    std::vector<std::vector<Tile>> m_Tiles;  // 瓦片数据
};