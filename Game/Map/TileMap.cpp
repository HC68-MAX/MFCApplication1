// Game/Map/TileMap.cpp
#include <afxwin.h>
#include "TileMap.h"

CTileMap::CTileMap()
    : m_nWidth(0), m_nHeight(0), m_nTileSize(0), m_MarioStartX(0), m_MarioStartY(0)
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

    BOOL result = FALSE;
    switch (levelNumber)
    {
    case 1:
        result = LoadLevel1();
        break;
    case 2:
        result = LoadLevel2();
        break;
    case 3:
        result = LoadLevel3();
        break;
    default:
        result = LoadLevel1(); // 默认加载第一关
        break;
    }

    if (result && m_pMario)
    {
        m_pMario->SetPosition(m_MarioStartX, m_MarioStartY);
    }

    return result;
}
// 第一关
BOOL CTileMap::LoadLevel1()
{
    // 初始化基础地图
    if (!LoadMap(CGameConfig::TILE_MAP_WIDTH, CGameConfig::TILE_MAP_HEIGHT, CGameConfig::TILE_SIZE))
        return FALSE;

    // 设置马里奥初始位置
    m_MarioStartX = 5 * CGameConfig::TILE_SIZE;
    m_MarioStartY = 19*CGameConfig::TILE_SIZE;

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
        int y = 16;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y-1) * CGameConfig::TILE_SIZE);
    }

    // 平台2
    for (int x = 10; x < 15; x++)
    {
        int y = 14;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y-1) * CGameConfig::TILE_SIZE);
    }
    // 硬砖块
    for (int x = 12; x < 14; x++)
    {
        SetTile(x, 10, 3, TRUE, _T("question_brick"));
          
    }

    // 水管
    SetTile(20, 17, 5, TRUE, _T("pipe"));
    SetTile(20, 18, 7, TRUE, _T("pipe"));
    SetTile(20, 19, 7, TRUE, _T("pipe"));
    SetTile(21, 17, 6, TRUE, _T("pipe"));
    SetTile(21, 18, 8, TRUE, _T("pipe"));
    SetTile(21, 19, 8, TRUE, _T("pipe"));

    // 添加怪物 (使用瓦片坐标)
    AddMonsterAtTile(12, 13,2,3); // 在平台2上
    AddMonsterAtTile(5, 15,2,3);  // 在平台1上
    AddMonsterAtTile(25, 19,2,5); // 地面上
    AddMonsterAtTile(35, 19,5,5); // 地面上

    AddFlagpole(40 * CGameConfig::TILE_SIZE-20, 14 * CGameConfig::TILE_SIZE+10);
    // 顶部（tileID=10）
    SetTile(40, 13, 10, FALSE, _T("flagpole_top"));
    // 杆体（tileID=11，堆叠5个瓦片）
    for (int y = 14; y <= 19; y++) {
    SetTile(40, y, 11, FALSE, _T("flagpole_pole"));
    }
    TRACE(_T("关卡1加载完成: 砖块=%d, 水管=%d, 怪物=%d\n"), m_Bricks.size(), m_Pipes.size(), m_Monsters.size());
    return TRUE;
}
// 第二关
BOOL CTileMap::LoadLevel2()
{
    // 初始化瓦片地图
    if (!LoadMap(CGameConfig::TILE_MAP_WIDTH, CGameConfig::TILE_MAP_HEIGHT, CGameConfig::TILE_SIZE))
        return FALSE;

    // 设置马里奥初始位置
    m_MarioStartX = 2 * CGameConfig::TILE_SIZE;
    m_MarioStartY = 21*CGameConfig::TILE_SIZE;

    TRACE(_T("加载关卡2\n"));

    // 第二关的地面更高
    int groundLevel = m_nHeight - 3; // 离底部3行的位置

    for (int x = 0; x < m_nWidth; x++)
    {
        for (int y = groundLevel; y < m_nHeight; y++)
        {
            SetTile(x, y, 1, TRUE, _T("ground"));
        }
    }

    // 添加一些平台
    for (int x = 5; x < 12; x++)
    {
        int y = 16;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y-1) * CGameConfig::TILE_SIZE);
    }
    // 添加一些平台
    for (int x = 14; x < 20; x++)
    {
        int y = 18;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y - 1) * CGameConfig::TILE_SIZE);
    }
    // 添加问号砖块
    for (int x = 15; x < 18; x++)
    {
        int y = 14;
        SetTile(x, y, 3, TRUE, _T("question_brick"));
    }

    // 添加水管
    SetTile(25, 20, 5, TRUE, _T("pipe"));
    SetTile(25, 21, 7, TRUE, _T("pipe"));
    SetTile(26, 20, 6, TRUE, _T("pipe"));
    SetTile(26, 21, 8, TRUE, _T("pipe"));


    TRACE(_T("关卡2加载完成: 砖块=%d, 水管=%d, 金币=%d\n"),
        m_Bricks.size(), m_Pipes.size(), m_Coins.size());
    return TRUE;
}
// 第三关 
BOOL CTileMap::LoadLevel3()
{
    // 初始化瓦片地图
    if (!LoadMap(CGameConfig::TILE_MAP_WIDTH, CGameConfig::TILE_MAP_HEIGHT, CGameConfig::TILE_SIZE))
        return FALSE;

    // 设置马里奥初始位置
    m_MarioStartX = 2 * CGameConfig::TILE_SIZE;
    m_MarioStartY = 19*CGameConfig::TILE_SIZE;

    TRACE(_T("加载关卡3\n"));

    // 第三关使用阶梯式地面
    for (int x = 0; x < m_nWidth; x++)
    {
        int groundLevel = m_nHeight - 5 + (x / 10) % 3; // 每10个瓦片高度变化

        for (int y = groundLevel; y < m_nHeight; y++)
        {
            SetTile(x, y, 1, TRUE, _T("ground"));
        }
    }

    // 添加多个平台
    for (int x = 4; x < 8; x++)
    {
        int y = 15;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y-1) * CGameConfig::TILE_SIZE);
    }

    for (int x = 12; x < 16; x++)
    {
        int y = 15;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y - 1) * CGameConfig::TILE_SIZE);
    }

    for (int x = 20; x < 24; x++)
    {
        int y = 14;
        SetTile(x, y, 2, TRUE, _T("brick"));
        AddCoin(x * CGameConfig::TILE_SIZE, (y - 1) * CGameConfig::TILE_SIZE);
    }

    // 添加问号砖块
    SetTile(6, 12, 3, TRUE, _T("question_brick"));
    SetTile(14, 12, 3, TRUE, _T("question_brick"));
    SetTile(22, 11, 3, TRUE, _T("question_brick"));


   
    TRACE(_T("关卡3加载完成: 砖块=%d, 水管=%d, 金币=%d\n"),
        m_Bricks.size(), m_Pipes.size(), m_Coins.size());
    return TRUE;
}
// 同时绘制瓦片和独立对象
void CTileMap::Draw(CDC* pDC, int offsetX, int offsetY)
{
    // 绘制瓦片（静态事物）
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
                if (screenX + m_nTileSize < 0 || screenX >= CGameConfig::SCREEN_WIDTH ||
                    screenY + m_nTileSize < 0 || screenY >= CGameConfig::SCREEN_HEIGHT)
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
                case 9: // 被顶过的问号砖块
                    spriteCoord = CSpriteConfig::BRICK_QUESTION_HIT;
                    break;
                case 10:  // 旗杆顶部
                    spriteCoord = CSpriteConfig::FLAGPOLE_TOP;
                    break;
                case 11:  // 旗杆主体
                    spriteCoord = CSpriteConfig::FLAGPOLE_POLE;
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
    // 2.1 绘制动态旗子（带动画）
    for (auto& flagpole : m_Flagpoles) {
        if (!flagpole.IsVisible()) continue;
        int screenX = flagpole.GetX() - offsetX;
        int screenY = flagpole.GetY() - offsetY;
        // 只绘制屏幕范围内的旗子
        if (screenX + 16 < 0 || screenX >= CGameConfig::SCREEN_WIDTH ||
            screenY + 16 < 0 || screenY >= CGameConfig::SCREEN_HEIGHT) {
            continue;
        }
        // 绘制动态旗子（仅绘制旗子部分，杆体已在静态瓦片绘制）
        flagpole.DrawFlag(pDC, screenX, screenY);
    }
    // 绘制金币 
    for (auto& coin : m_Coins)
    {
        if (coin.IsVisible() && !coin.IsCollected())
        {
            int screenX = coin.GetX() - offsetX;
            int screenY = coin.GetY() - offsetY;

            // 只绘制在屏幕范围内的金币
            if (screenX + m_nTileSize < 0 || screenX >= CGameConfig::SCREEN_WIDTH ||
                screenY + m_nTileSize < 0 || screenY >= CGameConfig::SCREEN_HEIGHT)
            {
                continue;
            }

            // 使用新的绘制方法，传入屏幕坐标
            coin.DrawAt(pDC, screenX, screenY);
        }
    }

    // 绘制怪物
    for (auto& monster : m_Monsters)
    {
        if (monster.IsVisible())
        {
            int screenX = monster.GetX() - offsetX;
            int screenY = monster.GetY() - offsetY;

            // 简单的屏幕剔除
            if (screenX + monster.GetWidth() > 0 && screenX < CGameConfig::SCREEN_WIDTH)
            {
                monster.DrawAt(pDC, screenX, screenY);
            }
        }
    }

    // 5. 绘制 Mario
    // Draw Flagpoles
    for (auto& flagpole : m_Flagpoles)
    {
        int screenX = flagpole.GetX() - offsetX;
        int screenY = flagpole.GetY() - offsetY;
        flagpole.DrawFlag(pDC, screenX, screenY);
    }

    if (m_pMario && m_pMario->IsVisible())
    {
        int marioScreenX = m_pMario->GetX() - offsetX;
        int marioScreenY = m_pMario->GetY() - offsetY;

        // 只绘制在屏幕范围内的 Mario
        if (marioScreenX + m_nTileSize < 0 || marioScreenX >= CGameConfig::SCREEN_WIDTH ||
                marioScreenY + m_nTileSize < 0 || marioScreenY >= CGameConfig::SCREEN_HEIGHT)
        {
            return;
        }

        m_pMario->DrawAt(pDC, marioScreenX, marioScreenY);
    }

}

