#include "Monster.h"

CMonster::CMonster(int x, int y) : CGameObject(x, y, CGameConfig::ENEMY_WIDTH, CGameConfig::ENEMY_HEIGHT)
{
    m_fX = (float)x;
    m_fY = (float)y;
    m_fVelocityX = -1.0f; // 向左移动 (pixels per frame)
    m_fVelocityY = 0.0f;
    m_fGravity = CGameConfig::GRAVITY;
    m_bIsDead = FALSE;
    m_bIsSquished = FALSE;
    m_fSquishTimer = 0.0f;
    m_fAnimTimer = 0.0f;
    m_nAnimFrame = 0;
    m_fAnimSpeed = 0.2f;
}

CMonster::~CMonster()
{
}

void CMonster::Update(float deltaTime)
{
    if (m_bIsDead) return;

    if (m_bIsSquished)
    {
        m_fSquishTimer += deltaTime;
        if (m_fSquishTimer > 0.5f) // 0.5秒后消失
        {
            m_bIsDead = TRUE;
            m_bVisible = FALSE;
        }
        return;
    }

    // 应用重力 (Frame-based physics to match Mario)
    m_fVelocityY += m_fGravity;

    // 水平移动范围检测（核心修改）
    m_fX += m_fVelocityX;  // 按时间更新位置（原代码可能缺少deltaTime，建议加上）

    // 碰到左边界，向右转向
    if (m_fX <= m_fLeftBound) {
        m_fX = m_fLeftBound;
        m_fVelocityX = fabs(m_fVelocityX);  // 确保向右
    }
    // 碰到右边界，向左转向
    else if (m_fX + m_nWidth >= m_fRightBound) {
        m_fX = m_fRightBound - m_nWidth;
        m_fVelocityX = -fabs(m_fVelocityX); // 确保向左
    }

    // 垂直位置更新（保留重力效果）
    m_fY += m_fVelocityY * deltaTime;
    m_nX = (int)m_fX;
    m_nY = (int)m_fY;
    // 动画更新
    m_fAnimTimer += deltaTime;
    if (m_fAnimTimer >= m_fAnimSpeed)
    {
        m_fAnimTimer = 0.0f;
        m_nAnimFrame = (m_nAnimFrame + 1) % 2;
    }
}

void CMonster::CheckCollisions(const std::vector<CRect>& platforms)
{
    if (m_bIsSquished || m_bIsDead) return;

    CRect monsterRect = GetRect();

    for (const auto& platform : platforms)
    {
        CRect intersect;
        if (intersect.IntersectRect(monsterRect, platform))
        {
            // 垂直碰撞
            if (intersect.Width() > intersect.Height())
            {
                if (m_nY < platform.top) // 在上方
                {
                    m_nY = platform.top - m_nHeight;
                    m_fY = (float)m_nY;
                    m_fVelocityY = 0;
                }
            }
        }
    }
    
    // 简单的边界检查
    if (m_nY > 1200) m_bIsDead = TRUE; // 掉出屏幕
}

void CMonster::OnCollisionWithMario(bool fromTop)
{
    if (fromTop)
    {
        m_bIsSquished = TRUE;
        m_fVelocityX = 0;
        // 调整高度为压扁的高度
        int oldHeight = m_nHeight;
        m_nHeight = 16; // 变矮
        m_nY += (oldHeight - m_nHeight); // 底部对齐
        m_fY = (float)m_nY;
    }
}

void CMonster::DrawAt(CDC* pDC, int screenX, int screenY)
{
    if (!m_bVisible) return;

    CResourceManager& resMgr = CResourceManager::GetInstance();
    CBitmap* pBitmap = resMgr.GetBitmap(CSpriteConfig::TILESET_ENEMIES);
    
    if (!pBitmap)
    {
        // 资源缺失时的后备绘制 - 红色方块
        pDC->FillSolidRect(screenX, screenY, m_nWidth, m_nHeight, RGB(255, 0, 0));
        return;
    }

    SSpriteCoord spriteCoord;
    int drawHeight = m_nHeight;
    int drawY = screenY;

    if (m_bIsSquished)
    {
        spriteCoord = CSpriteConfig::GOOMBA_SQUISHED;
        drawHeight = m_nHeight / 2; // 压扁后高度减半
        drawY = screenY + m_nHeight / 2; // 向下偏移以对齐底部
    }
    else
    {
        spriteCoord = (m_nAnimFrame == 0) ? CSpriteConfig::GOOMBA_WALK1 : CSpriteConfig::GOOMBA_WALK2;
    }

    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, drawY,
        m_nWidth, drawHeight, spriteCoord.x, spriteCoord.y,
        spriteCoord.width, spriteCoord.height);
}
