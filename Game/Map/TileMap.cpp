// Game/Map/TileMap.cpp
#include <afxwin.h>
#include "TileMap.h"

CTileMap::CTileMap()
    : m_nWidth(0), m_nHeight(0), m_nTileSize(0)
{
}

CTileMap::~CTileMap()
{
}

BOOL CTileMap::LoadMap(int width, int height, int tileSize)
{
    m_nWidth = width;
    m_nHeight = height;
    m_nTileSize = tileSize;

    // 初始化瓦片数组
    m_Tiles.resize(height);
    for (int y = 0; y < height; y++)
    {
        m_Tiles[y].resize(width);
        for (int x = 0; x < width; x++)
        {
            m_Tiles[y][x] = { 0, x, y, FALSE, _T("empty") };
        }
    }

    return TRUE;
}

void CTileMap::SetTile(int x, int y, int tileID, BOOL solid, const CString& type)
{
    if (x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight)
    {
        m_Tiles[y][x].tileID = tileID;
        m_Tiles[y][x].solid = solid;
        m_Tiles[y][x].type = type;
    }
}

int CTileMap::GetTile(int x, int y) const
{
    if (x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight)
    {
        return m_Tiles[y][x].tileID;
    }
    return -1;
}

void CTileMap::Draw(CDC* pDC, int offsetX, int offsetY)
{
    // 这里先绘制简单的颜色块，后面会替换为贴图
    for (int y = 0; y < m_nHeight; y++)
    {
        for (int x = 0; x < m_nWidth; x++)
        {
            int tileID = m_Tiles[y][x].tileID;
            if (tileID > 0)
            {
                int screenX = x * m_nTileSize - offsetX;
                int screenY = y * m_nTileSize - offsetY;

                // 根据瓦片ID选择颜色
                COLORREF color = RGB(128, 128, 128); // 默认灰色
                switch (tileID)
                {
                case 1: // 地面
                    color = RGB(180, 90, 40);
                    break;
                case 2: // 砖块
                    color = RGB(200, 76, 12);
                    break;
                case 3: // 问号砖块
                    color = RGB(255, 200, 0);
                    break;
                case 4: // 硬砖块
                    color = RGB(128, 128, 128);
                    break;
                case 5: // 水管
                    color = RGB(0, 150, 0);
                    break;
                }

                pDC->FillSolidRect(screenX, screenY, m_nTileSize, m_nTileSize, color);

                // 绘制瓦片边框
                pDC->Draw3dRect(screenX, screenY, m_nTileSize, m_nTileSize,
                    RGB(100, 100, 100), RGB(200, 200, 200));

                // 如果是问号砖块，绘制问号
                if (tileID == 3)
                {
                    pDC->SetTextColor(RGB(0, 0, 0));
                    pDC->SetBkMode(TRANSPARENT);
                    pDC->SetTextAlign(TA_CENTER);
                    pDC->TextOut(screenX + m_nTileSize / 2, screenY + m_nTileSize / 4, _T("?"));
                    pDC->SetTextAlign(TA_LEFT);
                }
            }
        }
    }
}

BOOL CTileMap::IsSolid(int worldX, int worldY) const
{
    int tileX = worldX / m_nTileSize;
    int tileY = worldY / m_nTileSize;

    if (tileX >= 0 && tileX < m_nWidth && tileY >= 0 && tileY < m_nHeight)
    {
        return m_Tiles[tileY][tileX].solid;
    }

    return FALSE;
}

CRect CTileMap::GetTileRect(int tileX, int tileY) const
{
    return CRect(tileX * m_nTileSize, tileY * m_nTileSize,
        (tileX + 1) * m_nTileSize, (tileY + 1) * m_nTileSize);
}

std::vector<CRect> CTileMap::GetSolidTileRects() const
{
    std::vector<CRect> solidRects;

    for (int y = 0; y < m_nHeight; y++)
    {
        for (int x = 0; x < m_nWidth; x++)
        {
            if (m_Tiles[y][x].solid && m_Tiles[y][x].tileID > 0)
            {
                solidRects.push_back(GetTileRect(x, y));
            }
        }
    }

    return solidRects;
}