CRect CTileMap::GetTileRect(int tileX, int tileY) const
{
    return CRect(tileX * m_nTileSize, tileY * m_nTileSize,
        (tileX + 1) * m_nTileSize, (tileY + 1) * m_nTileSize);
}
// 检查问号砖块碰撞 - 简化版
BOOL CTileMap::CheckQuestionBlockHit(const CRect& rect, BOOL isMovingUp)
{
    // 只有当马里奥向上移动时才检测问号砖块碰撞
    if (!isMovingUp)
        return FALSE;

    // 将马里奥头部矩形转换为瓦片坐标
    int tileLeft = rect.left / m_nTileSize;
    int tileRight = rect.right / m_nTileSize;
    int tileY = rect.top / m_nTileSize;

  //  TRACE(_T("检查问号砖块碰撞 - 头部区域: (%d,%d,%d,%d), 向上移动: %d\n"),
 //       rect.left, rect.top, rect.right, rect.bottom, isMovingUp);

    BOOL hitAny = FALSE;

    // 检查头部区域上方的所有瓦片
    int checkY = tileY - 1;

    for (int x = tileLeft; x <= tileRight; x++)
    {
        if (checkY >= 0 && checkY < m_nHeight &&
            x >= 0 && x < m_nWidth)
        {
            const Tile& tile = m_Tiles[checkY][x];

            // 如果是问号砖块
            if (tile.tileID == 3 && tile.solid)
            {
                TRACE(_T("顶到问号砖块! 瓦片位置: (%d, %d)\n"), x, checkY);

                // 生成金币
                AddCoin(x * m_nTileSize, (checkY - 1) * m_nTileSize);

                // 将问号砖块变为普通砖块
                SetTile(x, checkY, 9, TRUE, _T("question_brick"));

                hitAny = TRUE;
            }
        }
    }

    return hitAny;
}
//瓦片生成
void CTileMap::SetTile(int x, int y, int tileID, BOOL solid, const CString& type)
{
    if (x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight)
    {
        m_Tiles[y][x].tileID = tileID;
        m_Tiles[y][x].solid = solid;
        m_Tiles[y][x].type = type;
    }
}
// 新增：添加砖块
void CTileMap::AddBrick(int x, int y, CBrick::BrickType type)
{
    CBrick brick(x, y);
    brick.SetBrickType(type);
    m_Bricks.push_back(brick);
}

