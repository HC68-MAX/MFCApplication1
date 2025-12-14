#include "Monster.h"

CMonster::CMonster(int x, int y) : CGameObject(x, y, 16, 16)
{
    m_fVelocityX = -30.0f; // 向左移动
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

    // 应用重力
    m_fVelocityY += m_fGravity * deltaTime;

    // 更新位置
    m_nX += (int)(m_fVelocityX * deltaTime);
    m_nY += (int)(m_fVelocityY * deltaTime);

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
                    m_fVelocityY = 0;
                }
            }
            // 水平碰撞
            else
            {
                // 简单的反向逻辑
                if (m_fVelocityX < 0 && m_nX > platform.right - 5) // 向左撞墙
                {
                     m_nX = platform.right;
                     m_fVelocityX = -m_fVelocityX;
                }
                else if (m_fVelocityX > 0 && m_nX + m_nWidth < platform.left + 5) // 向右撞墙
                {
                    m_nX = platform.left - m_nWidth;
                    m_fVelocityX = -m_fVelocityX;
                }
                else 
                {
                     // 如果无法区分，简单反向
                     m_fVelocityX = -m_fVelocityX;
                }
            }
        }
    }
    
    // 简单的边界检查
    if (m_nY > 800) m_bIsDead = TRUE; // 掉出屏幕
}

void CMonster::OnCollisionWithMario(bool fromTop)
{
    if (fromTop)
    {
        m_bIsSquished = TRUE;
        m_fVelocityX = 0;
        // 调整高度为压扁的高度
        // m_nHeight = 8; // 视觉上变矮，但为了简单可能保持原样或调整绘制
    }
}

void CMonster::Draw(CDC* pDC)
{
    // 默认实现，可能不被调用
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
    if (m_bIsSquished)
    {
        spriteCoord = CSpriteConfig::GOOMBA_SQUISHED;
    }
    else
    {
        spriteCoord = (m_nAnimFrame == 0) ? CSpriteConfig::GOOMBA_WALK1 : CSpriteConfig::GOOMBA_WALK2;
    }

    CSpriteRenderer::DrawSprite(pDC, pBitmap, screenX, screenY,
        m_nWidth, m_nHeight, spriteCoord.x, spriteCoord.y,
        spriteCoord.width, spriteCoord.height);
}
