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
// 新增：加载指定关卡
BOOL CTileMap::LoadLevel(int levelNumber)
{
    // 清空现有对象
    ClearObjects();

    TRACE(_T("加载关卡: %d\n"), levelNumber);

    switch (levelNumber)
    {
    case 1:
        return LoadLevel1();
    case 2:
        return LoadLevel2();
    case 3:
        return LoadLevel3();
    default:
        return LoadLevel1(); // 默认加载第一关
    }
}
// 第一关
BOOL CTileMap::LoadLevel1()
{
    // 初始化基础地图
    if (!LoadMap(CGameConfig::TILE_MAP_WIDTH, CGameConfig::TILE_MAP_HEIGHT, CGameConfig::TILE_SIZE))
        return FALSE;

    // 创建地面
    int groundLevel = m_nHeight - 5; // 离底部5行的位置
    for (int x = 0; x < m_nWidth; x++)
    {
        for (int y = groundLevel; y < m_nHeight; y++)
        {
            SetTile(x, y, 1, TRUE, _T("ground"));
        }
    }

    // 平台1
    for (int x = 3; x < 8; x++)
    {
        SetTile(x, 7, 2, TRUE, _T("brick"));
    }

    // 平台2
    for (int x = 10; x < 15; x++)
    {
        SetTile(x, 5, 2, TRUE, _T("brick"));
    }

    // 问号砖块
    SetTile(7, 3, 3, TRUE, _T("question"));
    SetTile(8, 3, 3, TRUE, _T("question"));

    // 硬砖块
    for (int x = 12; x < 14; x++)
    {
        SetTile(x, 2, 4, TRUE, _T("hard_brick"));
    }

    // 水管
    SetTile(20, 7, 5, TRUE, _T("pipe"));
    SetTile(20, 8, 7, TRUE, _T("pipe"));
    SetTile(20, 9, 7, TRUE, _T("pipe"));
    SetTile(21, 7, 6, TRUE, _T("pipe"));
    SetTile(21, 8, 8, TRUE, _T("pipe"));
    SetTile(21, 9, 8, TRUE, _T("pipe"));


    TRACE(_T("关卡1加载完成: 砖块=%d, 水管=%d\n"), m_Bricks.size(), m_Pipes.size());
    return TRUE;
}

// 第二关（示例）
BOOL CTileMap::LoadLevel2()
{
    // 初始化基础地图
    if (!LoadMap(CGameConfig::TILE_MAP_WIDTH, CGameConfig::TILE_MAP_HEIGHT, CGameConfig::TILE_SIZE))
        return FALSE;

    // 第二关的地图设计...
    int groundLevel = m_nHeight - 4;
    for (int x = 0; x < m_nWidth; x++)
    {
        for (int y = groundLevel; y < m_nHeight; y++)
        {
            SetTile(x, y, 1, TRUE, _T("ground"));
        }
    }

    // 第二关的特殊设计...
    for (int x = 5; x < 10; x++)
    {
        SetTile(x, 6, 2, TRUE, _T("brick"));
    }

    // 第二关的独立对象
    AddBrick(400, 250, CBrick::QUESTION);
    AddBrick(450, 250, CBrick::QUESTION);
    AddPipe(550, 300, 128);

    TRACE(_T("关卡2加载完成: 砖块=%d, 水管=%d\n"), m_Bricks.size(), m_Pipes.size());
    return TRUE;
}

// 第三关（示例）
BOOL CTileMap::LoadLevel3()
{
    // 类似上面的实现...
    return LoadLevel1(); // 暂时用第一关代替
}

// 修改绘制方法，同时绘制瓦片和独立对象
void CTileMap::Draw(CDC* pDC, int offsetX, int offsetY)
{
    // 绘制瓦片（原有代码）
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
                case 5: // 水管左上
                    spriteCoord = CSpriteConfig::PIPE_TOP_LEFT; // 临时使用
                    break;
                case 6: // 水管右上
                    spriteCoord = CSpriteConfig::PIPE_TOP_RIGHT; // 临时使用
                    break;
                case 7: // 水管管体左
                    spriteCoord = CSpriteConfig::PIPE_BODY_LEFT; // 临时使用
                    break;
                case 8: // 水管管体右
                    spriteCoord = CSpriteConfig::PIPE_BODY_RIGHT; // 临时使用
                    break;
                default:
                    spriteCoord = CSpriteConfig::BRICK_NORMAL; // 默认
                    break;
                }

                if (pBitmap)
                {
                    // 使用精灵渲染器绘制
                    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
                        CGameConfig::TILE_SIZE, CGameConfig::TILE_SIZE,
                        spriteCoord.x, spriteCoord.y,
                        spriteCoord.width, spriteCoord.height);
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

// 新增：获取所有碰撞矩形
std::vector<CRect> CTileMap::GetAllCollisionRects() const
{
    std::vector<CRect> allRects = GetSolidTileRects();

    // 添加砖块的碰撞矩形
    for (const auto& brick : m_Bricks)
    {
        allRects.push_back(brick.GetRect());
    }

    // 添加水管的碰撞矩形
    for (const auto& pipe : m_Pipes)
    {
        allRects.push_back(pipe.GetRect());
    }

    return allRects;
}

// 其他现有方法保持不变...
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
// 新增：添加砖块
void CTileMap::AddBrick(int x, int y, CBrick::BrickType type)
{
    CBrick brick(x, y);
    brick.SetBrickType(type);
    m_Bricks.push_back(brick);
}

// 新增：添加水管
void CTileMap::AddPipe(int x, int y, int height)
{
    CPipe pipe(x, y, height);
    m_Pipes.push_back(pipe);
}

// 新增：清空所有对象
void CTileMap::ClearObjects()
{
    m_Bricks.clear();
    m_Pipes.clear();
}