// 金币碰撞检测
BOOL CTileMap::CheckCoinCollisions(const CRect& rect)
{
    BOOL collectedAny = FALSE;

    for (auto& coin : m_Coins)
    {
        if (!coin.IsCollected() && coin.CheckCollision(rect))
        {
            coin.Collect();
            collectedAny = TRUE;

            // 更新游戏状态
            CGameState::GetInstance().AddCoin();

            TRACE(_T("金币被收集! 当前金币: %d, 分数: %d\n"),
                CGameState::GetInstance().GetCoins(),
                CGameState::GetInstance().GetScore());
        }
    }

    return collectedAny;
}
// 移除金币（标记为已收集）
BOOL CTileMap::CheckBrickCollisions(const CRect& rect)
{
    BOOL hitAny = FALSE;

    for (auto& brick : m_Bricks)
    {
        // 检查砖块是否可以被顶并且与头部矩形相交
        if (brick.CanBeHitFromBelow() && brick.CheckCollision(rect))
        {
            TRACE(_T("检测到砖块碰撞! 砖块位置: (%d, %d)\n"), brick.GetX(), brick.GetY());
            brick.OnHitFromBelow();
            hitAny = TRUE;
        }
    }

    return hitAny;
}

void CTileMap::RemoveCoin(int index)
{
    if (index >= 0 && index < m_Coins.size())
    {
        m_Coins[index].Collect();
    }
}

