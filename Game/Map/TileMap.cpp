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

                // 只绘制在屏幕范围内的瓦片
                if (screenX + m_nTileSize < 0 || screenX >= 800 ||
                    screenY + m_nTileSize < 0 || screenY >= 600)
                {
                    continue;
                }

                // 根据瓦片ID选择颜色
                COLORREF color = RGB(128, 128, 128); // 默认灰色
                COLORREF darkColor = RGB(100, 100, 100);
                COLORREF lightColor = RGB(200, 200, 200);

                switch (tileID)
                {
                case 1: // 地面 - 棕色
                    color = RGB(146, 73, 0);
                    darkColor = RGB(100, 50, 0);
                    lightColor = RGB(180, 120, 60);
                    break;
                case 2: // 砖块 - 橙色
                    color = RGB(220, 100, 0);
                    darkColor = RGB(160, 70, 0);
                    lightColor = RGB(255, 150, 50);
                    break;
                case 3: // 问号砖块 - 黄色
                    color = RGB(255, 200, 0);
                    darkColor = RGB(200, 150, 0);
                    lightColor = RGB(255, 255, 100);
                    break;
                case 4: // 硬砖块 - 灰色
                    color = RGB(128, 128, 128);
                    darkColor = RGB(80, 80, 80);
                    lightColor = RGB(180, 180, 180);
                    break;
                case 5: // 水管 - 绿色
                    color = RGB(0, 180, 0);
                    darkColor = RGB(0, 120, 0);
                    lightColor = RGB(0, 220, 0);
                    break;
                }

                // 绘制瓦片主体 - 确保使用正确的尺寸
                pDC->FillSolidRect(screenX, screenY, m_nTileSize, m_nTileSize, color);

                // 绘制3D效果边框
                pDC->Draw3dRect(screenX, screenY, m_nTileSize, m_nTileSize, darkColor, lightColor);

                // 对于某些瓦片类型，添加额外的细节
                if (tileID == 3) // 问号砖块
                {
                    // 绘制大问号
                    CString strQuestion = _T("?");
                    pDC->SetTextColor(RGB(0, 0, 0));
                    pDC->SetBkMode(TRANSPARENT);

                    // 使用系统字体，但设置较大尺寸
                    CFont font;
                    font.CreatePointFont(240, _T("Arial")); // 24点字体
                    CFont* pOldFont = pDC->SelectObject(&font);

                    pDC->SetTextAlign(TA_CENTER);
                    pDC->TextOut(screenX + m_nTileSize / 2, screenY + m_nTileSize / 4, strQuestion);

                    // 恢复原来的字体
                    pDC->SelectObject(pOldFont);
                    pDC->SetTextAlign(TA_LEFT);
                    font.DeleteObject();
                }
                else if (tileID == 2) // 砖块
                {
                    // 添加砖块纹理
                    for (int i = 0; i < m_nTileSize; i += 8)
                    {
                        pDC->FillSolidRect(screenX, screenY + i, m_nTileSize, 2, darkColor);
                    }
                }
                else if (tileID == 5) // 水管
                {
                    // 添加水管纹理
                    for (int i = 0; i < m_nTileSize; i += 8)
                    {
                        pDC->FillSolidRect(screenX, screenY + i, 4, 2, darkColor);
                        pDC->FillSolidRect(screenX + m_nTileSize - 4, screenY + i, 4, 2, lightColor);
                    }
                }

                // 调试：绘制瓦片边界和坐标（仅在调试模式下）
                if (FALSE) // 设置为TRUE可以显示瓦片坐标
                {
                    CString coord;
                    coord.Format(_T("%d,%d"), x, y);
                    pDC->SetTextColor(RGB(255, 255, 255));
                    pDC->SetBkMode(TRANSPARENT);
                    pDC->TextOut(screenX + 2, screenY + 2, coord);
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