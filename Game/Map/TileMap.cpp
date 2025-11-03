// Game/Map/TileMap.cpp
#include <afxwin.h>
#include "TileMap.h"
#include "../Core/ResourceManager.h"
#include "../Core/SpriteRenderer.h"
#include "../Core/SpriteConfig.h"
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
    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(_T("TilesetMain"));

    for (int y = 0; y < m_nHeight; y++)
    {
        for (int x = 0; x < m_nWidth; x++)
        {
            int tileID = m_Tiles[y][x].tileID;
            if (tileID > 0)
            {
                int screenX = x * m_nTileSize - offsetX;
                int screenY = y * m_nTileSize - offsetY;

                // 只绘制在屏幕范围内的瓦片
                if (screenX + m_nTileSize < 0 || screenX >= 800 ||
                    screenY + m_nTileSize < 0 || screenY >= 600)
                {
                    continue;
                }

                SSpriteCoord spriteCoord;

                // 根据瓦片ID选择贴图坐标
                switch (tileID)
                {
                case 1: // 地面
                    spriteCoord = CSpriteConfig::GROUND;
                    break;
                case 2: // 砖块
                    spriteCoord = CSpriteConfig::BRICK_NORMAL;
                    break;
                case 3: // 问号砖块
                    spriteCoord = CSpriteConfig::BRICK_QUESTION;
                    break;
                case 4: // 硬砖块
                    spriteCoord = CSpriteConfig::BRICK_HARD;
                    break;
                case 5: // 水管
                    spriteCoord = CSpriteConfig::PIPE_BODY_LEFT; // 临时使用
                    break;
                default:
                    spriteCoord = CSpriteConfig::BRICK_NORMAL; // 默认
                    break;
                }

                if (pBitmap)
                {
                    // 使用精灵渲染器绘制
                    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
                        spriteCoord.x, spriteCoord.y,
                        m_nTileSize, m_nTileSize, TRUE);
                }
                else
                {
                    // 备用：如果没有贴图，使用颜色绘制
                    COLORREF color = RGB(128, 128, 128);
                    switch (tileID)
                    {
                    case 1: color = RGB(146, 73, 0); break;
                    case 2: color = RGB(220, 100, 0); break;
                    case 3: color = RGB(255, 200, 0); break;
                    case 4: color = RGB(128, 128, 128); break;
                    case 5: color = RGB(0, 180, 0); break;
                    }
                    pDC->FillSolidRect(screenX, screenY, m_nTileSize, m_nTileSize, color);
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