// 新增：添加金币
void CTileMap::ClearObjects()
{
    m_Bricks.clear();
    m_Pipes.clear();
    m_Flagpoles.clear();
    m_Coins.clear();  // 清空金币
    m_Monsters.clear(); // 清空怪物
}
// 添加金币方法
void CTileMap::AddCoin(int x, int y)
{
    CCoin coin(x, y);

    m_Coins.push_back(coin);
}

// 在瓦片坐标添加怪物
void CTileMap::AddMonsterAtTile(int tileX, int tileY,int leftBound_,int rightBound_)
{
    int x = tileX * m_nTileSize;
    int y = tileY * m_nTileSize;

    // 创建怪物并添加到列表
    m_Monsters.emplace_back(x, y);
    // 获取刚添加的怪物引用
    CMonster& monster = m_Monsters.back();

    // 设置移动范围：以生成位置为中心，左右各3格瓦片（可根据需要调整）
    int leftBound = x - leftBound_ * m_nTileSize;
    int rightBound = x + rightBound_ * m_nTileSize;
    monster.SetMoveBounds(leftBound, rightBound);
}

void CTileMap::UpdateCoins(float deltaTime)
{
    // 现在只需要调用一次静态方法来更新所有金币的动画
    CCoin::UpdateGlobalAnimation(deltaTime);

    // 不再需要遍历每个金币来更新动画
    // 所有金币共享同一个动画状态
}

void CTileMap::UpdateMonsters(float deltaTime)
{
    // 获取所有固体碰撞平台（包括瓦片砖块、地面等）
    std::vector<CRect> platforms = GetSolidTileRects();

    // 如果m_Bricks是独立于瓦片的特殊砖块（如可击碎的动态砖块），再添加
    for (const auto& brick : m_Bricks)
    {
        platforms.push_back(brick.GetRect());
    }

    // 添加水管碰撞
    for (const auto& pipe : m_Pipes)
    {
        platforms.push_back(pipe.GetRect());
    }

    // 更新每个怪物的状态并检测碰撞
    for (auto& monster : m_Monsters)
    {
        if (!monster.IsDead())
        {
            monster.Update(deltaTime);
            monster.CheckCollisions(platforms);
        }
    }
}

void CTileMap::CheckMonsterCollisions(CMario* pMario)
{
    if (!pMario) return;

    CRect marioRect = pMario->GetRect();
    float marioVelocityY = pMario->GetVelocityY();

    for (auto& monster : m_Monsters)
    {
        if (monster.IsDead() || monster.IsSquished()) continue;

        if (monster.CheckCollision(marioRect))
        {
            // 检查是否踩在怪物头上
            // 简单的判定：Mario在下落，且Mario底部在怪物中心以上
            if (marioVelocityY > 0 && marioRect.bottom < monster.GetY() + monster.GetHeight() / 2 + 10)
            {
                monster.OnCollisionWithMario(true);
                pMario->SetVelocity(pMario->GetVelocityX(), -15.0f); // 弹起
                CGameState::GetInstance().AddScore(100);
            }
            else
            {
                monster.OnCollisionWithMario(false);
                // Mario 受伤或死亡
                pMario->Die();
            }
        }
    }
}
// 清空金币
void CTileMap::ClearCoins()
{
    m_Coins.clear();
}

void CTileMap::AddFlagpole(int x, int y)
{
    CFlagpole flagpole(x, y);
    m_Flagpoles.push_back(flagpole);
}

BOOL CTileMap::CheckFlagpoleCollision(const CRect& rect)
{
    for (auto& flagpole : m_Flagpoles)
    {
        if (flagpole.CheckTouch(rect))
        {
            TRACE(_T("马里奥碰到旗杆了！触发旗子下落\n"));
            // 触发旗子下落（调用旗子类的方法）
            flagpole.TriggerFlagDown();
            // 碰撞后直接返回（避免多次触发）
            return TRUE;
        }
    }
    return FALSE;
}

// 在TileMap.cpp中添加实现
std::vector<CRect> CTileMap::GetSolidTileRects() const
{
    std::vector<CRect> solidRects;

    // 遍历所有瓦片，收集固体瓦片的碰撞矩形
    for (int y = 0; y < m_nHeight; y++)
    {
        for (int x = 0; x < m_nWidth; x++)
        {
            const Tile& tile = m_Tiles[y][x];
            if (tile.solid) // 只处理固体瓦片
            {
                // 计算瓦片在世界坐标系中的矩形
                int tileX = x * m_nTileSize;
                int tileY = y * m_nTileSize;
                CRect tileRect(tileX, tileY,
                    tileX + m_nTileSize,
                    tileY + m_nTileSize);
                solidRects.push_back(tileRect);
            }
        }
    }

    return solidRects;
}